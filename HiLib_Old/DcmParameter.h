#pragma once

#ifndef PARAMETER_TYPE
#define PARAMETER_TYPE
namespace Parameter{
	enum Type{
		None,
		Undefined,
		Festwert,
		FestwertEnum,
		FestwertBool,
		Kennlinie,
		Kennfeld,
		Festwerteblock
	};
}
#endif

namespace ParameterCompare{
	enum Type{
		None,
		BothNotExist,
		Same,
		DiffParam,
		LhsNotExist,
		RhsNotExist,
		DiffValue
	};
}
std::string ParameterType2String(const Parameter::Type& type, bool germanType = true);
#ifndef ST_TYPE
#define ST_TYPE
namespace ST{
	enum Type{
		None,
		STX,
		STY,
		WERT
	};
}
#endif

class CDcm;

class CDcmParameter
{
protected:
	typedef char char_type;
	typedef std::string string_type;
	typedef int size_type;

	string_type m_strName;//defalut
	string_type m_strLang;//default

	CDcm* m_pDcm;

public:
	CDcmParameter(CDcm* pDcm = nullptr):m_pDcm(pDcm){}
	CDcmParameter(CDcm* pDcm, const string_type& strName):m_pDcm(pDcm),m_strName(strName){}
	virtual ~CDcmParameter(){}
	virtual CDcmParameter* CloneRaw()const{return new CDcmParameter(*this);}
	std::shared_ptr<CDcmParameter> Clone()const{return std::shared_ptr<CDcmParameter>(CloneRaw());}


	virtual Parameter::Type GetType(){return Parameter::Undefined;}
	string_type GetTypeName(){return ParameterType2String(GetType());}

	virtual void Output(std::ostream& os){/*Do Nothing*/}

	void SetDcmPtr(CDcm* pDcm){m_pDcm = pDcm;}
	CDcm* GetDcmPtr(){return m_pDcm;}

	string_type SetName(const string_type& strName){m_strName=strName;return m_strName;}
	string_type GetName()const{return m_strName;}
	string_type SetLangName(const string_type& strLang){m_strLang=strLang;return m_strLang;}
	string_type GetLangName()const{return m_strLang;}

	//bool Parse(const std::string& source){}

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
        static_cast<void>(version); // No use
		archive & boost::serialization::make_nvp("name",m_strName);
		archive & boost::serialization::make_nvp("langname",m_strLang);
    }
};

BOOST_CLASS_EXPORT_KEY(CDcmParameter);

std::ostream& operator <<(std::ostream& os, const std::shared_ptr<CDcmParameter>& spPara);
std::ostream& operator <<(std::ostream& os, CDcmParameter* pPara);

class CDcmParameterData
{
protected:
	typedef char char_type;
	typedef std::string string_type;
	typedef int size_type;

	string_type m_strName;//defalut
	string_type m_strLang;//default
	Parameter::Type m_type;//default

public:
	CDcmParameterData(){}
	CDcmParameterData(const string_type& strName, const string_type& strLang, const Parameter::Type& type = Parameter::Undefined)
	:m_strName(strName), m_strLang(strLang), m_type(type){}

	Parameter::Type GetType()const{return m_type;}
	void SetType(Parameter::Type type){m_type = type;}

	string_type GetName()const{return m_strName;}
	void SetName(const string_type& strName){m_strName=strName;}
	string_type SetLangName(const string_type& strLang){m_strLang=strLang;return m_strLang;}
	string_type GetLangName()const{return m_strLang;}




};



