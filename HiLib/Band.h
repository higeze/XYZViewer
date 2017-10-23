#pragma once
#include "Sheet_WArchive.h"
class CSheet;
class CCell;

/**
 *  CBand
 *  Base class for Row and Column
 */
class CBand
{
public:
	typedef int size_type;
	typedef int coordinates_type;
	typedef std::shared_ptr<CCell> cell_type;
	typedef std::wstring string_type;
protected:
	CSheet* m_pSheet; /**< Parent sheet pointer*/
	bool m_bVisible; /**< Visible or not*/
	bool m_bSelected; /**< Selected or not*/
	bool m_bMeasureValid; /**< Measurement for width/height is valid or not*/
	/**
	 *  boost::serialization
	 */
    friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER();
    template <class Archive>
    void save(Archive& ar, unsigned int version)const
    {
		ar & boost::serialization::make_nvp("visible", m_bVisible);
    }
    template <class Archive>
    void load(Archive& ar, unsigned int version)
    {
		//TODO m_pSheet = static_cast<sheet_xml_wiarchive&>(ar).GetSheetPtr();

		ar & boost::serialization::make_nvp("visible", m_bVisible);
		m_bMeasureValid = true;//Width or Height are serialized
    }
public:
	/**
	 *  Constructor
	 */
	CBand(CSheet* pSheet = nullptr)
		:m_pSheet(pSheet),m_bVisible(true),m_bSelected(false),m_bMeasureValid(false){}
	/**
	 *  Destructor
	 */
	virtual ~CBand(){}
	CBand& ShallowCopy(const CBand& band)
	{
		m_pSheet = band.m_pSheet;
		m_bVisible = band.m_bVisible;
		m_bSelected = band.m_bSelected;
		m_bMeasureValid = band.m_bMeasureValid;	
		return *this;
	}
	/**
	 *  Getter for Sheet pointer
	 */
	CSheet* GetSheetPtr()const{return m_pSheet;}
	/**
	 *  Setter for Sheet pointer
	 */
	void SetSheetPtr(CSheet* pSheet){m_pSheet = pSheet;}
	/**
	 *  Getter for MeasureValid
	 */
	bool GetMeasureValid()const{return m_bMeasureValid;}
	/**
	 *  Setter for MeasureValid
	 */
	void SetMeasureValid(bool bMeasureValid){m_bMeasureValid = bMeasureValid;}
	/**
	 *  Offset from window client
	 */
	virtual coordinates_type Offset()const=0;
	/**
	 *  Getter for Visible
	 */
	virtual bool GetVisible()const{return m_bVisible;}
	/**
	 *  Setter for Visible
	 *  @param notify call propertychanged or not
	 */
	virtual void SetVisible(const bool& bVisible, bool notify = true)=0;
	/**
	 *  Getter for Selected
	 */
	virtual bool GetSelected()const{return m_bSelected;}
	/**
	 *  Setter for Selected
	 */
	virtual void SetSelected(const bool& bSelected)=0;
	/**
	 *  Get Visible index from Sheet
	 */
	virtual size_type GetVisibleIndex()const=0;
	/**
	 *  Get All index from Sheet
	 */
	virtual size_type GetAllIndex()const=0;
	/**
	 *  Dragable Trackable
	 */
	virtual bool IsDragTrackable()const{return false;}
};

BOOST_CLASS_EXPORT_KEY(CBand);
