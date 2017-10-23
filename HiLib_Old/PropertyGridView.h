#pragma once
#include "GridView.h"

class CCellProperty;
class CBackgroundProperty;

class CPropertyGridView:public CGridView
{
private:

public:
	CPropertyGridView(
			std::shared_ptr<CBackgroundProperty> spBackgroundProperty,
			std::shared_ptr<CCellProperty> spPropHeader,
			std::shared_ptr<CCellProperty> spPropFilter,
			std::shared_ptr<CCellProperty> spPropCell,
			std::shared_ptr<int> spDeltaScroll);
	virtual ~CPropertyGridView(){}
	boost::signals2::signal<void()> PropertyChanged;
	virtual void EnsureVisibleCell(const cell_type& pCell){}
	virtual void CellValueChanged(CellEventArgs& e)
	{
		this->PropertyChanged();
		CGridView::CellValueChanged(e);
	}
};

