#pragma once

#include "DcmParameter.h"
class CDcmParameter;

#ifndef VECTORMAPTAG
	#define VECTORMAPTAG
	struct VectorTag {};
	struct MapTag {};
#endif

typedef boost::multi_index::multi_index_container<
	std::shared_ptr<CDcmParameter>,
	boost::multi_index::indexed_by<
		boost::multi_index::random_access<boost::multi_index::tag<VectorTag>>,
		boost::multi_index::hashed_unique<boost::multi_index::tag<MapTag>, boost::multi_index::const_mem_fun<CDcmParameter, std::string, &CDcmParameter::GetName>>
	>
> ParameterContainer;

template <typename Iterator,typename char_type>struct CParameterParser;

class CDcm
{
public:
	typedef char char_type;
	typedef std::string string_type;
	typedef ParameterContainer container_type;
	friend struct CParameterParser<string_type::const_iterator,char_type>;

private:
	std::wstring m_strPath;
	std::vector<string_type> m_strHeader;
	std::wstring m_strName;
	string_type m_dcmString;

	container_type m_paraContainer;
	bool m_isModified;
	bool m_isNew;

    friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER();
    template <class Archive>
    void save(Archive& ar, unsigned int version)const
    {
		ar & boost::serialization::make_nvp("name", m_strName);
		ar & boost::serialization::make_nvp("dcmstring", m_dcmString);
		//m_strPath : string()
		//m_srHeader : Serialized by LoadString
		//m_paraContainer : Serialized by LoadString
		//m_isModified : Serialized by LoadString
		//m_isNew : Serialized by LoadString
	}
    template <class Archive>
    void load(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("name", m_strName);
		ar & boost::serialization::make_nvp("dcmstring", m_dcmString);
		LoadString(m_strName, m_dcmString);
		//m_strPath : string()
		//m_srHeader : by LoadString
		//m_paraContainer : by LoadString
		//m_isModified : by LoadString
		//m_isNew : by LoadString
	}


public:
	boost::signals2::signal<void(const std::string&)> PropertyChanged;

private:
	string_type CreateParameterRegExString(CDcmParameter* pPara);

public:
	CDcm():m_strName(),m_strPath(),m_dcmString(),m_isModified(false),m_isNew(true){}
	virtual ~CDcm(){}
	std::shared_ptr<CDcm> Clone()const;
	std::wstring GetName()const{return m_strName;}
	void SetName(const std::wstring& strName, bool notify = true)
	{
		if(m_strName!=strName){
			m_strName=strName;
			if(notify){
				OnNameChanged();
			}
		}
	}

	std::wstring GetPath()const{return m_strPath;}
	void SetPath(const std::wstring& strPath){m_strPath=strPath;}

	std::vector<string_type> GetHeader()const{return m_strHeader;}
	void SetHeader(const std::vector<string_type>& strHeader, bool notify = true)
	{
		if(m_strHeader!=strHeader){
			m_strHeader=strHeader;
			if(notify){
				OnHeaderChanged();
			}
		}
	}

	string_type ToString()
	{
		if(m_dcmString.empty()){
			m_dcmString = Generate();
		}
		return m_dcmString;
	}

	bool GetIsModified()const{return m_isModified;}
	void SetIsModified(bool isModified)
	{
		if(m_isModified!=isModified){
			m_isModified = isModified;
			PropertyChanged("IsModified");
		}
	}

	bool GetIsNew()const{return m_isNew;}

	std::string GetDcmString()const{return m_dcmString;}

	void Parse(const string_type& strParse);
	string_type Generate();
	void Load(const std::string& strPath);
	void Load(const std::wstring& strPath);
	void LoadString(const std::wstring& strName, const std::string& strContents);
	void Save(const std::wstring& strPath);

	std::shared_ptr<CDcmParameter> GetParameterPtr(const std::basic_string<char_type>& strName);

	std::vector<string_type> GetAllNameVector();

	container_type& GetParameterContainer(){return m_paraContainer;}

	void Add(const std::shared_ptr<CDcmParameter>& spPara);
	void Replace(const std::shared_ptr<CDcmParameter>& spPara);
	void Erase(const std::shared_ptr<CDcmParameter>& spPara);
	bool Empty()const{return m_paraContainer.empty();}

	void OnAdded(CDcmParameter* pNewPara);
	void OnReplaced(CDcmParameter* pNewPara);
	void OnErased(CDcmParameter* pNewPara);
	void OnChanged(CDcmParameter* pNewPara);

	void OnNameChanged();
	void OnHeaderChanged();


};

BOOST_CLASS_EXPORT_KEY(CDcm);