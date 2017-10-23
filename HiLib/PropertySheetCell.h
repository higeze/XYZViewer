#pragma once
#include "SheetCell.h"
#include "Row.h"
#include "PropertySerializer.h"
#include "SheetEventArgs.h"

template<typename T>
class CPropertySheetCell:public CSheetCell
{
public:
	virtual bool CanResizeRow()const override{return true;}
	virtual bool CanResizeColumn()const override{return false;}

	void Resize(size_type row, size_type col)override
	{
		size_type curRowSize = GetPlusRowSize();
		size_type curColSize = GetPlusColumnSize();

		if(row==curRowSize)return;

		if(CanResizeRow() && row>curRowSize){
			for(auto i=0;i<row-curRowSize;i++){
				auto spRow=std::make_shared<CChildRow>(this);
				InsertRow(100,spRow);
				auto pColValue=AllColumn(1);

				CCellSerializer serializer(std::dynamic_pointer_cast<CSheet>(Cell(m_pRow,m_pColumn)),m_spHeaderProperty,m_spFilterProperty,m_spCellProperty);
				serializer.SerializeValue(std::make_shared<T::value_type>(),spRow.get(),pColValue.get());
			}
		}else if(CanResizeRow() && row<curRowSize && row>0){
			for(auto i=0;i<curRowSize-row;i++){
				auto& indexDic = m_rowAllDictionary.get<IndexTag>();
				auto iter=boost::prior(indexDic.end());
				EraseRow(iter->DataPtr.get());
			}
		}
		CellSizeChanged(CellEventArgs(this));
		SubmitUpdate();

		//Column is not resizable
	}
public:

	//Constructor
	CPropertySheetCell(
		CSheet* pSheet = nullptr,
		CRow* pRow = nullptr,
		CColumn* pColumn = nullptr,
		std::shared_ptr<CCellProperty> spProperty = nullptr,
		std::shared_ptr<CCellProperty> spHeaderProperty = nullptr,
		std::shared_ptr<CCellProperty> spFilterProperty = nullptr,
		std::shared_ptr<CCellProperty> spCellProperty = nullptr,
		CMenu* pMenu=nullptr)
		:CSheetCell(pSheet,pRow,pColumn,spProperty,spHeaderProperty,spFilterProperty,spCellProperty){}
	virtual ~CPropertySheetCell(){}
};
