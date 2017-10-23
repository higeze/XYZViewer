#include "FestwertParameter.h"
#include "Dcm.h"
#include "MyString.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CFestwertParameter);

void CFestwertParameter::Output(std::ostream& os)
{
	os	<<"FESTWERT "<<m_strName<<"\r\n"
		<<"   LANGNAME \""<<m_strLang<<"\""<<"\r\n"
		<<"   EINHEIT_W \""<<m_strEinW<<"\""<<"\r\n";
	if(m_isText){
		os<<"   TEXT \""<<m_strWertText<<"\""<<"\r\n";
	}else{
		os<<"   WERT "<<m_strWertText<<"\r\n";
	}
	os	<<"END";	
}

CFestwertParameter::string_type CFestwertParameter::SetWertText(const string_type& strWertText)
{
	if(m_strWertText!=strWertText){
		m_strWertText=strWertText;
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
	return m_strWertText;
}

//bool CFestwertParameter::Parse(const std::string& source)
//{
//	namespace qi = boost::spirit::qi;
//	namespace karma = boost::spirit::karma;
//	namespace standard = boost::spirit::qi::standard;
//	namespace enc = boost::spirit::standard;
//	namespace phx = boost::phoenix;
//
//	using boost::spirit::qi::standard::space;
//	using boost::spirit::qi::standard::blank;		
//	using boost::spirit::qi::standard::char_;
//
//	qi::rule<std::string::const_iterator,std::basic_string<char_type>()> quoted_string;
//	qi::rule<std::string::const_iterator,std::basic_string<char_type>()> name;
//	qi::rule<std::string::const_iterator,std::basic_string<char_type>()> value;
//	qi::rule<std::string::const_iterator,std::basic_string<char_type>()> noeol;
//
//	// Rule definitions
//	quoted_string='"'>>*(~char_('"'))>>'"';
//	name=+(~space/*-'.'*/);
//	value=+(~space);
//	noeol=*(char_-qi::eol);
//	qi::rule<std::string::const_iterator> festwert;
//	festwert="FESTWERT">>+space>>name[phx::ref(m_strName) = qi::_1]>>+space
//		>>*(
//			("LANGNAME">>+space>>quoted_string[phx::ref(m_strLang) = qi::_1]>>+space)|
//			("EINHEIT_W">>+space>>quoted_string[phx::ref(m_strEinW) = qi::_1]>>+space)
//			)
//		>>*space>>(	
//			(qi::lit("WERT")>>+space>>value[phx::ref(m_strWertText) = qi::_1])|
//			(qi::lit("TEXT")>>+space>>quoted_string[phx::ref(m_strWertText) =qi::_1, phx::ref(m_isText)=true])
//			)>>+space
//		>>"END";
//	auto first = source.begin();
//	auto last = source.end();
//	bool b = qi::parse(first, last, festwert);
//
//	if(!b || first!=last){
//		return false;
//	}else{
//		return true;
//	}
//}