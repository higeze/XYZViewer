#pragma once
#include "MyWnd.h"
#include "Sheet.h"
#include "MyGdiPlusHelper.h"
class IKeyObserver;
#include "UnDoReDoManager.h"
class CBackgroundProperty;

class CGridView:public CWnd,public CSheet
{
public:
	static CMenu ContextMenu;

public:
	typedef int size_type;
	typedef std::wstring string_type;

protected:
	CScrollBar m_vertical;
	CScrollBar m_horizontal;
	UPBufferDC m_upBuffDC;

	std::vector<std::shared_ptr<IKeyObserver>> m_keyObservers;
	row_type m_rowHeaderHeader; /**< Header Header row */
	row_type m_rowNameHeader; /**< Name Header row */
	row_type m_rowFilter; /**< Filter row */
	std::shared_ptr<CUnDoReDoManager> m_spUndoRedoManager; 
private:
	boost::asio::io_service m_iosv;
	boost::asio::io_service::work m_work;
	boost::asio::deadline_timer m_timer;
	//std::mutex m_mtx;
	std::shared_ptr<int> m_spDeltaScroll;
	std::shared_ptr<CBackgroundProperty> m_spBackgroundProperty;

	std::shared_ptr<CRect> m_spEditRect;
public:

	boost::signals2::signal<void(CColumn*)> SignalColumnInserted;
	boost::signals2::signal<void(CColumn*)> SignalColumnErased;
	boost::signals2::signal<void(CColumn*, size_type, size_type)> SignalColumnMoved;

	virtual void ColumnInserted(CColumnEventArgs& e)override;
	virtual void ColumnErased(CColumnEventArgs& e)override;
	virtual void ColumnMoved(CColumnMovedEventArgs& e)override;


public:
	CGridView(
			std::shared_ptr<CBackgroundProperty> spBackgroundProperty,
			std::shared_ptr<CCellProperty> spPropHeader,
			std::shared_ptr<CCellProperty> spPropFilter,
			std::shared_ptr<CCellProperty> spPropCell,
			std::shared_ptr<int> spDeltaScroll,
			CMenu* pContextMenu= &CGridView::ContextMenu);
	virtual ~CGridView(){}
	void SetEditRect(CRect rcEdit){m_spEditRect = std::make_shared<CRect>(rcEdit);}
	std::shared_ptr<CUnDoReDoManager>& GetUndoReDoManagerPtr(){return m_spUndoRedoManager;}
protected:
	virtual LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnVScroll(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnHScroll(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnRButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnLButtonDblClk(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnContextMenu(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnFilter(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual LRESULT OnLButtonDblClkTimeExceed(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnCmdEnChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);

	virtual LRESULT OnCommandEditHeader(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandDeleteColumn(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandResizeSheetCell(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual std::shared_ptr<CDC> GetClientDCPtr()const;

	virtual void OnContextMenu(ContextMenuEventArgs& e);

	virtual void OnCellLButtonClk(CellEventArgs& e);
	virtual void OnCellContextMenu(CellContextMenuEventArgs& e);

	virtual void OnCellPropertyChanged(CCell* pCell,LPCTSTR lpszProperty);
	virtual void OnColumnPropertyChanged(LPCTSTR lpszProperty);
	virtual void CGridView::RowVisibleChanged(CRowEventArgs& e) override;
	virtual void OnRowPropertyChanged(LPCTSTR lpszProperty);

	virtual void OnKeyDown(KeyEventArgs& e);
	virtual void OnPaintAll(PaintEventArgs& e);
public:
	virtual void ClearFilter();
	virtual void FilterAll();




public:
	boost::asio::deadline_timer* GetTimerPtr(){return &m_timer;}

	virtual row_type GetHeaderHeaderRowPtr()const{return m_rowHeaderHeader;}
	virtual void SetHeaderHeaderRowPtr(row_type row){m_rowHeaderHeader=row;}

	virtual row_type GetNameHeaderRowPtr()const{return m_rowNameHeader;}
	virtual void SetNameHeaderRowPtr(row_type row){m_rowNameHeader=row;}

	virtual row_type GetFilterRowPtr()const{return m_rowFilter;}
	virtual void SetFilterRowPtr(row_type row){m_rowFilter=row;}
	virtual void OnPaint(PaintEventArgs& e);
	virtual void EnsureVisibleCell(const cell_type& pCell);
	void Jump(std::shared_ptr<CCell>& spCell);
	virtual void Clear();

	virtual std::wstring FullXMLSave(){return std::wstring();}
	virtual void FullXMLLoad(const std::wstring& str){}

public:

	virtual LRESULT OnCommandPrintScreen(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandSelectAll(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandDelete(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){return 0;}
	virtual LRESULT OnCommandCopy(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandPaste(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled){return 0;}
	virtual LRESULT OnCommandUnDo(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandReDo(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	virtual LRESULT OnCommandFind(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	void FindNext(const std::wstring& findWord, bool matchCase, bool matchWholeWord);
	void FindPrev(const std::wstring& findWord, bool matchCase, bool matchWholeWord);
	//virtual void UpdateData(size_type rowData,size_type colData);

	virtual CGridView* GetGridPtr(){return this;};
	//virtual BOOL InvalidateRect(const LPRECT lpRect,BOOL bErase);

	virtual CPoint GetScrollPos()const;
	virtual void SetScrollPos(const CPoint& ptScroll);

	virtual coordinates_type GetVerticalScrollPos()const;
	virtual coordinates_type GetHorizontalScrollPos()const;
	virtual CRect GetPaintRect();
	virtual CRect GetPageRect();
std::pair<bool, bool> GetHorizontalVerticalScrollNecessity();
	
	virtual void UpdateAll();

	virtual void SortAll();

		virtual void UpdateScrolls();
		virtual void Invalidate();
		//virtual void ColumnErased(CColumnEventArgs& e);
		virtual void ColumnVisibleChanged(CColumnEventArgs& e);
		virtual void CellValueChanged(CellEventArgs& e);
		virtual void SubmitUpdate();


		Status SaveGIFWithNewColorTable(Image *pImage,IStream* pIStream,const CLSID* clsidEncoder,DWORD nColors,BOOL fTransparent);

	virtual CColumn* GetParentColumnPtr(CCell* pCell)override;

	virtual void MoveColumn(size_type colTo, column_type spFromColumn)override;
	virtual void EraseColumn(column_type spColumn)override;
	virtual void InsertColumn(size_type colTo, column_type pColumn)override;
	
	
	HGLOBAL GetPaintMetaFileData();
	HENHMETAFILE GetPaintEnhMetaFileData();
	HENHMETAFILE GetAllEnhMetaFileData();

private:


};