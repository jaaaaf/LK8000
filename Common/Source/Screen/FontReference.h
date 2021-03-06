/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   FontReference.h
 * Author: Bruno de Lacheisserie
 *
 * Created on 7 d�cembre 2014, 19:03
 */

#ifndef FONTREFERENCE_H
#define	FONTREFERENCE_H

#ifdef USE_GDI

#include <windows.h>

typedef HFONT FontReference;

#else

class LKFont;
typedef const LKFont *FontReference;

#endif

#endif	/* FONTREFERENCE_H */

