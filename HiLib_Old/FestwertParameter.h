#pragma once
#include "DcmParameter.h"
#include "MyString.h"

/**********************/
/* CFestwertParameter */
/**********************/
template <typename Iterator,typename char_type>struct CParameterParser;

class CFestwertParameter:public CDcmParameter
{
	friend struct CParameterParser<string_type::const_iterator,char_type>;
protected:
	string_type m_strWertText;//default
	string_type m_strEinW;//default
	bool m_isText;//false
public:

	CFestwertParameter(CDcm* pDcm = nullptr):CDcmParameter(pDcm),m_isText(false){}
	CFestwertParameter(CDcm* pDcm, const string_type& strName):CDcmParameter(pDcm, strName),m_isText(false){}
	virtual ~CFestwertParameter(){}
	virtual CFestwertParameter* CloneRaw()const{return new CFestwertParameter(*this);}
	std::shared_ptr<CFestwertParameter> Clone()const{return std::shared_ptr<CFestwertParameter>(CloneRaw());}

	Parameter::Type GetType()
	{
		if(m_isText){
			if(::_stricmp(GetWertText().c_str(),"true")==0 || ::_stricmp(GetWertText().c_str(),"false")==0 ){
				return Parameter::FestwertBool;
			}else{
				return Parameter::FestwertEnum;
			}
		}else{
			return Parameter::Festwert;
		}
	}
	string_type SetWertText(const string_type& strWertText);
	string_type GetWertText(){return m_strWertText;}

	string_type GetEinheitW()const{return m_strEinW;}

	void Output(std::ostream& os);
	//bool Parse(const std::string& source);

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
        static_cast<void>(version); // No use
		archive & boost::serialization::make_nvp("dcmparameter",boost::serialization::base_object<CDcmParameter>(*this));
		archive & boost::serialization::make_nvp("werttext",m_strWertText);
		archive & boost::serialization::make_nvp("einheitw",m_strEinW);
		archive & boost::serialization::make_nvp("istext",m_isText);
    }
};

BOOST_CLASS_EXPORT_KEY(CFestwertParameter);
