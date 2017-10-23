#pragma once
class CRow;
#include "Column.h"

struct Data
{
	typedef int size_type;
	typedef size_type index_type;
	size_type Index;
	Data(size_type dispIndex = 0)
		: Index(dispIndex){}
	virtual ~Data(){}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("index", Index);
	}
};

BOOST_CLASS_EXPORT_KEY(Data);

struct RowData:public Data
{
	typedef CRow* pointer_type;
	std::shared_ptr<CRow> DataPtr;
	RowData(size_type dispIndex, std::shared_ptr<CRow> rowPtr)
		: Data(dispIndex), DataPtr(rowPtr) {}
	virtual ~RowData(){}
	const CRow* Get()const{return DataPtr.get();}
};

struct ColumnData:public Data
{
	typedef CColumn* pointer_type;
	std::shared_ptr<CColumn> DataPtr;
	ColumnData(size_type dispIndex = 0, std::shared_ptr<CColumn> columnPtr = nullptr)
		: Data(dispIndex), DataPtr(columnPtr) {}
	virtual ~ColumnData(){}
	const CColumn* Get()const{return DataPtr.get();}
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("data", boost::serialization::base_object<Data>(*this));
		ar & boost::serialization::make_nvp("dataptr", DataPtr);
	}
};

BOOST_CLASS_EXPORT_KEY(ColumnData);

struct IndexTag {};
struct PointerTag {};

typedef boost::multi_index::multi_index_container<
	RowData,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<boost::multi_index::tag<IndexTag>, boost::multi_index::member<Data, Data::size_type, &Data::Index>>,
		boost::multi_index::hashed_unique<boost::multi_index::tag<PointerTag>, boost::multi_index::const_mem_fun<RowData, const CRow*, &RowData::Get>>
	>
> RowDictionary;

typedef boost::multi_index::multi_index_container<
	ColumnData,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<boost::multi_index::tag<IndexTag>, boost::multi_index::member<Data, Data::size_type, &Data::Index>>,
		boost::multi_index::hashed_unique<boost::multi_index::tag<PointerTag>, boost::multi_index::const_mem_fun<ColumnData, const CColumn*, &ColumnData::Get>>
	>
> ColumnDictionary;
