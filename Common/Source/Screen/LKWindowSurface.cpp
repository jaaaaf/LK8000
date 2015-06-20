/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   LKWindowSurface.cpp
 * Author: Bruno de Lacheisserie
 * 
 * Created on 19 octobre 2014, 12:31
 */

#ifdef WIN32
#include <windows.h>
#else
#include "Screen/WindowCanvas.hpp"
#endif

#include "LKWindowSurface.h"
#include "LKAssert.h"

LKWindowSurface::LKWindowSurface() : LKSurface()
#ifdef WIN32
    , _hWnd() 
#endif
{

}

#ifdef WIN32
LKWindowSurface::LKWindowSurface(HWND hWnd) : LKSurface(), _hWnd() {
    LKASSERT(hWnd);
    LKASSERT(::IsWindow(hWnd));

    if(!Attach(::GetDC(hWnd))) {
        LKASSERT(false);
    }
}
#endif

LKWindowSurface::~LKWindowSurface() {
    Release();
}

LKWindowSurface::LKWindowSurface(Window& Wnd){
    Create(Wnd);
}

void LKWindowSurface::Create(Window& Wnd){
#ifdef WIN32
    HWND hWnd = Wnd.Handle();
    LKASSERT(hWnd);
    LKASSERT(::IsWindow(hWnd));

    if(!Attach(::GetDC(hWnd))) {
        LKASSERT(false);
    }
#else
    _pCanvas = new WindowCanvas(Wnd);
#endif
}

void LKWindowSurface::Release() {
#ifdef WIN32
    ReleaseTempDC();
    if(_OutputDC && _hWnd) {
        ::ReleaseDC(_hWnd, Detach());
    }
#else
    delete _pCanvas;
    _pCanvas = nullptr;
#endif
    LKSurface::Release();
}

#ifdef WIN32
LKPaintSurface::LKPaintSurface(HWND hWnd) : LKSurface(), _hWnd(hWnd) {
    LKASSERT(hWnd);
    LKASSERT(::IsWindow(hWnd));

    if (!Attach(::BeginPaint(hWnd, &_ps))) {
        LKASSERT(false);
    }
}
#endif

LKPaintSurface::~LKPaintSurface() {
    Release();
}

void LKPaintSurface::Release() {
#ifdef WIN32
    LKASSERT(_hWnd);
    Detach(); // avoid _OutputDC deleted twice and cleanup base class.
    ::EndPaint(_hWnd, &_ps);
    
    LKSurface::Release();
#endif
}
