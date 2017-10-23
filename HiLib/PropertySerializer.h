#pragma once

#include "MyMPL.h"
#include "MyDC.h"
#include "MyString.h"
#include "PropertyGridView.h"

#include "Row.h"
#include "ParentRowHeaderColumn.h"
#include "PropertyColumn.h"

#include "CellProperty.h"
#include "Sheet.h"
#include "SheetCell.h"
#include "ColorCell.h"
#include "FontCell.h"
#include "BoolCell.h"
#include "MySerializer.h"

#include "PropertySheetCell.h"
class CCellSerializer;


#define ENABLE_IF_DEFAULT typename std::enable_if<\
									!has_serialize<T,CCellSerializer>::value &&\
									!(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0
//#define ENABLE_IF_DEFAULT typename std::enable_if<\
//									!has_serialize<T,CCellSerializer>::value &&\
//									!has_save<T,CCellSerializer>::value &&\
//									!std::is_enum<T>::value &&\
//									!is_shared_ptr<T>::value\
//									>::type* = 0

#define ENABLE_IF_ENUM typename std::enable_if<\
									!has_serialize<T,CCellSerializer>::value &&\
									!(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
										std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0
//#define ENABLE_IF_ENUM typename std::enable_if<\
//									!has_serialize<T,CCellSerializer>::value &&\
//									!has_save<T,CCellSerializer>::value &&\
//										std::is_enum<T>::value &&\
//									!is_shared_ptr<T>::value\
//									>::type* = 0

#define ENABLE_IF_SERIALIZE typename std::enable_if<\
										has_serialize<T,CCellSerializer>::value &&\
									!(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0
//#define ENABLE_IF_SERIALIZE typename std::enable_if<\
//										has_serialize<T,CCellSerializer>::value &&\
//									!has_save<T,CCellSerializer>::value &&\
//									!std::is_enum<T>::value &&\
//									!is_shared_ptr<T>::value\
//									>::type* = 0

#define ENABLE_IF_VECTOR typename std::enable_if<\
									!has_serialize<T,CCellSerializer>::value &&\
									!(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
									!std::is_enum<T>::value &&\
										is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0

#define ENABLE_IF_SHARED_PTR typename std::enable_if<\
									!has_serialize<T,CCellSerializer>::value &&\
									!(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
										is_shared_ptr<T>::value \
									>::type* = 0
//#define ENABLE_IF_SHARED_PTR typename std::enable_if<\
//									!has_serialize<T,CCellSerializer>::value &&\
//									!has_save<T,CCellSerializer>::value &&\
//									!std::is_enum<T>::value &&\
//										is_shared_ptr<T>::value \
//									>::type* = 0

#define ENABLE_IF_SAVE_LOAD typename std::enable_if<\
									!has_serialize<T,CCellSerializer>::value &&\
										(has_save<T,CCellSerializer>::value && has_load<T,CCellSerializer>::value) &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value \
									>::type* = 0
//#define ENABLE_IF_SAVE_LOAD typename std::enable_if<\
//									!has_serialize<T,CCellSerializer>::value &&\
//										has_save<T,CCellSerializer>::value &&\
//									!std::is_enum<T>::value &&\
//									!is_shared_ptr<T>::value \
//									>::type* = 0


class CCellSerializer
{
private:
	std::weak_ptr<CSheet> m_pSheet;
	CDC* m_pDC;
	std::shared_ptr<CCellProperty> m_spPropSheetCellHeader;
	std::shared_ptr<CCellProperty> m_spPropSheetCellFilter;
	std::shared_ptr<CCellProperty> m_spPropSheetCellCell;

public:
	CCellSerializer(
		std::shared_ptr<CSheet> pSheet,
		std::shared_ptr<CCellProperty> spPropSheetCellHeader,
		std::shared_ptr<CCellProperty> spPropSheetCellFilter,
		std::shared_ptr<CCellProperty> spPropSheetCellCell)
		:m_pSheet(pSheet),
		m_spPropSheetCellHeader(spPropSheetCellHeader),
		m_spPropSheetCellFilter(spPropSheetCellFilter),
		m_spPropSheetCellCell(spPropSheetCellCell){}
	virtual ~CCellSerializer(){}

	template<class T>
	void Serialize(std::shared_ptr<CPropertyGridView> pGrid,const TCHAR* lpszRootName,T& t,ENABLE_IF_SERIALIZE)
	{
		//Serialize
		t.serialize(*this);
	}
	template<class T>
	void Serialize(std::shared_ptr<CPropertyGridView> pGrid,const TCHAR* lpszRootName,T& t,ENABLE_IF_SAVE_LOAD)
	{
		//Serialize
		t.save(*this);
	}

	template<class T>
	void Serialize(std::shared_ptr<CPropertyGridView> pGrid,const TCHAR* lpszRootName,T* pT)
	{
		Serialize(pGrid,lpszRootName,*pT);
	}

	template<class T>
	void Serialize(std::shared_ptr<CPropertyGridView> pGrid,const TCHAR* lpszRootName,std::shared_ptr<T> pT)
	{
		Serialize(pGrid,lpszRootName,*pT);
	}

	template<class char_type,class T>
	void operator()(const char_type* lpszName,T& t)
	{

		if(auto spSheet=m_pSheet.lock()){
			std::shared_ptr<CRow> pRow;
			std::shared_ptr<CColumn> pColProperty;
			std::shared_ptr<CColumn> pColValue;	
			if(auto pSheet=std::dynamic_pointer_cast<CGridView>(spSheet)){
				if(pSheet->Empty()){
					auto pRowHeader=std::make_shared<CParentHeaderRow>(pSheet.get());
					auto pRowFilter=std::make_shared<CParentRow>(pSheet.get());
					auto pColHeader=std::make_shared<CParentRowHeaderColumn>(pSheet.get());
					pSheet->SetNameHeaderRowPtr(pRowHeader);
					pSheet->SetFilterRowPtr(pRowFilter);
					pSheet->SetHeaderColumnPtr(pColHeader);
					pSheet->InsertRow(-1,pRowHeader);
					pSheet->InsertRow(-1,pRowFilter);
					pSheet->InsertColumnImpl(-1,pColHeader);

					pRow=std::make_shared<CParentRow>(pSheet.get());
					pSheet->InsertRow(100,pRow);
			
					pColProperty=std::make_shared<CParentPropertyNameColumn>(pSheet.get());
					pColValue=std::make_shared<CParentPropertyValueColumn>(pSheet.get());

					pSheet->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColProperty);
					pSheet->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColValue);
				}else{
					pRow=std::make_shared<CParentRow>(pSheet.get());
					pSheet->InsertRow(100,pRow);
					pColProperty=pSheet->AllColumn(0);
					pColValue=pSheet->AllColumn(1);
				}
			
			}else if(auto pSheet=std::dynamic_pointer_cast<CSheetCell>(spSheet)){

				if(pSheet->Empty()){
					auto pChildRowHeader=std::make_shared<CChildHeaderRow>(pSheet.get());
					pSheet->SetHeaderRowPtr(pChildRowHeader);
					pSheet->InsertRow(-1,pChildRowHeader);

					pRow=std::make_shared<CChildRow>(pSheet.get());
					pSheet->InsertRow(100,pRow);
			
					pColProperty=std::make_shared<CChildPropertyNameColumn>(pSheet.get());
					pColValue=std::make_shared<CChildPropertyValueColumn>(pSheet.get());

					pSheet->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColProperty);
					pSheet->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColValue);
				}else{
					pRow=std::make_shared<CChildRow>(pSheet.get());
					pSheet->InsertRow(100,pRow);
					pColProperty=pSheet->AllColumn(0);
					pColValue=pSheet->AllColumn(1);
				}
			}

			//spSheet->UpdateRowVisibleDictionary();
			//spSheet->UpdateColumnVisibleDictionary();

			pColProperty->Cell(pRow.get())->SetStringCore(std::wstring(lpszName,(lpszName+strlen(lpszName))));
			SerializeValue(t,pRow.get(),pColValue.get());	
		}else{
			return;
		}
	}

	//For base
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_DEFAULT)
	{
		pCol->Cell(pRow)->SetStringCore(boost::lexical_cast<std::wstring>(t));
	}

	//For std::string boost::lexical cast couldn't cast std::string to std::wstring
	void SerializeValue(std::string& t,CRow* pRow,CColumn* pCol)
	{
		pCol->Cell(pRow)->SetStringCore(str2wstr(t));
	}
	//For Color
	void SerializeValue(CColor& t,CRow* pRow,CColumn* pCol)
	{
		if(auto spSheet=m_pSheet.lock()){
			pCol->Cell(pRow)=std::make_shared<CColorCell>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				t);
		}
	}
	//For Font
	void SerializeValue(CFont& t,CRow* pRow,CColumn* pCol)
	{
		if(auto spSheet=m_pSheet.lock()){
			pCol->Cell(pRow)=std::make_shared<CFontCell>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				t);
		}
	}

	//For bool
	void SerializeValue(bool& t,CRow* pRow,CColumn* pCol)
	{
		if(auto spSheet=m_pSheet.lock()){
			pCol->Cell(pRow)=std::make_shared<CBoolCell>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				t);
		}
	}

	//For enum
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_ENUM)
	{
		pCol->Cell(pRow)->SetStringCore(boost::lexical_cast<std::wstring>(static_cast<int>(t)));
	}
	
	//For serialize
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_SERIALIZE)
	{
		if(auto spSheet=m_pSheet.lock()){
			std::shared_ptr<CSheetCell> spSheetCell(
				std::make_shared<CSheetCell>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				m_spPropSheetCellHeader,
				m_spPropSheetCellFilter,
				m_spPropSheetCellCell));

			pCol->Cell(pRow)=spSheetCell;

			t.serialize(CCellSerializer(std::dynamic_pointer_cast<CSheet>(spSheetCell),m_spPropSheetCellHeader,m_spPropSheetCellFilter,m_spPropSheetCellCell));
			spSheetCell->UpdateAll();
		}
	}

	//For vector
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_VECTOR)
	{
		if(auto spSheet=m_pSheet.lock()){
			std::shared_ptr<CRow> spRow;
			std::shared_ptr<CColumn> pColIndex;
			std::shared_ptr<CColumn> pColValue;	
			auto spSheetCell = std::make_shared<CPropertySheetCell<T>>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				m_spPropSheetCellHeader,
				m_spPropSheetCellFilter,
				m_spPropSheetCellCell);

			auto pChildRowHeader=std::make_shared<CChildHeaderRow>(spSheetCell.get());
			spSheetCell->SetHeaderRowPtr(pChildRowHeader);
			spSheetCell->InsertRow(-1,pChildRowHeader);
			
			pColIndex=std::make_shared<CChildPropertyIndexColumn>(spSheetCell.get());
			pColValue=std::make_shared<CChildPropertyValueColumn>(spSheetCell.get());

			spSheetCell->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColIndex);
			spSheetCell->InsertColumnImpl(CSheet::COLUMN_INDEX_MAX,pColValue);
			CCellSerializer serializer(spSheetCell,m_spPropSheetCellHeader,m_spPropSheetCellFilter,m_spPropSheetCellCell);
			for(auto& val : t){
				spRow=std::make_shared<CChildRow>(spSheetCell.get());
				spSheetCell->InsertRow(100,spRow);
				//pColProperty=pSheet->AllColumn(0);
				pColValue=spSheetCell->AllColumn(1);
				serializer.SerializeValue(val,spRow.get(),pColValue.get());
			}
			CSheet::Cell(pRow, pCol) = spSheetCell;
			spSheetCell->UpdateAll();
		}
	}
	//For shared_ptr
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_SHARED_PTR)
	{
		SerializeValue(*t,pRow,pCol);
	}

	//For save load
	template<class T>
	void SerializeValue(T& t,CRow* pRow,CColumn* pCol,ENABLE_IF_SAVE_LOAD)
	{
		if(auto spSheet=m_pSheet.lock()){
			std::shared_ptr<CSheetCell> spSheetCell(
				std::make_shared<CSheetCell>(
				spSheet.get(),
				pRow,
				pCol,
				spSheet->GetCellProperty(),
				m_spPropSheetCellHeader,
				m_spPropSheetCellFilter,
				m_spPropSheetCellCell));

			pCol->Cell(pRow)=spSheetCell;

			t.save(CCellSerializer(std::dynamic_pointer_cast<CSheet>(spSheetCell),m_spPropSheetCellHeader,m_spPropSheetCellFilter,m_spPropSheetCellCell));
			spSheetCell->UpdateAll();
		}
	}
};


class CCellDeserializer
{
private:
	std::weak_ptr<CSheet> m_pSheet;
	std::unordered_set<CCell*> m_setCellPtr;

public:
	CCellDeserializer(std::shared_ptr<CSheet> pSheet):m_pSheet(pSheet){}
	virtual ~CCellDeserializer(){}

	template<class T>
	void Deserialize(const TCHAR* lpszRootName,T& t,ENABLE_IF_SERIALIZE)
	{
		//Deserialize
		t.serialize(*this);
		m_setCellPtr.clear();
	}

	template<class T>
	void Deserialize(const TCHAR* lpszRootName,T& t,ENABLE_IF_SAVE_LOAD)
	{
		//Deserialize
		t.load(*this);
		m_setCellPtr.clear();
	}

	template<class T>
	void Deserialize(const TCHAR* lpszRootName,T* pT)
	{
		Deserialize(lpszRootName,*pT);
	}
	template<class T>
	void Deserialize(const TCHAR* lpszRootName,std::shared_ptr<T> pT)
	{
		Deserialize(lpszRootName,*pT);
	}

	template<class char_type,class T>
	void operator()(const char_type* lpszName,T& t)
	{
		if(auto spSheet=m_pSheet.lock()){
			if(!spSheet->Empty()){
				std::wstring wstrName(lpszName,(lpszName+strlen(lpszName)));
				auto pCol=spSheet->AllColumn(0);
				for(auto rowIter=spSheet->RowAllBegin(),rowEnd=spSheet->RowAllEnd();rowIter!=rowEnd;++rowIter){
					auto pCell = CSheet::Cell(rowIter->DataPtr, pCol);
					if(m_setCellPtr.find(pCell.get()) == m_setCellPtr.end() && pCol->Cell(rowIter->DataPtr.get())->GetString()==wstrName){
						m_setCellPtr.insert(pCell.get());
						DeserializeValue(t,rowIter->DataPtr.get(),spSheet->AllColumn(1).get());
						break;
					}
				}
			}
		}
	}

	//For base
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_DEFAULT)
	{
		t=boost::lexical_cast<T>(pColumn->Cell(pRow)->GetString());
	}
	//For std::string boost::lexical_cast couldn't cast std::wstring to std::string
	void DeserializeValue(std::string& t,CRow* pRow,CColumn* pColumn)
	{
		t=wstr2str(pColumn->Cell(pRow)->GetString());
	}
	//For Color
	void DeserializeValue(CColor& t,CRow* pRow,CColumn* pColumn)
	{
		if(auto p=std::dynamic_pointer_cast<CColorCell>(pColumn->Cell(pRow))){
			t=p->GetColor();
		}
	}
	//For Font
	void DeserializeValue(CFont& t,CRow* pRow,CColumn* pColumn)
	{
		if(auto p=std::dynamic_pointer_cast<CFontCell>(pColumn->Cell(pRow))){
			t=p->GetFont();
//			t.Create();
		}
	}	
	//For enum
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_ENUM)
	{
		t=static_cast<T>(boost::lexical_cast<int>(pColumn->Cell(pRow)->GetString()));
	}
	
	//For serialize
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_SERIALIZE)
	{
		if(auto pSheet=std::dynamic_pointer_cast<CSheet>(pColumn->Cell(pRow))){
			t.serialize(CCellDeserializer(pSheet));
		}
	}

	//For vector
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_VECTOR)
	{
		t.clear();
		if(auto spSheet = std::dynamic_pointer_cast<CSheet>(CSheet::Cell(pRow, pColumn))){
			auto& rowAllDict = spSheet->RowAllDictionary();
			auto& rowDict = rowAllDict.get<IndexTag>();
			for(auto rowIter=rowDict.find(0);rowIter!=rowDict.end();rowIter++){
				T::value_type val;
				DeserializeValue(val,rowIter->DataPtr.get(),spSheet->AllColumn(1).get());
				t.push_back(val);
			}
			
		}
	}
	//For shared_ptr
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_SHARED_PTR)
	{
		DeserializeValue(*t,pRow,pColumn);
	}

	//For save load
	template<class T>
	void DeserializeValue(T& t,CRow* pRow,CColumn* pColumn,ENABLE_IF_SAVE_LOAD)
	{
		if(auto pSheet=std::dynamic_pointer_cast<CSheet>(pColumn->Cell(pRow))){
			t.load(CCellDeserializer(pSheet));
		}
	}
};

#undef ENABLE_IF_DEFAULT
#undef ENABLE_IF_ENUM
#undef ENABLE_IF_SERIALIZE
#undef ENABLE_IF_VECTOR
#undef ENABLE_IF_SHARED_PTR
#undef ENABLE_IF_SAVE_LOAD

