#pragma once

#include <shlobj.h>
#pragma comment(lib, "Ole32.lib")
#include "SingletonMalloc.h"

class CShellExtension
{
public:
	LPMALLOC m_pMalloc;
	LPSHELLFOLDER m_pshfDesktop;

	CShellExtension();
	~CShellExtension();

	LPITEMIDLIST GetItemIDList(LPWSTR lpwstrPath);
	LPITEMIDLIST CreateItemID(UINT uSize);
	LPITEMIDLIST CopyItemID(LPITEMIDLIST pidl);
	LPITEMIDLIST ConcatItemID(LPITEMIDLIST pidl1,LPITEMIDLIST pidl2);

	LPITEMIDLIST GetNextItemID(LPITEMIDLIST pidl);
	LPITEMIDLIST GetPreviousItemID(LPITEMIDLIST pidl);

	UINT GetItemIDSize(LPITEMIDLIST pidl);
	BOOL STRRET2WSTR(LPITEMIDLIST pidl,LPSTRRET pStrret,LPWSTR lpwstrRet);

//	BOOL Execute(HWND,LPITEMIDLIST pidl);
//	BOOL ShowContextMenu(HWND hWnd, LPITEMIDLIST pidl);
//	BOOL InitImageLists(CListCtrl&);
//	UINT GetIcon(LPITEMIDLIST pidl);

};