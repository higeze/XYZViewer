#pragma once

#include "MyColor.h"
#include "MyPen.h"
#include "GDIUIElement.h"

class CChart;
class CSeriesProperty;

struct CDoublePoint
{
	double x;
	double y;
	CDoublePoint():x(0),y(0){}
	CDoublePoint(double x, double y):x(x),y(y){}
	virtual ~CDoublePoint(){}
};

enum class ExtrapolationType
{
	None,
	Constant,
};

class ILineData
{
public:
	typedef UINT size_type;
public:
	ILineData(){}
	virtual ~ILineData(){}
	virtual std::wstring GetName()const = 0;
	virtual ExtrapolationType GetExtrapolationType()const = 0;
	virtual size_type GetSize()const = 0;
	virtual CDoublePoint GetPoint(const size_type& index)const = 0;
};

class CLineSeries:public CGDIUIElement
{
	typedef UINT size_type;
private:
	static const int m_markRadius=3;
	std::shared_ptr<ILineData> m_dataPtr;
	std::shared_ptr<CSeriesProperty> m_spSeriesProperty;
public:
	CLineSeries(CChart* pChart);
	virtual ~CLineSeries(){}

	std::shared_ptr<CSeriesProperty> GetSeriesPropertyPtr()const{return m_spSeriesProperty;}
	CSeriesProperty GetSeriesProperty()const{return *m_spSeriesProperty;}
	void SetSeriesProperty(const CSeriesProperty& prop){*m_spSeriesProperty = prop;}
	void SetSeriesPropertyPtr(std::shared_ptr<CSeriesProperty> spSeriesProperty){m_spSeriesProperty=spSeriesProperty;}

	void SetData(const std::shared_ptr<ILineData>& dataPtr){m_dataPtr = dataPtr;}

	std::wstring GetName()const{return m_dataPtr->GetName();}

	ExtrapolationType GetExtrapolationType()const{return m_dataPtr->GetExtrapolationType();}
	size_type GetSize()const{return m_dataPtr->GetSize();}
	CDoublePoint GetPoint(const size_type& index)const{return m_dataPtr->GetPoint(index);}


	
	virtual void OnPaint(PaintEventArgs& e);
	virtual void OnMouseMove(MouseEventArgs& e);
	virtual void SetState(const UIElementState::Type& state)
	{

		if(GetState()!=UIElementState::Hot && state == UIElementState::Hot){
			auto spPen = m_spSeriesProperty->GetPenPtr();
			m_spSeriesProperty->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()*2, spPen->GetColor()));
		}else if(GetState()==UIElementState::Hot && state != UIElementState::Hot){
			auto spPen = m_spSeriesProperty->GetPenPtr();
			m_spSeriesProperty->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()/2, spPen->GetColor()));
		}

		CGDIUIElement::SetState(state);
	}

};

class CBarSeries:public CGDIUIElement
{
private:
	static const int MARGIN_BAR_VALUE = 3;
	std::wstring m_wstrName;
	std::shared_ptr<CSeriesProperty> m_spSeriesProperty;
public:
	double m_value;
	CBarSeries(CChart* pChart);
	virtual ~CBarSeries(){}
	
	std::shared_ptr<CSeriesProperty> GetSeriesPropertyPtr()const{return m_spSeriesProperty;}
	CSeriesProperty GetSeriesProperty()const{return *m_spSeriesProperty;}
	void SetSeriesProperty(const CSeriesProperty& prop){*m_spSeriesProperty = prop;}

	void SetSeriesPropertyPtr(std::shared_ptr<CSeriesProperty> spSeriesProperty){m_spSeriesProperty=spSeriesProperty;}
	std::wstring GetName()const{return m_wstrName;}
	void SetName(const std::wstring wstrName){m_wstrName=wstrName;}

	double GetValue()const{return m_value;}
	void SetValue(const double& value){m_value = value;}
	
	virtual void OnPaint(PaintEventArgs& e);

};