#pragma once

#include <type_traits>
#include <utility>
#include "MyCom.h"
#include "MyMPL.h"
#include "MySerializer.h"
#include "CDataString.h"
#include "MyString.h"


class CSerializer;

#define ENABLE_IF_DEFAULT typename std::enable_if<\
									!has_serialize<T,CSerializer>::value &&\
									!has_save<T,CSerializer>::value &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0

#define ENABLE_IF_ENUM typename std::enable_if<\
									!has_serialize<T,CSerializer>::value &&\
									!has_save<T,CSerializer>::value &&\
										std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0

#define ENABLE_IF_SERIALIZE typename std::enable_if<\
										has_serialize<T,CSerializer>::value &&\
									!has_save<T,CSerializer>::value &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0

#define ENABLE_IF_VECTOR typename std::enable_if<\
									!has_serialize<T,CSerializer>::value &&\
									!has_save<T,CSerializer>::value &&\
									!std::is_enum<T>::value &&\
										is_vector<T>::value &&\
									!is_shared_ptr<T>::value\
									>::type* = 0


#define ENABLE_IF_SHARED_PTR typename std::enable_if<\
									!has_serialize<T,CSerializer>::value &&\
									!has_save<T,CSerializer>::value &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
										is_shared_ptr<T>::value \
									>::type* = 0

#define ENABLE_IF_SAVE_LOAD typename std::enable_if<\
									!has_serialize<T,CSerializer>::value &&\
										has_save<T,CSerializer>::value &&\
									!std::is_enum<T>::value &&\
									!is_vector<T>::value &&\
									!is_shared_ptr<T>::value \
									>::type* = 0

class CSerializer{
private:
	MSXML2::IXMLDOMDocumentPtr m_pDoc;
	MSXML2::IXMLDOMElementPtr m_pElem;
	CSerializer(){}

public:
	CSerializer(MSXML2::IXMLDOMDocumentPtr pDoc,
		MSXML2::IXMLDOMElementPtr pElem):m_pDoc(pDoc),m_pElem(pElem){}
	virtual ~CSerializer(){}

	template<class char_type,class T>
	void operator()(const char_type* lpszName,T& tValue)
	{
		MSXML2::IXMLDOMElementPtr pElem=m_pDoc->createElement(_bstr_t(lpszName));
		if(m_pElem){
			m_pElem->appendChild(pElem);
		}else{
			m_pDoc->appendChild(pElem);		
		}
		SerializeValue(tValue,pElem);
	}

	//For base
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_DEFAULT)
	{
		pElem->Puttext(_bstr_t(boost::lexical_cast<std::wstring>(tValue).c_str()));
	}

	//For std::string
	void SerializeValue(std::string& tValue,MSXML2::IXMLDOMElementPtr pElem)
	{
		//pElem->Puttext(_bstr_t(str2wstr(tValue).c_str()));
	}
	
	//For enum
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_ENUM)
	{
		pElem->Puttext(_bstr_t(boost::lexical_cast<std::wstring>(static_cast<int>(tValue)).c_str()));
	}
	
	//For serialize
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SERIALIZE)
	{
		tValue.serialize(CSerializer(m_pDoc,pElem));
	}

	//For CData
	void SerializeValue(CCDataString& tValue,MSXML2::IXMLDOMElementPtr pElem)
	{
		//pElem->appendChild(m_pDoc->createCDATASection(_bstr_t(tValue.c_str())));
	}

	//For vector
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_VECTOR)
	{
		//MSXML2::IXMLDOMElementPtr pCountElem=m_pDoc->createElement(_bstr_t("count"));
		//pElem->appendChild(pCountElem);
		//pCountElem->Puttext(_bstr_t(boost::lexical_cast<std::wstring>(tValue.size()).c_str()));

		for(auto iter=tValue.begin(),end=tValue.end();iter!=end;++iter){
			MSXML2::IXMLDOMElementPtr pItemElem=m_pDoc->createElement(_bstr_t("item"));
			pElem->appendChild(pItemElem);
			SerializeValue(*iter,pItemElem);
		}
	}
	//For shared_ptr
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SHARED_PTR)
	{
		SerializeValue(*tValue,pElem);
	}

	//For save load
	template<class T>
	void SerializeValue(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SAVE_LOAD)
	{
		tValue.save(CSerializer(m_pDoc,pElem));
	}
};

class CDeserializer{
private:
	MSXML2::IXMLDOMDocumentPtr m_pDoc;
	MSXML2::IXMLDOMElementPtr m_pElem;
	CDeserializer(){}

public:
	CDeserializer(MSXML2::IXMLDOMDocumentPtr pDoc,
		MSXML2::IXMLDOMElementPtr pElem):m_pDoc(pDoc),m_pElem(pElem){}
	virtual ~CDeserializer(){}

	//operator()
	template<class char_type,class T>
	void operator()(const char_type* lpszName,T& tValue)
	{
		std::vector<MSXML2::IXMLDOMElementPtr> vpElem;
		if(m_pElem){
			vpElem=GetChildElementsByTagName(m_pElem,lpszName);
		}else{
			vpElem=GetChildElementsByTagName(m_pDoc,lpszName);		
		}
		if(vpElem.size()==1){
			MSXML2::IXMLDOMElementPtr pElem=vpElem[0];
			DeserializeElement(tValue,pElem);
		}
	}
	
	//For base
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_DEFAULT)
	{
		tValue=boost::lexical_cast<T>(std::wstring(pElem->Gettext()));
	}

	//For std::string
	void DeserializeElement(std::string& tValue,MSXML2::IXMLDOMElementPtr pElem)
	{
		//tValue=wstr2str(std::wstring(pElem->Gettext()));
	}

	//For CData
	void DeserializeElement(CCDataString& tValue,MSXML2::IXMLDOMElementPtr pElem)
	{
		//tValue=CCDataString(boost::lexical_cast<std::wstring>(std::wstring(pElem->Gettext())));
	}

	//For enum
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_ENUM)
	{
		tValue=static_cast<T>(boost::lexical_cast<int>(std::wstring(pElem->Gettext())));
	}

	//For serialize
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SERIALIZE)
	{
		tValue.serialize(CDeserializer(m_pDoc,pElem));
	}

	//For vector
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_VECTOR)
	{
		//std::vector<MSXML2::IXMLDOMElementPtr> vpCountElem=GetChildElementsByTagName(pElem,"count");
		//if(vpCountElem.size()==1){
			std::vector<MSXML2::IXMLDOMElementPtr> vpItemElem=GetChildElementsByTagName(pElem,"item");
			//if(boost::lexical_cast<int>(vpCountElem[0]->Gettext())==vpItemElem.size()){
			if(!vpItemElem.empty()){
				tValue.resize(vpItemElem.size());
				for(UINT n=0,nSize=vpItemElem.size();n<nSize;++n){
					DeserializeElement(tValue[n],vpItemElem[n]);
				}
			}
			//}
		//}
	}

	//For shared_ptr
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SHARED_PTR)
	{
		if(!tValue){
			tValue=std::make_shared<T::element_type>();
		}
		DeserializeElement(*tValue,pElem);
	}

	//For save load
	template<class T>
	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SAVE_LOAD)
	{
		tValue.load(CDeserializer(m_pDoc,pElem));
	}

};

//class CDCMXMLDeserializer:public CDeserializer
//{
//public:
//	CDCMXMLDeserializer(MSXML2::IXMLDOMDocumentPtr pDoc,
//		MSXML2::IXMLDOMElementPtr pElem):CDeserializer(pDoc,pElem){}
//
//	//For shared_ptr
//	template<class T>
//	void DeserializeElement(T& tValue,MSXML2::IXMLDOMElementPtr pElem,ENABLE_IF_SHARED_PTR)
//	{
//		if(!tValue){
//			tValue=std::make_shared<T::element_type>();
//		}
//		DeserializeElement(*tValue,pElem);
//	}
//};

template<class T>
class CXMLSerializer
{
public:
	CXMLSerializer(){}
	virtual ~CXMLSerializer(){}

	template<class char_type>
	void Serialize(const char_type* lpszFilePath,
					const char_type* lpszRootName,
					T& tObj)
	{
		CCoInitializer coinit;
		MSXML2::IXMLDOMDocumentPtr pDoc;
		throw_if_failed(pDoc.CreateInstance("Msxml2.DOMDocument.6.0"));
		throw_if_failed(pDoc->put_async(VARIANT_FALSE));
	
		MSXML2::IXMLDOMProcessingInstructionPtr pProcInst=pDoc->createProcessingInstruction("xml", " version='1.0' encoding='UTF-8'");
		pDoc->appendChild(pProcInst);

		//Serialize
		CSerializer serializer(pDoc,nullptr);
		serializer(lpszRootName,tObj);	
		//Save
		throw_if_failed(pDoc->save(_variant_t(lpszFilePath)));
	}

	template<class char_type>
	void Deserialize(const char_type* lpszFilePath,
					const char_type* lpszRootName,
					T& tObj)
	{
		CCoInitializer coinit;
		MSXML2::IXMLDOMDocumentPtr pDoc;
		throw_if_failed(pDoc.CreateInstance("Msxml2.DOMDocument.6.0"));
		throw_if_failed(pDoc->put_async(VARIANT_FALSE));
		VARIANT_BOOL vtbSuccess=pDoc->load(_variant_t(lpszFilePath));
		throw_if_false(vtbSuccess==VARIANT_TRUE);

		//Deserialize
		CDeserializer deserializer(pDoc,nullptr);
		deserializer(lpszRootName,tObj);		
	}

	template<class char_type>
	MSXML2::IXMLDOMDocumentPtr Serialize(const char_type* lpszRootName, T& tObj)
	{
		CCoInitializer coinit;
		MSXML2::IXMLDOMDocumentPtr pDoc;
		throw_if_failed(pDoc.CreateInstance("Msxml2.DOMDocument.6.0"));
		throw_if_failed(pDoc->put_async(VARIANT_FALSE));
	
		MSXML2::IXMLDOMProcessingInstructionPtr pProcInst=pDoc->createProcessingInstruction("xml", " version='1.0' encoding='UTF-8'");
		pDoc->appendChild(pProcInst);

		//Serialize
		CSerializer serializer(pDoc,nullptr);
		serializer(lpszRootName,tObj);	
		//Save
		return pDoc;
	}

	//template<class char_type>
	//MSXML2::IXMLDOMElementPtr Serialize(MSXML2::IXMLDOMDocumentPtr pDoc,const char_type* lpszRootName, T& tObj)
	//{
	//	CCoInitializer coinit;
	//	MSXML2::IXMLDOMElementPtr pElem=pDoc->createElement(_bstr_t(lpszRootName));
	//	//Serialize
	//	CSerializer serializer(pDoc,pElem);
	//	serializer(lpszRootName,tObj);	
	//	//Save
	//	return pDoc;
	//}

	template<class char_type>
	void Deserialize(MSXML2::IXMLDOMDocumentPtr pDoc, const char_type* lpszRootName, T& tObj)
	{
		//Deserialize
		CDeserializer deserializer(pDoc,nullptr);
		deserializer(lpszRootName,tObj);		
	}
};

#undef ENABLE_IF_DEFAULT
#undef ENABLE_IF_ENUM
#undef ENABLE_IF_SERIALIZE
#undef ENABLE_IF_VECTOR
#undef ENABLE_IF_SHARED_PTR
#undef ENABLE_IF_SAVE_LOAD