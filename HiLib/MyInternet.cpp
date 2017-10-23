#include "MyInternet.h"
#include "MyString.h"
#include "MyFile.h"

std::vector<UINT> GetInternetVersion(const std::wstring& url)
{
    // Initialization
    auto net = InternetPtr(::InternetOpen( L"Version Check Program",
                           INTERNET_OPEN_TYPE_PRECONFIG,
                           NULL,
                           NULL,
                           0 ));
    if( net.get() == NULL ){
        throw std::exception("Error on InternetOpen");
	}

    // Open URL
    auto urlnet = InternetPtr(::InternetOpenUrl( net.get(),
                              url.c_str(),
                              NULL,
                              0,
                              INTERNET_FLAG_RELOAD,
                              0 ));
    if( urlnet.get() == NULL ){
        throw std::exception("Error on InternetOpen");
    }

    // Read
	std::string buffer;
    DWORD dwReadSize;
    if(!::InternetReadFile( urlnet.get(), GetBuffer(buffer,256), 256, &dwReadSize )){
		throw std::exception("Error on InternetReadFile");
	}
	ReleaseBuffer(buffer);

    // 変換
    //char m_cBuf[ 256 ];

    //const int cchWideByte = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)m_wBuf, -1, NULL, NULL );
    //LPWSTR lpw = new WCHAR[ cchWideByte ];
    //::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)m_wBuf, -1, lpw, cchWideByte );

    //const int cchMultiByte = ::WideCharToMultiByte( CP_ACP, 0, lpw, -1, NULL, 0, NULL, NULL );
    //LPSTR lpa = new CHAR[ cchMultiByte ];

    //const int nMultiCount = ::WideCharToMultiByte( CP_ACP, 0, lpw, -1, lpa, cchMultiByte, NULL, NULL );

    //::strcpy_s( m_cBuf, 256, lpa );

    //delete[] lpa;
    //delete[] lpw;

    //// 解析
    //char cVersion[ 256 ];
    //char *cTmp = NULL, *cNext;
    //cTmp = ::strtok_s( m_cBuf, ":", &cNext );
    //if( ::strcmp( cTmp, "version" ) == 0 )
    //{
    //    cTmp = ::strtok_s( NULL, "\n", &cNext );
    //    ::strcpy_s( cVersion, 256, cTmp );
    //}
	std::vector<std::string> vstr;
	std::vector<UINT> version;
    boost::algorithm::split(vstr, buffer, boost::is_any_of("."));
	std::transform( vstr.begin(), vstr.end(),
                  std::back_inserter(version),
                  ( [] (const std::string& x) -> UINT { return boost::lexical_cast<UINT>(x); } ) );

    return version;
}

void InternetCopyFile(const std::wstring& serverName, const std::wstring& objectName, const std::wstring& path, std::function<void(InternetCopyFileCallbackArgs)> callBack)
{
	if( ::InternetAttemptConnect( 0 ) != ERROR_SUCCESS ){
		throw std::exception("Error on InternetAttemptConnect");
	}

	// Initialization
	auto session = InternetPtr(::InternetOpen( L"Update Program",
								 INTERNET_OPEN_TYPE_PRECONFIG,
								 NULL,
								 NULL,
								 0 ));
	if( session.get() == NULL ){
		throw std::exception("Error on InternetOpen");
	}

	// Connect
	auto connect = InternetPtr(::InternetConnect( session.get(),
									serverName.c_str(),
									INTERNET_DEFAULT_HTTP_PORT,
									NULL,
									NULL,
									INTERNET_SERVICE_HTTP,
									0,
									NULL ));
	if( connect.get() == NULL ){
		throw std::exception("Error on InternetConnect");
	}

	//const TCHAR* const AppName = L"DcmDesigner.exe";
	//// 取得するファイル名
	//TCHAR szFile[ MAX_PATH + 1 ];
	//::swprintf_s( szFile, MAX_PATH, L"/twiki/pub/Tools/DcmDesigner/%s", AppName );

	// HTTPリクエスト
	auto http = InternetPtr(::HttpOpenRequest( connect.get(),
								 L"GET",
								 objectName.c_str(),
								 NULL,
								 NULL,
								 NULL,
								 INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION,
								 NULL ));
	if( http.get() == NULL ){
		throw std::exception("Error on HttpOpenRequest");
	}

	// HTTP Request
	if( !( ::HttpSendRequest( http.get(), NULL, 0, NULL, 0 ) ) ){
		throw std::exception("Error on HttpSendRequest");
	}

	DWORD			dwContentLen; //!< ファイルサイズ合計値
	DWORD			dwBufLen = sizeof(dwContentLen);					

	// コンテンツの長さを取得
	::HttpQueryInfo( http.get(),
					 HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
					 (LPVOID)&dwContentLen,
					 &dwBufLen,
					 NULL );

	// ファイルの作成
	CFile file( path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS ,
							FILE_ATTRIBUTE_NORMAL, NULL );
	if( (HANDLE)file == INVALID_HANDLE_VALUE ){
		throw std::exception("Error on CreateFile");
	}

	int step = 5;
	DWORD dwRead = static_cast<DWORD>(std::floor((dwContentLen)*step/100));
	DWORD dwByte = 0;
	DWORD dwTransfered = 0;
	DWORD dwWritten = 0;
	std::vector<char> buffer(dwRead);
	callBack(InternetCopyFileCallbackArgs(dwContentLen, dwTransfered, InternetCopyFileCallbackReason::Start));
	while( 1 )
	{
		::InternetReadFile( http.get(), &buffer[0], buffer.size(), &dwByte );
		if( dwByte == 0 ) break;

		// ファイルの書き込み
		file.WriteFile( &buffer[0], dwByte, &dwWritten, NULL );
		dwTransfered += dwByte;
		callBack(InternetCopyFileCallbackArgs(dwContentLen, dwTransfered, InternetCopyFileCallbackReason::InProgress));
	}

	file.FlushFileBuffers();
	callBack(InternetCopyFileCallbackArgs(dwContentLen, dwTransfered, InternetCopyFileCallbackReason::Finish));
}