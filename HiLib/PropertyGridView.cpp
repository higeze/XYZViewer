#include "PropertyGridView.h"

CPropertyGridView::CPropertyGridView(
	std::shared_ptr<CBackgroundProperty> spBackgroundProperty,
	std::shared_ptr<CCellProperty> spPropHeader,
	std::shared_ptr<CCellProperty> spPropFilter,
	std::shared_ptr<CCellProperty> spPropCell,
	std::shared_ptr<int> spDeltaScroll)
	:CGridView(spBackgroundProperty,spPropHeader,spPropFilter,spPropCell,spDeltaScroll)
{
	//RegisterArgs and CreateArgs
	RegisterClassExArgument()
		.lpszClassName(_T("CPropertyGridView"))
		.style(CS_DBLCLKS)
		.hCursor(::LoadCursor(NULL, IDC_ARROW))
		.hbrBackground((HBRUSH)(COLOR_3DFACE+1));
	CreateWindowExArgument()
		.lpszClassName(_T("CPropertyGridView"))
		.lpszWindowName(_T("PropertyGridView"))
		.dwStyle(WS_CHILD | WS_VISIBLE)
		.hMenu((HMENU)1233215); 
}


