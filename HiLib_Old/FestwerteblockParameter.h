#pragma once
#include "DcmParameter.h"

/****************************/
/* CFestwerteblockParameter */
/****************************/
template <typename Iterator,typename char_type>struct CParameterParser;

class CFestwerteblockParameter:public CDcmParameter
{
	friend struct CParameterParser<string_type::const_iterator,char_type>;
protected:
	std::vector<string_type> m_vstrWert;//default]
	string_type m_strEinW;//default
public:
	CFestwerteblockParameter(CDcm* pDcm = nullptr):CDcmParameter(pDcm){}
	CFestwerteblockParameter(CDcm* pDcm, const string_type& strName):CDcmParameter(pDcm, strName){}
	virtual ~CFestwerteblockParameter(){}
	virtual CFestwerteblockParameter* CloneRaw()const{return new CFestwerteblockParameter(*this);}
	std::shared_ptr<CFestwerteblockParameter> Clone()const{return std::shared_ptr<CFestwerteblockParameter>(CloneRaw());}

	Parameter::Type GetType(){return Parameter::Festwerteblock;}
	string_type SetWert(size_type n,const string_type& strWert);
	string_type GetWert(size_type n)const
	{
		if(0<=n && n<((size_type)m_vstrWert.size())){
			return m_vstrWert[n];
		}else{
			return string_type();
		}
	}
	virtual string_type GetSTValue(ST::Type type,size_type n)const;

	virtual string_type SetSTValue(ST::Type type,size_type n,string_type value);

	virtual size_type GetSTSize(ST::Type type)const;

	void SetWertSize(size_type n);

	size_type GetWertSize()const{return (size_type)m_vstrWert.size();}

	virtual void InsertWert(size_type n, const string_type& str);
	virtual void EraseWert(size_type n);

	virtual string_type GetEinheitW()const{return m_strEinW;}

	void Output(std::ostream& os);
	//bool Parse(const std::string& source);
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
        static_cast<void>(version); // No use
		archive & boost::serialization::make_nvp("dcmparameter",boost::serialization::base_object<CDcmParameter>(*this));
		archive & boost::serialization::make_nvp("wert",m_vstrWert);
		archive & boost::serialization::make_nvp("einheitw",m_strEinW);
    }
};

BOOST_CLASS_EXPORT_KEY(CFestwerteblockParameter);