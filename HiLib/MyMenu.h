#pragma once
#include "MenuItem.h"

class CMenu
{
private:
	HMENU m_hMenu;
public:
	explicit CMenu():m_hMenu(NULL){}
	explicit CMenu(HMENU hMenu):m_hMenu(hMenu){}
	virtual ~CMenu(){
		if(m_hMenu!=NULL){
			::DestroyMenu(m_hMenu);
			m_hMenu = NULL;
		}
	}
	CMenu(const CMenu& menu)
	{
		Copy(menu);
	}

	CMenu& operator=(const CMenu& menu)
	{
		Copy(menu);
		return *this;
	}

	void Copy(const CMenu& menu)
	{
		CreatePopupMenu();
		int cnt = menu.GetMenuItemCount();
		for( int nItem = 0; nItem < cnt; nItem++)
		{
			MENUITEMINFO mii = {0};
			mii.cbSize = sizeof(MENUITEMINFO);

			//Get MENUITEMINFO
			mii.fMask = MIIM_TYPE|MIIM_ID|MIIM_STATE;
			BOOL b = menu.GetMenuItemInfo(nItem, TRUE, &mii); 

			if( mii.wID == -1)
			{
				throw std::exception("CMenu::Copy couldn't support this type of menuitem.");
			}else{
				std::basic_string<TCHAR> name;
				if(mii.fType == MFT_STRING){
					mii.cch++;
					mii.dwTypeData = GetBuffer(name, mii.cch);
					menu.GetMenuItemInfo(nItem, TRUE, &mii);	
				}else if(mii.fType == MFT_SEPARATOR){
				}else{
					throw std::exception("CMenu::Copy couldn't support this type of menuitem.");
				}
				InsertMenuItem(GetMenuItemCount(), TRUE, &mii);			
			}
		}
	}

	bool IsNull() const { return (m_hMenu == NULL); }
	void Attach(HMENU hMenu)
	{
		if(m_hMenu != NULL && m_hMenu != hMenu)
			::DestroyMenu(m_hMenu);
		m_hMenu = hMenu;
	}

	HMENU Detach()
	{
		HMENU hMenu = m_hMenu;
		m_hMenu = NULL;
		return hMenu;
	}
	//InsertMenuItem(0, TRUE, &mii);

	HMENU CreatePopupMenu()
	{
		Attach(::CreatePopupMenu());
		return m_hMenu;
	}

	int GetMenuItemCount()const
	{
		return ::GetMenuItemCount(m_hMenu);
	}

	std::basic_string<TCHAR> GetMenuString(UINT uIDItem,UINT flags)const
	{
		std::basic_string<TCHAR> string;
		::GetMenuString(m_hMenu,uIDItem,GetBuffer(string,256),256,flags);
		ReleaseBuffer(string);
		return string;
	}

	UINT GetMenuItemID(int nPos)const
	{
		return ::GetMenuItemID(m_hMenu, nPos);
	}

	UINT GetMenuState(UINT uId,UINT uFlags)const
	{
		return ::GetMenuState(m_hMenu,uId,uFlags);
	}
	HMENU GetSubMenu(int nPos)const
	{
		return ::GetSubMenu(m_hMenu,nPos);
	}
	BOOL GetMenuItemInfo(UINT item, BOOL fByPosition, LPMENUITEMINFO lpmii)const
	{
		return ::GetMenuItemInfo(m_hMenu, item, fByPosition, lpmii);
	}

	BOOL InsertMenuItem(UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii)
	{
		return ::InsertMenuItem(m_hMenu, uItem, fByPosition, lpmii);
	}

	BOOL InsertMenuItem(UINT uItem, BOOL fByPosition,CMenuItem* pMenuItem)
	{
		return InsertMenuItem(uItem,fByPosition,pMenuItem->GetMenuItemInfoPtr());
	}

	BOOL InsertSeparator(UINT uItem, BOOL fByPosition)
	{
		MENUITEMINFO mii = {0};
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_FTYPE;
		mii.fType = MFT_SEPARATOR;
		return InsertMenuItem(uItem, TRUE, &mii);
	}

	BOOL TrackPopupMenu(UINT uFlags, int x, int y, HWND hWnd)
	{
		return ::TrackPopupMenu(m_hMenu, uFlags, x, y, 0, hWnd, NULL);
	}

	BOOL DeleteMenu(UINT uPosition, UINT uFlags)
	{
		return ::DeleteMenu(m_hMenu, uPosition, uFlags);
	}

	operator HMENU()const{return m_hMenu;}
	operator bool()const { return m_hMenu!=NULL; }
};