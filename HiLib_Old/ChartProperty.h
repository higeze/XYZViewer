#pragma once
#include "MySize.h"
#include "MyRect.h"
#include "MyColor.h"
#include "MyPen.h"
#include "MyFont.h"
#include "SeriesProperty.h"
#include "MyFriendSerializer.h"

class CPlotAreaProperty
{
private:
	std::shared_ptr<CPen> m_spPen;
	std::shared_ptr<CPen> m_spOriginPen;
public:
	CPlotAreaProperty(std::shared_ptr<CPen> spPen, std::shared_ptr<CPen> spOriginPen)
		:m_spPen(spPen), m_spOriginPen(spOriginPen){}
	virtual ~CPlotAreaProperty(){}
	std::shared_ptr<CPen> GetPenPtr(){return m_spPen;}
	std::shared_ptr<CPen> GetOriginPenPtr(){return m_spOriginPen;}
};


class CAxisProperty
{
private:
	std::shared_ptr<CFont> m_spFont;
	std::shared_ptr<CPen> m_spPen;
public:
	CAxisProperty(std::shared_ptr<CFont> spFont, std::shared_ptr<CPen> spPen)
		:m_spFont(spFont), m_spPen(spPen){}
	virtual ~CAxisProperty(){}

	std::shared_ptr<CFont> GetFontPtr(){return m_spFont;}
	std::shared_ptr<CPen> GetPenPtr(){return m_spPen;}
};

class CTitleProperty
{
private:
	std::shared_ptr<CFont> m_spFont;
public:
	CTitleProperty(std::shared_ptr<CFont> spFont)
		:m_spFont(spFont){}
	virtual ~CTitleProperty(){}

	std::shared_ptr<CFont> GetFontPtr(){return m_spFont;}
};

class CLabelProperty
{
private:
	std::shared_ptr<CFont> m_spFont;
public:
	CLabelProperty(std::shared_ptr<CFont> spFont)
		:m_spFont(spFont){}
	virtual ~CLabelProperty(){}

	std::shared_ptr<CFont> GetFontPtr(){return m_spFont;}
};


class CChartProperty
{
private:
	std::shared_ptr<CPlotAreaProperty> m_spPlotAreaProp;
	std::shared_ptr<CAxisProperty> m_spXAxisProp;
	std::shared_ptr<CAxisProperty> m_spYAxisProp;

	std::shared_ptr<CTitleProperty> m_spTitleProp;
	std::shared_ptr<CLabelProperty> m_spXLabelProp;
	std::shared_ptr<CLabelProperty> m_spYLabelProp;

	
	//std::shared_ptr<std::vector<std::shared_ptr<CSeriesProperty>>> m_spvspSeriesProperty;
public:
	CChartProperty()
		:m_spPlotAreaProp(std::make_shared<CPlotAreaProperty>(std::make_shared<CPen>(PS_SOLID, 1, RGB(220,220,220)), std::make_shared<CPen>(PS_SOLID, 1, RGB(0,0,0)))),
		m_spXAxisProp(std::make_shared<CAxisProperty>(std::make_shared<CFont>(8, L"Arial"), std::make_shared<CPen>(PS_SOLID, 1, RGB(0,0,0)))),
		m_spYAxisProp(std::make_shared<CAxisProperty>(std::make_shared<CFont>(8, L"Arial"), std::make_shared<CPen>(PS_SOLID, 1, RGB(0,0,0)))),
		m_spTitleProp(std::make_shared<CTitleProperty>(std::make_shared<CFont>(12, L"Arial"))),
		m_spXLabelProp(std::make_shared<CLabelProperty>(std::make_shared<CFont>(8, L"Arial"))),
		m_spYLabelProp(std::make_shared<CLabelProperty>(std::make_shared<CFont>(8, L"Arial", 900)))
		//m_spvspSeriesProperty(std::make_shared<std::vector<std::shared_ptr<CSeriesProperty>>>())
	{
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x222584));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xb80117));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x00904a));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xedc600));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x261e1c));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x6d1782));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x8f253b));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xa0c238));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xd16b16));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x0168b3));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xb88b26));
		//m_spvspSeriesProperty->push_back(std::make_shared<CSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xc30068));
	}
	virtual ~CChartProperty(){}

	std::shared_ptr<CPlotAreaProperty> GetPlotAreaPropertyPtr(){return m_spPlotAreaProp;}
	std::shared_ptr<CAxisProperty> GetXAxisPropertyPtr(){return m_spXAxisProp;}
	std::shared_ptr<CAxisProperty> GetYAxisPropertyPtr(){return m_spYAxisProp;}
	std::shared_ptr<CTitleProperty> GetTitlePropertyPtr(){return m_spTitleProp;}
	std::shared_ptr<CLabelProperty> GetXLabelPropertyPtr(){return m_spXLabelProp;}
	std::shared_ptr<CLabelProperty> GetYLabelPropertyPtr(){return m_spYLabelProp;}
	//std::shared_ptr<std::vector<std::shared_ptr<CSeriesProperty>>> GetSeriesPropertiesPtr(){return m_spvspSeriesProperty;}

	FRIEND_SERIALIZER

    template <class Archive>
    void serialize(Archive& ar)
    {
		//ar("GridPen", m_spGridPen);
		//ar("AxisPen", m_spAxisPen);
		//ar("TitleFont", m_spTitleFont);
		//ar("AxisFont", m_spAxisFont);
		//ar("SeriesPens", m_spPens);
    }


};

//class COGLChartProperty
//{
//public:
//	std::shared_ptr<CFont> m_spFont;
//	std::shared_ptr<COGLAxisProperty> m_spAxisProperty;
//	std::vector<std::shared_ptr<COGLSeriesProperty>> m_vspSeriesProperty;
//
//	COGLChartProperty():
//		m_spFont(std::make_shared<CFont>(10,L"Arial")),
//		m_spAxisProperty(std::make_shared<COGLAxisProperty>()),
//		m_vspSeriesProperty(){
//
//			m_vspSeriesProperty.push_back(std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xb80117));
//			m_vspSeriesProperty.push_back(std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x222584));
//			m_vspSeriesProperty.push_back(std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x00904a));
//			m_vspSeriesProperty.push_back(std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0xedc600));
//			m_vspSeriesProperty.push_back(std::make_shared<COGLSeriesProperty>(8, L"Arial", PS_SOLID, 2, 0x261e1c));
//		}
//
//};
