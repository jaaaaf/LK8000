/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: LKDrawLook8000.cpp,v 1.11 2011/01/06 01:20:11 root Exp root $
*/

#include "externs.h"
#include "LKInterface.h"
#include "Logger.h"
#include "LKProcess.h"
#include "LKObjects.h"
#include "RGB.h"
#include "DoInits.h"
#include "McReady.h"
#include "Multimap.h"
#include "Sideview.h"
#include "Screen/FontReference.h"
#ifndef UNICODE
#include "Util/UTF8.hpp"
#endif

#ifndef WIN32
#define AUTORES 1
#endif

/*
 * Draw Text Overlay. 
 * @Surface : surface to draw
 * @rc : Rect to draw, Screen rect minus bottom bar height.
 * @bThermaBar : true if place older for Thermal bar is needed ( use return value of #DrawThermalBand in most case).
 */
void MapWindow::DrawLook8000(LKSurface& Surface,  const RECT& rc, bool bThermalBar)
{
  LKSurface::OldFont oldfont;
  LKSurface::OldBrush oldbrush;
  LKSurface::OldPen oldpen;

  SIZE TextSize;
  TCHAR Buffer[LKSIZEBUFFERLARGE];
  TCHAR BufferValue[LKSIZEBUFFERVALUE];
  TCHAR BufferUnit[LKSIZEBUFFERUNIT];
  TCHAR BufferTitle[LKSIZEBUFFERTITLE];
  int index=-1;
  double Value;
  short rcx, rcy;
  short wlen;
  bool redwarning; // 091203
  int gatechrono=0;

  FontReference bigFont, medFont;

  short leftmargin=0;

  static bool flipflop=true;
  static short flipflopcount=0;

  short tlen;
  static int ySizeLK8BigFont;
  static int ySizeLK8MediumFont;
#ifdef AUTORES
  static int ySizeLK8SmallFont;
  static int ySizeLK8TitleFont;
  static int xSizeLK8TitleFont;
#endif
  static int ySizeLK8TargetFont;
  static short tlenFullScreen;

#ifndef AUTORES
  #define ssSizeScreenSize 20
  // position of AutoMC and Safety ALtitude indicators
  static short rectLeft_AutoMc[ssSizeScreenSize];
  static short rectRight_AutoMc[ssSizeScreenSize];
  static short rectTop_AutoMc[ssSizeScreenSize];
  static short rectBottom_AutoMc[ssSizeScreenSize];
  static short writeX_AutoMc[ssSizeScreenSize];
  static short writeY_AutoMc[ssSizeScreenSize];
#endif

  static int yrightoffset;
  static short smacOffset=0; // the vertical offset for fine tuning positioning the safetyMc indicator

#ifndef AUTORES
  static unsigned short yClockposition;
#endif

  // This is going to be the START 1/3  name replacing waypoint name when gates are running
  TCHAR StartGateName[12]; // 100506
  static TCHAR StartGateNameFS[12];

  redwarning=false;

    if (INVERTCOLORS) {
        oldfont = Surface.SelectObject(LKINFOFONT);
        oldbrush = Surface.SelectObject(LKBrush_Black);
        oldpen = Surface.SelectObject(LKPen_Grey_N1);
    } else {
        oldfont = Surface.SelectObject(LKINFOFONT);
        oldbrush = Surface.SelectObject(LKBrush_White);
        oldpen = Surface.SelectObject(LKPen_Grey_N1);
    }


  if ( ++flipflopcount >2 ) {
	flipflop = !flipflop;
	flipflopcount=0;
  }

  if (OverlaySize==0) {
	bigFont=LK8BigFont;
	medFont=LK8TargetFont;
  } else {
	bigFont=LK8TargetFont;
	medFont=LK8TargetFont;
  }

  if (DoInit[MDI_DRAWLOOK8000]) {
	TCHAR Tdummy[]=_T("T");
	Surface.SelectObject(bigFont);
	Surface.GetTextSize(Tdummy, _tcslen(Tdummy), &TextSize);
	ySizeLK8BigFont = TextSize.cy;

	Surface.SelectObject(medFont);
	Surface.GetTextSize(Tdummy, _tcslen(Tdummy), &TextSize);
	ySizeLK8TargetFont = TextSize.cy;

#ifdef AUTORES
	Surface.SelectObject(LK8SmallFont);
	Surface.GetTextSize(Tdummy, _tcslen(Tdummy), &TextSize);
	ySizeLK8SmallFont = TextSize.cy;
	Surface.SelectObject(LK8TitleFont);
	Surface.GetTextSize(Tdummy, _tcslen(Tdummy), &TextSize);
	ySizeLK8TitleFont = TextSize.cy;
	xSizeLK8TitleFont = TextSize.cx;
#endif

	Surface.SelectObject(LK8MediumFont);
	Surface.GetTextSize(Tdummy, _tcslen(Tdummy), &TextSize);
	ySizeLK8MediumFont = TextSize.cy;

#ifdef AUTORES
	//
	// TIME GATES OFFSETS FOR PARAGLIDERS
	//
        if ( !ScreenLandscape ) {
            _tcscpy(StartGateNameFS,_T("ST "));
            tlenFullScreen=8;
        } else {
            _tcscpy(StartGateNameFS,_T("Start "));
            tlenFullScreen=9;
        }
#else
	if ( !ScreenLandscape ) {
		switch (ScreenSize) {			// portrait fullscreen
			case (ScreenSize_t)ss240x320:
				tlenFullScreen=8;
				// ST 1/3
				_tcscpy(StartGateNameFS,_T("ST "));
				break;
			default:
				_tcscpy(StartGateNameFS,_T("ST "));
				tlenFullScreen=8;
				break;
		}
	} else  {
		switch (ScreenSize) {			// landscape fullscreen
			case (ScreenSize_t)ss800x480:
			case (ScreenSize_t)ss400x240:
			case (ScreenSize_t)ss480x272:
			case (ScreenSize_t)ss720x408:
				// START 1/3
				_tcscpy(StartGateNameFS,_T("Start "));
				tlenFullScreen=9;
				break;
			case (ScreenSize_t)ss320x240:
				// STRT 1/3
				_tcscpy(StartGateNameFS,_T("Start "));
				tlenFullScreen=8; // 091114 reduced from 9
				break;
			case (ScreenSize_t)ss640x480:
				// STRT 1/3
				_tcscpy(StartGateNameFS,_T("Start "));
				tlenFullScreen=8;
				break;
			case (ScreenSize_t)ss896x672:
				_tcscpy(StartGateNameFS,_T("Start "));
				tlenFullScreen=9;
				break;
			default:
				_tcscpy(StartGateNameFS,_T("Start "));
				tlenFullScreen=9;
				break;
		}
	}
#endif // NOT AUTORES

#ifndef AUTORES
	short ii;
	// Set tuned positions for AutoMC indicator
	for (ii=0; ii<ssSizeScreenSize; ii++) {
	  rectLeft_AutoMc[ii]=0;
	  rectRight_AutoMc[ii]=0;
	  rectTop_AutoMc[ii]=0;
	  rectBottom_AutoMc[ii]=0;
	  writeX_AutoMc[ii]=0;
	  writeY_AutoMc[ii]=0;
	}
	//
	// Big fonts
	//
	if (OverlaySize==0) {
	  // ss240x320 OK
	  rectLeft_AutoMc[ss240x320]=231;
	  rectRight_AutoMc[ss240x320]=240;
	  rectTop_AutoMc[ss240x320]=43;
	  rectBottom_AutoMc[ss240x320]=60;
	  writeX_AutoMc[ss240x320]=239;
	  writeY_AutoMc[ss240x320]=45;

	  // ss240x400 Not yet used, because 240x400 is set as ss240x320 temporarily
	  if (ScreenSizeX==240 && ScreenSizeY==400) { // OK
	  rectLeft_AutoMc[ss240x320]=231;
	  rectRight_AutoMc[ss240x320]=240;
	  rectTop_AutoMc[ss240x320]=70;
	  rectBottom_AutoMc[ss240x320]=87;
	  writeX_AutoMc[ss240x320]=239;
	  writeY_AutoMc[ss240x320]=72;
	  }

	  // ss272x480 OK
	  rectLeft_AutoMc[ss272x480]=262;
	  rectRight_AutoMc[ss272x480]=272;
	  rectTop_AutoMc[ss272x480]=86;
	  rectBottom_AutoMc[ss272x480]=106;
	  writeX_AutoMc[ss272x480]=272;
	  writeY_AutoMc[ss272x480]=87;

	  // ss480x640 OK
	  rectLeft_AutoMc[ss480x640]=462;
	  rectRight_AutoMc[ss480x640]=480;
	  rectTop_AutoMc[ss480x640]=89;
	  rectBottom_AutoMc[ss480x640]=121;
	  writeX_AutoMc[ss480x640]=478;
	  writeY_AutoMc[ss480x640]=92;

	  // ss480x800
	  rectLeft_AutoMc[ss480x800]=462;
	  rectRight_AutoMc[ss480x800]=480;
	  rectTop_AutoMc[ss480x800]=138;
	  rectBottom_AutoMc[ss480x800]=171;
	  writeX_AutoMc[ss480x800]=478;
	  writeY_AutoMc[ss480x800]=140;

	  // ss320x240 OK
	  rectLeft_AutoMc[ss320x240]=310;
	  rectRight_AutoMc[ss320x240]=320;
	  rectTop_AutoMc[ss320x240]=53;
	  rectBottom_AutoMc[ss320x240]=72;
	  writeX_AutoMc[ss320x240]=319;
	  writeY_AutoMc[ss320x240]=54;

	  // ss400x240 OK
	  rectLeft_AutoMc[ss400x240]=390;
	  rectRight_AutoMc[ss400x240]=400;
	  rectTop_AutoMc[ss400x240]=45;
	  rectBottom_AutoMc[ss400x240]=66;
	  writeX_AutoMc[ss400x240]=400;
	  writeY_AutoMc[ss400x240]=46;

	  // ss480x234 OK
	  rectLeft_AutoMc[ss480x234]=470;
	  rectRight_AutoMc[ss480x234]=480;
	  rectTop_AutoMc[ss480x234]=42;
	  rectBottom_AutoMc[ss480x234]=64;
	  writeX_AutoMc[ss480x234]=480;
	  writeY_AutoMc[ss480x234]=43;

	  // ss480x272 OK
	  rectLeft_AutoMc[ss480x272]=469;
	  rectRight_AutoMc[ss480x272]=480;
	  rectTop_AutoMc[ss480x272]=42;
	  rectBottom_AutoMc[ss480x272]=67;
	  writeX_AutoMc[ss480x272]=480;
	  writeY_AutoMc[ss480x272]=44;

	  // ss640x480 OK
	  rectLeft_AutoMc[ss640x480]=620;
	  rectRight_AutoMc[ss640x480]=640;
	  rectTop_AutoMc[ss640x480]=108;
	  rectBottom_AutoMc[ss640x480]=144;
	  writeX_AutoMc[ss640x480]=639;
	  writeY_AutoMc[ss640x480]=108;

	  // ss800x480 OK
	  rectLeft_AutoMc[ss800x480]=781;
	  rectRight_AutoMc[ss800x480]=800;
	  rectTop_AutoMc[ss800x480]=95;
	  rectBottom_AutoMc[ss800x480]=rectTop_AutoMc[ss800x480]+39; // 134
	  writeX_AutoMc[ss800x480]=800;
	  writeY_AutoMc[ss800x480]=95;

	  // ss896x672 OK
	  rectLeft_AutoMc[ss896x672]=871;
	  rectRight_AutoMc[ss896x672]=896;
	  rectTop_AutoMc[ss896x672]=153;
	  rectBottom_AutoMc[ss896x672]=200;
	  writeX_AutoMc[ss896x672]=895;
	  writeY_AutoMc[ss896x672]=155;

	  // custom 854x358, preliminar
	  if (ScreenSizeX==854 && ScreenSizeY==358) { 
	  rectLeft_AutoMc[ss480x272]=843;
	  rectRight_AutoMc[ss480x272]=854;
	  rectTop_AutoMc[ss480x272]=80;
	  rectBottom_AutoMc[ss480x272]=105;
	  writeX_AutoMc[ss480x272]=854;
	  writeY_AutoMc[ss480x272]=82;
	  }

	} else {
	//
	// Small fonts
	//

	  // ss240x320
	  rectLeft_AutoMc[ss240x320]=230;
	  rectRight_AutoMc[ss240x320]=240;
	  rectTop_AutoMc[ss240x320]=56;
	  rectBottom_AutoMc[ss240x320]=72;
	  writeX_AutoMc[ss240x320]=240;
	  writeY_AutoMc[ss240x320]=56;
	
	  // ss240x400 Not yet used, because 240x400 is set as ss240x320 temporarily
	  if (ScreenSizeX==240 && ScreenSizeY==400) {
	  rectLeft_AutoMc[ss240x320]=230;
	  rectRight_AutoMc[ss240x320]=240;
	  rectTop_AutoMc[ss240x320]=83;
	  rectBottom_AutoMc[ss240x320]=99;
	  writeX_AutoMc[ss240x320]=240;
	  writeY_AutoMc[ss240x320]=83;
	  }

	  // ss272x480
	  rectLeft_AutoMc[ss272x480]=261;
	  rectRight_AutoMc[ss272x480]=272;
	  rectTop_AutoMc[ss272x480]=98;
	  rectBottom_AutoMc[ss272x480]=118;
	  writeX_AutoMc[ss272x480]=272;
	  writeY_AutoMc[ss272x480]=98;

	  // ss480x640
	  rectLeft_AutoMc[ss480x640]=460;
	  rectRight_AutoMc[ss480x640]=480;
	  rectTop_AutoMc[ss480x640]=109;
	  rectBottom_AutoMc[ss480x640]=141;
	  writeX_AutoMc[ss480x640]=480;
	  writeY_AutoMc[ss480x640]=109;

	  // ss480x800
	  rectLeft_AutoMc[ss480x800]=460;
	  rectRight_AutoMc[ss480x800]=480;
	  rectTop_AutoMc[ss480x800]=162;
	  rectBottom_AutoMc[ss480x800]=194;
	  writeX_AutoMc[ss480x800]=480;
	  writeY_AutoMc[ss480x800]=162;

	  // ss320x240
	  rectLeft_AutoMc[ss320x240]=310;
	  rectRight_AutoMc[ss320x240]=320;
	  rectTop_AutoMc[ss320x240]=66;
	  rectBottom_AutoMc[ss320x240]=82;
	  writeX_AutoMc[ss320x240]=320;
	  writeY_AutoMc[ss320x240]=66;

	  // ss400x240
	  rectLeft_AutoMc[ss400x240]=390;
	  rectRight_AutoMc[ss400x240]=400;
	  rectTop_AutoMc[ss400x240]=59;
	  rectBottom_AutoMc[ss400x240]=80;
	  writeX_AutoMc[ss400x240]=400;
	  writeY_AutoMc[ss400x240]=59;

	  // ss480x234
	  rectLeft_AutoMc[ss480x234]=470;
	  rectRight_AutoMc[ss480x234]=480;
	  rectTop_AutoMc[ss480x234]=57;
	  rectBottom_AutoMc[ss480x234]=81;
	  writeX_AutoMc[ss480x234]=480;
	  writeY_AutoMc[ss480x234]=57;

	  // ss480x272
	  rectLeft_AutoMc[ss480x272]=469;
	  rectRight_AutoMc[ss480x272]=480;
	  rectTop_AutoMc[ss480x272]=67;
	  rectBottom_AutoMc[ss480x272]=93;
	  writeX_AutoMc[ss480x272]=480;
	  writeY_AutoMc[ss480x272]=67;

	  // ss640x480
	  rectLeft_AutoMc[ss640x480]=620;
	  rectRight_AutoMc[ss640x480]=640;
	  rectTop_AutoMc[ss640x480]=136;
	  rectBottom_AutoMc[ss640x480]=168;
	  writeX_AutoMc[ss640x480]=640;
	  writeY_AutoMc[ss640x480]=136;

	  // ss800x480
	  rectLeft_AutoMc[ss800x480]=780;
	  rectRight_AutoMc[ss800x480]=800;
	  rectTop_AutoMc[ss800x480]=122;
	  rectBottom_AutoMc[ss800x480]=163;
	  writeX_AutoMc[ss800x480]=800;
	  writeY_AutoMc[ss800x480]=122;

	  // ss896x672
	  rectLeft_AutoMc[ss896x672]=869;
	  rectRight_AutoMc[ss896x672]=896;
	  rectTop_AutoMc[ss896x672]=196;
	  rectBottom_AutoMc[ss896x672]=240;
	  writeX_AutoMc[ss896x672]=896;
	  writeY_AutoMc[ss896x672]=196;

	  // custom 854x358, preliminar
	  if (ScreenSizeX==854 && ScreenSizeY==358) { 
	  rectLeft_AutoMc[ss480x272]=843;
	  rectRight_AutoMc[ss480x272]=854;
	  rectTop_AutoMc[ss480x272]=106;
	  rectBottom_AutoMc[ss480x272]=131;
	  writeX_AutoMc[ss480x272]=854;
	  writeY_AutoMc[ss480x272]=108;
	  }
	}
#endif // NOT AUTORES

  	if (ScreenLandscape)
		yrightoffset=((rc.bottom + rc.top)/2)-NIBLSCALE(10);
	  else
		yrightoffset=((rc.bottom + rc.top)/3)-NIBLSCALE(10);

#ifdef AUTORES 
	//
	// Calculate the position of the SAFETY MACCREADY indicator
	//
	if (OverlaySize==0) { // BIG FONT overlays
            if (ScreenLandscape) {
                /* 
                   Currently we draw the safety MC indicator over the MC value, except
                   for the 1.76 ratio (480x272) which requires fine tuning due to low res.
                   Even if we have a low res in 480x272 we keep big fonts, at the cost of 
                   using the space BELOW mv value to draw the mc safety. Since we cannot 
                   split bits, and make the safety indicator smaller, this is a good choice.

                   Fot information, common ratios are:
                    PDA  640x480 1.33  
                    PNA  800x480 1.66 
                    PNA  480x272 1.76 
                    KOBO 800x600 1.33

                */
                if ( (ScreenSizeX / (double)ScreenSizeY) > 1.70) { 
                    // draw sMc below MC
                    smacOffset = yrightoffset -(ySizeLK8BigFont)- (ySizeLK8BigFont/6);
                } else {
                    // draw sMc on top of MC
                    smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6);
                }
            } else {
                smacOffset = yrightoffset -(ySizeLK8BigFont*2) - ySizeLK8SmallFont;
            }
        } else {
            if (ScreenLandscape) {
                smacOffset = yrightoffset -(ySizeLK8BigFont*2) - ySizeLK8SmallFont -NIBLSCALE(2);
            } else {
                smacOffset = yrightoffset -(ySizeLK8BigFont*2) - ySizeLK8SmallFont;
            }
        }
#else

	// goto only to keep easy indentation.
	if (OverlaySize!=0) goto smalloverlays;

	//
	// Big overlay fonts
	//
	switch (ScreenSize) {
		case ss320x240:
		case ss640x480:
		case ss400x240:
		case ss272x480:
		case ss480x800:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(1);
			break;
		case ss240x320:
		case ss240x400:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(2);
			break;
		case ss800x480:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)+NIBLSCALE(2);
			break;
		case ss480x234:
		case ss896x672:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)+NIBLSCALE(1);
			break;
		case ss480x272:
			smacOffset = yrightoffset -(ySizeLK8BigFont)- (ySizeLK8BigFont/6)+NIBLSCALE(1);
			break;
		default:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6);
			break;
	}
	goto nextinit;

smalloverlays:
	//
	// Small overlay fonts
	//
	switch (ScreenSize) {
		case ss320x240:
		case ss640x480:
		case ss400x240:
		case ss480x800:
		case ss480x234:
		case ss480x272:
		case ss896x672:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(5);
			break;
		case ss240x320:
		case ss272x480:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(6);
			break;
		case ss800x480:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(2);
			break;
		default:
			smacOffset = yrightoffset -(ySizeLK8BigFont*2) - (ySizeLK8BigFont/6)-NIBLSCALE(5);
			break;
	}

nextinit:
#endif // no autores for safety maccready indicator

#ifndef AUTORES
	// Clock overlay position for portrait modes only: unfortunately, clock in this case will be positioned
	// in different places, depending on screen resolution. If possible, on top right as usual.
	// Otherwise, after right side overlays, slightly lower than them.
	switch (ScreenSize) {
		case ss272x480:
		case ss480x800:
		case ss240x400:
			yClockposition=yrightoffset - ySizeLK8BigFont- (ySizeLK8MediumFont*4);
			break;
		case ss480x640:
			yClockposition=yrightoffset +(ySizeLK8BigFont*3);
                        break;
		default:
			yClockposition=yrightoffset +(ySizeLK8BigFont*3);
			break;
	}
	// ss240x400 Not yet used, because 240x400 is set as ss240x320 temporarily
	if (ScreenSizeX==240 && ScreenSizeY==400)
		yClockposition=yrightoffset - ySizeLK8BigFont- (ySizeLK8MediumFont*4);
#endif

	DoInit[MDI_DRAWLOOK8000]=false; 

  } // end doinit

  LKColor overcolor,distcolor;
  overcolor=OverColorRef;
  distcolor=OverColorRef;

  tlen=tlenFullScreen;
  _tcscpy(StartGateName,StartGateNameFS);


  // First we draw flight related values such as instant efficiency, altitude, new infoboxes etc.

  if (IsMultimapOverlaysGauges() && (LKVarioBar && !mode.AnyPan()) ) {
	leftmargin=(LKVarioSize+NIBLSCALE(3)); // VARIOWIDTH + middle separator right extension
	tlen-=2; // 091115
	
  } else {
	leftmargin=0;
  }

  // no overlay - but we are still drawing MC and the wind on bottom left!
  if ( Look8000 == (Look8000_t)lxcNoOverlay ) goto drawOverlay;

  // PRINT WP TARGET NAME
  if ( ISPARAGLIDER && UseGates() && ActiveWayPoint==0) {
	// if running a task, use the task index normally
	if ( ValidTaskPoint(ActiveWayPoint) != false ) {
		if (DoOptimizeRoute())
			index = RESWP_OPTIMIZED;
		else
			index = Task[ActiveWayPoint].Index;
	} else
		index=-1;
  } else {
	index = GetOvertargetIndex();
  }
 
		if (bThermalBar) { 
      		rcx=rc.left+leftmargin+NIBLSCALE(40);
  		} else {
   			rcx=rc.left+leftmargin+NIBLSCALE(1);
 	    }
		rcy=rc.top+NIBLSCALE(1);
			
		// Waypoint name and distance
		Surface.SelectObject(medFont);

	if ( index >=0 ) {
		#if 0
		// Active colours
		if (WayPointList[index].Reachable) {
			TextDisplayMode.AsFlag.Color = TEXTGREEN; 
		} else {
			TextDisplayMode.AsFlag.Color = TEXTRED;
		}
		#endif
		// OVERTARGET reswp not using redwarning because Reachable is not calculated
		if (index<=RESWP_END)
			redwarning=false;
		else {
			if (WayPointCalc[index].AltArriv[AltArrivMode]>0 && !WayPointList[index].Reachable ) 
				redwarning=true;
			else	
				redwarning=false;
		}

		int gateinuse=-2;
        if(UseGates() && ActiveWayPoint==0) {
            gateinuse=ActiveGate;
        }
		if (flipflop && (gateinuse != -2) ) {
			if (!HaveGates()) {
				gateinuse=-1;
			} else {
				// this is set here for the first time, when havegates
				gatechrono=GateTime(ActiveGate)-LocalTime();
			}
			if (gateinuse<0) {
	// LKTOKEN  _@M157_ = "CLOSED" 
				_tcscpy(Buffer,MsgToken(157));
			} else {
				_stprintf(Buffer,_T("%s%d/%d"),StartGateName,gateinuse+1,PGNumberOfGates);
			}

	 		LKWriteText(Surface, Buffer, rcx+NIBLSCALE(2), rcy,0, WTMODE_OUTLINED, WTALIGN_LEFT, overcolor, true);
		} else {
			TCHAR buffername[LKSIZEBUFFERLARGE];
			GetOvertargetName(buffername);
			wlen=_tcslen(buffername);
 			if (wlen>tlen) {
 			 	LK_tcsncpy(Buffer, buffername, tlen);
			} else {
 			 	LK_tcsncpy(Buffer, buffername, wlen);
			}

 			 CharUpper(Buffer);
			 LKWriteText(Surface, Buffer, rcx+NIBLSCALE(2), rcy,0, WTMODE_OUTLINED, WTALIGN_LEFT, overcolor, true);
		}

		 if (gateinuse>=-1 ) {
			// if we are still painting , it means we did not start yet..so we use colors
			if (CorrectSide() ) {
				distcolor=overcolor;
			} else {
				distcolor=RGB_AMBER;
			}
		 	LKFormatValue(LK_START_DIST, false, BufferValue, BufferUnit, BufferTitle);
		 } else {
			switch (OvertargetMode) {
				case OVT_TASK:
					// Using FormatDist will give PGs 3 decimal units on overlay only
					// because changing FormatValue to 3 digits would bring them also
					// on bottom bar, and there is no space for 1.234km on the bottom bar.
					if (ACTIVE_WP_IS_AAT_AREA || DoOptimizeRoute() )
		 				LKFormatDist(RESWP_OPTIMIZED, true, BufferValue, BufferUnit);
					else 
		 				LKFormatDist(index, false, BufferValue, BufferUnit);
					break;
                case OVT_TASKCENTER:
		 			LKFormatDist(index, false, BufferValue, BufferUnit);
                    break;
				case OVT_BALT:
		 			LKFormatDist(BestAlternate, false, BufferValue, BufferUnit);
					break;
				case OVT_ALT1:
		 			LKFormatDist(Alternate1, false, BufferValue, BufferUnit);
					break;
				case OVT_ALT2:
		 			LKFormatDist(Alternate2, false, BufferValue, BufferUnit);
					break;
				case OVT_HOME:
		 			LKFormatDist(HomeWaypoint, false, BufferValue, BufferUnit);
					break;
				case OVT_THER:
		 			LKFormatDist(RESWP_LASTTHERMAL, true, BufferValue, BufferUnit);
					break;
				case OVT_MATE:
		 			LKFormatDist(RESWP_TEAMMATE, true, BufferValue, BufferUnit);
					break;
				case OVT_FLARM:
		 			LKFormatDist(RESWP_FLARMTARGET, true, BufferValue, BufferUnit);
					break;
				default:
		 			LKFormatValue(LK_NEXT_DIST, false, BufferValue, BufferUnit, BufferTitle);
					break;
			}
		}

		if ( (!OverlayClock || Look8000==lxcStandard) && ScreenLandscape && (!(ISPARAGLIDER && UseGates())) ) {
                  _stprintf(BufferValue + _tcslen(BufferValue), _T(" %s"),BufferUnit);
			Surface.SelectObject(medFont);
			LKWriteText(Surface,  BufferValue, rc.right-NIBLSCALE(30),rc.top+NIBLSCALE(1), 0, WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);
		} else
			LKWriteText(Surface, BufferValue, rcx+NIBLSCALE(2), rcy+ ySizeLK8TargetFont, 0, WTMODE_OUTLINED, WTALIGN_LEFT, distcolor, true);

 		Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
		if (!HideUnits) {
			Surface.SelectObject(LKMAPFONT); // TODO FIX BUG here.. using different font from size
			if ( (!OverlayClock || Look8000==lxcStandard) && ScreenLandscape && !(ISPARAGLIDER && UseGates())) {

			} else {
			 LKWriteText(Surface,  BufferUnit, rcx+NIBLSCALE(4)+TextSize.cx,rcy+ySizeLK8TargetFont+(ySizeLK8TargetFont/3)-NIBLSCALE(1), 0, WTMODE_OUTLINED, WTALIGN_LEFT, overcolor, true);
			}
		}

		// DIFF Bearing value displayed only when not circling
	  	// if (!MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING)) {
			switch (OvertargetMode) {
				case OVT_TASK:
					// Do not use FormatBrgDiff for TASK, could be AAT!
//		 			LKFormatValue(LK_BRGDIFF, false, BufferValue, BufferUnit, BufferTitle);
		 			LKFormatBrgDiff(index, false, BufferValue, BufferUnit);
					break;
				case OVT_TASKCENTER:
		 			LKFormatBrgDiff(index, false, BufferValue, BufferUnit);
					break;
				case OVT_BALT:
		 			LKFormatBrgDiff(BestAlternate, false, BufferValue, BufferUnit);
					break;
				case OVT_ALT1:
		 			LKFormatBrgDiff(Alternate1, false, BufferValue, BufferUnit);
					break;
				case OVT_ALT2:
		 			LKFormatBrgDiff(Alternate2, false, BufferValue, BufferUnit);
					break;
				case OVT_HOME:
		 			LKFormatBrgDiff(HomeWaypoint, false, BufferValue, BufferUnit);
					break;
				case OVT_THER:
		 			LKFormatBrgDiff(RESWP_LASTTHERMAL, true, BufferValue, BufferUnit);
					break;
				case OVT_MATE:
		 			LKFormatBrgDiff(RESWP_TEAMMATE, true, BufferValue, BufferUnit);
					break;
				case OVT_FLARM:
		 			LKFormatBrgDiff(RESWP_FLARMTARGET, true, BufferValue, BufferUnit);
					break;
				default:
		 			LKFormatValue(LK_BRGDIFF, false, BufferValue, BufferUnit, BufferTitle);
					break;
			}

            SIZE tsize;
            TCHAR Tmp[150];
            
#ifdef UNICODE
            int len = _tcslen(BufferValue);
            if(len > 0) {
                const TCHAR* last = &BufferValue[len-1];
#else
            size_t len = LengthUTF8(BufferValue);
            if(len > 0) {
                const char* last = BufferValue;
                std::pair<unsigned, const char *> next = NextUTF8(last);
                while(next.second && *(next.second)) {
                    last = next.second;
                    next = NextUTF8(last);
                }
#endif
                Surface.GetTextSize(last, _tcslen(last), &tsize);
                tsize.cx = -tsize.cx;
            } else {
                tsize.cx = 0;
            }
            _stprintf(Tmp, TEXT("%s"), gettext(_T("_@M2185_")));
            if(BufferValue[0] == Tmp[0])
            {
                Surface.GetTextSize(Tmp, _tcslen(Tmp), &tsize);
            }
            _stprintf(Tmp, TEXT("%s"), gettext(_T("_@M2182_")));
            if(BufferValue[0] == Tmp[0])
            {
                Surface.GetTextSize(Tmp, _tcslen(Tmp), &tsize);
            }
            _stprintf(Tmp, TEXT("%s"), gettext(_T("_@M2184_")));
            if(BufferValue[0] == Tmp[0])
            {
                Surface.GetTextSize(Tmp, _tcslen(Tmp), &tsize);
                _stprintf(Tmp, TEXT("%s"), gettext(_T("_@M2185_")));
        //    if(BufferValue[len-1] == Tmp[0])
        //    tsize.cx =0;
            }



			Surface.SelectObject(medFont);
			if (!ISGAAIRCRAFT) {
				if (ScreenLandscape)
					LKWriteText(Surface,  BufferValue, (rc.right+rc.left)/2-tsize.cx, rc.top+ NIBLSCALE(15), 0,
						WTMODE_OUTLINED, WTALIGN_CENTER, overcolor, true);
				else
					LKWriteText(Surface,  BufferValue, ((rc.right+rc.left)/3)*2-tsize.cx/2, rc.top+ NIBLSCALE(15), 0,
						WTMODE_OUTLINED, WTALIGN_CENTER, overcolor, true);
			}

		// } // only when circling

		// Draw efficiency required and altitude arrival for destination waypoint
		// For paragliders, average efficiency and arrival destination

		Surface.SelectObject(bigFont); // use this font for big values

		if ( ISGLIDER) {
			switch (OvertargetMode) {
				case OVT_TASK:
		 			LKFormatValue(LK_NEXT_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_TASKCENTER:
		 			LKFormatValue(LK_NEXT_CENTER_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_BALT:
		 			LKFormatValue(LK_BESTALTERN_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_ALT1:
		 			LKFormatValue(LK_ALTERN1_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_ALT2:
		 			LKFormatValue(LK_ALTERN2_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_HOME:
		 			LKFormatGR(HomeWaypoint, false, BufferValue, BufferUnit);
					break;
				case OVT_THER:
		 			LKFormatGR(RESWP_LASTTHERMAL, true, BufferValue, BufferUnit);
					break;
				case OVT_MATE:
		 			LKFormatGR(RESWP_TEAMMATE, true, BufferValue, BufferUnit);
					break;
				case OVT_FLARM:
		 			LKFormatGR(RESWP_FLARMTARGET, true, BufferValue, BufferUnit);
					break;
				default:
		 			LKFormatValue(LK_NEXT_GR, false, BufferValue, BufferUnit, BufferTitle);
					break;
			}

			Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
			//	rcy=(rc.bottom + rc.top)/2 -TextSize.cy-NIBLSCALE(10); OLD
			rcy=yrightoffset -TextSize.cy; // 101112
			rcx=rc.right-NIBLSCALE(10);
			if (redwarning)  // 091203
				LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, RGB_AMBER, true);
			else
				LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, overcolor, true);

			// Altitude difference with current MC
			switch (OvertargetMode) {
				case OVT_TASK:
		 			LKFormatValue(LK_NEXT_ALTDIFF, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_TASKCENTER:
		 			LKFormatValue(LK_NEXT_CENTER_ALTDIFF, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_BALT:
		 			LKFormatValue(LK_BESTALTERN_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_ALT1:
		 			LKFormatValue(LK_ALTERN1_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_ALT2:
		 			LKFormatValue(LK_ALTERN2_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
					break;
				case OVT_HOME:
		 			LKFormatAltDiff(HomeWaypoint, false, BufferValue, BufferUnit);
					break;
				case OVT_THER:
		 			LKFormatAltDiff(RESWP_LASTTHERMAL, true, BufferValue, BufferUnit);
					break;
				case OVT_MATE:
		 			LKFormatAltDiff(RESWP_TEAMMATE, true, BufferValue, BufferUnit);
					break;
				case OVT_FLARM:
		 			LKFormatAltDiff(RESWP_FLARMTARGET, true, BufferValue, BufferUnit);
					break;
				default:
		 			LKFormatValue(LK_NEXT_ALTDIFF, false, BufferValue, BufferUnit,BufferTitle);
					break;
			}
			if (redwarning) 
				LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0,
					WTMODE_OUTLINED,WTALIGN_RIGHT,RGB_AMBER, true);
			else
				LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0,
					WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);

			//
			// SAFETY ALTITUDE INDICATOR (NOT FOR PARAGLIDERS)
			// For PGs there is a separate drawing, although it should be identical right now.
			//
			if (IsSafetyAltitudeInUse(index)) {
				Surface.SelectObject(LK8SmallFont);
				_stprintf(BufferValue,_T(" + %.0f %s "),SAFETYALTITUDEARRIVAL/10*ALTITUDEMODIFY,
				Units::GetUnitName(Units::GetUserAltitudeUnit()));
#ifdef AUTORES
				LKWriteBoxedText(Surface, rc,BufferValue, rcx,rcy+(TextSize.cy*2)- TextSize.cy/6 - (ySizeLK8SmallFont/4), 0, WTALIGN_RIGHT, RGB_WHITE,RGB_BLACK);
#else
				LKWriteBoxedText(Surface, rc,BufferValue, rcx,rcy+(TextSize.cy*2)-TextSize.cy/6, 0, WTALIGN_RIGHT, RGB_WHITE,RGB_BLACK);
#endif
			}
		}

	} // index>0
	// no valid index for current overmode, but we print something nevertheless
	else {
		TCHAR buffername[LKSIZEBUFFERLARGE];
		GetOvertargetName(buffername);
		wlen=_tcslen(buffername);
	 	if (wlen>tlen) {
	 	 	LK_tcsncpy(Buffer, buffername, tlen);
		} else {
	 	 	LK_tcsncpy(Buffer, buffername, wlen);
		}
 		CharUpper(Buffer);
		LKWriteText(Surface, Buffer, rcx+NIBLSCALE(2), rcy,0, WTMODE_OUTLINED, WTALIGN_LEFT, overcolor, true);
	}

  // moved out from task paragliders stuff - this is painted on the right
  if ( ISPARAGLIDER ) {

	if (UseGates()&&ActiveWayPoint==0) {
		Surface.SelectObject(bigFont); // use this font for big values

		if (HaveGates()) {
            gatechrono=GateTime(ActiveGate)-LocalTime(); // not always already set, update it ... 

			Units::TimeToTextDown(BufferValue,gatechrono ); 
			rcx=rc.right-NIBLSCALE(10);
			Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
			rcy=yrightoffset -TextSize.cy; // 101112
			LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, overcolor, true);

			Value=WayPointCalc[DoOptimizeRoute()?RESWP_OPTIMIZED:Task[0].Index].NextETE-gatechrono;
			Units::TimeToTextDown(BufferValue, (int)Value);
			if (Value<=0) 
				LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_RIGHT,RGB_AMBER, true);
			else
				LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);
		}

	} else {
		Surface.SelectObject(bigFont); // use this font for big values
	  	if (MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING))
			LKFormatValue(LK_TC_30S, false, BufferValue, BufferUnit, BufferTitle);
		else
			LKFormatValue(LK_LD_AVR, false, BufferValue, BufferUnit, BufferTitle);

		Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
		rcy=yrightoffset -TextSize.cy; // 101112

		rcx=rc.right-NIBLSCALE(10);
		LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, overcolor, true);

		// Altitude difference with current MC
		switch (OvertargetMode) {
			case OVT_TASK:
	 			LKFormatValue(LK_NEXT_ALTDIFF, false, BufferValue, BufferUnit, BufferTitle);
				break;
			case OVT_TASKCENTER:
	 			LKFormatValue(LK_NEXT_CENTER_ALTDIFF, false, BufferValue, BufferUnit, BufferTitle);
				break;
			case OVT_BALT:
	 			LKFormatValue(LK_BESTALTERN_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
				break;
			case OVT_ALT1:
	 			LKFormatValue(LK_ALTERN1_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
				break;
			case OVT_ALT2:
	 			LKFormatValue(LK_ALTERN2_ARRIV, false, BufferValue, BufferUnit, BufferTitle);
				break;
			case OVT_HOME:
				LKFormatAltDiff(HomeWaypoint, false, BufferValue, BufferUnit);
				break;
			case OVT_THER:
				LKFormatAltDiff(RESWP_LASTTHERMAL, true, BufferValue, BufferUnit);
				break;
			case OVT_MATE:
				LKFormatAltDiff(RESWP_TEAMMATE, true, BufferValue, BufferUnit);
				break;
			case OVT_FLARM:
		 		LKFormatAltDiff(RESWP_FLARMTARGET, true, BufferValue, BufferUnit);
				break;
			default:
	 			LKFormatValue(LK_NEXT_ALTDIFF, false, BufferValue, BufferUnit, BufferTitle);
				break;
		}
		if (redwarning)
			LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_RIGHT,RGB_AMBER, true);
		else
			LKWriteText(Surface,  BufferValue, rcx,rcy+TextSize.cy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);

                //
                // SAFETY ALTITUDE INDICATOR (FOR PARAGLIDERS)
                // Should be identical to that for other aircrafts, normally.
                //
		if (IsSafetyAltitudeInUse(GetOvertargetIndex())) {
			Surface.SelectObject(LK8SmallFont);
			_stprintf(BufferValue,_T(" + %.0f %s "),SAFETYALTITUDEARRIVAL/10*ALTITUDEMODIFY,
			Units::GetUnitName(Units::GetUserAltitudeUnit()));
#ifdef AUTORES
			LKWriteBoxedText(Surface, rc,BufferValue, rcx,rcy+(TextSize.cy*2)- TextSize.cy/6 - (ySizeLK8SmallFont/4), 0, WTALIGN_RIGHT, RGB_WHITE,RGB_BLACK);
#else
			LKWriteBoxedText(Surface, rc,BufferValue, rcx,rcy+(TextSize.cy*2)-TextSize.cy/6, 0, WTALIGN_RIGHT, RGB_WHITE,RGB_BLACK);
#endif

		}
	} // end no UseGates()
  } // is paraglider

drawOverlay:
  // In place of MC, print gate time
  // Even if lxcNoOverlay, we print startgates..
  if (UseGates()&&ActiveWayPoint==0) {
	Surface.SelectObject(LK8MediumFont);

	if (HaveGates()) {
		Units::TimeToText(BufferTitle,GateTime(ActiveGate)); 
		_stprintf(BufferValue,_T("START %s"),BufferTitle);
	} else {
	// LKTOKEN  _@M316_ = "GATES CLOSED" 
		_tcscpy(BufferValue,MsgToken(316));
	}
	rcy=yrightoffset -ySizeLK8BigFont-(ySizeLK8MediumFont*2); // 101112
	rcx=rc.right-NIBLSCALE(10);
	LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, overcolor, true);

	// USE THIS SPACE FOR MESSAGES TO THE PILOT
	rcy+=ySizeLK8MediumFont;
	if (HaveGates()) {
         if(!DerivedDrawInfo.Flying) {
            // LKTOKEN  _@M922_ = "NOT FLYING"
            _tcscpy(BufferValue,MsgToken(922));
         } else {
            if (gatechrono>0) {
                // IsInSector works reversed!
                if (PGStartOut && DerivedDrawInfo.IsInSector) {
                    // LKTOKEN  _@M923_ = "WRONG inSIDE"
                    _tcscpy(BufferValue,MsgToken(923));
                } else if (!PGStartOut && !DerivedDrawInfo.IsInSector) {
                    // LKTOKEN  _@M924_ = "WRONG outSIDE"
                    _tcscpy(BufferValue,MsgToken(924));
                } else {
                    // LKTOKEN  _@M921_ = "countdown"
                    _tcscpy(BufferValue,MsgToken(921));
                }
			} else {
                BufferValue[0] = _T('\0');
                // gate is open
                int CloseTime = GateCloseTime();
                if ( flipflopcount > 0) {
                    if( (ActiveGate<(PGNumberOfGates-1) || CloseTime < 86340) && flipflopcount == 1) {
                        if (CloseTime < 86340)  {
                            Units::TimeToText(BufferTitle,CloseTime); 
                            _stprintf(BufferValue,_T("CLOSE %s"),BufferTitle);
                        } else {
                            Units::TimeToText(BufferTitle,GateTime(ActiveGate+1)); 
                            _stprintf(BufferValue,_T("NEXT %s"),BufferTitle);
                        } 
                    } else {
                        // IsInSector works reversed!
                        if (PGStartOut && DerivedDrawInfo.IsInSector) {
                            // LKTOKEN  _@M923_ = "WRONG inSIDE"
                            _tcscpy(BufferValue,MsgToken(923));
                        } else if (!PGStartOut && !DerivedDrawInfo.IsInSector) {
                            // LKTOKEN  _@M924_ = "WRONG outSIDE"
                            _tcscpy(BufferValue,MsgToken(924));
                        }
                    }
                } 
                if(BufferValue[0] == _T('\0')) {
                    // LKTOKEN  _@M314_ = "GATE OPEN" 
                    _tcscpy(BufferValue,MsgToken(314));
                }
            }
		}
	} else {
		// LKTOKEN  _@M925_ = "NO TSK START"
		_tcscpy(BufferValue,MsgToken(925));
	}
	LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, distcolor, true);

  } else
  if (McOverlay && Look8000>lxcNoOverlay && (ISGLIDER || ISPARAGLIDER)) {

	Surface.SelectObject(bigFont);
	LKFormatValue(LK_MC, false, BufferValue, BufferUnit, BufferTitle);
	// rcy=(rc.bottom + rc.top)/2 -ySizeLK8BigFont-NIBLSCALE(10)-ySizeLK8BigFont; VERTICAL CENTERED
	rcy=yrightoffset -ySizeLK8BigFont-ySizeLK8BigFont;
#if AUTORES
	rcx=rc.right-xSizeLK8TitleFont-NIBLSCALE(4);
#else
	rcx=rc.right-NIBLSCALE(10);
#endif
	LKWriteText(Surface,  BufferValue, rcx,rcy, 0, WTMODE_OUTLINED, WTALIGN_RIGHT, overcolor, true);

        //
        // SAFETY MAC CREADY INDICATOR
        //
	extern bool IsSafetyMacCreadyInUse(int val);
	if (IsSafetyMacCreadyInUse(GetOvertargetIndex()) && GlidePolar::SafetyMacCready>0) {
		Surface.SelectObject(LK8SmallFont);
		_stprintf(BufferValue,_T(" %.1f %s "),GlidePolar::SafetyMacCready*LIFTMODIFY,
		Units::GetUnitName(Units::GetUserVerticalSpeedUnit()));
		LKWriteBoxedText(Surface, rc,BufferValue, rcx,smacOffset, 0, WTALIGN_RIGHT, RGB_WHITE, RGB_BLACK);
	}

	//
	// AUTO MC INDICATOR
	//
	if (DerivedDrawInfo.AutoMacCready == true) {
	  Surface.SelectObject(LK8TitleFont);

	LKSurface::OldBrush ob {}; 
	if (LKTextBlack) {
		ob=Surface.SelectObject(LKBrush_White);
	}

#if AUTORES
	TCHAR amcmode[10];
        switch(AutoMcMode) {
            case amcFinalGlide:
                _tcscpy(amcmode,MsgToken(1676));
                break;
            case amcAverageClimb:
                _tcscpy(amcmode,MsgToken(1678));
                break;
            case amcFinalAndClimb:
                if (DerivedDrawInfo.FinalGlide)
                    _tcscpy(amcmode,MsgToken(1676));
                else
                     _tcscpy(amcmode,MsgToken(1678));
                break;
            case amcEquivalent:
                _tcscpy(amcmode,MsgToken(1680));
                break;
            default:
                _tcscpy(amcmode,_T("?"));
                break;
        }

	if (OverlaySize==0) { // BIG FONT
            //
            // BIG FONT (overlaysize==0)
            //
	    int yoff=0; // on portrait set vertical center offset relative to big font MC value
            if (!ScreenLandscape) yoff= (ySizeLK8BigFont- ySizeLK8TitleFont )/4 - NIBLSCALE(1); 

	    Surface.Rectangle(
	        rcx,rcy+ ySizeLK8BigFont/4+yoff,
                rc.right, rcy+ySizeLK8MediumFont/2+yoff +ySizeLK8TitleFont);

            if (LKTextBlack) Surface.SelectObject(ob);

	    LKWriteText(Surface, amcmode, rc.right-NIBLSCALE(1),rcy+ySizeLK8BigFont/4+yoff, 
                0, WTMODE_NORMAL, WTALIGN_RIGHT, RGB_WHITE, true);

	} else { 
            //
            // SMALL FONT (overlaysize==1)
            //
	    int yoff=0;
            if (!ScreenLandscape) yoff= (ySizeLK8BigFont- ySizeLK8TitleFont)/4 - NIBLSCALE(1); 

	    Surface.Rectangle(
	        rcx,rcy+ySizeLK8MediumFont/4+yoff,  
                rc.right, rcy+ySizeLK8MediumFont/4+yoff +ySizeLK8TitleFont);

            if (LKTextBlack) Surface.SelectObject(ob);

            LKWriteText(Surface,  amcmode, rc.right-NIBLSCALE(1),rcy+ySizeLK8MediumFont/4+yoff, 
                0, WTMODE_NORMAL, WTALIGN_RIGHT, RGB_WHITE, true);
        }

#else
	  Surface.Rectangle(
		rectLeft_AutoMc[ScreenSize],
	  	rectTop_AutoMc[ScreenSize],
	  	rectRight_AutoMc[ScreenSize],
	  	rectBottom_AutoMc[ScreenSize]);

	if (LKTextBlack) {
		Surface.SelectObject(ob);
	}

	  TCHAR amcmode[10];
	  switch(AutoMcMode) {
		case amcFinalGlide:
			_tcscpy(amcmode,MsgToken(1676));
			break;
		case amcAverageClimb:
			_tcscpy(amcmode,MsgToken(1678));
			break;
		case amcFinalAndClimb:
			if (DerivedDrawInfo.FinalGlide)
				_tcscpy(amcmode,MsgToken(1676));
			else
				_tcscpy(amcmode,MsgToken(1678));
			break;
		case amcEquivalent:
			_tcscpy(amcmode,MsgToken(1680));
			break;
		default:
			_tcscpy(amcmode,_T("?"));
			break;
	 }

	  LKWriteText(Surface,  amcmode, writeX_AutoMc[ScreenSize], writeY_AutoMc[ScreenSize],
		0, WTMODE_NORMAL, WTALIGN_RIGHT, RGB_WHITE, true);

#endif // NOT AUTORES

	} // AutoMacCready true AUTO MC INDICATOR

  }
  if ( Look8000 == (Look8000_t)lxcNoOverlay ) goto Drawbottom;


  if ( (Look8000==(Look8000_t)lxcAdvanced) ) {

	Surface.SelectObject(bigFont);
	if (ISPARAGLIDER) {
		LKFormatValue(LK_HNAV, false, BufferValue, BufferUnit, BufferTitle); // 091115
	} else {
		if (MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING))
			LKFormatValue(LK_TC_30S, false, BufferValue, BufferUnit, BufferTitle);
		else
			LKFormatValue(LK_LD_AVR, false, BufferValue, BufferUnit, BufferTitle);
	}
	Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
	if (IsMultimapOverlaysGauges() && !mode.AnyPan())
		rcx=rc.left+NIBLSCALE(10)+leftmargin+GlideBarOffset;
	else
		rcx=rc.left+NIBLSCALE(10)+leftmargin;   // 091115
	if (ISPARAGLIDER||(IsMultimapOverlaysGauges()&&LKVarioBar))
		rcy=(rc.bottom + rc.top-BottomSize)/2 -TextSize.cy-NIBLSCALE(5);
	else
		rcy=(rc.bottom + rc.top)/2 -TextSize.cy-NIBLSCALE(10);

	if (ISPARAGLIDER) {
		LKWriteText(Surface,  BufferValue, rcx, rcy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		if (!HideUnits) {
			Surface.SelectObject(LKMAPFONT);  // FIXFONT
			LKWriteText(Surface,  BufferUnit, rcx+TextSize.cx+NIBLSCALE(2),rcy+(TextSize.cy/3), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		}

	} else {
		if (ISGLIDER) {
			LKWriteText(Surface,  BufferValue, rcx+NIBLSCALE(9), rcy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
			if (!HideUnits) {
				Surface.SelectObject(LKMAPFONT);  // FIXFONT
				LKWriteText(Surface,  BufferUnit, rcx+TextSize.cx+NIBLSCALE(9),rcy+(TextSize.cy/3), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
			}
		}
	}

	if (ISPARAGLIDER || ((IsMultimapOverlaysGauges()&&LKVarioBar)&&!ISCAR)) {
		if (MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING) || LKVarioVal==vValVarioVario) {
			LKFormatValue(LK_VARIO, false, BufferValue, BufferUnit, BufferTitle);
			// wcscpy(BufferUnit,_T("VAR"));
		} else {
			switch(LKVarioVal) {
				case vValVarioNetto:
					LKFormatValue(LK_NETTO, false, BufferValue, BufferUnit, BufferTitle);
					_tcscpy(BufferUnit,_T("NT"));
					break;
				case vValVarioSoll:
				default:
					LKFormatValue(LK_SPEED_DOLPHIN, false, BufferValue, BufferUnit, BufferTitle);
					_tcscpy(BufferUnit,_T("SF"));
					break;
			}
		}

		Surface.SelectObject(bigFont);
		Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
		rcy+=TextSize.cy;
		LKWriteText(Surface,  BufferValue, rcx,rcy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		if (!HideUnits) {
			Surface.SelectObject(LKMAPFONT); // FIXFONT
			LKWriteText(Surface,  BufferUnit, rcx+TextSize.cx+NIBLSCALE(2),rcy+(TextSize.cy/3), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		}
	}
	if (!ISGAAIRCRAFT && !ISCAR) {
		if (ISPARAGLIDER) {
			LKFormatValue(LK_GNDSPEED, false, BufferValue, BufferUnit, BufferTitle);
		} else {
			if (MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING)) {
				LKFormatValue(LK_HNAV, false, BufferValue, BufferUnit, BufferTitle);
			} else {
				LKFormatValue(LK_GNDSPEED, false, BufferValue, BufferUnit, BufferTitle);
			}
		}
		Surface.SelectObject(bigFont);
		Surface.GetTextSize(BufferValue, _tcslen(BufferValue), &TextSize);
		rcy+=TextSize.cy;
		LKWriteText(Surface,  BufferValue, rcx,rcy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		if (!HideUnits) {
			Surface.SelectObject(LKMAPFONT);
			LKWriteText(Surface,  BufferUnit, rcx+TextSize.cx+NIBLSCALE(2),rcy+(TextSize.cy/3), 0,
				WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
		}
	}

	LKFormatValue(LK_TIME_LOCALSEC, false, BufferValue, BufferUnit, BufferTitle);

	if (OverlayClock || (ISPARAGLIDER && UseGates()) ) {
		Surface.SelectObject(medFont);
		if ( !ScreenLandscape ) {
#if AUTORES
		// Clock overlay position for portrait modes only: unfortunately, clock in this case will be positioned
		// in different places, depending on screen resolution. If possible, on top right as usual.
		// Otherwise, after right side overlays, slightly lower than them.
			unsigned short yClockposition;
			if ( (ScreenSizeY / (double)ScreenSizeX) > 1.5) {
				yClockposition=yrightoffset - ySizeLK8BigFont- (ySizeLK8MediumFont*4);
			} else {
				yClockposition=yrightoffset +(ySizeLK8MediumFont*3+NIBLSCALE(4));
			}
#endif
			LKWriteText(Surface,  BufferValue, rc.right-NIBLSCALE(10),
				yClockposition,
				0, WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);
	        } else { // landscape
			LKWriteText(Surface,  BufferValue, rc.right-NIBLSCALE(30),rc.top+NIBLSCALE(1), 0,
				WTMODE_OUTLINED,WTALIGN_RIGHT,overcolor, true);
                }
	}

  }

Drawbottom:

  if (MapSpaceMode != MSM_MAP && Current_Multimap_SizeY!=SIZE4) goto TheEnd;


  //
  // Draw wind 
  //
  Surface.SelectObject(medFont);

  if (Look8000 == lxcNoOverlay) goto afterWind; // 100930

  if (ISCAR || ISGAAIRCRAFT) {
	if (!HideUnits) {
		LKFormatValue(LK_GNDSPEED, false, BufferValue, BufferUnit, BufferTitle);
		_stprintf(Buffer,_T("%s %s"),BufferValue,BufferUnit);
	} else {
		LKFormatValue(LK_GNDSPEED, false, Buffer, BufferUnit, BufferTitle);
	}
  } else {
	LKFormatValue(LK_WIND, false, Buffer, BufferUnit, BufferTitle);
  }

  rcy=ySizeLK8TargetFont;
 
  if (DrawBottom)
  	LKWriteText(Surface,  Buffer, rc.left+NIBLSCALE(5)+leftmargin, rc.bottom - BottomSize- rcy-NIBLSCALE(2), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);
  else
  	LKWriteText(Surface,  Buffer, rc.left+NIBLSCALE(5)+leftmargin, rc.bottom - rcy-NIBLSCALE(5), 0, WTMODE_OUTLINED,WTALIGN_LEFT,overcolor, true);


afterWind:


TheEnd:

  // restore objects and return
  Surface.SelectObject(oldpen);
  Surface.SelectObject(oldbrush);
  Surface.SelectObject(oldfont);


}

