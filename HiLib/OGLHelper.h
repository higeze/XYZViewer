#pragma once

inline void throw_if_false(bool b,const char* file,int line)
{
	if(!b){
		char message[128]={0};
		::sprintf_s(message,"file=%s,line=%d",file,line);
		throw std::exception(message);
	}
}
inline void throw_if_true(bool b,const char* file,int line)
{
	if(b){
		char message[128]={0};
		::sprintf_s(message,"file=%s,line=%d",file,line);
		throw std::exception(message);
	}
}

#define THROW_IF_FALSE(b) throw_if_false(b,__FILE__,__LINE__)

#define THROW_IF_TRUE(b) throw_if_true(b,__FILE__,__LINE__)

//UHGLRC
struct delete_glrc
{
	void operator()(HGLRC hGLRC)
	{
		if(hGLRC!=NULL){
			THROW_IF_FALSE(!!::wglDeleteContext(hGLRC));
			hGLRC = NULL;
		}
	}
};

typedef std::unique_ptr<std::remove_pointer<HGLRC>::type,delete_glrc> UHGLRC; 

//CWglMakeCurrenter
class CWglMakeCurrenter
{
private:
	bool currented_;
	CWglMakeCurrenter(const CWglMakeCurrenter&){};
	CWglMakeCurrenter operator=(const CWglMakeCurrenter&);
public:
	explicit CWglMakeCurrenter(HDC hDC,HGLRC hGLRC):currented_(false)
	{
		//THROW_IF_FALSE(::wglMakeCurrent(hDC,hGLRC));
		//If already currenterd, false, otherwise true;
		currented_ = !!::wglMakeCurrent(hDC,hGLRC);
	}
	virtual ~CWglMakeCurrenter()
	{
		if(currented_){
			//::wglMakeCurrent(NULL,NULL);
			THROW_IF_FALSE(!!::wglMakeCurrent(NULL,NULL));
		}
	}
};