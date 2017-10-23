#pragma once
#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <memory>

/********/
/* UHDC */
/********/
struct release_dc
{
	HWND m_hWnd;
	release_dc():m_hWnd(NULL){}
	release_dc(HWND hWnd):m_hWnd(hWnd){}
	void operator()(HDC hDC)
	{
		if(m_hWnd && hDC){
			BOOL bRet=::ReleaseDC(m_hWnd,hDC);
			if(bRet){
				hDC=NULL;
			}else{
				bRet=FALSE;
			}
		}
	}
};
typedef std::unique_ptr<std::remove_pointer<HDC>::type,release_dc> UHDC; 

/********************/
/* UHBRUSH,UHBITMAP */
/********************/
struct delete_object
{
	delete_object(){}
	void operator()(HGDIOBJ ho){
		if(ho){
			BOOL bRet=::DeleteObject(ho);
			if(bRet){
				ho=NULL;
			}else{
				bRet=FALSE;
			}
		}
	}
};

typedef std::unique_ptr<std::remove_pointer<HBRUSH>::type,delete_object> UHBRUSH; 
typedef std::unique_ptr<std::remove_pointer<HBITMAP>::type,delete_object> UHBITMAP; 
typedef std::unique_ptr<std::remove_pointer<HFONT>::type,delete_object> UHFONT; 
//typedef std::unique_ptr<std::remove_pointer<HBRUSH>::type,decltype(&::DeleteObject)> UHBRUSH; 

/***************/
/* UHIMAGELIST */
/***************/
struct imagelist_destroy
{
	void operator()(HIMAGELIST himl){if(himl){::ImageList_Destroy(himl);}}
};
typedef std::unique_ptr<std::remove_pointer<HIMAGELIST>::type,imagelist_destroy> UHIMAGELIST; 

/**********/
/* UHMENU */
/**********/
struct destroy_menu
{
	void operator()(HMENU hMenu){if(hMenu){::DestroyMenu(hMenu);}}
};
typedef std::unique_ptr<std::remove_pointer<HMENU>::type,destroy_menu> UHMENU; 

/***********/
/* UHANDLE */
/***********/

struct close_handle
{
	void operator()(HANDLE handle){if(handle){::CloseHandle(handle);}}
};
typedef std::unique_ptr<std::remove_pointer<HANDLE>::type,close_handle> UHANDLE; 

/***********/
/* UHTHEME */
/***********/

struct close_theme_data
{
	void operator()(HTHEME hTheme){if(hTheme){::CloseThemeData(hTheme);}}
};
typedef std::unique_ptr<std::remove_pointer<HTHEME>::type,close_theme_data> UHTHEME; 