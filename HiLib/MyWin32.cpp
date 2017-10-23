#include "MyWin32.h"
#include <Windows.h>

std::string GetLastErrorString()
{
    std::string error;
    LPVOID lpMessageBuffer = NULL ;
    DWORD dwResult = FormatMessageA(
                            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL,
                            ::GetLastError(),
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPSTR) &lpMessageBuffer,
                            0,
                            NULL);
    if ( dwResult != 0 ) {
        error = (LPCSTR)lpMessageBuffer;
        LocalFree(lpMessageBuffer);
    }
    return error;
}