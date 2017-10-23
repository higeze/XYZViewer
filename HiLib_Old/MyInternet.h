#include <wininet.h>
#pragma comment( lib, "wininet.lib" )

struct hinternet_deleter
{
    void operator()( HINTERNET hInternet ) const
    {
        if(hInternet){
			::InternetCloseHandle(hInternet);
		}
    }
};

enum class InternetCopyFileCallbackReason
{
	None,
	Start,
	InProgress,
	Finish
};

struct InternetCopyFileCallbackArgs
{
	InternetCopyFileCallbackArgs(DWORD totalFileSize, DWORD totalBytesTransferred, InternetCopyFileCallbackReason callbackReason)
		:TotalFileSize(totalFileSize), TotalBytesTransferred(totalBytesTransferred), CallbackReason(callbackReason){}
	DWORD TotalFileSize;
	DWORD TotalBytesTransferred;
	InternetCopyFileCallbackReason CallbackReason;

};

//struct hfile_deleter
//{
//    void operator()( HFILE hFIle ) const
//    {
//        if(hFIle){
//			::CloseHandle(hFile);
//		}
//    }
//};

typedef std::unique_ptr<std::remove_pointer<HINTERNET>::type, hinternet_deleter> InternetPtr;
//typedef std::unique_ptr<std::remove_pointer<HFILE>::type, hfile_deleter> FilePtr

std::vector<UINT> GetInternetVersion(const std::wstring& url);
void InternetCopyFile(const std::wstring& serverUrl, const std::wstring& fileUrl, const std::wstring& path, std::function<void(InternetCopyFileCallbackArgs)> callBack);