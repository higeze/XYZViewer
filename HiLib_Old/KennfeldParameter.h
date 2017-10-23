#pragma once
#include "KennlinieParameter.h"

/**********************/
/* CKennfeldParameter */
/**********************/
template <typename Iterator,typename char_type>struct CParameterParser;

class CKennfeldParameter:public CKennlinieParameter
{
	friend struct CParameterParser<string_type::const_iterator,char_type>;

protected:
	std::vector<string_type> m_vstrSTY;//default
	string_type m_strEinY;//default
public:

	CKennfeldParameter(CDcm* pDcm = nullptr):CKennlinieParameter(pDcm){}
	CKennfeldParameter(CDcm* pDcm, const string_type& strName):CKennlinieParameter(pDcm, strName){}
	virtual ~CKennfeldParameter(){}
	virtual CKennfeldParameter* CloneRaw()const{return new CKennfeldParameter(*this);}
	std::shared_ptr<CKennfeldParameter> Clone()const{return std::shared_ptr<CKennfeldParameter>(CloneRaw());}

	string_type SetSTY(size_type n,string_type strValue);
	string_type GetSTY(size_type n)const
	{
		if(0<=n && n<((size_type)m_vstrSTY.size())){
			return m_vstrSTY[n];
		}else{
			return string_type();
		}
	}
	void SetSTYSize(size_type n);

	size_type GetSTYSize()const{return (size_type)m_vstrSTY.size();}
	virtual void InsertSTX(size_type n, const string_type& str);
	virtual void EraseSTX(size_type n);
	virtual void InsertSTY(size_type n, const string_type& str);
	virtual void EraseSTY(size_type n);

	void Resize(size_type x, size_type y);

	virtual Parameter::Type GetType(){return Parameter::Kennfeld;}

	virtual string_type GetSTValue(ST::Type type,size_type n)const;

	virtual string_type SetSTValue(ST::Type type,size_type n,string_type value);

	virtual size_type GetSTSize(ST::Type type)const;

	virtual string_type GetEinheitY()const{return m_strEinY;}

	void Output(std::ostream& os);

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
        static_cast<void>(version); // No use
		archive & boost::serialization::make_nvp("kennlinieparameter",boost::serialization::base_object<CKennlinieParameter>(*this));
		archive & boost::serialization::make_nvp("sty",m_vstrSTY);
		archive & boost::serialization::make_nvp("einheity",m_strEinY);
    }

};

BOOST_CLASS_EXPORT_KEY(CKennfeldParameter);