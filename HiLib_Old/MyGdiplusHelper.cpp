#include "MyGdiplusHelper.h"

int GdiplusHelper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0, size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

BOOL GdiplusHelper::SaveBitmapAsBmpFile(std::wstring wstrFilePath, HBITMAP hbm)
{
    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GdiplusHelper::GetEncoderClsid(L"image/bmp", &clsid) >= 0)
    {
        b.Save(wstrFilePath.c_str(), &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL GdiplusHelper::SaveBitmapAsPngFile(std::wstring wstrFilePath, HBITMAP hbm)
{
    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GdiplusHelper::GetEncoderClsid(L"image/png", &clsid) >= 0)
    {
        b.Save(wstrFilePath.c_str(), &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL GdiplusHelper::SaveBitmapAsJpegFile(std::wstring wstrFilePath, HBITMAP hbm)
{
    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GdiplusHelper::GetEncoderClsid(L"image/jpeg", &clsid) >= 0)
    {
        b.Save(wstrFilePath.c_str(), &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL GdiplusHelper::SaveBitmapAsGifFile(std::wstring wstrFilePath, HBITMAP hbm)
{
    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GdiplusHelper::GetEncoderClsid(L"image/gif", &clsid) >= 0)
    {
        b.Save(wstrFilePath.c_str(), &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL GdiplusHelper::SaveBitmapAsTiffFile(std::wstring wstrFilePath, HBITMAP hbm)
{
    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GdiplusHelper::GetEncoderClsid(L"image/tiff", &clsid) >= 0)
    {
        b.Save(wstrFilePath.c_str(), &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}