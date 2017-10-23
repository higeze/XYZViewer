#pragma once
class CSheet;
/**
 *  sheet_xml_wiarchive
 *  this archive hold sheet pointer to pass in serialization
 */
class sheet_xml_wiarchive : public boost::archive::xml_wiarchive
{
private:
    CSheet* m_pSheet;
public:
	sheet_xml_wiarchive(std::wistream& is, CSheet* pSheet) :
        boost::archive::xml_wiarchive(is),
		m_pSheet(pSheet) {}
	CSheet* GetSheetPtr(){return m_pSheet;}
};
/**
 *  sheet_xml_woarchive
 *  this archive hold sheet pointer to pass in serialization
 */
class sheet_xml_woarchive : public boost::archive::xml_woarchive
{
private:
    CSheet* m_pSheet;
public:
    sheet_xml_woarchive(std::wostream& os, CSheet* pSheet) :
        boost::archive::xml_woarchive(os),
		m_pSheet(pSheet) {}
	CSheet* GetSheetPtr(){return m_pSheet;}
};

/**
 *  sheet_xml_wiarchive
 *  this archive hold sheet pointer to pass in serialization
 */
class sheet_bin_iarchive : public boost::archive::binary_iarchive
{
private:
    CSheet* m_pSheet;
public:
    sheet_bin_iarchive(std::istream& is, CSheet* pSheet) :
        boost::archive::binary_iarchive(is),
		m_pSheet(pSheet) {}
	CSheet* GetSheetPtr(){return m_pSheet;}
};
/**
 *  sheet_xml_woarchive
 *  this archive hold sheet pointer to pass in serialization
 */
class sheet_bin_oarchive : public boost::archive::binary_oarchive
{
private:
    CSheet* m_pSheet;
public:
    sheet_bin_oarchive(std::ostream& os, CSheet* pSheet) :
        boost::archive::binary_oarchive(os),
		m_pSheet(pSheet) {}
	CSheet* GetSheetPtr(){return m_pSheet;}
};