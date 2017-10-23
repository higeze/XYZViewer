#include "FestwerteblockParameter.h"
#include "Dcm.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CFestwerteblockParameter);

CFestwerteblockParameter::string_type CFestwerteblockParameter::SetWert(size_type n,const string_type& strWert)
{
	if(m_vstrWert[n]!=strWert){
		m_vstrWert[n]=strWert;
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
	return m_vstrWert[n];
}

CFestwerteblockParameter::string_type CFestwerteblockParameter::GetSTValue(ST::Type type,size_type n)const
{
	switch(type){
		case ST::WERT:
			return GetWert(n);				
			break;
		default:
			return std::basic_string<char_type>();
			break;		
	}
}

CFestwerteblockParameter::string_type CFestwerteblockParameter::SetSTValue(ST::Type type,size_type n,string_type value)
{
	switch(type){
		case ST::WERT:
			return SetWert(n,value);
			break;
		default:
			return std::basic_string<char_type>();
			break;		
	}
}

CFestwerteblockParameter::size_type CFestwerteblockParameter::GetSTSize(ST::Type type)const
{
	switch(type){
		case ST::WERT:
			return m_vstrWert.size();
			break;
		default:
			return 0;
			break;		
	}
}
void CFestwerteblockParameter::Output(std::ostream& os)
{
	os	<<"FESTWERTEBLOCK "<<m_strName<<" "<<m_vstrWert.size()<<"\r\n"
		<<"   LANGNAME \""<<m_strLang<<"\""<<"\r\n"
		<<"   EINHEIT_W \""<<m_strEinW<<"\""<<"\r\n";
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

void CFestwerteblockParameter::SetWertSize(size_type n)
{
	if(n!=GetWertSize()){
		m_vstrWert.resize(n,std::string("0.0"));
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
}

void CFestwerteblockParameter::InsertWert(size_type n, const string_type& str)
{
	auto iter = m_vstrWert.begin();
	std::advance(iter, n);
	m_vstrWert.insert(iter, str);
	if(m_pDcm)m_pDcm->OnChanged(this);
}

void CFestwerteblockParameter::EraseWert(size_type n)
{
	auto iter = m_vstrWert.begin();
	std::advance(iter, n);
	m_vstrWert.erase(iter);
	if(m_pDcm)m_pDcm->OnChanged(this);
}

//bool CFestwerteblockParameter::Parse(const std::string& source)
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
//	qi::rule<std::string::const_iterator> festwerteblock;
//	festwerteblock="FESTWERTEBLOCK">>+space>>name[phx::ref(m_strName) = qi::_1]>>+space>>qi::int_>>+space
//		>>*(
//			("LANGNAME">>+space>>quoted_string[phx::ref(m_strLang) = qi::_1]>>+space)|
//			("EINHEIT_W">>+space>>quoted_string[phx::ref(m_strEinW) = qi::_1]>>+space)
//			)
//		>>+(*space>>(qi::lit("WERT")|qi::lit("TEXT"))>>+(+blank>>value[phx::push_back(phx::ref(m_vstrWert),qi::_1)])>>*blank>>qi::eol)
//		>>*space>>"END";
//	auto first = source.begin();
//	auto last = source.end();
//	bool b = qi::parse(first, last, festwerteblock);
//
//	if(!b || first!=last){
//		return false;
//	}else{
//		return true;
//	}
//}