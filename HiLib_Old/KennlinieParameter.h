#pragma once
#include "FestwerteblockParameter.h"

#include "MyMenu.h"

/***********************/
/* CKennlinieParameter */
/***********************/

class CKennlinieParameter:public CFestwerteblockParameter
{
	friend struct CParameterParser<string_type::const_iterator,char_type>;
protected:
	std::vector<string_type> m_vstrSTX;//default
	string_type m_strEinX;//default
public:
	CKennlinieParameter(CDcm* pDcm = nullptr):CFestwerteblockParameter(pDcm){}
	CKennlinieParameter(CDcm* pDcm, const std::basic_string<char_type>& strName):CFestwerteblockParameter(pDcm, strName){}
	virtual ~CKennlinieParameter(){}
	virtual CKennlinieParameter* CloneRaw()const{return new CKennlinieParameter(*this);}
	std::shared_ptr<CKennlinieParameter> Clone()const{return std::shared_ptr<CKennlinieParameter>(CloneRaw());}

	string_type SetSTX(size_type n, string_type strValue);
	string_type GetSTX(size_type n)const
	{
		if(0<=n && n<((size_type)m_vstrSTX.size())){
			return m_vstrSTX[n];
		}else{
			return string_type();
		}
	}
	void SetSTXSize(size_type n);

	size_type GetSTXSize()const{return (size_type)m_vstrSTX.size();}

	virtual void InsertSTX(size_type n, const string_type& str);
	virtual void EraseSTX(size_type n);
	virtual string_type GetEinheitX()const{return m_strEinX;}

	virtual Parameter::Type GetType(){return Parameter::Kennlinie;}
	virtual string_type GetSTValue(ST::Type type,size_type n)const;
	virtual string_type SetSTValue(ST::Type type,size_type n,string_type value);
	virtual size_type GetSTSize(ST::Type type)const;
	virtual string_type GetExpandNameVector(){return std::basic_string<char_type>();}
	//virtual void DeleteSTX(size_type n)
	//{
	//	std::function<std::vector<string_type>::const_iterator(const std::vector<string_type>&,size_type)> getAdvancedIterator = 
	//		[](const std::vector<string_type>& vec, size_type n)->std::vector<string_type>::const_iterator{
	//			auto iter = vec.begin();
	//			std::advance(iter, n);
	//			return iter;
	//	};
	//	
	//	m_vstrSTX.erase(getAdvancedIterator(m_vstrSTX, n));
	//	m_vstrWert.erase(getAdvancedIterator(m_vstrWert, n));

	//}
	void Output(std::ostream& os);
	//bool Parse(const std::string& source);

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
        static_cast<void>(version); // No use
		archive & boost::serialization::make_nvp("festwerteblockparameter",boost::serialization::base_object<CFestwerteblockParameter>(*this));
		archive & boost::serialization::make_nvp("stx",m_vstrSTX);
		archive & boost::serialization::make_nvp("einheitw",m_strEinW);
    }

};
BOOST_CLASS_EXPORT_KEY(CKennlinieParameter);