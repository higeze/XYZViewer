#include "Chart.h"
#include "ChartElements.h"
#include "Series.h"
//#include "iostream.h"

std::wstring roundtowstring(const double& arg, int precision)
{
	std::wstring result;
	std::wstringstream sstream;
	sstream.setf(std::ios::fixed);
	sstream.precision(precision);
	if(!(sstream<<arg && sstream>>result))throw std::exception("roundtowstring error");
	int dotIndex=result.find_first_of(L'.');
	if(dotIndex!=std::wstring::npos){
		int nonZeroIndex=dotIndex-1;
		for(unsigned i=dotIndex+1;i<result.size();i++){
			if(result[i]!=L'0'){
				nonZeroIndex=i;
			}		
		}
		result.erase(nonZeroIndex+1);
	}
	return result;
}


void CPlotArea::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc = GetRect();
	HPEN hPen = e.DCPtr->SelectPen(*m_spProp->GetPenPtr());
	double xTick=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetTick();
	double xMin=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMinimum();
	double xMax=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMaximum();
	double yTick=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetTick();
	double yMin=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMinimum();
	double yMax=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMaximum();
	//Draw GridLine
	for(double xValue = xMin;xValue<=xMax;xValue+=xTick){
		int x = (int)((xValue-xMin)/(xMax-xMin)*rc.Width()) + rc.left;
		e.DCPtr->MoveToEx(x,rc.top);
		e.DCPtr->LineTo(x,rc.bottom);		
	}
	for(double yValue = yMin;yValue<yMax;yValue+=yTick){
			int y = (int)(rc.bottom - (yValue-yMin)/(yMax - yMin)*rc.Height());
			e.DCPtr->MoveToEx(rc.left,y);
			e.DCPtr->LineTo(rc.right,y);
	}
	//Draw Origin Line
	e.DCPtr->SelectPen(*m_spProp->GetOriginPenPtr());
		int x = (int)((0-xMin)/(xMax-xMin)*rc.Width()) + rc.left;
		e.DCPtr->MoveToEx(x,rc.top);
		e.DCPtr->LineTo(x,rc.bottom);		
		int y = (int)(rc.bottom - (0-yMin)/(yMax - yMin)*rc.Height());
		e.DCPtr->MoveToEx(rc.left,y);
		e.DCPtr->LineTo(rc.right,y);

	e.DCPtr->SelectPen(hPen);
}

double CAxis::CalcAutoTick(double min,double max,int minNum,int maxNum)
{
	double range = max - min;
	double place = floor(log10(exp(1.0f)) * log(range));
	double interval = pow(10, place - 1);
	double da[] = { 10, 5, 2 };

	for(int i=0;i<3;i++)
	{
		if (range / interval > maxNum)
		{
			interval *= da[i];
		}
		else if (range / interval < minNum)
		{
			interval /= da[i];
		}
	}
	return interval;
}


void CXAxis::Measure(CDC* pDC)
{	
	if(!m_bVisible){
		m_size = CSize();
	}

	HFONT hFont = pDC->SelectFont(*m_spProp->GetFontPtr());
	CRect rc;
	pDC->DrawTextExW(L"0123456789",-1,rc,DT_CALCRECT,NULL);

	//CRect rcLabel;
	//if(!m_wstrLabel.empty()){
	//	pDC->DrawTextExW((LPTSTR)m_wstrLabel.c_str(),-1,rcLabel,DT_CALCRECT,NULL);
	//}
	//m_size = CSize(0, m_tickLength + rc.Height() + rcLabel.Height());
	m_size = CSize(0, m_tickLength + rc.Height());

	pDC->SelectFont(hFont);
}

void CXAxis::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc = GetRect();
	HPEN hPen = e.DCPtr->SelectPen(*m_spProp->GetPenPtr());
	HFONT hFont = e.DCPtr->SelectFont(*m_spProp->GetFontPtr());
	int maxTickHeight = 0;
	for(double tick = GetMinimum();tick<=GetMaximum();tick+=GetTick()){
		int x = (int)((tick-GetMinimum())/(GetMaximum()-GetMinimum())*rc.Width() + rc.left);
		int y = rc.top;
		e.DCPtr->MoveToEx(x,y);
		e.DCPtr->LineTo(x,y+m_tickLength);
		auto wstrTick = roundtowstring(tick,8);
		CRect rcTick;
		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(wstrTick.c_str()),-1,rcTick,DT_CALCRECT,NULL);
		rcTick.SetRect(x-rcTick.Width()/2, y+m_tickLength, x+rcTick.Width()/2, y+m_tickLength+rcTick.Height());
		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(wstrTick.c_str()),-1,rcTick,DT_CENTER,NULL);
		maxTickHeight = max(maxTickHeight, rcTick.Height());
	}
	//if(!m_wstrLabel.empty())
	//{
	//	CRect rcLabel;
	//	e.DCPtr->DrawTextExW(const_cast<LPTSTR>(m_wstrLabel.c_str()),-1,rcLabel,DT_CALCRECT,NULL);
	//	UINT format = DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER;
	//	rcLabel.SetRect(rc.left, rc.top + m_tickLength + maxTickHeight, rc.right, rc.top + m_tickLength + maxTickHeight + rcLabel.Height());
	//	e.DCPtr->DrawTextExW(const_cast<LPTSTR>(m_wstrLabel.c_str()),-1,rcLabel, format, NULL);
	//}
	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectFont(hFont);
}

//CSize CYAxis::GetMeasuredSize()const
//{
//	long width=0;
//	width=m_tickLength + m_font.GetPointSize();
//	return CSize(0,width);
//}

void CYAxis::Measure(CDC* pDC)
{
	if(!m_bVisible){
		m_size = CSize();
	}

	HFONT hFont = pDC->SelectFont(*m_spProp->GetFontPtr());
	int maxWidth= 0;
	for(double tick = GetMinimum();tick<=GetMaximum();tick+=GetTick()){
		auto wstrTick = roundtowstring(tick,8);
		CRect rcTick;
		pDC->DrawTextExW(const_cast<LPTSTR>(wstrTick.c_str()),-1,rcTick,DT_CALCRECT,NULL);
		maxWidth = max(maxWidth, rcTick.Width());
	}
	pDC->SelectFont(hFont);
	m_size = CSize(m_tickLength + maxWidth, 0);

}

void CYAxis::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc = GetRect();
	HPEN hPen = e.DCPtr->SelectPen(*m_spProp->GetPenPtr());
	HFONT hFont = e.DCPtr->SelectFont(*m_spProp->GetFontPtr());
	for(double tick = GetMinimum();tick<=GetMaximum();tick+=GetTick()){
		int y = (int)(rc.bottom - (tick-GetMinimum())/(GetMaximum()-GetMinimum())*rc.Height());
		int x = rc.right;
		e.DCPtr->MoveToEx(x,y);
		e.DCPtr->LineTo(x-m_tickLength,y);
		auto wstrTick = roundtowstring(tick,8);
		CRect rcTick;
		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(wstrTick.c_str()),-1,rcTick,DT_CALCRECT,NULL);
		rcTick.SetRect(x - m_tickLength - rcTick.Width(), y - rcTick.Height()/2, x - m_tickLength, y + rcTick.Height()/2);
		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(wstrTick.c_str()),-1,rcTick,0,NULL);
	}

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectFont(hFont);

}

void CXBarAxis::Measure(CDC* pDC)
{	
	if(!m_bVisible){
		m_size = CSize();
	}

	CRect rc;
	HFONT hFont = pDC->SelectFont(*m_spProp->GetFontPtr());
	pDC->DrawTextExW(L"0123456789",-1,rc,DT_CALCRECT,NULL);
	pDC->SelectFont(hFont);
	m_size = CSize(0, rc.Height());
}

void CXBarAxis::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc = GetRect();
	HPEN hPen = e.DCPtr->SelectPen(*m_spProp->GetPenPtr());
	HFONT hFont = e.DCPtr->SelectFont(*m_spProp->GetFontPtr());

	auto spvspSeries = static_cast<CBarChart*>(m_pParent)->GetSeriesPtr();
	CRect rcPlot = static_cast<CBarChart*>(m_pParent)->GetPlotAreaPtr()->GetRect();

	//Paint only visible Series

	std::vector<std::shared_ptr<CBarSeries>> vspVisibleSeries;
	std::copy_if(spvspSeries->begin(),spvspSeries->end(),std::back_inserter(vspVisibleSeries),
		[&](const std::shared_ptr<CBarSeries>& spSeries)->bool{
			return spSeries->GetVisible();
		});
	if(!vspVisibleSeries.empty()){
		int rCLineSeriesWidth = rcPlot.Width()/vspVisibleSeries.size();
		for(int n=0;n<(int)vspVisibleSeries.size();n++)
		{
			auto spSeries=vspVisibleSeries[n];
			if(spSeries->GetVisible()){
				CRect rcText;
		//		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(spSeries->at(n).GetName().c_str()),-1,rcText,DT_CALCRECT,NULL);
				rcText.SetRect(
					rc.left+rCLineSeriesWidth*n,
					rc.top,
					rc.left+rCLineSeriesWidth*(n+1),
					rc.bottom);
				e.DCPtr->DrawTextExW(const_cast<LPTSTR>(spSeries->GetName().c_str()),-1,rcText,DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER,NULL);
			}
		}
	}

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectFont(hFont);
}

void CLineChartLegend::OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CLineSeries>>& e)
{
	switch(e.Action){
	case NotifyContainerChangedAction::Add:
		boost::for_each(e.NewItems,[&](std::shared_ptr<CLineSeries> spSeries){
			auto spStackPanel = std::make_shared<CStackPanel>(this);
			spStackPanel->SetOrientation(Orientation::Horizontal);
			auto spChildren = spStackPanel->GetChildrenPtr();
			spChildren->push_back(std::make_shared<CCheckBox>(spStackPanel.get(), 
				[spSeries]()->bool{return spSeries->GetVisible();},
				[spSeries](const bool& b)->void{spSeries->SetVisible(b);}));

			spChildren->push_back(std::make_shared<CLineMark>(spStackPanel.get(), 
				[spSeries]()->std::shared_ptr<CSeriesProperty>{return spSeries->GetSeriesPropertyPtr();},
				[spSeries](const std::shared_ptr<CSeriesProperty>& prop)->void{spSeries->SetSeriesPropertyPtr(prop);}));

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

void CBarChartLegend::OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CBarSeries>>& e)
{
	switch(e.Action){
	case NotifyContainerChangedAction::Add:
		boost::for_each(e.NewItems,[&](std::shared_ptr<CBarSeries> spSeries){
			auto spStackPanel = std::make_shared<CStackPanel>(this);
			spStackPanel->SetOrientation(Orientation::Horizontal);
			auto spChildren = spStackPanel->GetChildrenPtr();
			spChildren->push_back(std::make_shared<CCheckBox>(spStackPanel.get(), 
				[spSeries]()->bool{return spSeries->GetVisible();},
				[spSeries](const bool& b)->void{spSeries->SetVisible(b);}));

			//spChildren->push_back(std::make_shared<CLineMark>(spStackPanel.get(), 
			//	[spSeries]()->CSeriesProperty{return spSeries->GetSeriesProperty();},
			//	[spSeries](const CSeriesProperty& prop)->void{spSeries->SetSeriesProperty(prop);}));

			spChildren->push_back(std::make_shared<CBindTextBlock>(spStackPanel.get(), 
				spSeries->GetSeriesPropertyPtr()->GetFontPtr(),
				std::make_shared<UINT>(DT_CENTER | DT_SINGLELINE | DT_VCENTER),
				[spSeries]()->std::wstring{return spSeries->GetName();},
				[spSeries](const std::wstring& wstr)->void{spSeries->SetName(wstr);}));

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

//void CChartLegend::Measure(CDC* pDC)
//{
//	if(!m_bVisible)return CSize();
//	int maxWidth= 0;
//	int maxHeight=0;
//	auto spvspSeries = static_cast<CChart*>(m_pParent)->GetSeriesPtr();
//	boost::for_each(*spvspSeries,[&](std::shared_ptr<CLineSeries> spSeries){
//		auto spProperty = spSeries->GetSeriesPropertyPtr();
//		HFONT hFont = pDC->SelectFont(*(spProperty->GetFontPtr()));
//		CRect rcText;
//		pDC->DrawTextExW(const_cast<LPTSTR>(spSeries->GetName().c_str()),-1,rcText,DT_CALCRECT,NULL);
//		maxWidth = max(maxWidth, rcText.Width());
//		maxHeight += rcText.Height();
//		pDC->SelectFont(hFont);
//	});
//	m_size = CSize(m_lineLength + maxWidth,maxHeight);
//}

//void CChartLegend::OnPaint(PaintEventArgs& e)
//{
//	if(!m_bVisible)return;
//	CRect rc = GetRect();
//	//HPEN hPen = e.DCPtr->SelectPen(*m_spPen);
//	//HFONT hFont = e.DCPtr->SelectFont(*m_spFont);
//	auto spvspSeries = static_cast<CChart*>(m_pParent)->GetSeriesPtr();
//	int x=rc.left,y=rc.top;
//	boost::for_each(*spvspSeries,[&](std::shared_ptr<CLineSeries> spSeries){
//		auto spProperty = spSeries->GetSeriesPropertyPtr();
//		HFONT hFont = e.DCPtr->SelectFont(*(spProperty->GetFontPtr()));
//		HPEN hPen = e.DCPtr->SelectPen(*(spProperty->GetPenPtr()));
//		COLORREF cr = e.DCPtr->SetTextColor(spProperty->GetColor());
//
//		CRect rcTextSize;
//		//Measure Text
//		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(spSeries->GetName().c_str()),-1,rcTextSize,DT_CALCRECT,NULL);
//		//Paint Line
//		e.DCPtr->MoveToEx(x, y + rcTextSize.Height()/2);
//		e.DCPtr->LineTo(x + m_lineLength, y + rcTextSize.Height()/2);
//		//Paint Mark
//		e.DCPtr->Ellipse(x + m_lineLength/2 - m_markRadius, y + rcTextSize.Height()/2 - m_markRadius,
//						x + m_lineLength/2 + m_markRadius, y + rcTextSize.Height()/2 + m_markRadius);
//		//Paint Text
//		CRect rcText(x + m_lineLength, y, x + m_lineLength + rcTextSize.Width(), y + rcTextSize.Height());
//		e.DCPtr->DrawTextExW(const_cast<LPTSTR>(spSeries->GetName().c_str()),-1,rcText,0,NULL);
//		y += rcTextSize.Height();
//		
//		e.DCPtr->SelectPen(hPen);
//		e.DCPtr->SelectFont(hFont);
//		e.DCPtr->SetTextColor(cr);
//	});
//
//}


