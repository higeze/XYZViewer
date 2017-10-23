#pragma once
#include "Column.h"

/**
 *  CParentMapColumn
 *  This class hold cell in map<CRow, std::shared_ptr<CCell>>
 */
class CParentMapColumn:public CParentColumn
{
protected:
	std::unordered_map<CRow*,cell_type> m_mapCell; /**<Map to hold row-cell*/
	/**
	 *  boost::serialization
	 */
    friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER();
  //  template <class Archive>
  //  void serialize(Archive& ar, unsigned int version)
  //  {
		//ar & boost::serialization::make_nvp("ParentColumn", boost::serialization::base_object<CParentColumn>(*this));
  //  }
    template <class Archive>
    void save(Archive& ar, unsigned int version)const
    {
		ar & boost::serialization::make_nvp("ParentColumn", boost::serialization::base_object<CParentColumn>(*this));
    }
    template <class Archive>
    void load(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("ParentColumn", boost::serialization::base_object<CParentColumn>(*this));
		m_mapCell.clear();
    }
public:
	/**
	 *  Constructor
	 */
	CParentMapColumn(CGridView* pGrid = nullptr):CParentColumn(pGrid){}
	/**
	 *  Destructor
	 */
	virtual ~CParentMapColumn(){}
	CParentMapColumn(const CParentMapColumn& column):CParentColumn(column)
	{
		//Do not copy m_mapCell
	}
	virtual CColumn& ShallowCopy(const CColumn& column)override
	{
		CParentColumn::ShallowCopy(column);
		auto c = dynamic_cast<const CParentMapColumn&>(column);
		m_mapCell = c.m_mapCell;

		return *this;
	}
	virtual CParentMapColumn* CloneRaw()const{return new CParentMapColumn(*this);}
	std::shared_ptr<CParentMapColumn> Clone()const{return std::shared_ptr<CParentMapColumn>(CloneRaw());}
	/**
	 *  Cell accessor
	 */
	virtual cell_type& Cell(CRow* pRow )override;
	/**
	 *  Header Cell Template
	 */
	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn)override;
	/**
	 *  Header Header Cell Template
	 */
	virtual cell_type HeaderHeaderCellTemplate(CRow* pRow, CColumn* pColumn)override;
	/**
	 *  Name Header Cell Template
	 */
	virtual cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)override;
	/**
	 *  Filter Cell Template
	 */
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn)override;
	/**
	 *  Cell Template
	 */
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn)override;

	virtual void Clear(){m_mapCell.clear();}
};

BOOST_CLASS_EXPORT_KEY(CParentMapColumn);

class CParentDefaultMapColumn:public CParentMapColumn
{
protected:
	/**
	 *  boost::serialization
	 */
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("ParentMapColumn", boost::serialization::base_object<CParentMapColumn>(*this));
    }

public:
	/**
	 *  Constructor
	 */
	CParentDefaultMapColumn(CGridView* pGrid = nullptr):CParentMapColumn(pGrid){}
	/**
	 *  Destructor
	 */
	virtual ~CParentDefaultMapColumn(){}
	CParentDefaultMapColumn(const CParentDefaultMapColumn& column):CParentMapColumn(column)
	{}
	virtual CColumn& ShallowCopy(const CColumn& column)override
	{
		CParentMapColumn::ShallowCopy(column);
		return *this;
	}
	virtual CParentDefaultMapColumn* CloneRaw()const{return new CParentDefaultMapColumn(*this);}
	std::shared_ptr<CParentDefaultMapColumn> Clone()const{return std::shared_ptr<CParentDefaultMapColumn>(CloneRaw());}
	/**
	 *  Header Cell Template
	 */
	cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn)override;

};

BOOST_CLASS_EXPORT_KEY(CParentDefaultMapColumn);