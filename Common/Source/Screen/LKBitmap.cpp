/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   LKBitmap.cpp
 * Author: Bruno de Lacheisserie
 * 
 * Created on 16 octobre 2014, 20:07
 */

#include <utility>
#include "tchar.h"
#include "LKBitmap.h"

#ifndef WIN32
#include "resource_data.h"
#endif

LKBitmap::LKBitmap(LKBitmap&& Bitmap) {
#ifdef WIN32    
    bitmap = Bitmap.bitmap;
    Bitmap.bitmap = nullptr;
#elif defined(USE_MEMORY_CANVAS)
    std::swap(buffer, Bitmap.buffer);
#else
#error "Not Implemented"
#endif
}

LKBitmap::LKBitmap() {
    
}

LKBitmap::~LKBitmap() {
    Release();
}

LKBitmap& LKBitmap::operator= (LKBitmap&& Bitmap) {
#ifdef WIN32    
    std::swap(bitmap, Bitmap.bitmap);
#elif defined(USE_MEMORY_CANVAS)
    std::swap(buffer, Bitmap.buffer);
#else
#error "Not Implemented"
#endif
    return * this;
}

bool LKBitmap::LoadFromFile(const TCHAR* FilePath) {
    Reset();
#ifdef WIN32
#ifdef UNDER_CE
    bitmap = (HBITMAP) SHLoadDIBitmap(FilePath);
#else
    bitmap = (HBITMAP) LoadImage(GetModuleHandle(NULL), FilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
#endif
    return IsDefined();
#else 
    return LoadPNGFile(FilePath);
#endif
    return false;
}

#ifdef WIN32
    extern HINSTANCE _hInstance;
#endif
    
bool LKBitmap::LoadFromResource(const TCHAR* ResourceName) {
    Reset();
#ifdef WIN32
    bitmap = LoadBitmap(_hInstance, ResourceName);
    if (bitmap) {
        return true;
    }
#else
    const TCHAR* szID = ResourceName;
    TCHAR szTmp[10] = {};
    if((ptrdiff_t)ResourceName < (ptrdiff_t)std::numeric_limits<unsigned short>::max()) {
        // we have resource ID
        _stprintf(szTmp, _T("%u"), (unsigned short)(ptrdiff_t)ResourceName);        
        szID = szTmp;
    }
    if(szID) {
        return Load(GetNamedResource(szID), Type::STANDARD);
    }
#endif
    return false;
}
