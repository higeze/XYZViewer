#pragma once
#include "MyPen.h"
#include "MyFont.h"
#include "OGLUIElement.h"
#include "OGLFont.h"
#include "OGLEventArgs.h"
#include "Vertex.h"
#include "MyString.h"
class COGLAxisProperty;

//COGLAxis
class COGLAxis:public COGLUIElement
{
protected:
	//std::shared_ptr<COGLAxisProperty> m_spAxisProperty;
	//std::shared_ptr<COGLFont> m_spOGLFont;
	std::string m_strName;
	float m_unit;
	float m_maximum;
	float m_minimum;
	float m_maximumValue;
	float m_minimumValue;

	float m_tick;
	float m_tickLength;

	static const int m_maxTickNumber=10;
	static const int m_minTickNumber=5;
public:
	COGLAxis(COGLUIElement* pParent, const std::string& strName, float unit)
		:COGLUIElement(pParent),m_strName(strName),m_unit(unit),m_maximumValue(0),m_minimumValue(0),m_tick(0),m_minimum(0),m_maximum(0),m_tickLength(0.4f){}
	virtual ~COGLAxis(){}
	//virtual void SetOGLFont(std::shared_ptr<COGLFont> spOGLFont){m_spOGLFont = spOGLFont;}
	virtual void OnPaint(OGLPaintEventArgs& e);

	virtual void PaintAxis(OGLPaintEventArgs& e) = 0;
	virtual void PaintAxisLabel(OGLPaintEventArgs& e) = 0;
	virtual void PaintTick(OGLPaintEventArgs& e) = 0;
	virtual void PaintTickLabel(OGLPaintEventArgs& e) = 0;
	virtual void PaintGrid(OGLPaintEventArgs& e) = 0;
	virtual void SetMaximum(float max){m_maximum = max;}
	virtual float GetMaximum()const{return m_maximum;}
	virtual void SetMinimum(float min){m_minimum = min;}
	virtual float GetMinimum()const{return m_minimum;};
	virtual void SetMaximumValue(float max){m_maximumValue = max;};
	virtual float GetMaximumValue()const{return m_maximumValue;};
	virtual void SetMinimumValue(float min){m_minimumValue = min;};
	virtual float GetMinimumValue()const{return m_minimumValue;};
	float GetUnit()const{return m_unit;}
	void SetUnit(const float unit){m_unit=unit;}
	float GetTick()const{return m_tick;}
	void SetTick(const float tick){m_tick=tick;}
	std::string GetName()const{return m_strName;}
	void SetName(const std::string& name){m_strName=name;}

	//Static
	float static CalcAutoTick(float min,float max,int minNum,int maxNum);
	float static CalcTickStart(float min, float tick);
	float static CalcTickEnd(float max,float tick);

};
//COGLXAxis
class COGLXAxis:public COGLAxis
{
public:
	COGLXAxis(COGLUIElement* pParent,  const std::string& strName,float unit)
		:COGLAxis(pParent, strName, unit){}
	virtual ~COGLXAxis(){}

	virtual void PaintAxis(OGLPaintEventArgs& e);
	virtual void PaintAxisLabel(OGLPaintEventArgs& e);
	virtual void PaintTick(OGLPaintEventArgs& e);
	virtual void PaintTickLabel(OGLPaintEventArgs& e);
	virtual void PaintGrid(OGLPaintEventArgs& e);
};
//COGLYAxis
class COGLYAxis:public COGLAxis
{
public:
	COGLYAxis(COGLUIElement* pParent,  const std::string& strName,float unit)
		:COGLAxis(pParent, strName, unit){}
	virtual ~COGLYAxis(){}
	virtual void PaintAxis(OGLPaintEventArgs& e);
	virtual void PaintAxisLabel(OGLPaintEventArgs& e);
	virtual void PaintTick(OGLPaintEventArgs& e);
	virtual void PaintTickLabel(OGLPaintEventArgs& e);
	virtual void PaintGrid(OGLPaintEventArgs& e);
};

//COGLZAxis
class COGLZAxis:public COGLAxis
{
public:
	COGLZAxis(COGLUIElement* pParent,  const std::string& strName, float unit)
		:COGLAxis(pParent, strName, unit){}
	virtual ~COGLZAxis(){}

	virtual void PaintAxis(OGLPaintEventArgs& e);
	virtual void PaintAxisLabel(OGLPaintEventArgs& e);
	virtual void PaintTick(OGLPaintEventArgs& e);
	virtual void PaintTickLabel(OGLPaintEventArgs& e);
	virtual void PaintGrid(OGLPaintEventArgs& e);
};

