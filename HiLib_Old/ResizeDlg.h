#pragma once
#include "MyWnd.h"
#include "MyDlg.h"
#include "MyString.h"
#include "SheetCell.h"
#include "SheetCellResource.h"

extern HWND g_hDlgModeless;

class CResizeDlg:public CDlg<IDD_DIALOG_RESIZE>
{
private:
	CSheetCell* m_pSheetCell;
	//CEdit m_rowSizeEdit;
	//CEdit m_colSizeEdit;
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
		SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

		CEdit rowSizeEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_ROWSIZE);
		CEdit colSizeEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_COLUMNSIZE);
		int rowSize = m_pSheetCell->GetPlusRowSize();
		int colSize = m_pSheetCell->GetPlusColumnSize();
		rowSizeEdit.SetWindowTextW(boost::lexical_cast<std::wstring>(rowSize).c_str());
		colSizeEdit.SetWindowTextW(boost::lexical_cast<std::wstring>(colSize).c_str());

		rowSizeEdit.EnableWindow(m_pSheetCell->CanResizeRow());
		colSizeEdit.EnableWindow(m_pSheetCell->CanResizeColumn());

		return TRUE;
	}

	INT_PTR OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
        DestroyWindow();
		return TRUE;
	}

	INT_PTR OnCommandOK(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		CEdit rowSizeEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_ROWSIZE);
		CEdit colSizeEdit = (HWND)::GetDlgItem(m_hWnd, IDC_EDIT_COLUMNSIZE);
		int rowSize = boost::lexical_cast<int>(rowSizeEdit.GetWindowText());
		int colSize = boost::lexical_cast<int>(colSizeEdit.GetWindowText());
		m_pSheetCell->Resize(rowSize,colSize);
		::SendMessage(m_hWnd,WM_CLOSE,NULL,NULL);
		return TRUE;
	}	

	INT_PTR OnCommandClose(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		return OnClose(WM_CLOSE,NULL,NULL,bHandled);
	}

public:
	CResizeDlg(CSheetCell* pSheetCell):CDlg<IDD_DIALOG_RESIZE>(),m_pSheetCell(pSheetCell),m_canDeleteThis(true)
	{
		AddMsgHandler(WM_INITDIALOG,&CResizeDlg::OnInitDialog,this);
		AddMsgHandler(WM_CLOSE,&CResizeDlg::OnClose,this);
		AddCmdIDHandler(IDOK,&CResizeDlg::OnCommandOK,this);
		AddCmdIDHandler(IDCANCEL,&CResizeDlg::OnCommandClose,this);
	}

	virtual ~CResizeDlg(){}

};