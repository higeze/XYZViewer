#pragma once

template<class T>
class CGdiObj
{
protected:
	T m_hGdiObj;
public:
	CGdiObj(T hGdiObj=NULL):m_hGdiObj(hGdiObj){}
	virtual ~CGdiObj()
	{
		if(m_hGdiObj!=NULL){
			::DeleteObject(m_hGdiObj);
			m_hGdiObj = NULL;
		}
	}
	void Attach(T hGdiObj)
	{
		if(m_hGdiObj != NULL && m_hGdiObj != hGdiObj)
			::DeleteObject(m_hGdiObj);
		m_hGdiObj = hGdiObj;
	}

	T Detach()
	{
		T hGdiObj = m_hGdiObj;
		m_hGdiObj = NULL;
		return hGdiObj;
	}
	operator T()const{return m_hGdiObj;}
	bool IsNull() const { return (m_hGdiObj == NULL); }


};