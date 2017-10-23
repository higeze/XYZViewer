#include "OGLChart.h"
#include "OGLAxis.h"
#include "OGLSeries.h"
#include "MyFont.h"
#include <math.h>
#include "OGLBitmapRenderer.h"
#include "OGLWindowRenderer.h"
#include "ChartElements.h"
#include "MySize.h"
#include "UIElement.h"
#include "MyMenu.h"
#include "MyWnd.h"
#include "3DLineChartResource.h"

#ifndef	M_PI
#define	M_PI	3.1415926535897932384626433832795
#endif
std::shared_ptr<COGLBitmapRenderer> COGLChart::m_spOGLBitmapRenderer = std::make_shared<COGLBitmapRenderer>();

void COGLChartLegend::OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>& e)
{
	switch(e.Action){
	case NotifyContainerChangedAction::Add:
		boost::for_each(e.NewItems,[&](std::shared_ptr<COGLSeries> spSeries){
			auto spStackPanel = std::make_shared<CStackPanel>(this);
			spStackPanel->SetOrientation(Orientation::Horizontal);
			auto spChildren = spStackPanel->GetChildrenPtr();
			spChildren->push_back(std::make_shared<CCheckBox>(spStackPanel.get(), 
				[spSeries]()->bool{return spSeries->GetVisible();},
				[spSeries](const bool& b)->void{spSeries->SetVisible(b);}));

			spChildren->push_back(std::make_shared<CLine>(spStackPanel.get(), 
				[spSeries]()->std::shared_ptr<COGLSeriesProperty>{return spSeries->GetSeriesPropertyPtr();},
				[spSeries](const std::shared_ptr<COGLSeriesProperty>& prop)->void{spSeries->SetSeriesPropertyPtr(prop);}));

			spChildren->push_back(std::make_shared<CBindTextBlock>(spStackPanel.get(), 
				spSeries->GetSeriesPropertyPtr()->GetFontPtr(),
				std::make_shared<UINT>(DT_CENTER | DT_SINGLELINE | DT_VCENTER),
				[spSeries]()->std::wstring{return spSeries->GetName();},
				[spSeries](const std::wstring& wstr)->void{/*spSeries->SetName(wstr);*/}));

			m_children->push_back(spStackPanel);

		});
		break;
	case NotifyContainerChangedAction::Clear:
		m_children->clear();
		break;
	default:
		break;
	}

}

COGLChart::COGLChart(CGDIUIElement* pParent, std::shared_ptr<COGLChartProperty>& spChartProperty) :
	/*COGLUIElement(nullptr),*/
	CGDIUIElement(pParent),
	m_spChartProperty(spChartProperty),
	m_uiDisplayListIndex_StockScene(0),
	m_trackMode(TrackMode::TM_NONE),
	m_renderingCenter(5,5,5),
	m_fRenderingRate(12),
	m_lastPoint(0,0),
	m_lastVertex(0,0,0),
	m_initialized(false)
{
	::ZeroMemory(m_objectXform, 4 * 4 * sizeof(GLfloat));

	m_objectXform[0][0] = 1.0f; m_objectXform[0][1] = 0.0f; m_objectXform[0][2] = 0.0f; m_objectXform[0][3] = 0.0f;
	m_objectXform[1][0] = 0.0f; m_objectXform[1][1] = 1.0f; m_objectXform[1][2] = 0.0f; m_objectXform[1][3] = 0.0f;
	m_objectXform[2][0] = 0.0f; m_objectXform[2][1] = 0.0f; m_objectXform[2][2] = 1.0f; m_objectXform[2][3] = 0.0f;
	m_objectXform[3][0] = 0.0f; m_objectXform[3][1] = 0.0f; m_objectXform[3][2] = 0.0f; m_objectXform[3][3] = 1.0f;	
	m_spXAxis = std::make_shared<COGLXAxis>(this, "X", 10);
	m_spYAxis = std::make_shared<COGLYAxis>(this, "Y", 10);
	m_spZAxis = std::make_shared<COGLZAxis>(this, "Z", 10);

	m_children.push_back(m_spXAxis);
	m_children.push_back(m_spYAxis);
	m_children.push_back(m_spZAxis);

	::ZeroMemory( m_auiName_pick, NAMEARRAYSIZE * sizeof(unsigned int) );
}

void COGLChart::Initialize(COGLRenderer* pRenderer)
{
	m_uiDisplayListIndex_StockScene = 0;
	m_trackMode = TrackMode::TM_NONE;
	m_renderingCenter.SetVertex(5, 5, 5);
	m_fRenderingRate = 12;
	m_lastPoint.SetPoint(0,0);
	m_lastVertex.SetVertex(0,0,0);

	::ZeroMemory(m_objectXform, 4 * 4 * sizeof(GLfloat));

	m_objectXform[0][0] = 1.0f; m_objectXform[0][1] = 0.0f; m_objectXform[0][2] = 0.0f; m_objectXform[0][3] = 0.0f;
	m_objectXform[1][0] = 0.0f; m_objectXform[1][1] = 1.0f; m_objectXform[1][2] = 0.0f; m_objectXform[1][3] = 0.0f;
	m_objectXform[2][0] = 0.0f; m_objectXform[2][1] = 0.0f; m_objectXform[2][2] = 1.0f; m_objectXform[2][3] = 0.0f;
	m_objectXform[3][0] = 0.0f; m_objectXform[3][1] = 0.0f; m_objectXform[3][2] = 0.0f; m_objectXform[3][3] = 1.0f;	

	DoRotation(pRenderer, -90.0f, 1.0f, 0.0f, 0.0f);	
	DoRotation(pRenderer, 45.0f, 0.0f, 1.0f, 0.0f);	
	DoRotation(pRenderer, 30.0f, 1.0f, 0.0f, 0.0f);	

	//::glSelectBuffer( BUFSIZE, m_uiSelectionBuffer );
	//::glInitNames();
	//::glPushName( 0 );
}

void COGLChart::ClearSeries()
{
	std::function<bool(std::shared_ptr<COGLUIElement>)> isCOGLSeries=
	[&](std::shared_ptr<COGLUIElement> spElement)->bool{
		const type_info& info=typeid(*spElement);
		if(info == typeid(COGLSeries)){
			return true;
		}else{
			return false;
		}
	};
	auto iter = boost::remove_if(m_children, isCOGLSeries);
	m_children.erase(iter, m_children.end());
}


void COGLChart::AddSeries(std::shared_ptr<COGLSeries> spSeries)
{
	//Add Series
	m_children.push_back(spSeries);
	//Initialize Min Max	
	auto xMin=min(0, m_spXAxis->GetMinimumValue());
	auto xMax=max(0, m_spXAxis->GetMaximumValue());
	auto yMin=min(0, m_spYAxis->GetMinimumValue());
	auto yMax=max(0, m_spYAxis->GetMaximumValue());
	auto zMin=min(0, m_spZAxis->GetMinimumValue());
	auto zMax=max(0, m_spZAxis->GetMaximumValue());

	//Calc Min Max Value
	for(auto& p : spSeries->m_points)
	{
		xMax=max(xMax, p.x);
		xMin=min(xMin, p.x);
		yMax=max(yMax, p.y);
		yMin=min(yMin, p.y);
		zMax=max(zMax, p.z);
		zMin=min(zMin, p.z);

	}
	m_spXAxis->SetMinimumValue(xMin);
	m_spXAxis->SetMaximumValue(xMax);
	m_spYAxis->SetMinimumValue(yMin);
	m_spYAxis->SetMaximumValue(yMax);
	m_spZAxis->SetMinimumValue(zMin);
	m_spZAxis->SetMaximumValue(zMax);

	//Calc Tick and Set Tick, Min and Max
	//In case Tick==0 (Min=Max=0), Set 1,1,-1.
	if(xMin==xMax){
		m_spXAxis->SetTick(1);
		m_spXAxis->SetMinimum(xMin-1);
		m_spXAxis->SetMaximum(xMax+1);
	}else{
		auto xTick=COGLAxis::CalcAutoTick(xMin,xMax,4,8);
		m_spXAxis->SetTick(xTick);
		m_spXAxis->SetMinimum(COGLAxis::CalcTickStart(xMin,xTick));
		m_spXAxis->SetMaximum(COGLAxis::CalcTickEnd(xMax,xTick));
	}

	if(yMin==yMax){
		m_spYAxis->SetTick(1);
		m_spYAxis->SetMinimum(yMin-1);
		m_spYAxis->SetMaximum(yMax+1);
	}else{

		auto yTick=COGLAxis::CalcAutoTick(yMin,yMax,4,8);
		m_spYAxis->SetTick(yTick);
		m_spYAxis->SetMinimum(COGLAxis::CalcTickStart(yMin,yTick));
		m_spYAxis->SetMaximum(COGLAxis::CalcTickEnd(yMax,yTick));	
	}

	if(zMin==zMax){
		m_spZAxis->SetTick(1);
		m_spZAxis->SetMinimum(zMin-1);
		m_spZAxis->SetMaximum(zMax+1);
	}else{

		auto zTick=COGLAxis::CalcAutoTick(zMin,zMax,4,8);
		m_spZAxis->SetTick(zTick);
		m_spZAxis->SetMinimum(COGLAxis::CalcTickStart(zMin,zTick));
		m_spZAxis->SetMaximum(COGLAxis::CalcTickEnd(zMax,zTick));	
	}
}

void COGLChart::RenderStockScene()
{
	if( 0 == m_uiDisplayListIndex_StockScene )
	{	// 軸描画処理
		m_uiDisplayListIndex_StockScene = ::glGenLists(1);
		if( 0 == m_uiDisplayListIndex_StockScene )
		{
			return;
		}
		::glNewList( m_uiDisplayListIndex_StockScene, GL_COMPILE );
		//RenderAxis();
		::glEndList();
	}
	
	if( 0 != m_uiDisplayListIndex_StockScene )
	{
		::glCallList( m_uiDisplayListIndex_StockScene );
	}
}

void COGLChart::Render(COGLRenderer* pOGLRenderer)
{
	//Make current
	//CWglMakeCurrenter currenter(pOGLRenderer->GetHDC(),pOGLRenderer->GetHGLRC());

	if(!m_initialized){
		Initialize(pOGLRenderer);
		m_initialized = true;
	}

	//ChangeSize
	ChangeSize(pOGLRenderer->GetRect());

	//Render
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	PreRenderScene();
//	::glEnable( GL_BLEND );
//	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	OGLPaintEventArgs e(pOGLRenderer);
	boost::for_each(m_children,[&](std::shared_ptr<COGLUIElement>& pElement)->void{
		pElement->OnPaint(e);
	});

//	::glDisable( GL_BLEND );

	PostRenderScene();

	::glFinish();

	if(pOGLRenderer->GetPixelFormatDescriptor().dwFlags & PFD_DOUBLEBUFFER){
		::SwapBuffers(pOGLRenderer->GetHDC());
	}

}

void COGLChart::OnPaint(PaintEventArgs& e)
{
	CRect rc=GetRect();
	m_spOGLBitmapRenderer->Update(e.DCPtr, CRect(0, 0, rc.Width(), rc.Height()));
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	Render(m_spOGLBitmapRenderer.get());
	::TransparentBlt(*(e.DCPtr), rc.left, rc.top, rc.Width(), rc.Height(),
				m_spOGLBitmapRenderer->GetHDC(), 0, m_spOGLBitmapRenderer->GetBufferSize().cy - rc.Height(), rc.Width(), rc.Height(),RGB(255,255,255));
}

void COGLChart::BeginTrack(COGLRenderer* pRenderer, CPoint point, TrackMode::Type trackMode )
{
//	SetCapture();
	m_trackMode = trackMode;
	switch( m_trackMode )
	{
	case TrackMode::TM_NONE:
			break;
		case TrackMode::TM_SPIN:
			ptov( point, m_lastVertex, (GLfloat)pRenderer->GetRect().Width(), (GLfloat)pRenderer->GetRect().Height() );
			break;
		case TrackMode::TM_PAN:
			m_lastPoint = point;
			break;
		case TrackMode::TM_ZOOM:
			m_lastPoint = point;
			break;
		default:
			break;
	}
}

void COGLChart::EndTrack(COGLRenderer* pRenderer)
{
	ReleaseCapture();
	m_trackMode = TrackMode::TM_NONE;
}

bool COGLChart::Track(COGLRenderer* pRenderer,  CPoint point )
{
	switch( m_trackMode )
	{
		case TrackMode::TM_NONE:
			return false;
		case TrackMode::TM_SPIN:
			{
				CVertex curVertex;
				ptov( point, curVertex, (GLfloat)pRenderer->GetRect().Width(), (GLfloat)pRenderer->GetRect().Height() );
				GLfloat dx = curVertex.x - m_lastVertex.x;
				GLfloat dy = curVertex.y - m_lastVertex.y;
				GLfloat dz = curVertex.z - m_lastVertex.z;
				float fAngle = 180.0f * (GLfloat)sqrt(dx*dx + dy*dy + dz *dz );
				float fX = m_lastVertex.y*curVertex.z - m_lastVertex.z*curVertex.y;
				float fY = m_lastVertex.z*curVertex.x - m_lastVertex.x*curVertex.z;
				float fZ = m_lastVertex.x*curVertex.y - m_lastVertex.y*curVertex.x;
				m_lastVertex.x = curVertex.x;
				m_lastVertex.y = curVertex.y;
				m_lastVertex.z = curVertex.z;
				//::glTranslatef(m_renderingCenter.x, m_renderingCenter.y, m_renderingCenter.z);
				DoRotation(pRenderer, fAngle, fX, fY, fZ );
				//::glTranslatef(-m_renderingCenter.x, -m_renderingCenter.y, -m_renderingCenter.z);
			}
			return true;
		case TrackMode::TM_PAN:
			{
				m_renderingCenter.x -= ( point.x - m_lastPoint.x ) / m_fRenderingRate;
				//m_renderingCenter.y -= ( point.x - m_lastPoint.x ) / m_fRenderingRate;
				m_renderingCenter.y += ( point.y - m_lastPoint.y ) / m_fRenderingRate;
				//m_renderingCenter.z += ( point.y - m_lastPoint.y ) / m_fRenderingRate;
				m_lastPoint = point;
	
				::glMatrixMode( GL_MODELVIEW );
				::glLoadIdentity();
				SetupViewingTransform();
			}
			return true;
		case TrackMode::TM_ZOOM:
			{	
				m_fRenderingRate *= (float)pow( 2, ( m_lastPoint.y - point.y ) * 0.002f );
				m_lastPoint = point;
				//視野角錐台の設定
				::glMatrixMode( GL_PROJECTION );
				::glLoadIdentity();
				SetupViewingFrustum(pRenderer->GetRect());
				::glMatrixMode( GL_MODELVIEW );
			}
			return true;
		default:
			assert( !"未対応" );
			break;
	}
	return false;
}

void COGLChart::DoRotation(COGLRenderer* pRenderer,  float fAngle, float fX, float fY, float fZ )
{
	// 回転マトリックスを現在のオブジェクトフォームマトリックスに掛け合わせる
	::glPushMatrix();
		::glLoadIdentity();
						::glTranslatef(m_renderingCenter.x, m_renderingCenter.y, m_renderingCenter.z);
		::glRotatef( fAngle, fX, fY, fZ );
						::glTranslatef(-m_renderingCenter.x, -m_renderingCenter.y, -m_renderingCenter.z);
		::glMultMatrixf( (GLfloat*) m_objectXform );
		::glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)m_objectXform );
	::glPopMatrix();

	// モデリング＆ビューイング変換行列
	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	SetupViewingTransform();

}

void COGLChart::OnLButtonDown(MouseEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	BeginTrack(m_spOGLBitmapRenderer.get(), e.Point, TrackMode::TM_SPIN );
	//Pick(m_spOGLBitmapRenderer.get(), e.Point.x, e.Point.y );

}

void COGLChart::OnLButtonUp(MouseEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	EndTrack(m_spOGLBitmapRenderer.get());
}

void COGLChart::OnMButtonDown(MouseEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	BeginTrack(m_spOGLBitmapRenderer.get(), e.Point, TrackMode::TM_PAN );
}

void COGLChart::OnMButtonUp(MouseEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	EndTrack(m_spOGLBitmapRenderer.get());
}

void COGLChart::OnMouseMove(MouseEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	Track(m_spOGLBitmapRenderer.get(), e.Point );
	Pick(m_spOGLBitmapRenderer.get(), e.Point );
}

void COGLChart::OnMouseWheel(MouseWheelEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	BeginTrack(m_spOGLBitmapRenderer.get(), e.Point, TrackMode::TM_ZOOM );
	CPoint	deltaPoint( e.Point.x - e.Delta / 3, e.Point.y - e.Delta / 3 );
	Track( m_spOGLBitmapRenderer.get(), deltaPoint );
	EndTrack(m_spOGLBitmapRenderer.get());
}

void COGLChart::OnKeyDown(KeyEventArgs& e)
{
	CWglMakeCurrenter currenter(m_spOGLBitmapRenderer->GetHDC(),m_spOGLBitmapRenderer->GetHGLRC());
	switch (e.Char)
	{
	case VK_SPACE:
		{
			Initialize(m_spOGLBitmapRenderer.get());
			break;
		}
	default:
		break;
	}
};

void COGLChart::OnContextMenu(ContextMenuEventArgs& e)
{
	CMenu menu = CMenu(::GetSubMenu(::LoadMenu(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_CONTEXTMENU_3DLINECHART)),0));

	CPoint ptScreen(e.Point);
	::ClientToScreen(e.WindowPtr->m_hWnd, &ptScreen);
	::SetForegroundWindow(e.WindowPtr->m_hWnd);
	menu.TrackPopupMenu(0,ptScreen.x,ptScreen.y,e.WindowPtr->m_hWnd);

}

float COGLChart::NormalizeX(const float& value)
{
	return	(value-m_spXAxis->GetMinimum())/(m_spXAxis->GetMaximum()-m_spXAxis->GetMinimum())*m_spXAxis->GetUnit();
}
float COGLChart::NormalizeY(const float& value)
{
	return	NormalizeX(value);//(value-m_spYAxis->GetMinimum())/(m_spYAxis->GetMaximum()-m_spYAxis->GetMinimum())*m_spYAxis->GetUnit();
}
float COGLChart::NormalizeZ(const float& value)
{
	return	(value-m_spZAxis->GetMinimum())/(m_spZAxis->GetMaximum()-m_spZAxis->GetMinimum())*m_spZAxis->GetUnit();
}

CVertex COGLChart::NormalizeVertex(const CVertex& vertex)
{
	return CVertex(
		NormalizeX(vertex.x),
		NormalizeY(vertex.y),
		NormalizeZ(vertex.z));
}

void COGLChart::ptov( CPoint& point, CVertex& vertex, GLfloat width, GLfloat height )
{
	GLfloat d, a;
//	v[0] = ( 2.0f * p[0]   - fWidth ) / fWidth;
//	v[1] = ( fHeight - 2.0f * p[1]  ) / fHeight;
	vertex.x = ( 2.0f * point.x   - width ) / width * 0.5f;
	vertex.y = ( height - 2.0f * point.y  ) / height * 0.5f;
	d = (GLfloat)sqrt( vertex.x*vertex.x + vertex.y*vertex.y );
	vertex.z = (GLfloat)cos( (M_PI/2.0f) * ( ( d < 1.0f ) ? d : 1.0f ) );

	a = 1.0f / (GLfloat)sqrt( vertex.x*vertex.x + vertex.y*vertex.y + vertex.z*vertex.z );
	vertex.x *= a;
	vertex.y *= a;
	vertex.z *= a;
}

bool COGLChart::SetupViewport(const CRect& rc)
{
	//auto rcBuffer = m_upBufferDC->GetRect();
	::glViewport(	0,
					0,//rcBuffer - rc.Height(),
					rc.Width(), 
					rc.Height() );
	return true;
}

bool COGLChart::SetupViewingFrustum(const CRect& rc)
{
	//TODO
	::glOrtho(	- rc.Width() * 0.5 / m_fRenderingRate,	// left
				  rc.Width() * 0.5 / m_fRenderingRate,	// right
				- rc.Height() * 0.5 / m_fRenderingRate,	// buttom
				  rc.Height() * 0.5 / m_fRenderingRate,	// top
				  0.1,			// near
				  2000 );			// far

	return true;
}

bool COGLChart::SetupViewingTransform()
{

	::gluLookAt (	m_renderingCenter.x,  m_renderingCenter.y, m_renderingCenter.z + 1000,	
					m_renderingCenter.x,  m_renderingCenter.y, m_renderingCenter.z,			
					0.0,  1.0, 0.0 );	

	::glMultMatrixf( (GLfloat*)m_objectXform );	

	return true;
}

void COGLChart::ChangeSize(const CRect& rc)
{
	SetupViewport(rc);

	::glMatrixMode( GL_PROJECTION );
	::glLoadIdentity();
	SetupViewingFrustum(rc);


	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	SetupViewingTransform();
}

void COGLChart::OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>& e)
{
	switch(e.Action){
	case NotifyContainerChangedAction::Add:
		boost::for_each(e.NewItems,[&](std::shared_ptr<COGLSeries> spSeries){
			AddSeries(spSeries);
		});
		break;
	case NotifyContainerChangedAction::Clear:
		ClearSeries();
		break;
	default:
		break;
	}
}


int COGLChart::Pick(COGLRenderer* pRenderer, CPoint pt )
{
	GLuint							uiSelectionBuffer[BUFSIZE] = {0};
	GLint							iCountHit = 0;
	GLint							viewport[4] = {0};
	int	i = 0;

	::glGetIntegerv( GL_VIEWPORT, viewport );

	::glSelectBuffer( BUFSIZE, uiSelectionBuffer );
	::glRenderMode( GL_SELECT );
	::glInitNames();
	::glPushName( 0 );
	::glMatrixMode( GL_PROJECTION );
	::glPushMatrix();
	::glLoadIdentity();


	::gluPickMatrix(	(GLdouble)    pt.x,					
						(GLdouble)  viewport[3]-pt.y+viewport[1],				
						5,									
						5,									
						viewport );							

	SetupViewingFrustum(pRenderer->GetRect());
	::glMatrixMode( GL_MODELVIEW );


	//Render(pRenderer);
	OGLPaintEventArgs e(pRenderer);
	boost::for_each(m_children,[&](std::shared_ptr<COGLUIElement>& pElement)->void{
		pElement->OnPaint(e);
	});
	::glFinish();

	::glMatrixMode( GL_PROJECTION );
	::glPopMatrix();

	::glFlush ();
	::glMatrixMode( GL_MODELVIEW );

	iCountHit = ::glRenderMode( GL_RENDER );
	if( iCountHit <= 0 )
	{	
		if(m_spHotSeries){
			m_spHotSeries->OnMouseLeave(MouseEventArgs(0, pt));
			m_spHotSeries = nullptr;
		}
		return iCountHit;
	}

	unsigned int uiNameArray[NAMEARRAYSIZE]={0};
	SelectionBuffer2NameArray(	iCountHit, uiSelectionBuffer, uiNameArray );

	if( NULL == uiNameArray )
	{
		if(m_spHotSeries){
			m_spHotSeries->OnMouseLeave(MouseEventArgs(0, pt));
			m_spHotSeries = nullptr;
		}
		return -1;
	}

	auto spCurrentSeries = std::dynamic_pointer_cast<COGLSeries>(m_children[uiNameArray[0]]);
	if(m_spHotSeries && spCurrentSeries && m_spHotSeries==spCurrentSeries){
		//Do nothing. keep hot.
	}else if(m_spHotSeries && spCurrentSeries && m_spHotSeries!=spCurrentSeries){
		m_spHotSeries->OnMouseLeave(MouseEventArgs(0,pt));
		spCurrentSeries->OnMouseEnter(MouseEventArgs(0,pt));
		m_spHotSeries = spCurrentSeries;
	}else if(!m_spHotSeries && spCurrentSeries){
		spCurrentSeries->OnMouseEnter(MouseEventArgs(0,pt));
		m_spHotSeries = spCurrentSeries;
	}else if(m_spHotSeries && !spCurrentSeries){
		m_spHotSeries->OnMouseLeave(MouseEventArgs(0,pt));	
		m_spHotSeries = nullptr;
	}else{
		m_spHotSeries = nullptr;
	}

	//RenderObjects( RM_SELECT, uiNameArray );
	//if(auto sp = std::dynamic_pointer_cast<COGLSeries>(m_children[uiNameArray[0]])){
	//	auto spPen = sp->GetSeriesPropertyPtr()->GetPenPtr();
	//	sp->GetSeriesPropertyPtr()->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()*2, spPen->GetColor()));
	//}

	return iCountHit;
}

void COGLChart::SelectionBuffer2NameArray(	GLuint uiCountHit,
											GLuint uiSelectionBuffer[],
											unsigned int uiNameArray_dest[NAMEARRAYSIZE] )
{
	int		iIndexName;
	GLuint	ui;
	GLuint	*puiSelectionBuffer_work;
	int		iCountName;
	float	fRange_near;
	float	fRange_far;
	unsigned int	uiNameArray[NAMEARRAYSIZE];
	float	fRange_far_dest;

	puiSelectionBuffer_work = (GLuint *) uiSelectionBuffer;
	for( ui = 0; ui < uiCountHit; ui++ )
	{
		iCountName = (int) *puiSelectionBuffer_work;					

		puiSelectionBuffer_work++;
		fRange_near = (float) *puiSelectionBuffer_work / 0x7fffffff;	

		puiSelectionBuffer_work++;
		fRange_far  = (float) *puiSelectionBuffer_work / 0x7fffffff;	

		for( iIndexName = 0; iIndexName < iCountName; iIndexName++ )
		{
			puiSelectionBuffer_work++;
			uiNameArray[iIndexName] = *puiSelectionBuffer_work;
		}
		puiSelectionBuffer_work++;

		if( NAMEARRAYSIZE < iCountName )
		{
			//assert( !"COpenGLRendererPick::SelectionBuffer2NameArray" );
			continue;
		}

		if( 0 == ui )
		{
			for( iIndexName = 0; iIndexName < iCountName; iIndexName++ )
			{
				uiNameArray_dest[iIndexName] = uiNameArray[iIndexName];
			}
			fRange_far_dest = fRange_far;
		}
		else
		{
			if( fRange_far < fRange_far_dest )
			{
				for( iIndexName = 0; iIndexName < iCountName; iIndexName++ )
				{
					uiNameArray_dest[iIndexName] = uiNameArray[iIndexName];
				}
				fRange_far_dest = fRange_far;
			}
			else if( fRange_far == fRange_far_dest
			 && uiNameArray[1] < uiNameArray_dest[1] )
			{
				for( iIndexName = 0; iIndexName < iCountName; iIndexName++ )
				{
					uiNameArray_dest[iIndexName] = uiNameArray[iIndexName];
				}
				fRange_far_dest = fRange_far;
			}
		}
	}
}

int COGLChart::GetSeriesIndex(COGLSeries* pSeries)
{
	auto iter = boost::find_if(m_children, [pSeries](const std::shared_ptr<COGLUIElement>& spElement)->bool{
		return spElement.get() == pSeries;
	});

	if(iter!=m_children.end()){
		return std::distance(m_children.begin(),iter);
	}else{
		return -1;
	}
}







