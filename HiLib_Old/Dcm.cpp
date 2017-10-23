#include "Dcm.h"
#include "DcmParameter.h"
#include "FestwertParameter.h"
#include "FestwerteblockParameter.h"
#include "KennlinieParameter.h"
#include "KennfeldParameter.h"
#include "ParameterParser.h"

#include <boost/regex.hpp>
#include "MyFile.h"
#include "MyString.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CDcm);

CDcm::string_type CDcm::CreateParameterRegExString(CDcmParameter* pPara)
{
	// \s = [ \f\n\r\t\v]
    //return std::regex("^[ \f\t\v]*" + pPara->GetTypeName() + "\\s+" + pPara->GetName() + "\\s+[^]+?\\s+" + "^[ \f\t\v]*END");
    return "^[ \f\t\v]*" + pPara->GetTypeName() + "\\s+" + pPara->GetName() + "\\s+.+?\\s+" + "^[ \f\t\v]*END";
}

std::shared_ptr<CDcm> CDcm::Clone()const
{
	auto spDcm = std::make_shared<CDcm>();
	spDcm->m_strPath = m_strPath;
	spDcm->m_strHeader = m_strHeader;
	spDcm->m_strName = m_strName;
	spDcm->m_dcmString = m_dcmString;
	spDcm->m_isModified = m_isModified;
	spDcm->m_isNew = m_isNew;
	for(auto spParameter : m_paraContainer){
		auto spNewParameter = spParameter->Clone();
		spNewParameter->SetDcmPtr(spDcm.get());
		spDcm->m_paraContainer.push_back(spNewParameter);
	}
	return spDcm;
}

void CDcm::Add(const std::shared_ptr<CDcmParameter>& spPara)
{
	auto& map = m_paraContainer.get<MapTag>();
	auto iter = map.find(spPara->GetName());
	if(iter!=map.end()){
		throw std::exception("CDcm::Add");
	}else{
		spPara->SetDcmPtr(this);
		m_paraContainer.push_back(spPara);
		OnAdded(spPara.get());
	}
		
}

void CDcm::Replace(const std::shared_ptr<CDcmParameter>& spPara)
{
	//Erase
	auto& map = m_paraContainer.get<MapTag>();
	auto iter = map.find(spPara->GetName());
	if(iter!=map.end()){
		map.erase(map.find(spPara->GetName()));
		OnReplaced(spPara.get());
	}else{
		throw std::exception("CDcm::Replace");
	}
	//Add
	spPara->SetDcmPtr(this);
	m_paraContainer.push_back(spPara);
}

void CDcm::Erase(const std::shared_ptr<CDcmParameter>& spPara)
{
	auto& map = m_paraContainer.get<MapTag>();
	auto iter = map.find(spPara->GetName());
	if(iter!=map.end()){
		map.erase(map.find(spPara->GetName()));
		OnErased(spPara.get());
	}else{
		throw std::exception("CDcm::Erase");
	}
}

void CDcm::OnAdded(CDcmParameter* pNewPara)
{
	SetIsModified(true);

	std::ostringstream stream;
    stream << pNewPara;//See definition of DcmParaemter ostream	
	boost::regex re("^[ \f\t\v]*END\\s*\\z");
	if(boost::regex_search(m_dcmString, re, boost::regex_constants::match_default)){
		//Add to End
		m_dcmString = boost::regex_replace(m_dcmString, re, "END\r\n\r\n" + stream.str() + "\r\n", boost::regex_constants::match_default);
	}else{
		//No parameter then generate
		m_dcmString = Generate();
	}
}
void CDcm::OnReplaced(CDcmParameter* pNewPara)
{
	SetIsModified(true);
    boost::regex re(CreateParameterRegExString(pNewPara));
	std::ostringstream stream;
    stream << pNewPara;//See definition of DcmParaemter ostream	    
	if(boost::regex_search(m_dcmString, re, boost::regex_constants::match_default)){
		m_dcmString = boost::regex_replace(m_dcmString, re, stream.str(), boost::regex_constants::match_default);
	}else{
		throw std::exception("CDcm::OnReplace");
	}
}
void CDcm::OnErased(CDcmParameter* pNewPara)
{
	SetIsModified(true);

    boost::regex re("\r?\n?" + CreateParameterRegExString(pNewPara) + "\r?\n?");
	    
	if(boost::regex_search(m_dcmString, re, boost::regex_constants::match_default)){
		m_dcmString = boost::regex_replace(m_dcmString, re, "", boost::regex_constants::match_default);
	}else{
		throw std::exception("CDcm::OnErase");
	}
}
void CDcm::OnChanged(CDcmParameter* pNewPara)
{
	SetIsModified(true);

    boost::regex re(CreateParameterRegExString(pNewPara));
	std::ostringstream stream;
    stream << pNewPara;//See definition of DcmParaemter ostream	  
	if(boost::regex_search(m_dcmString, re, boost::regex_constants::match_default)){
		m_dcmString = boost::regex_replace(m_dcmString, re, stream.str(), boost::regex_constants::match_default);
	}else{
		throw std::exception("CDcm::OnChange");
	}
}

void CDcm::OnNameChanged()
{
	SetIsModified(true);

	if(!m_strPath.empty()){
		boost::filesystem::path path(m_strPath);
		m_strPath = path.branch_path().leaf().wstring() + L"/" + path.filename().wstring();
	}
}

void CDcm::OnHeaderChanged()
{
	SetIsModified(true);

	boost::regex re("\\A.*?^[ \f\t\v]*KONSERVIERUNG_FORMAT");
	if(boost::regex_search(m_dcmString, re, boost::regex_constants::match_default)){
		//Replace Header
		string_type strHeader = "* " + boost::algorithm::join(m_strHeader, "\r\n* ") + "\r\n";
		m_dcmString = boost::regex_replace(m_dcmString, re, strHeader + "\r\nKONSERVIERUNG_FORMAT", boost::regex_constants::match_default);
	}else{
		throw std::exception("CDcm::OnHeaderChanged");
	}
}

void CDcm::Parse(const string_type& strParse)
{
	m_dcmString = strParse;
	CParameterParser<string_type::const_iterator,char_type> parser(this);
	auto first=strParse.begin(),last=strParse.end();
	//CDcm d;

	bool b=qi::parse(first,last,parser,*((CDcm*)this));

	if(!b || first!=last){
		std::string error = std::string(first,last);
		if(error.size()>200){
			error.resize(200);
			error.append("...");
		}
		throw std::exception(("CDcm::Parse\r\n" "DCM Parse Error on\r\n\r\n" + error).c_str());

		//throw std::exception(("CDcm::Parse\r\n" "Dcm parse error on" + std::string(first,last)).c_str());
	}
}

CDcm::string_type CDcm::Generate()
{
	std::string strGenerate;
	if(m_strHeader.empty()){
		strGenerate = "KONSERVIERUNG_FORMAT 2.0\r\n\r\n";
	}else{
		string_type strHeader = "* " + boost::algorithm::join(m_strHeader, "\r\n* ") + "\r\n";
		strGenerate = strHeader+"\r\nKONSERVIERUNG_FORMAT 2.0\r\n\r\n";
	}

	using boost::spirit::karma::stream;
	using boost::spirit::karma::generate;
	//using boost::spirit::karma::eol;

	bool b=generate(
			std::back_inserter(strGenerate),
			stream % "\r\n\r\n",
			m_paraContainer
	);
	if(!b){
		throw std::exception("CDcm::Generate");
	}
	return strGenerate;
}

void CDcm::Load(const std::string& strPath)
{
	Load(str2wstr(strPath));
}

void CDcm::Load(const std::wstring& strPath)
{
	m_strPath = strPath;
	std::string strContents=CFile::ReadAllString<char>(tstring_cast<TCHAR>(wstr2str(m_strPath).c_str(),m_strPath.c_str()));
	boost::filesystem::path path(strPath);
	m_strName=path.filename().wstring();
	
	m_paraContainer.clear();
	Parse(strContents);

	SetIsModified(false);
	m_isNew = false;

}

void CDcm::LoadString(const std::wstring& strName, const std::string& strContents)
{
	m_strPath = std::wstring();
	m_strName = strName;
	
	m_paraContainer.clear();
	Parse(strContents);

	SetIsModified(false);
	m_isNew = false;

}

void CDcm::Save(const std::wstring& strPath)
{
	m_strPath = strPath;
	boost::filesystem::path path(strPath);
	m_strName = path.filename().wstring();

	CFile::WriteAllString<char>(tstring_cast<TCHAR>(wstr2str(m_strPath).c_str(),m_strPath.c_str()), m_dcmString);

	SetIsModified(false);
	m_isNew = false;
}

std::shared_ptr<CDcmParameter> CDcm::GetParameterPtr(const std::basic_string<char_type>& strName)
{
	auto& map = m_paraContainer.get<MapTag>();
	auto iter=map.find(strName);
	if(iter!=map.end()){
		return *iter;
	}else{
		return std::shared_ptr<CDcmParameter>();
	}
}

std::vector<CDcm::string_type> CDcm::GetAllNameVector()
{
	std::vector<string_type> vstr;
	auto& vec = m_paraContainer.get<VectorTag>();
	for(auto iter=vec.begin(),end=vec.end();iter!=end;++iter){
		vstr.push_back((*iter)->GetName());
	}
	return vstr;
}