#include "KennlinieParameter.h"
#include "Dcm.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CKennlinieParameter);

CKennlinieParameter::string_type CKennlinieParameter::SetSTX(size_type n,string_type strValue)
{
	if(m_vstrSTX[n]!=strValue){
		m_vstrSTX[n]=strValue;
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
	return m_vstrSTX[n];
}

CKennlinieParameter::string_type CKennlinieParameter::GetSTValue(ST::Type type,size_type n)const
{
	switch(type){
		case ST::STX:
			return GetSTX(n);
			break;
		//case ST::STY:
		//	if(m_vstrSTY.size()>n){return m_vstrSTY[n];}
		//	break;
		case ST::WERT:
			return GetWert(n);				
			break;
		default:
			return std::basic_string<char_type>();
			break;		
	}
	return std::basic_string<char_type>();
}

CKennlinieParameter::string_type CKennlinieParameter::SetSTValue(ST::Type type,size_type n,string_type value)
{
	switch(type){
		case ST::STX:
			return SetSTX(n,value);
			break;
/*			case ST::STY:
			if(m_vstrSTY.size()>n){return m_vstrSTY[n]=value;}
			break;*/
		case ST::WERT:
			return SetWert(n,value);
			break;
		default:
			return std::basic_string<char_type>();
			break;		
	}
	return std::basic_string<char_type>();
}

CKennlinieParameter::size_type CKennlinieParameter::GetSTSize(ST::Type type)const
{
	switch(type){
		case ST::STX:
			return m_vstrSTX.size();
			break;
//		case ST::STY:
//			return m_vstrSTY.size();
//			break;
		case ST::WERT:
			return m_vstrWert.size();
			break;
		default:
			return 0;
			break;		
	}
	return 0;
}

void CKennlinieParameter::Output(std::ostream& os)
{
	os	<<"KENNLINIE "<<m_strName<<" "<<m_vstrSTX.size()<<"\r\n"
		<<"   LANGNAME \""<<m_strLang<<"\""<<"\r\n"
		<<"   EINHEIT_X \""<<m_strEinX<<"\""<<"\r\n"
		<<"   EINHEIT_W \""<<m_strEinW<<"\""<<"\r\n";

	for(int i=0,size=m_vstrSTX.size();i<size;++i){
		if(i%6==0){
			os<<"   ST/X";
		}
		os<<"  "<<m_vstrSTX[i];
		if((i+1)%6==0 || i+1==size){
			os<<"\r\n";
		}
	}
	for(int i=0,size=m_vstrWert.size();i<size;++i){
		if(i%6==0){
			os<<"   WERT";
		}
		os<<"  "<<m_vstrWert[i];
		if((i+1)%6==0 || i+1==size){
			os<<"\r\n";
		}
	}
	os	<<"END";	
}

void CKennlinieParameter::SetSTXSize(size_type n){
	if(n!=GetSTXSize()){
		m_vstrSTX.resize(n,std::string("0.0"));
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
}

void CKennlinieParameter::InsertSTX(size_type n, const string_type& str)
{
	{
		auto iter = m_vstrSTX.begin();
		std::advance(iter, n);
		m_vstrSTX.insert(iter, str);
	}
	{
		auto iter = m_vstrWert.begin();
		std::advance(iter, n);
		m_vstrWert.insert(iter, str);
	}

	if(m_pDcm)m_pDcm->OnChanged(this);
}

void CKennlinieParameter::EraseSTX(size_type n)
{
	{
		auto iter = m_vstrSTX.begin();
		std::advance(iter, n);
		m_vstrSTX.erase(iter);
	}
	{
		auto iter = m_vstrWert.begin();
		std::advance(iter, n);
		m_vstrWert.erase(iter);
	}
	if(m_pDcm)m_pDcm->OnChanged(this);
}

//bool CKennlinieParameter::Parse(const std::string& source)
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
//	qi::rule<std::string::const_iterator> kennlinie;
//		kennlinie=/*qi::eol>>*blank>>*/"KENNLINIE">>+space>>name[qi::_val=phx::bind(&CParameterParser<Iterator,char_type>::NewKennlinieParameterPtr,this,qi::_1)]>>+space>>qi::int_>>+space
//			>>*(
//				("LANGNAME">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strLang,qi::_val)=qi::_1]>>+space)|
//				("EINHEIT_X">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strEinX,qi::_val)=qi::_1]>>+space)|
//				("EINHEIT_W">>+space>>quoted_string[phx::bind(&CKennlinieParameter::m_strEinW,qi::_val)=qi::_1]>>+space)
//			   )
//			>>+(
//				(*space>>"ST/X">>+(+blank>>value[phx::push_back(phx::bind(&CKennlinieParameter::m_vstrSTX,qi::_val),qi::_1)])>>*blank>>qi::eol)|
//				(*space>>"WERT">>+(+blank>>value[phx::push_back(phx::bind(&CKennlinieParameter::m_vstrWert,qi::_val),qi::_1)])>>*blank>>qi::eol)
//			   )
//			>>*space>>"END";
//	auto first = source.begin();
//	auto last = source.end();
//	bool b = qi::parse(first, last, kennlinie);
//
//	if(!b || first!=last){
//		return false;
//	}else{
//		return true;
//	}
//}