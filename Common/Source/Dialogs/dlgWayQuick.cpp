/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: dlgWayQuick.cpp,v 1.1 2011/12/21 10:29:29 root Exp root $
*/

#include "externs.h"
#include "LKProfiles.h"
#include "Dialogs.h"
#include "LKObjects.h"
#include "Bitmaps.h"
#include "WindowControls.h"
#include "dlgTools.h"

static WndForm *wf=NULL;
#define WPLSEL WayPointList[SelectedWaypoint]


static short retStatus;

static void OnPaintWaypointPicto(WindowControl * Sender, LKSurface& Surface) {
    (void) Sender;
    if (wf) {

        WndFrame *wPicto = ((WndFrame *) wf->FindByName(TEXT("frmWaypointPicto")));
        if (wPicto) {
            const RECT rc = wPicto->GetClientRect();

            MapWindow::DrawWaypointPictoBg(Surface, rc);
            LKASSERT(ValidWayPoint(SelectedWaypoint));
            if (WayPointCalc[SelectedWaypoint].IsLandable) {
                MapWindow::DrawRunway(Surface, &WayPointList[SelectedWaypoint], rc, 4000, true);
            } else {
                MapWindow::DrawWaypointPicto(Surface, rc, &WayPointList[SelectedWaypoint]);
            }
        }
    }
}

static void OnCancelClicked(WndButton* pWnd) {
    if (wf) {
        wf->SetModalResult(mrOK);
    }
}

static void OnSetAlt1Clicked(WndButton* pWnd){
  LockTaskData();
  Alternate1 = SelectedWaypoint;
  RefreshTask();
  UnlockTaskData();
  if (ValidWayPoint(Alternate1))
  	DoStatusMessage(_T("Altern.1="),WayPointList[Alternate1].Name);
  retStatus=3;
  wf->SetModalResult(mrOK);
}

static void OnSetAlt2Clicked(WndButton* pWnd){
  LockTaskData();
  Alternate2 = SelectedWaypoint;
  RefreshTask();
  UnlockTaskData();
  if (ValidWayPoint(Alternate2))
  	DoStatusMessage(_T("Altern.2="),WayPointList[Alternate2].Name);
  retStatus=4;
  wf->SetModalResult(mrOK);
}

static void OnGotoClicked(WndButton* pWnd){
  GotoWaypoint(SelectedWaypoint);
  retStatus=2;
  wf->SetModalResult(mrOK);
}

static void OnDetailsClicked(WndButton* pWnd){
  retStatus=1;
  wf->SetModalResult(mrOK);
}

static void OnTaskClicked(WndButton* pWnd){
  retStatus=5;
  wf->SetModalResult(mrOK);
}

static CallBackTableEntry_t CallBackTable[]={
  ClickNotifyCallbackEntry(OnGotoClicked),
  ClickNotifyCallbackEntry(OnSetAlt1Clicked),
  ClickNotifyCallbackEntry(OnSetAlt2Clicked),
  ClickNotifyCallbackEntry(OnTaskClicked),
  ClickNotifyCallbackEntry(OnCancelClicked),
  OnPaintCallbackEntry(OnPaintWaypointPicto),
  EndCallBackEntry()
};

// Will return 0 if cancel or error, 1 if details needed, 2 if goto, 3 if alt1, 4 if alt2
short dlgWayQuickShowModal(void){

  wf = NULL;

  TCHAR filename[MAX_PATH];
  TCHAR sTmp[128];

  if (ScreenLandscape) {
	LocalPathS(filename, TEXT("dlgWayQuick.xml"));
	wf = dlgLoadFromXML(CallBackTable, filename, TEXT("IDR_XML_WAYPOINTQUICK"));
  } else {
	LocalPathS(filename, TEXT("dlgWayQuick_P.xml"));
	wf = dlgLoadFromXML(CallBackTable, filename, TEXT("IDR_XML_WAYPOINTQUICK_P"));
  }

  if (!wf) return 0;

  ((WndButton *)wf->FindByName(TEXT("cmdGoto"))) ->SetOnClickNotify(OnGotoClicked);
  ((WndButton *)wf->FindByName(TEXT("cmdSetAlt1"))) ->SetOnClickNotify(OnSetAlt1Clicked);
  ((WndButton *)wf->FindByName(TEXT("cmdSetAlt2"))) ->SetOnClickNotify(OnSetAlt2Clicked);
  ((WndButton *)wf->FindByName(TEXT("cmdDetails"))) ->SetOnClickNotify(OnDetailsClicked);
  ((WndButton *)wf->FindByName(TEXT("cmdTask"))) ->SetOnClickNotify(OnTaskClicked);
  ((WndButton *)wf->FindByName(TEXT("cmdCancel"))) ->SetOnClickNotify(OnCancelClicked);

  retStatus=0;
  if (WPLSEL.Format == LKW_CUP) {
        TCHAR ttmp[50];
        // and it is landable
        if ((WPLSEL.Style>1) && (WPLSEL.Style<6) ) {

                _stprintf(sTmp, TEXT("%s "), WPLSEL.Name);
		if (_tcslen(sTmp)>9) {
			sTmp[9]='\0';
			_tcscat(sTmp, _T(" "));
		}

                if ( _tcslen(WPLSEL.Freq)>0 )  {
                        _stprintf(ttmp,_T("%s "),WPLSEL.Freq);
                        _tcscat(sTmp, ttmp);
                }

                if ( WPLSEL.RunwayDir>=0 )  {
                        _stprintf(ttmp,_T("RW %d "),WPLSEL.RunwayDir);
                        _tcscat(sTmp, ttmp);
                }
                if ( WPLSEL.RunwayLen>0 )  {
                        // we use Altitude instead of distance, to keep meters and feet
                        _stprintf(ttmp,_T("%.0f%s"),Units::ToUserAltitude((double)WPLSEL.RunwayLen), Units::GetAltitudeName());
                        _tcscat(sTmp, ttmp);
                }

        } else {
		_stprintf(sTmp, _T(" %s"),WayPointList[SelectedWaypoint].Name);
        }
  } else {
	_stprintf(sTmp, _T(" %s"),WayPointList[SelectedWaypoint].Name);
  }
  wf->SetCaption(sTmp);

  if (ScreenLandscape) {
//	((WndButton *)wf->FindByName(TEXT("cmdGoto"))) ->SetLeft(NIBLSCALE(3));
	  int left = ((WndButton *)wf->FindByName(TEXT("cmdGoto"))) ->GetLeft();
	((WndButton *)wf->FindByName(TEXT("cmdGoto"))) ->SetWidth(ScreenSizeX-NIBLSCALE(5)-left);

	((WndButton *)wf->FindByName(TEXT("cmdSetAlt1"))) ->SetWidth((ScreenSizeX/2)-NIBLSCALE(5));
	((WndButton *)wf->FindByName(TEXT("cmdSetAlt1"))) ->SetLeft(NIBLSCALE(3));

	((WndButton *)wf->FindByName(TEXT("cmdSetAlt2"))) ->SetWidth((ScreenSizeX/2)-NIBLSCALE(7));
	((WndButton *)wf->FindByName(TEXT("cmdSetAlt2"))) ->SetLeft((ScreenSizeX/2)+NIBLSCALE(2));


	((WndButton *)wf->FindByName(TEXT("cmdDetails"))) ->SetWidth((ScreenSizeX/2)-NIBLSCALE(5));
	((WndButton *)wf->FindByName(TEXT("cmdDetails"))) ->SetLeft(NIBLSCALE(3));

	((WndButton *)wf->FindByName(TEXT("cmdTask"))) ->SetWidth((ScreenSizeX/2)-NIBLSCALE(7));
	((WndButton *)wf->FindByName(TEXT("cmdTask"))) ->SetLeft((ScreenSizeX/2)+NIBLSCALE(2));

	((WndButton *)wf->FindByName(TEXT("cmdCancel"))) ->SetLeft(NIBLSCALE(3));
	((WndButton *)wf->FindByName(TEXT("cmdCancel"))) ->SetWidth((ScreenSizeX)-NIBLSCALE(8));
  } 

  wf->ShowModal();

  delete wf;

  wf = NULL;

  return retStatus;

}
