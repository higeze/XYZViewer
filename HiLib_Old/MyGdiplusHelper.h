#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

namespace GdiplusHelper
{

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

BOOL SaveBitmapAsBmpFile(std::wstring wstrFilePath, HBITMAP hbm);

BOOL SaveBitmapAsPngFile(std::wstring wstrFilePath, HBITMAP hbm);

BOOL SaveBitmapAsJpegFile(std::wstring wstrFilePath, HBITMAP hbm);

BOOL SaveBitmapAsGifFile(std::wstring wstrFilePath, HBITMAP hbm);

BOOL SaveBitmapAsTiffFile(std::wstring wstrFilePath, HBITMAP hbm);
}