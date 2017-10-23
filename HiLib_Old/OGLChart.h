#pragma once

#include "Vertex.h"
#include "OGLUIElement.h"
#include "OGLEventArgs.h"
#include "ObservableContainer.h"
//#include "ChartProperty.h"
#include "MyFont.h"
#include "MyPen.h"
#include "MyColor.h"
#include "MyRect.h"
#include "MyDC.h"
#include "MyFriendSerializer.h"
#include "OGLBitmapRenderer.h"
#include "UIElement.h"
#include "ChartElements.h"
#include <array>

class COGLAxis;
class COGLSeries;
class CChartLegend;

class COGLLineProperty
{
private:
	float m_width;
	//CColor m_color;
	std::array<GLubyte, 3> m_color;
public:
	COGLLineProperty(float width, const std::array<GLubyte, 3>& color):m_width(width),m_color(color){}
	virtual ~COGLLineProperty(){}
	float GetWidth()const{return m_width;}
	const GLubyte* GetColor()const{return m_color.data();}
};

class COGLFontProperty
{
private:
	float m_scale;
	std::array<GLubyte, 3> m_color;
public:
	COGLFontProperty(float scale, const std::array<GLubyte, 3>& color):m_scale(scale),m_color(color){}
	virtual ~COGLFontProperty(){}
	float GetScale()const{return m_scale;}
	const GLubyte* GetColor()const{return m_color.data();}
};
class COGLAxisProperty
{
private:
	std::shared_ptr<COGLLineProperty> m_spLineProperty;
	std::shared_ptr<COGLFontProperty> m_spFontProperty;
public:
	COGLAxisProperty(const std::shared_ptr<COGLLineProperty>& spLineProperty,const std::shared_ptr<COGLFontProperty>& spFontProperty)
		:m_spLineProperty(spLineProperty), m_spFontProperty(spFontProperty){}
	virtual ~COGLAxisProperty(){}
	std::shared_ptr<COGLLineProperty> GetLineProperty()const{return m_spLineProperty;}
	std::shared_ptr<COGLFontProperty> GetFontProperty()const{return m_spFontProperty;}


};

class COGLTickProperty
{
private:
	std::shared_ptr<COGLLineProperty> m_spLineProperty;
	std::shared_ptr<COGLFontProperty> m_spFontProperty;
public:
	COGLTickProperty(const std::shared_ptr<COGLLineProperty>& spLineProperty,const std::shared_ptr<COGLFontProperty>& spFontProperty)
		:m_spLineProperty(spLineProperty), m_spFontProperty(spFontProperty){}
	virtual ~COGLTickProperty(){}
	std::shared_ptr<COGLLineProperty> GetLineProperty()const{return m_spLineProperty;}
	std::shared_ptr<COGLFontProperty> GetFontProperty()const{return m_spFontProperty;}

};

class COGLGridProperty
{
private:
	std::shared_ptr<COGLLineProperty> m_spLineProperty;
public:
	COGLGridProperty(const std::shared_ptr<COGLLineProperty>& spLineProperty):m_spLineProperty(spLineProperty){}
	virtual ~COGLGridProperty(){}
	std::shared_ptr<COGLLineProperty> GetLineProperty()const{return m_spLineProperty;}
};

class COGLSeriesProperty
{
public:
	//float m_lineWidth;
	//CColor m_color;

	std::shared_ptr<CPen> m_spPen;
	std::shared_ptr<CFont> m_spFont;
	std::shared_ptr<CBrush> m_spBrush;

	COGLSeriesProperty(int nPointSize,std::wstring strFaceName, int nPenStyle, int nWidth, CColor color)
		:m_spFont(std::make_shared<CFont>(nPointSize, strFaceName)),
		m_spPen(std::make_shared<CPen>(nPenStyle, nWidth, color)),
		m_spBrush(std::make_shared<CBrush>(color))/*,m_lineWidth((float)nWidth), m_color(color)*/{}
	virtual ~COGLSeriesProperty(){}
	std::shared_ptr<CFont> GetFontPtr()const{return m_spFont;}
	std::shared_ptr<CPen> GetPenPtr()const{return m_spPen;}
	void SetFontPtr(std::shared_ptr<CFont>& spFont){m_spFont = spFont;}
	void SetPenPtr(std::shared_ptr<CPen>& spPen){m_spPen = spPen;}
	std::shared_ptr<CBrush> GetBrushPtr()const{return m_spBrush;}
	//float GetLineWidth()const{return m_lineWidth;}
	//CColor GetColor()const{return m_color;}
};

class COGLChartProperty
{
private:
	std::shared_ptr<CFont> m_spFont;

	std::shared_ptr<COGLAxisProperty> m_spAxisProperty;
	std::shared_ptr<COGLTickProperty> m_spTickProperty;
	std::shared_ptr<COGLGridProperty> m_spGridProperty;
public:

	std::shared_ptr<CFont> GetFontPtr()const{return m_spFont;}

	std::shared_ptr<COGLAxisProperty> GetAxisProperty()const{return m_spAxisProperty;}
	std::shared_ptr<COGLTickProperty> GetTickProperty()const{return m_spTickProperty;}
	std::shared_ptr<COGLGridProperty> GetGridProperty()const{return m_spGridProperty;}

	COGLChartProperty():
		m_spFont(std::make_shared<CFont>(10,L"Arial"))
	{
		std::array<GLubyte, 3> black = {0, 0, 0};
		std::array<GLubyte, 3> lightGray = {220, 220, 220};

		m_spAxisProperty = std::make_shared<COGLAxisProperty>(std::make_shared<COGLLineProperty>(2.0f, black), std::make_shared<COGLFontProperty>(0.7f, black));
		m_spTickProperty = std::make_shared<COGLTickProperty>(std::make_shared<COGLLineProperty>(2.0f, black), std::make_shared<COGLFontProperty>(0.5f, black));
		m_spGridProperty = std::make_shared<COGLGridProperty>(std::make_shared<COGLLineProperty>(1.0f, lightGray));
	}
	virtual ~COGLChartProperty(){}

};

class CLine:public CGDIUIElement
{
private:
	std::function<std::shared_ptr<COGLSeriesProperty>()> m_getter;
	std::function<void(const std::shared_ptr<COGLSeriesProperty>&)> m_setter;

	static const int m_lineLength = 16;
	static const int m_markRadius = 3;
	//std::shared_ptr<CSeriesProperty> m_spSeriesProperty;

public:
	CLine(CGDIUIElement* pParent, 
		std::function<std::shared_ptr<COGLSeriesProperty>()> getter,
		std::function<void(const std::shared_ptr<COGLSeriesProperty>&)> setter)
		:CGDIUIElement(pParent), m_getter(getter), m_setter(setter){}
	virtual ~CLine(){}
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
	virtual void SetState(const UIElementState::Type& state)
	{
		if(GetState()!=UIElementState::Hot && state == UIElementState::Hot){
			auto spPen = m_getter()->GetPenPtr();
			m_getter()->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()*2, spPen->GetColor()));
		}else if(GetState()==UIElementState::Hot && state != UIElementState::Hot){
			auto spPen = m_getter()->GetPenPtr();
			m_getter()->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()/2, spPen->GetColor()));
		}

		CGDIUIElement::SetState(state);
	}
};

class COGLChartLegend:public CChartLegend
{
public:
	COGLChartLegend(CGDIUIElement* pChart):CChartLegend(pChart){}
	virtual ~COGLChartLegend(){}
	void OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>& e);

//	virtual CSize Measure(CDC* pDC)const;
//	virtual void OnPaint(PaintEventArgs& e);
};

//Track
namespace TrackMode{
	enum Type
	{
		TM_NONE = 0,
		TM_SPIN,
		TM_PAN,
		TM_ZOOM,
	};
}

class COGLChart:public COGLUIElement, public CGDIUIElement
{
private:
	static const int NAMEARRAYSIZE = 10;
	static const int BUFSIZE = 512;
protected:
	//Chart
	std::shared_ptr<COGLChartProperty> m_spChartProperty;

	static std::shared_ptr<COGLBitmapRenderer> m_spOGLBitmapRenderer;
public:
	GLuint	m_uiDisplayListIndex_StockScene;

	//OpenGLRenderTrack
	TrackMode::Type m_trackMode;
	CVertex m_renderingCenter;
	GLfloat	m_fRenderingRate;
	CPoint m_lastPoint;
	CVertex m_lastVertex;
	GLfloat m_fAngle,m_fX, m_fY, m_fZ;
	GLfloat m_objectXform[4][4];
	bool m_initialized;
	std::shared_ptr<COGLSeries> m_spHotSeries;

	//Chart
	std::vector<std::shared_ptr<COGLUIElement>> m_children;
	//xaxis,yaxis,zaxis,series...
	std::shared_ptr<COGLAxis> m_spXAxis;
	std::shared_ptr<COGLAxis> m_spYAxis;
	std::shared_ptr<COGLAxis> m_spZAxis;

public:
	std::shared_ptr<COGLChartProperty> GetChartProperty()const{return m_spChartProperty;}
	std::shared_ptr<COGLAxis> GetXAxisPtr(){return m_spXAxis;}
	std::shared_ptr<COGLAxis> GetYAxisPtr(){return m_spYAxis;}
	std::shared_ptr<COGLAxis> GetZAxisPtr(){return m_spZAxis;}

//private:
	//OpenGLRendererBase
	virtual void Render(COGLRenderer* pOGLRenderer);
	virtual bool PreRenderScene(){ return true; }
 	virtual void RenderStockScene();
	virtual bool PostRenderScene(){ return true; }
	virtual bool RenderScene(){ return true; }
	virtual void ClearSeries();
	virtual void AddSeries(std::shared_ptr<COGLSeries> spSeries);

	void OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>& e);
public:

public:
	COGLChart(CGDIUIElement* pParent, std::shared_ptr<COGLChartProperty>& spChartProperty);
	virtual ~COGLChart(){}

public:

	//LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	void OnPaint(PaintEventArgs& e)override;
	//LRESULT OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	void OnLButtonDown(MouseEventArgs& e);
	void OnLButtonUp(MouseEventArgs& e);
	void OnMButtonDown(MouseEventArgs& e);
	void OnMButtonUp(MouseEventArgs& e);
	void OnMouseMove(MouseEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

	void OnKeyDown(KeyEventArgs& e);
	void OnContextMenu(ContextMenuEventArgs& e);

	float NormalizeX(const float& value);
	float NormalizeY(const float& value);
	float NormalizeZ(const float& value);
	CVertex NormalizeVertex(const CVertex& vertex);
protected:
	void	ptov( CPoint& point, CVertex& vertex, GLfloat width, GLfloat Height );

	void	BeginTrack(COGLRenderer* pRenderer, CPoint point, TrackMode::Type trackMode );
	void	EndTrack(COGLRenderer* pRenderer);
	bool	Track(COGLRenderer* pRenderer,  CPoint point );
	void	DoRotation(COGLRenderer* pRenderer,  const float fAngle, const float fX, const float fY, const float fZ );
	virtual void Initialize(COGLRenderer* pRenderer);
	virtual void ChangeSize(const CRect& rc);
	virtual bool SetupViewport(const CRect& rc);
	virtual bool SetupViewingFrustum(const CRect& rc);
	virtual bool SetupViewingTransform();

	unsigned int	m_auiName_pick[NAMEARRAYSIZE];
	GLuint	m_uiSelectionBuffer[BUFSIZE];
int Pick(COGLRenderer* pRenderer, CPoint pt );
void SelectionBuffer2NameArray(	GLuint uiCountHit,
											GLuint uiSelectionBuffer[],
											unsigned int uiNameArray_dest[NAMEARRAYSIZE] );
public:
int GetSeriesIndex(COGLSeries* pSeries);

};