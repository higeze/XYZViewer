#pragma once
#include "MyWnd.h"
#include "MyDlg.h"
#include "MyString.h"
#include "FindDlgResource.h"

#include "GridView.h"

extern HWND g_hDlgModeless;

class CFindDlg:public CDlg<IDD_FIND>
{
private:
	CGridView* m_pWnd;
	CEdit m_findEdit;
	bool m_canDeleteThis;
	void OnFinalMessage(HWND hWnd)override
	{
		if(m_canDeleteThis){
			g_hDlgModeless = NULL;
			delete this;
		}		
	}

	INT_PTR OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//Get Icon and Set Icon
		HICON hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_MAINFRAME), IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
		SendMessage( WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage( WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

		//m_findEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_FIND);
		//m_findEdit.SetFocus();
		//m_findEdit.SetSel(0, -1);
		//m_findEdit.AddMsgHandler(WM_KEYDOWN,&CFindDlg::OnKeyDown,this);

		return TRUE;
	}

	//LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	//{
	//	switch(wParam){
	//	case VK_RETURN:
	//		OnCommandNext(0,0,NULL,bHandled);
	//		break;
	//	case VK_ESCAPE:
	//		OnClose(0,0,0,bHandled);
	//		break;
	//	default:
	//		break;
	//	
	//	}
	//	return 0;
	//}

	INT_PTR OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
        DestroyWindow();
		return TRUE;
	}

	INT_PTR OnCommandNext(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		CButton matchCaseCheckBox = (HWND)::GetDlgItem(m_hWnd, IDC_CHECKBOX_FIND_MATCHCASE);
		CButton matchWholeWordCheckBox = (HWND)::GetDlgItem(m_hWnd, IDC_CHECKBOX_FIND_MATCHWHOLEWORD);
		CEdit findEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_FIND);
		bool matchCase = !!matchCaseCheckBox.GetCheck();
		bool matchWholeWord = !!matchWholeWordCheckBox.GetCheck();
		std::wstring findWord = findEdit.GetWindowTextW();

		m_pWnd->FindNext(findWord, matchCase, matchWholeWord);

		return TRUE;
	}	
	
	INT_PTR OnCommandPrev(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		CButton matchCaseCheckBox = (HWND)::GetDlgItem(m_hWnd, IDC_CHECKBOX_FIND_MATCHCASE);
		CButton matchWholeWordCheckBox = (HWND)::GetDlgItem(m_hWnd, IDC_CHECKBOX_FIND_MATCHWHOLEWORD);
		CEdit findEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_FIND);
		bool matchCase = !!matchCaseCheckBox.GetCheck();
		bool matchWholeWord = !!matchWholeWordCheckBox.GetCheck();
		std::wstring findWord = findEdit.GetWindowTextW();

		m_pWnd->FindPrev(findWord, matchCase, matchWholeWord);

		return TRUE;
	}	

	INT_PTR OnCommandClose(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		return OnClose(WM_CLOSE,NULL,NULL,bHandled);
	}

public:
	CFindDlg(CGridView* pWnd):CDlg<IDD_FIND>(),m_pWnd(pWnd),m_canDeleteThis(true)
	{
		AddMsgHandler(WM_INITDIALOG,&CFindDlg::OnInitDialog,this);
		AddMsgHandler(WM_CLOSE,&CFindDlg::OnClose,this);
		//AddMsgHandler(WM_KEYDOWN,&CFindDlg::OnKeyDown,this);
		AddCmdIDHandler(IDOK,&CFindDlg::OnCommandNext,this);
		AddCmdIDHandler(IDC_BUTTON_FIND_PREV,&CFindDlg::OnCommandPrev,this);
		AddCmdIDHandler(IDCANCEL,&CFindDlg::OnCommandClose,this);
	}

	virtual ~CFindDlg(){}

};