#include "KennfeldParameter.h"
#include "Dcm.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CKennfeldParameter);

CKennfeldParameter::string_type CKennfeldParameter::SetSTY(size_type n,string_type strValue)
{
	if(m_vstrSTY[n]!=strValue){
		m_vstrSTY[n]=strValue;
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
	return m_vstrSTY[n];
}

CKennfeldParameter::string_type CKennfeldParameter::GetSTValue(ST::Type type,size_type n)const
{
	switch(type){
	case ST::STX:
		return GetSTX(n);
		break;
	case ST::STY:
		return GetSTY(n);
		break;
	case ST::WERT:
		return GetWert(n);
		break;
	default:
		return string_type();
		break;		
	}
	return string_type();
}

CKennfeldParameter::string_type CKennfeldParameter::SetSTValue(ST::Type type,size_type n,string_type value)
{
	switch(type){
	case ST::STX:
		return SetSTX(n,value);
		break;
	case ST::STY:
		return SetSTY(n,value);
		break;
	case ST::WERT:
		return SetWert(n,value);
		break;
	default:
		return string_type();
		break;		
	}
	return string_type();
}

CKennfeldParameter::size_type CKennfeldParameter::GetSTSize(ST::Type type)const
{
	switch(type){
	case ST::STX:
		return m_vstrSTX.size();
		break;
	case ST::STY:
		return m_vstrSTY.size();
		break;
	case ST::WERT:
		return m_vstrWert.size();
		break;
	default:
		return 0;
		break;		
	}
	return 0;
}

void CKennfeldParameter::Output(std::ostream& os)
{
	os	<<"KENNFELD "<<m_strName<<" "<<m_vstrSTX.size()<<" "<<m_vstrSTY.size()<<"\r\n"
		<<"   LANGNAME \""<<m_strLang<<"\""<<"\r\n"
		<<"   EINHEIT_X \""<<m_strEinX<<"\""<<"\r\n"
		<<"   EINHEIT_Y \""<<m_strEinY<<"\""<<"\r\n"
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
	for(int iy=0,sizey=m_vstrSTY.size();iy<sizey;++iy){
		os<<"   ST/Y  "<<m_vstrSTY[iy]<<"\r\n";
		for(int i=0,size=m_vstrSTX.size();i<size;++i){//iw=iy*m_vstrSTX.size(),sizew=iw+m_vstrSTX.size();iw<sizew;++iw){
			if(i%6==0){
				os<<"   WERT";
			}
			os<<"  "<<m_vstrWert[i+iy*size];
			if((i+1)%6==0 || i+1==size){
				os<<"\r\n";
			}
		}
	}

	os	<<"END";	
}

void CKennfeldParameter::SetSTYSize(size_type n){
	if(n!=GetSTYSize()){
		m_vstrSTY.resize(n,std::string("0.0"));
		if(m_pDcm)m_pDcm->OnChanged(this);
	}
}

void CKennfeldParameter::Resize(size_type x, size_type y)
{
	size_type curXSize = GetSTXSize();
	size_type curYSize = GetSTYSize();
	size_type curZSize = GetWertSize();
	if(x!=curXSize || y!=curYSize){
		if(x>curXSize){		
			for(auto i=0;i<x-curXSize;i++){
				for(auto j = GetWertSize(); j > 0; j-=GetSTXSize()){	
					auto iter = m_vstrWert.begin();
					std::advance(iter, j);
					m_vstrWert.insert(iter,std::string("0.0"));
				}
				m_vstrSTX.push_back(std::string("0.0"));
			}
		}else if(x<curXSize){
			for(auto i=0;i<curXSize-x;i++){
				for(auto j = GetWertSize()-1; j > 0; j-=GetSTXSize()){	
					auto iter = m_vstrWert.begin();
					std::advance(iter, j);
					m_vstrWert.erase(iter);
				}
				m_vstrSTX.pop_back();
			}	
		}

		if(y>curYSize){
			m_vstrSTY.resize(y,std::string("0.0"));
			m_vstrWert.resize(GetSTXSize()*y,std::string("0.0"));//use new x size
		}else if(y<curYSize){
			m_vstrSTY.resize(y,std::string("0.0"));
			m_vstrWert.resize(GetSTXSize()*y,std::string("0.0"));
		}
		if(m_pDcm)m_pDcm->OnChanged(this);
	}

}

void CKennfeldParameter::InsertSTX(size_type n, const string_type& str)
{
	size_type stxSize = GetSTXSize();
	size_type stySize = GetSTYSize();
	for(auto i=stySize-1;i>=0;i--){
		auto iter = m_vstrWert.begin();
		std::advance(iter, i*stxSize+n);
		m_vstrWert.insert(iter, str);
	}
	{
		auto iter = m_vstrSTX.begin();
		std::advance(iter, n);
		m_vstrSTX.insert(iter, str);
	}
	if(m_pDcm)m_pDcm->OnChanged(this);
}

void CKennfeldParameter::EraseSTX(size_type n)
{
	size_type stxSize = GetSTXSize();
	size_type stySize = GetSTYSize();
	for(auto i=stySize-1;i>=0;i--){
		auto iter = m_vstrWert.begin();
		std::advance(iter, i*stxSize+n);
		m_vstrWert.erase(iter);
	}
	{
		auto iter = m_vstrSTX.begin();
		std::advance(iter, n);
		m_vstrSTX.erase(iter);
	}
	if(m_pDcm)m_pDcm->OnChanged(this);
}

void CKennfeldParameter::InsertSTY(size_type n, const string_type& str)
{
	{
		size_type stxSize = GetSTXSize();
		auto iter = m_vstrWert.begin();
		std::advance(iter, n*stxSize);
		m_vstrWert.insert(iter, stxSize, str);
	}
	{
		auto iter = m_vstrSTY.begin();
		std::advance(iter, n);
		m_vstrSTY.insert(iter, str);
	}
	if(m_pDcm)m_pDcm->OnChanged(this);
}

void CKennfeldParameter::EraseSTY(size_type n)
{
	{
		size_type stxSize = GetSTXSize();
		auto iter = m_vstrWert.begin();
		std::advance(iter, n*stxSize);
		auto endIter = m_vstrWert.begin();
		std::advance(endIter,(n+1)*stxSize);
		m_vstrWert.erase(iter, endIter);
	}
	{
		auto iter = m_vstrSTY.begin();
		std::advance(iter, n);
		m_vstrSTY.erase(iter);
	}
	if(m_pDcm)m_pDcm->OnChanged(this);
}