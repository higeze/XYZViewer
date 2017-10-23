#pragma once
#include "Band.h"
#include "SheetEnums.h"
class CCell;
class CRow;
class Sheet;

/**
 *  CColumn
 *  Base column class
 */
class CColumn:public CBand
{
private:
	static const coordinates_type MIN_WIDTH = 16;
protected:
	Sorts m_sort; /**<Indicate sort state*/
	coordinates_type m_left; /**<left position from parent sheet*/	
	coordinates_type m_width; /**<width*/
	bool m_isInit; /**<if init is set, initial width is used*/
	//bool m_isSerialized; /**< if Column is serialized, true*/

	size_type m_serializedIndex; /**<Index for old XDCM serialization.To be obsolete*/
	std::wstring m_filter; /**<Filter string*/
public:
	/**
	 *  boost::serialization
	 */
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("Band", boost::serialization::base_object<CBand>(*this));

		ar & boost::serialization::make_nvp("sort", m_sort);
		ar & boost::serialization::make_nvp("left", m_left);
		ar & boost::serialization::make_nvp("width", m_width);
		m_isInit = false;
		//m_isSerialized = true;
		ar & boost::serialization::make_nvp("filter", m_filter);
    }
public:
	/**
	 *  Constructor
	 */
	CColumn(CSheet* pSheet = nullptr)
		:CBand(pSheet),m_sort(Sorts::None),m_left(0),m_width(0),m_isInit(true)/*,m_isSerialized(false)*/{}
	/**
	 *  Destructor
	 */
	virtual ~CColumn(){}
	virtual CColumn& ShallowCopy(const CColumn& column)
	{
		CBand::ShallowCopy(column);
		m_sort =  column.m_sort;
		m_left = column.m_left;
		m_width = column.m_width;
		m_isInit = column.m_isInit;
		m_serializedIndex = column.m_serializedIndex;
		m_filter = column.m_filter;
		return *this;
	}

	virtual CColumn* CloneRaw()const{return nullptr;}
	std::shared_ptr<CColumn> Clone()const{return std::shared_ptr<CColumn>(CloneRaw());}

	virtual size_type GetSerializedIndex()const{return m_serializedIndex;} /**<Index for old XDCM serialization.To be obsolete*/
	virtual void SetSerializedIndex(size_type index){m_serializedIndex = index;} /**<Index for old XDCM serialization.To be obsolete*/
	/**
	 *  Getter for Filter
	 */
	virtual std::wstring GetFilter()const{return m_filter;}
	/**
	 *  Setter for Filter
	 */
	virtual void SetFilter(const std::wstring& filter){m_filter = filter;}
	/**
	 *  Getter for Width
	 */
	virtual coordinates_type GetWidth();
	/**
	 *  Setter for Width
	 */
	virtual void SetWidth(const coordinates_type& width);
	/**
	 *  Setter for Width without notify
	 */
	virtual void SetWidthWithoutSignal(const coordinates_type& width){m_width=max(width,MIN_WIDTH);}
	/**
	 *  Getter for Client left
	 */
	virtual coordinates_type GetLeft()const{return  m_left + Offset();}
	/**
	 *  Setter for Sheet left
	 */
	virtual void SetSheetLeft(const coordinates_type left){m_left=left;}
	/**
	 *  Setter for Sheet left without signal
	 */
	virtual void SetSheetLeftWithoutSignal(const coordinates_type left){m_left=left;}
	/**
	 *  Getter for Client right
	 */
	virtual coordinates_type GetRight(){return GetLeft() + GetWidth();}
	/**
	 *  Getter for Sort state
	 */
	virtual Sorts GetSort()const{return m_sort;};
	/**
	 *  Setter for Sort state
	 */
	virtual void SetSort(const Sorts& sort);
	/**
	 *  Setter for Visible
	 */
	virtual void SetVisible(const bool& bVisible, bool notify = true)override;
	/**
	 *  Setter for Selected
	 */
	virtual void SetSelected(const bool& bSelected)override;
	/**
	 *  Get all index from sheet
	 */
	virtual size_type GetAllIndex()const override;
	/**
	 *  Get visible index from sheet
	 */
	virtual size_type GetVisibleIndex()const override;
	/**
	 *  Paste
	 *  @param spCellDst Destination cell to be pasted
	 *  @param spCellSrc Source cell to be copied
	 *  @return true if success
	 */
	virtual bool Paste(std::shared_ptr<CCell> spCellDst, std::shared_ptr<CCell> spCellSrc){return false;}
	/**
	 *  Paste
	 *  @param spCellDst Destination cell to be pasted
	 *  @param source Source string to be copied
	 *  @return true if success
	 */
	virtual bool Paste(std::shared_ptr<CCell> spCellDst, std::wstring source){return false;}
	/**
	 *  Delete
	 *  @param spCellDst Destination cell to be deleted
	 */
	virtual void Delete(std::shared_ptr<CCell> spCellDst){}
	/**
	 *  Cell accessor
	 */
	virtual cell_type& Cell(CRow* pRow)=0;
	/**
	 *  Header Cell template
	 */
	virtual cell_type HeaderCellTemplate( CRow* pRow,  CColumn* pColumn)=0;
	/**
	 *  Filter Cell template
	 */
	virtual cell_type FilterCellTemplate( CRow* pRow,  CColumn* pColumn)=0;
	/**
	 *  Cell template
	 */
	virtual cell_type CellTemplate( CRow* pRow,  CColumn* pColumn)=0;
	/**
	 *  Insert necessary row to GridView
	 */
	virtual void InsertNecessaryRows(){};
	/**
	 *  Predicate function for sort ascending
	 */
	//virtual bool SortAscPredicate(const CCell& lhs, const CCell& rhs);
	/**
	 *  Predicate function for sort ascending
	 */
	//virtual bool SortAscPredicate(const CCell& lhs, const CCell& rhs);

};

BOOST_CLASS_EXPORT_KEY(CColumn);

class CGridView;
/**
 *  Parent column class
 *  Only GridView can hold this column
 */
class CParentColumn:public CColumn
{
protected:
public:
	/**
	 *  boost::serialization
	 */
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("Column", boost::serialization::base_object<CColumn>(*this));
    }
public:
	/**
	 *  Constructor
	 */
	CParentColumn(CGridView* pGrid = nullptr);
	/**
	 *  Destructor
	 */
	virtual ~CParentColumn(){}
	virtual CColumn& ShallowCopy(const CColumn& column)override
	{
		CColumn::ShallowCopy(column);
		return *this;
	}
	virtual CParentColumn* CloneRaw()const{return nullptr;}
	std::shared_ptr<CParentColumn> Clone()const{return std::shared_ptr<CParentColumn>(CloneRaw());}
	/**
	 *  Offset
	 *  Calculate offset from CGridView
	 */
	virtual coordinates_type Offset()const;
	/**
	 *  Header Header Cell Template
	 */
	virtual cell_type HeaderHeaderCellTemplate(CRow* pRow, CColumn* pColumn) = 0;
	/**
	 *  Name Header Cell Template
	 */
	virtual cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn) = 0;

};

BOOST_CLASS_EXPORT_KEY(CParentColumn);

class CSheetCell;
/**
 *  Child column class
 *  Only SheetCell can hold this column
 */
class CChildColumn:public CColumn
{
public:
	/**
	 *  Constructor
	 */
	CChildColumn(CSheetCell* pSheetCell);
	/**
	 *  Destructor
	 */
	virtual ~CChildColumn(){}
	/**
	 *  Offset
	 *  Calculate offset from SheetCell
	 */
	virtual coordinates_type Offset()const;
};
