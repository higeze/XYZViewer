#pragma once
#include "MyWnd.h"
#include "PropertySerializer.h"
#include "PropertyGridView.h"

#define IDC_BUTTON_DEFAULT						161
#define IDC_BUTTON_OK							162
#define IDC_BUTTON_CANCEL						163
#define IDC_BUTTON_APPLY						164

//class CPropertyGridView;
class CCellProperty;

template<class T>
class CPropertyWnd:public CWnd
{
private:

	CButton m_buttonDefault;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CButton m_buttonApply;

	std::shared_ptr<CPropertyGridView> m_spGrid;
	std::wstring m_wstrPropertyName;
	std::shared_ptr<T> m_prop;
	std::shared_ptr<CCellProperty> m_spPropSheetCellHeader;
	std::shared_ptr<CCellProperty> m_spPropSheetCellFilter;
	std::shared_ptr<CCellProperty> m_spPropSheetCellCell;

	bool m_showDefault;
	bool m_showApply;

	bool m_isModal;
public:
#pragma warning(disable: 4996)
	boost::signals2::signal<void(const std::wstring&)> PropertyChanged;
	boost::signals2::signal<void()> WindowClose;
#pragma warning(default: 4996)

public:
	CPropertyWnd(
		std::shared_ptr<CBackgroundProperty> spBackgroundProperty,
		std::shared_ptr<CCellProperty> spPropHeader,
		std::shared_ptr<CCellProperty> spPropFilter,
		std::shared_ptr<CCellProperty> spPropCell,
		std::shared_ptr<CCellProperty> spPropSheetCellHeader,
		std::shared_ptr<CCellProperty> spPropSheetCellFilter,
		std::shared_ptr<CCellProperty> spPropSheetCellCell,
		std::shared_ptr<int> spDeltaScroll,
		std::wstring wstrPropertyName,
		std::shared_ptr<T> prop,
		bool showDefault = true, bool showApply = true, bool isModal = false)
		:CWnd(),
		m_spGrid(std::make_shared<CPropertyGridView>(spBackgroundProperty,spPropHeader,spPropFilter,spPropCell,spDeltaScroll)),
		m_spPropSheetCellHeader(spPropSheetCellHeader),
		m_spPropSheetCellFilter(spPropSheetCellFilter),
		m_spPropSheetCellCell(spPropSheetCellCell),
		m_wstrPropertyName(wstrPropertyName),
		m_prop(prop),
		m_showDefault(showDefault),
		m_showApply(showApply),
		m_isModal(isModal)
	{
		m_rca
			.lpszClassName(L"CPropertyWnd")
			.style(CS_VREDRAW | CS_HREDRAW)
			.hCursor(::LoadCursor(NULL, IDC_ARROW))
			.hbrBackground((HBRUSH)GetStockObject(GRAY_BRUSH));

		DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		if(m_isModal)dwStyle |= WS_POPUP;

		m_cwa
			.lpszWindowName(wstrPropertyName.c_str()) 
			.lpszClassName(L"CPropertyWnd")
			.dwStyle(dwStyle); 


		AddMsgHandler(WM_CREATE,&CPropertyWnd::OnCreate,this);
		AddMsgHandler(WM_SIZE,&CPropertyWnd::OnSize,this);
		AddMsgHandler(WM_CLOSE,&CPropertyWnd::OnClose,this);
		AddMsgHandler(WM_DESTROY,&CPropertyWnd::OnDestroy,this);

		AddCmdIDHandler(IDC_BUTTON_DEFAULT,&CPropertyWnd::OnCommandDefault,this);
		AddCmdIDHandler(IDC_BUTTON_OK,&CPropertyWnd::OnCommandOK,this);
		AddCmdIDHandler(IDC_BUTTON_APPLY,&CPropertyWnd::OnCommandApply,this);
		AddCmdIDHandler(IDC_BUTTON_CANCEL,&CPropertyWnd::OnCommandCancel,this);

	}
	virtual ~CPropertyWnd(){}

	std::shared_ptr<CPropertyGridView> GetGridView(){return m_spGrid;}

	LRESULT OnCommandDefault(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		m_spGrid->Clear();
		CCellSerializer serializer(m_spGrid,m_spPropSheetCellHeader,m_spPropSheetCellFilter,m_spPropSheetCellCell);
		serializer.Serialize(m_spGrid,m_wstrPropertyName.c_str(),std::make_shared<T>());
		m_buttonApply.EnableWindow(TRUE);
		m_spGrid->SubmitUpdate();
		return 0;
	}

	LRESULT OnCommandOK(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		CCellDeserializer deserializer(m_spGrid);
		deserializer.Deserialize(m_wstrPropertyName.c_str(),m_prop);
		PropertyChanged(m_wstrPropertyName);
		SendMessage(WM_CLOSE,NULL,NULL);
		return 0;
	}

	LRESULT OnCommandApply(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		CCellDeserializer deserializer(m_spGrid);
		deserializer.Deserialize(m_wstrPropertyName.c_str(),m_prop);
		m_spGrid->UpdateAll();
		PropertyChanged(m_wstrPropertyName);
		m_buttonApply.EnableWindow(FALSE);
		return 0;
	}

	LRESULT OnCommandCancel(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		SendMessage(WM_CLOSE,NULL,NULL);
		return 0;
	}

	LRESULT OnCreate(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//Modal Window
		if(m_isModal && GetParent()){
			::EnableWindow(GetParent(), FALSE);
		}

		//Default button
		if(m_showDefault){
			m_buttonDefault.CreateWindowExArgument()
				.lpszClassName(WC_BUTTON)
				.hMenu((HMENU)IDC_BUTTON_DEFAULT)
				.lpszWindowName(L"Default")
				.dwStyle(WS_CHILD | WS_VISIBLE | BP_PUSHBUTTON);
		
			m_buttonDefault.RegisterClassExArgument().lpszClassName(WC_BUTTON);
			m_buttonDefault.Create(m_hWnd,CRect(0,0,50,20));
		}

		//OK button
		m_buttonOK.CreateWindowExArgument()
			.lpszClassName(WC_BUTTON)
			.hMenu((HMENU)IDC_BUTTON_OK)
			.lpszWindowName(L"OK")
			.dwStyle(WS_CHILD | WS_VISIBLE | BP_PUSHBUTTON);
		
		m_buttonOK.RegisterClassExArgument().lpszClassName(WC_BUTTON);

		m_buttonOK.Create(m_hWnd,CRect(0,0,50,20));

		//Cancel button
		m_buttonCancel.CreateWindowExArgument()
			.lpszClassName(WC_BUTTON)
			.hMenu((HMENU)IDC_BUTTON_CANCEL)
			.lpszWindowName(L"Cancel")
			.dwStyle(WS_CHILD | WS_VISIBLE | BP_PUSHBUTTON);
		
		m_buttonCancel.RegisterClassExArgument().lpszClassName(WC_BUTTON);

		m_buttonCancel.Create(m_hWnd,CRect(0,0,50,20));

		//Apply button
		if(m_showApply){
			m_buttonApply.CreateWindowExArgument()
				.lpszClassName(WC_BUTTON)
				.hMenu((HMENU)IDC_BUTTON_APPLY)
				.lpszWindowName(L"Apply")
				.dwStyle(WS_CHILD | WS_VISIBLE | BP_PUSHBUTTON | WS_DISABLED);
		
			m_buttonApply.RegisterClassExArgument().lpszClassName(WC_BUTTON);
			m_buttonApply.Create(m_hWnd,CRect(0,0,50,20));
		}

		CCellSerializer serializer(m_spGrid,m_spPropSheetCellHeader,m_spPropSheetCellFilter,m_spPropSheetCellCell);
		serializer.Serialize(m_spGrid,m_wstrPropertyName.c_str(),m_prop);

		m_spGrid->PropertyChanged.connect(
			[&](){
				if(m_buttonApply.IsWindow()){
					m_buttonApply.EnableWindow(TRUE);
				}
			}
		);

		m_spGrid->Create(m_hWnd);
		m_spGrid->SubmitUpdate();
		return 0;
	}

	LRESULT OnClose(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//Modal Window
		if(m_isModal && GetParent()){
			::EnableWindow(GetParent(), TRUE);
		}
		WindowClose();
		m_spGrid->DestroyWindow();
		DestroyWindow();
		return 0;
	}

	LRESULT OnDestroy(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		return 0;
	}

	void OnFinalMessage(HWND hWnd)
	{
		delete this;
	}

	LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CRect rc=GetClientRect();
		if(m_buttonDefault.IsWindow())m_buttonDefault.MoveWindow(rc.left+3,rc.bottom-25,52,22,TRUE);
		if(m_buttonApply.IsWindow()){
			m_buttonApply.MoveWindow(rc.right-60,rc.bottom-25,52,22,TRUE);
			m_buttonOK.MoveWindow(rc.right-170,rc.bottom-25,52,22,TRUE);
			m_buttonCancel.MoveWindow(rc.right-115,rc.bottom-25,52,22,TRUE);
		}else{
			m_buttonOK.MoveWindow(rc.right-115,rc.bottom-25,52,22,TRUE);
			m_buttonCancel.MoveWindow(rc.right-60,rc.bottom-25,52,22,TRUE);
		}

		m_spGrid->MoveWindow(rc.left,rc.top,rc.Width(),rc.Height()-30,TRUE);

		return 0;
	}

};