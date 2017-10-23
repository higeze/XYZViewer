#pragma once
#include "MyWnd.h"

class CMenuItem
{
protected:
	MENUITEMINFO m_mii;
public:
	CMenuItem()
	{
		::ZeroMemory(&m_mii,sizeof(MENUITEMINFO));m_mii.cbSize = sizeof(MENUITEMINFO);
	}
	virtual ~CMenuItem(){}

	MENUITEMINFO* GetMenuItemInfoPtr(){return &m_mii;}

	WORD GetID()const{return m_mii.wID;}
	void SetID(WORD id){m_mii.wID = id;}

	void SetMask(UINT fMask){m_mii.fMask = fMask;}
	void SetType(UINT fType){m_mii.fType = fType;}
	void SetTypeData(LPTSTR dwTypeData){m_mii.dwTypeData = dwTypeData;}

	virtual LRESULT OnInitMenu(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled) = 0;
	virtual LRESULT OnCommand(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled) = 0;

	virtual bool GetIsChecked(){return false;}
	virtual void SetIsChecked(bool isChecked){}

};

