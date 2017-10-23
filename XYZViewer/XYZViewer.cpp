// XYZViewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#if defined _M_IX86
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
# pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "XYZViewer.h"
#include "OGLChartWnd.h"
#include "XYZVWnd.h"
#include "OGLAxis.h"
#include "OGLSeries.h"
#include "3DLineChart.h"
#include "ApplicationProperty.h"
#include "MyRect.h"
#include "SEHException.h"

HWND g_hDlgModeless=NULL;
std::shared_ptr<CApplicationProperty> g_spApplicationProperty;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//
	//auto spOGLChartProperty = std::make_shared<COGLChartProperty>();
	//auto spChart = std::make_shared<C3DLineChart>(spOGLChartProperty);
	//spChart->ClearSeries();

	//Catch SEH exception as CEH
	_set_se_translator(CSEHException::TransferSEHtoCEH);

	g_spApplicationProperty = std::make_shared<CApplicationProperty>();

	//Create Window
	CXYZVWnd* pWnd = new CXYZVWnd();
	HWND hWnd = pWnd->Create(NULL,CRect(0,0,500,500));
	pWnd->CenterWindow(NULL);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->UpdateWindow();

	//Accelerator
	//HACCEL hAccel=::LoadAccelerators(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_ACCEL_DCMWND));
	HACCEL hAccel = NULL;

	//Run
	MSG uMsg;
	BOOL bRet;
	//TCHAR className[256]={0};
	while( (bRet = GetMessage( &uMsg, NULL, 0, 0 )) != 0)
	{ 
		if (bRet == -1)
		{
			// handle the error and possibly exit
			break;
		}
		else
		{
			//Not Dialog Message, Not Accelerator for Wnd or Grid
			if ((g_hDlgModeless == (HWND) NULL || !IsDialogMessage(g_hDlgModeless, &uMsg)) && (!TranslateAccelerator(hWnd, hAccel, &uMsg))) 
			{ 
				TranslateMessage(&uMsg); 
				DispatchMessage(&uMsg); 
			}
		} 
	} 

	delete pWnd;
	return (int) uMsg.wParam;
}
