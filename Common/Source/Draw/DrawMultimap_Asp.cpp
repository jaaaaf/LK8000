/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id$
*/

#include "externs.h"
#include "LKMapWindow.h"
#include "LKObjects.h"
#include "RGB.h"
#include "DoInits.h"
#include "Sideview.h"
#include "Message.h"
#include "LKInterface.h"
#include "InputEvents.h"
#include "Multimap.h"

extern int XstartScreen, YstartScreen;

extern long  iSonarLevel;
bool Sonar_IsEnabled = true;

extern AirSpaceSonarLevelStruct sSonarLevel[];
extern TCHAR Sideview_szNearAS[];
extern double fZOOMScale;


AirSpaceSonarLevelStruct sSonarLevel[10] = {
    /* horizontal sonar levels */
    /* Dist , Delay *0.5s, V/H,      soundfile */
    {  150,     3,         true, TEXT("LK_SONAR_H1.WAV")},
    {  330,     3,         true, TEXT("LK_SONAR_H2.WAV")},
    {  500,     5,         true, TEXT("LK_SONAR_H3.WAV")},
    {  650,     5,         true, TEXT("LK_SONAR_H4.WAV")},
    {  850,     7,         true, TEXT("LK_SONAR_H5.WAV")},
    /* vertical sonar levels */
    {  30 ,     3,         false, TEXT("LK_SONAR_H1.WAV")},
    {  50 ,     3,         false, TEXT("LK_SONAR_H2.WAV")},
    {  70,      5,         false, TEXT("LK_SONAR_H3.WAV")},
    {  90,      5,         false, TEXT("LK_SONAR_H4.WAV")},
    {  110,     7,         false, TEXT("LK_SONAR_H5.WAV")}
   };



int SonarNotify(void)
{
  static unsigned long lSonarCnt = 0;
  lSonarCnt++;

  if((iSonarLevel >=0) && (iSonarLevel < 10))
	if( lSonarCnt > (unsigned)sSonarLevel[iSonarLevel].iSoundDelay)
	{
	  lSonarCnt = 0;
          // StartupStore(_T("... level=%d PLAY <%s>\n"),iSonarLevel,&sSonarLevel[iSonarLevel].szSoundFilename);
	  LKSound((TCHAR*) &(sSonarLevel[iSonarLevel].szSoundFilename));
	}
  return 0;
}



void MapWindow::LKDrawMultimap_Asp(HDC hdc, const RECT rc)
{


  RECT rci = rc;
  rci.bottom -= BottomSize;

  #if 0
  if (DoInit[MDI_MAPASP]) {
	DoInit[MDI_MAPASP]=false;
  }
  #endif

  switch(LKevent) {
	//
	// USABLE EVENTS
	// 

	case LKEVENT_TOPLEFT:
		InputEvents::setMode(_T("MMCONF"));
		break;
	break;

	case LKEVENT_TOPRIGHT:
	  if(GetSideviewPage()== IM_NEAR_AS)
	  {
	    Sonar_IsEnabled = !Sonar_IsEnabled;
	    if (EnableSoundModes) {
	    	if (Sonar_IsEnabled)
			LKSound(TEXT("LK_TONEUP.WAV"));
		else
			LKSound(TEXT("LK_TONEDOWN.WAV"));
	    }
	  }
	break;

	default:
		// THIS SHOULD NEVER HAPPEN, but always CHECK FOR IT!
		break;
  }


  //
  // This is doing all rendering, including terrain and topology, which is not good.
  //
#ifdef ENABLE_ALL_AS_FOR_SIDEVIEW
  int oldAltMode = AltitudeMode ;

  AltitudeMode = ALLON;
#endif

  RenderAirspace(hdc, rci);

#ifdef ENABLE_ALL_AS_FOR_SIDEVIEW
  AltitudeMode = oldAltMode;
#endif


  DrawMultimap_Topleft(hdc, rci);
  DrawMultimap_DynaLabel(hdc, rci);

  if(GetSideviewPage()== IM_NEAR_AS) SonarNotify();


  LKevent=LKEVENT_NONE;

}
