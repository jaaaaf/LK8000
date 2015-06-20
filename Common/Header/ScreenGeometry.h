/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   PLEASE USE COMMENTS ALSO HERE TO DESCRIBE YOUR GLOBALS!
   YOU CAN INITIALIZE VALUES TO true,false,zero and NULL, 
   or you can do it also inside Globals_Init.

*/

#ifndef SCREENGEO_H
#define SCREENGEO_H

#define SCREEN_GEOMETRY_INVALID  0

//
// Each geometry has Screen0Ratio rescaled upon a single fine-tuned setting
// different among portrait and landscape orientations. This is done by
// GetScreen0Ratio() . Always remember: Screen0Ratio is a vertical scale ratio.
// Because fonts are being rescaled vertically.
//
#define SCREEN_GEOMETRY_SQUARED  1 // Unknown
#define SCREEN_GEOMETRY_43       2 // L: 640x480 (480)  P: 480x640 (640)
#define SCREEN_GEOMETRY_53       3 // L: 800x480 (480)  P: 480x800 (800)
#define SCREEN_GEOMETRY_169      4 // L: 480x272 (272)  P: 272x480 (480)
#define SCREEN_GEOMETRY_21       5 // L: 480x234 (234)  P: 234x480 (480)
//
// In case of Unknown geometry we use L: 480x272 (272)  P: 272x480 (480)
//
#define SCREEN_GEOMETRY_COUNT    6

extern unsigned short GetScreenGeometry(unsigned int x, unsigned int y);
extern double GetScreen0Ratio(void);


#endif
