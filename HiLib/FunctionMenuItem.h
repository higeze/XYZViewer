#pragma once
#include "MenuItem.h"

class CFunctionMenuItem:public CMenuItem
{
protected:
	std::function<void()> m_function;
public:
	CFunctionMenuItem():CMenuItem(){}
	virtual ~CFunctionMenuItem(){}

	void SetFunction(std::function<void()> function){m_function = function;}

	virtual LRESULT OnInitMenu(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled){return 0;};
	virtual LRESULT OnCommand(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		m_function();
		return 0;
	};
};