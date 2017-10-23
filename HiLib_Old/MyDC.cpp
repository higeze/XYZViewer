#include "MyDC.h"

void DPtoHIMETRIC(LPSIZEL lpSizel)//For word, excel
{
	HDC hdc;
	int nWidthMM, nHeightMM, nWidthPixel, nHeightPixel;
	
	hdc = ::GetDC(NULL);
	nWidthMM = GetDeviceCaps(hdc, HORZSIZE);
	nHeightMM = GetDeviceCaps(hdc, VERTSIZE);
	nWidthPixel = GetDeviceCaps(hdc, HORZRES);
	nHeightPixel = GetDeviceCaps(hdc, VERTRES);
	::ReleaseDC(NULL, hdc);
	
	lpSizel->cx = lpSizel->cx * (nWidthMM * 100) / nWidthPixel;
	lpSizel->cy = lpSizel->cy * (nHeightMM * 100) / nHeightPixel;
}