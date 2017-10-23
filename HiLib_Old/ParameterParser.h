#pragma once
#include "Dcm.h"
#include "DcmParameter.h"
#include "FestwertParameter.h"
#include "FestwerteblockParameter.h"
#include "KennlinieParameter.h"
#include "KennfeldParameter.h"
#include "MyString.h"
#include <boost/phoenix/function.hpp>


//#include <boost/spirit/include/phoenix_core.hpp>
//#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/include/phoenix_stl.hpp>
//#include <boost/phoenix/bind/bind_member_variable.hpp>
namespace qi = boost::spirit::qi;
namespace karma = boost::spirit::karma;
namespace standard = boost::spirit::qi::standard;
namespace enc = boost::spirit::standard;
namespace phx = boost::phoenix;

/***********************************/
/* Use std::shared_ptr for phoenix */
/***********************************/
//namespace boost{namespace phoenix{namespace meta{
//	template<class T>
//	T declid();
//
//	template<class T>
//	struct pointed_type{typedef decltype(*declid<T>()) type;};
//}}}
//
//namespace boost{
//	template <class Pointee>
//	auto get_pointer(const Pointee& p) -> decltype(&*p){return &*p;}
//}

/********************/
/* CParameterParser */
/********************/

// lazy function for error reporting
struct ReportError {
  // the result type must be explicit for Phoenix
  template<typename>
  struct result { typedef void type; };

  // contract the string to the surrounding new-line characters
  template<typename IterA,typename IterB,typename IterC>
  void operator()(std::string& out,IterA first_iter, IterB last_iter,
                  IterC error_iter, const qi::info& what) const {
    std::string first(first_iter, error_iter);
    std::string last(error_iter, last_iter);
    auto first_pos = first.rfind('\n');
    auto last_pos = last.find('\n');
    auto error_line = ((first_pos == std::string::npos) ? first
                        : std::string(first, first_pos + 1))
                      + std::string(last, 0, last_pos);
    auto error_pos = (error_iter - first_iter) + 1;
    if (first_pos != std::string::npos) {
      error_pos -= (first_pos + 1);
    }
	std::stringstream stream;
    stream << "Parsing error in " << what << std::endl
              << error_line << std::endl
              << std::setw(error_pos) << '^'
              << std::endl;
	out = stream.str();
  }
};

const phx::function<ReportError> report_error = ReportError();

template <typename Iterator,typename char_type>
struct CParameterParser : qi::grammar<Iterator, CDcm()>
{	
	CDcm* m_pDcm;
	CParameterParser(CDcm* pDcm):CParameterParser::base_type(start),m_error("DCM Parse Error."),m_pDcm(pDcm)
	{
		using boost::spirit::qi::standard::space;
		using boost::spirit::qi::standard::blank;		
		using boost::spirit::qi::standard::char_;

		// Rule definitions
		quoted_string='"'>>*(~char_('"'))>>'"';
		//quoted_string='"'>*(~char_('"'))>'"';
		quoted_value='"'>>+(~space - char_('"'))>>'"';
		//quoted_value='"'>+(~space - char_('"'))>'"';
		name=+(~space/*-'.'*/);
		value=+(~space);
		noeol=*(char_-qi::eol);
		
		festwert="FESTWERT">>+space>>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewFestwertParameterPtr,this,qi::_1)]>>+space
			>>*(
				("LANGNAME">>+space>>quoted_string[phx::bind(&CFestwertParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_W">>+space>>quoted_string[phx::bind(&CFestwertParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
			   )
			>>*space>>(	
				(qi::lit("WERT")>>+space>>value[phx::bind(&CFestwertParameter::m_strWertText,qi::_val)=qi::_1])|
				(qi::lit("TEXT")>>+space>>quoted_string[phx::bind(&CFestwertParameter::m_strWertText,qi::_val)=qi::_1, phx::bind(&CFestwertParameter::m_isText,qi::_val)=true])
			  )>>+space
			>>"END";

		//festwert="FESTWERT">+space>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewFestwertParameterPtr,this,qi::_1)]>+space
		//	>>*(
		//		("LANGNAME">>+space>>quoted_string[phx::bind(&CFestwertParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
		//		("EINHEIT_W">>+space>>quoted_string[phx::bind(&CFestwertParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
		//	   )
		//	>*space>(	
		//		(qi::lit("WERT")>+space>value[phx::bind(&CFestwertParameter::m_strWertText,qi::_val)=qi::_1])|
		//		(qi::lit("TEXT")>+space>quoted_value[phx::bind(&CFestwertParameter::m_strWertText,qi::_val)=qi::_1, phx::bind(&CFestwertParameter::m_isText,qi::_val)=true])
		//	  )>+space
		//	>"END";

		festwerteblock="FESTWERTEBLOCK">>+space>>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewFestwerteblockParameterPtr,this,qi::_1)]>>+space>>qi::int_>>+space
			>>*(
				("LANGNAME">>+space>>quoted_string[phx::bind(&CFestwerteblockParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_W">>+space>>quoted_string[phx::bind(&CFestwerteblockParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
			   )
			>>+(*space>>(qi::lit("WERT")|qi::lit("TEXT"))>>+(+blank>>value[phx::push_back(phx::bind(&CFestwerteblockParameter::m_vstrWert,qi::_val),qi::_1)])>>*blank>>qi::eol)
			>>*space>>"END";

		kennlinie=/*qi::eol>>*blank>>*/"KENNLINIE">>+space>>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewKennlinieParameterPtr,this,qi::_1)]>>+space>>qi::int_>>+space
			>>*(
				("LANGNAME">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_X">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strEinX,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_W">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
			   )
			>>+(
				(*space>>"ST/X">>+(+blank>>value[phx::push_back(phx::bind(&CKennlinieParameter::m_vstrSTX,qi::_val),qi::_1)])>>*blank>>qi::eol)|
				(*space>>"WERT">>+(+blank>>value[phx::push_back(phx::bind(&CKennlinieParameter::m_vstrWert,qi::_val),qi::_1)])>>*blank>>qi::eol)
			   )
			>>*space>>"END";

		kennfeld=/*qi::eol>>*blank>>*/"KENNFELD">>+space>>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewKennfeldParameterPtr,this,qi::_1)]>>+space>>qi::int_>>+space>>qi::int_>>+space
			>>*(
				("LANGNAME">>+space>>quoted_string[phx::bind(&CKennfeldParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_X">>+space>>quoted_string[phx::bind(&CKennfeldParameter::m_strEinX,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_Y">>+space>>quoted_string[phx::bind(&CKennfeldParameter::m_strEinY,qi::_val)=qi::_1]>>+space)|
				("EINHEIT_W">>+space>>quoted_string[phx::bind(&CKennfeldParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
			   )
			>>+(
				(*space>>"ST/X">>+(+blank>>value[phx::push_back(phx::bind(&CKennfeldParameter::m_vstrSTX,qi::_val),qi::_1)])>>*blank>>qi::eol)|
				(*space>>"ST/Y">>+(+blank>>value[phx::push_back(phx::bind(&CKennfeldParameter::m_vstrSTY,qi::_val),qi::_1)])>>*blank>>qi::eol)|
				(*space>>"WERT">>+(+blank>>value[phx::push_back(phx::bind(&CKennfeldParameter::m_vstrWert,qi::_val),qi::_1)])>>*blank>>qi::eol)
			   )
			>>*space>>"END";

		comment=*space>>"*">>noeol[qi::_val=qi::_1]>>qi::eol;
		version="KONSERVIERUNG_FORMAT">>+space>>qi::double_>>*blank>>qi::eol;

		start=*(comment[phx::bind(&CParameterParser<Iterator,char_type>::PushComment,this,qi::_val,qi::_1)]|
				space|
				festwert[phx::bind(&CParameterParser<Iterator,char_type>::PushParameterPtr,this,qi::_val,qi::_1)]|
				kennlinie[phx::bind(&CParameterParser<Iterator,char_type>::PushParameterPtr,this,qi::_val,qi::_1)]|
				kennfeld[phx::bind(&CParameterParser<Iterator,char_type>::PushParameterPtr,this,qi::_val,qi::_1)]|
				festwerteblock[phx::bind(&CParameterParser<Iterator,char_type>::PushParameterPtr,this,qi::_val,qi::_1)]|
				version
		);
		
		//TODO This cause compile error on boost1.57 and no effect for program at this time.because I'm using >> but >. 
		
		//boost::spirit::qi::on_error<qi::fail>(
		//	start,
		//	phx::ref(m_error)=
		//	phx::construct<std::basic_string<char>>(phx::val("DCM Parse Failed! Expecting \""))
		//	+phx::bind(&boost::spirit::info::tag, qi::_4)
		//	+phx::val("\".\r\nBut got: \"") 
		//	+phx::construct<std::basic_string<char>>(qi::_3,qi::_2)
		//	+phx::construct<std::basic_string<char>>("\"")
		//);
		boost::spirit::qi::on_error<qi::fail>(
			start,
			report_error(phx::ref(m_error), qi::_1, qi::_2, qi::_3, qi::_4)
		);
	}

	virtual ~CParameterParser(){}
	std::basic_string<char> m_error;
	std::shared_ptr<CFestwertParameter> NewFestwertParameterPtr(const std::basic_string<char_type>& strName)
	{
		return std::make_shared<CFestwertParameter>(m_pDcm, strName);
	}
	std::shared_ptr<CFestwerteblockParameter> NewFestwerteblockParameterPtr(const std::basic_string<char_type>& strName)
	{
		return std::make_shared<CFestwerteblockParameter>(m_pDcm, strName);
	}
	std::shared_ptr<CKennlinieParameter> NewKennlinieParameterPtr(const std::basic_string<char_type>& strName)
	{
		return std::make_shared<CKennlinieParameter>(m_pDcm, strName);
	}
	std::shared_ptr<CKennfeldParameter> NewKennfeldParameterPtr(const std::basic_string<char_type>& strName)
	{
		return std::make_shared<CKennfeldParameter>(m_pDcm, strName);
	}
	void PushComment(CDcm& dcm,const std::basic_string<char_type>& strComment)
	{
		//std::basic_string<char_type> strName=pParam->GetName();
		//if(strName.find(TSTRING_CAST(char_type,"."))==std::basic_string<char_type>::npos){
			dcm.m_strHeader.push_back(strComment);
		//}
	}
	void PushParameterPtr(CDcm& dcm,std::shared_ptr<CDcmParameter> pParam)
	{
		//std::basic_string<char_type> strName=pParam->GetName();
		//if(strName.find(TSTRING_CAST(char_type,"."))==std::basic_string<char_type>::npos){
		auto& map = dcm.m_paraContainer.get<MapTag>();
		if(map.find(pParam->GetName())==map.end()){
			dcm.m_paraContainer.push_back(pParam);
		}else{
			throw std::exception(("CDcm::Parse\r\n" "DCM Parse Error on\r\n\r\n" + pParam->GetName() + " is duplicated.").c_str());
		}
		//}
	}

	//void InsertParameterPtr(std::unordered_map<std::basic_string<char_type>,std::shared_ptr<CDcmParameter>>& map,std::shared_ptr<CDcmParameter> pParam)
	//{
	//	std::basic_string<char_type> strName=pParam->GetName();
	//	//if(strName.find(TSTRING_CAST(char_type,"."))==std::basic_string<char_type>::npos){
	//		map.insert(std::unordered_map<std::basic_string<char_type>,std::shared_ptr<CDcmParameter>>::value_type(strName,pParam));
	//	//}
	//}

//	qi::rule<Iterator,std::vector<std::shared_ptr<CDcmParameter<char_type>>>()> start;
	qi::rule<Iterator,CDcm()> start;
	qi::rule<Iterator,std::shared_ptr<CFestwertParameter>()> festwert;
	qi::rule<Iterator,std::shared_ptr<CFestwerteblockParameter>()> festwerteblock;
	qi::rule<Iterator,std::shared_ptr<CKennlinieParameter>()> kennlinie;
	qi::rule<Iterator,std::shared_ptr<CKennfeldParameter>()> kennfeld;
	qi::rule<Iterator,std::basic_string<char_type>()> comment;
	qi::rule<Iterator,std::basic_string<char_type>()> version;
	qi::rule<Iterator,std::basic_string<char_type>()> quoted_string;
	qi::rule<Iterator,std::basic_string<char_type>()> quoted_value;
	qi::rule<Iterator,std::basic_string<char_type>()> name;
	qi::rule<Iterator,std::basic_string<char_type>()> value;
	qi::rule<Iterator,std::basic_string<char_type>()> noeol;

};
