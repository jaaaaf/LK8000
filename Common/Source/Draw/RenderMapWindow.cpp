/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id$
*/

#include "externs.h"
#include "Poco/Timestamp.h"


Poco::Timestamp MapWindow::timestamp_newdata = 0;

//
// Execution at 1hz inside RenderMapWindow
//
void MapWindow::DrawFunctions1HZ(LKSurface& Surface, const RECT& rc) {

  ONEHZLIMITER;

  DrawLKAlarms(Surface, rc);
#ifndef NO_DATARECORDER  
  DrawFDRAlarms(Surface, rc);
#endif
  #if (WINDOWSPC<1)
  LKBatteryManager();
  #endif
  DoSonar();
}



void MapWindow::UpdateTimeStats(bool start) {
  if (start) {
    timestamp_newdata.update();
  }
}

bool FastZoom;

//
// CALLED BY THE DRAW_THREAD MAIN LOOP
//
void MapWindow::RenderMapWindow(LKSurface& Surface, const RECT& rc)
{
  // First of all we set the flag for DrawBottom. This is critical.
  if (NOTANYPAN)
	DrawBottom=true;
  else
	DrawBottom=false;

  static Poco::Timestamp fastzoomStart = 0;
  // static short ZoomDelayTimes=0; // alternate rebouncing, with the lowest possible interval: 1 loop

  // We do this in order to wait for a late zoom request after another.
  // Did we get a BigZoom request?
  FastZoom=zoom.BigZoom();
  if (FastZoom) {
	zoom.BigZoom(false);
	// How many times we shall loop waiting for a next bigzoom to come
	// ZoomDelayTimes=1; shortest possible
	fastzoomStart.update(); // time granted delay
  } else {
	if (fastzoomStart.epochMicroseconds()) {
		// no bigzoom, but we wait a bit to detect another click for zoom
		// and avoid to redraw entirely in the meantime. We shall fall down here
		// because we have forced a map redraw after the first zoom, even with not
		// a click pressed.
		//if (ZoomDelayTimes >0) {
		//	ZoomDelayTimes--;
		if (!fastzoomStart.isElapsed(debounceTimeout.totalMicroseconds())) {
			#if (WINDOWSPC>0)	
			  #if TESTBENCH
			  FastZoom=true;
			  #endif
			#else
			  FastZoom=true;
			#endif
			// 
			return;
		} else {
			fastzoomStart=0;
		}
	} 
  }

  MapWindow::UpdateTimeStats(true);

  if (LockModeStatus) LockMode(9); // check if unlock is now possible 
  
  POINT Orig, Orig_Aircraft;

  SetAutoOrientation(false); // false for no reset Old values
  CalculateOrigin(rc, &Orig);

  //
  // When BigZoom trigger, we shall not calculate waypoints and olc.
  //
  if (!QUICKDRAW) {
    //
    // this is calculating waypoint visible, and must be executed before rendermapwindowbg which calls   
    // CalculateWayPointReachable new, setting values for visible wps!
    // This is also calculating CalculateScreenBounds 0.0  and placing it inside MapWindow::screenbounds_latlon
    //
    CalculateScreenPositions(Orig, rc, &Orig_Aircraft);
    LKUpdateOlc();
  } else {
	CalculateScreenPositions(Orig, rc, &Orig_Aircraft);
	FastZoom=true;
  }

  RenderMapWindowBg(Surface, rc, Orig, Orig_Aircraft);

  // No reason to check for bigzoom here, because we are not drawing the map
  if (DONTDRAWTHEMAP) {
  	DrawFlightMode(Surface, rc);
  	DrawGPSStatus(Surface, rc);
	DrawFunctions1HZ(Surface,rc);
	return;
  }

  // Logger indicator, flight indicator, battery indicator
  // Not while panning
  if (!INPAN) DrawFlightMode(Surface, rc);

  //
  // When fast zoom requested, do not loose time with frills
  //
  if (QUICKDRAW) {
	// do a mapdirty and rerun the loop
	MapWindow::RefreshMap();
	return;
  }
  
  // GPS FIX warnings
  DrawGPSStatus(Surface, rc);

  // Alarms &C.
  DrawFunctions1HZ(Surface,rc);

}


