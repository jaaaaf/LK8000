/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: dlgTools.cpp,v 1.1 2011/12/21 10:29:29 root Exp root $
*/

#include "externs.h"
#include <limits.h>
#include "dlgTools.h"
#include "WindowControls.h"
#include "xmlParser.h"
#include "RGB.h"
#include "Dialogs.h"
#include "utils/stringext.h"
#include "utils/stl_utils.h"
#include "LKInterface.h"

int DLGSCALE(int x) {
  int iRetVal = x;
  iRetVal = (int) ((x)*ScreenDScale);
  return iRetVal;
}


static void OnButtonClick(WndButton* pWnd){
    if(pWnd && pWnd->GetTopOwner()) {
        static_cast<WndForm*>(pWnd->GetTopOwner())->SetModalResult(pWnd->GetTag());
    }
}

MsgReturn_t MessageBoxX(LPCTSTR lpText, LPCTSTR lpCaption, MsgType_t uType, bool wfullscreen){

  WndForm *wf=NULL;
  WndFrame *wText=NULL;
  int X, Y, Width, Height;
  WndButton *wButtons[10];
  int ButtonCount = 0;
  int i,x,y,d,w,h,dY;
  MsgReturn_t res;
  RECT rc = MainWindow.GetClientRect();

  if (wfullscreen) {
	Width = rc.right;
	Height = rc.bottom;
  } else
  if (ScreenLandscape) {
	Width = DLGSCALE(280);
	Height = DLGSCALE(160);
  } else {
	Width = DLGSCALE(230);
	Height = DLGSCALE(160);
  }

  X = ((rc.right-rc.left) - Width)/2;
  Y = ((rc.bottom-rc.top) - Height)/2;

  if (wfullscreen) {
	dY=0;
	y = DLGSCALE(ScreenLandscape?160:200);
  } else {
	dY = DLGSCALE(-40);
	y = DLGSCALE(100);
  }
  w = DLGSCALE(60);
  h = DLGSCALE(32);
  
  Height += dY;

  wf = new WndForm(TEXT("frmXcSoarMessageDlg"), 
                   lpCaption, X, Y, Width, Height);
  wf->SetFont(MapWindowBoldFont);
  wf->SetTitleFont(MapWindowBoldFont);
  wf->SetBackColor(RGB_WINBACKGROUND);
  wf->SetBorderKind(BORDERTOP|BORDERRIGHT|BORDERBOTTOM|BORDERLEFT);

  wText = new WndFrame(wf, 
                       TEXT("frmMessageDlgText"), 
                       0, 
                       DLGSCALE(5), 
                       Width, 
                       Height - DLGSCALE(5));

  wText->SetCaption(lpText);
  wText->SetFont(MapWindowBoldFont);
  wText->SetCaptionStyle(
        DT_EXPANDTABS
      | DT_CENTER
      | DT_NOCLIP
      | DT_WORDBREAK
  );

  y += dY;

  if (uType == mbOk
      || uType == mbOkCancel)
  {
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), TEXT("OK"), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdOk);
    ButtonCount++;
  }

  if (uType == mbYesNo
      || uType == mbYesNoCancel)
  {
	// LKTOKEN  _@M827_ = "Yes" 
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M827_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdYes);
    ButtonCount++;
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M890_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdNo);
    ButtonCount++;
  }

  if (uType == mbAbortRetryIgnore
      || uType == mbRetryCancel)
  {
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M566_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdRetry);
    ButtonCount++;
  }

  if (uType == mbOkCancel
      || uType == mbRetryCancel
      || uType == mbYesNoCancel)
  {
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M161_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdCancel);
    ButtonCount++;
  }

  if (uType == mbAbortRetryIgnore) 
  {
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M47_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdAbort);
    ButtonCount++;
    wButtons[ButtonCount] = new WndButton(wf, TEXT(""), gettext(TEXT("_@M349_")), 0, y, w, h, OnButtonClick);
    wButtons[ButtonCount]->SetTag(IdIgnore);
    ButtonCount++;
  }

  LKASSERT(ButtonCount!=0);
  d = Width / (ButtonCount);
  x = d/2-w/2;

  for (i=0; i<ButtonCount; i++){
    wButtons[i]->SetLeft(x);
    x += d;
  }

  res = static_cast<MsgReturn_t>(wf->ShowModal());

  delete wf;

  return(res);

}




long StringToIntDflt(const TCHAR *String, long Default){
  if (String == NULL || String[0] == '\0')
    return(Default);
  return(_tcstol(String, NULL, 0));        
}

double StringToFloatDflt(const TCHAR *String, double Default){
  if (String == NULL || String[0] == '\0')
    return(Default);
  return(_tcstod(String, NULL));
}

const TCHAR *StringToStringDflt(const TCHAR *String, const TCHAR *Default){
  if (String == NULL || String[0] == '\0')
    return(Default);
  return(String);
}

void GetDefaultWindowControlProps(XMLNode *Node, TCHAR *Name, int *X, int *Y, int *Width, int *Height, int *Popup, int *Font, TCHAR *Caption){

  *Popup = StringToIntDflt(Node->getAttribute(TEXT("Popup")), 0); // don't make wndForm block entire screen
  *X = DLGSCALE(StringToIntDflt(Node->getAttribute(TEXT("X")), 0));
  *Y = StringToIntDflt(Node->getAttribute(TEXT("Y")), 0);
  if (*Y>=0) { // not -1
    (*Y) = DLGSCALE(*Y);
  }
  *Width = DLGSCALE(StringToIntDflt(Node->getAttribute(TEXT("Width")), 50));
  *Height = StringToIntDflt(Node->getAttribute(TEXT("Height")), 50);
  if (*Height>=0) {
    (*Height) = DLGSCALE(*Height);
  }
  *Font = StringToIntDflt(Node->getAttribute(TEXT("Font")), -1);
  _tcscpy(Name, StringToStringDflt(Node->getAttribute(TEXT("Name")), TEXT("")));
  _tcscpy(Caption,LKGetText(StringToStringDflt(Node->getAttribute(TEXT("Caption")), TEXT(""))));

}

void *CallBackLookup(CallBackTableEntry_t *LookUpTable, TCHAR *Name){

  int i;

  if (LookUpTable!=NULL && Name!=NULL && Name[0]!= '\0')
    for (i=0; LookUpTable[i].Ptr != NULL; i++){
      if (_tcscmp(LookUpTable[i].Name, Name) == 0){
        return(LookUpTable[i].Ptr);
      }
    }

  return(NULL);

}

void LoadChildsFromXML(WindowControl *Parent, CallBackTableEntry_t *LookUpTable, XMLNode *Node, int Font);

// The Font=n in dialogs.  0-4, 4 unused kept for compat issue with very old code
static FontReference FontMap[5];


#include <stdio.h>

#ifndef WIN32_RESOURCE
#include "resource_data.h"
#else
extern HINSTANCE _hInstance;
#endif

XMLNode xmlLoadFromResource(const TCHAR* lpName, LPCTSTR tag, XMLResults *pResults) {
    const char *lpRes = NULL;
    int l = 0;
    TCHAR * szXML = NULL;

#ifdef WIN32_RESOURCE
  HRSRC hResInfo;
  HGLOBAL hRes; 

  // Find the xml resource.
  hResInfo = FindResource (_hInstance, lpName, TEXT("XMLDialog"));

  if (hResInfo == NULL) {
    MessageBoxX(
      TEXT("Can't find resource"),
      TEXT("Dialog error"),
      mbOk);

    // unable to find the resource
    return XMLNode::emptyXMLNode;
  }

  // Load the wave resource. 
  hRes = LoadResource (_hInstance, hResInfo);

  if (hRes == NULL) {
    MessageBoxX(
      TEXT("Can't load resource"),
      TEXT("Dialog error"),
      mbOk);

    // unable to load the resource
    return XMLNode::emptyXMLNode;
  }

    // Retrieves a pointer to the xml resource in memory 
    lpRes = (const char*) LockResource(hRes);
    if (lpRes) {
        l = SizeofResource(_hInstance, hResInfo);
    }

#else
    lpRes = GetNamedResourceString(lpName);
    if(lpRes) {
        l = strlen(lpRes);
    }
#endif

    if (lpRes && l > 0) {
#ifdef _UNICODE
        char *buf = NULL;
        if(lpRes[l] != '\0') {
            buf = (char*) malloc(l + 2);
            if (!buf) {
                //StartupStore(_T("------ LoadFromRes malloc error%s"),NEWLINE); // 100101
                goto _errmem;
            }
            strncpy(buf, (char*) lpRes, l);
            buf[l] = 0; // need to explicitly null-terminate.
            buf[l + 1] = 0;
            lpRes = buf;
        }
        szXML = (TCHAR*) calloc(l + 2, sizeof (TCHAR));
        if (!szXML) {
            //StartupStore(_T("------ LoadFromRes malloc2 error%s"),NEWLINE); // 100101
            goto _errmem;
        }
        int nSize = utf2unicode(lpRes, szXML, l + 1);
        if(buf) {
            free(buf);
        }
#else
      int nSize = l;
      szXML= (char*)malloc(l+2);
      if (!szXML) {
        //StartupStore(_T("------ LoadFromRes malloc2 error%s"),NEWLINE); // 100101
          goto _errmem;
      }      
      strncpy(szXML,(char*)lpRes,l);
      szXML[l]=0; // need to explicitly null-terminate.
#endif
      if(nSize <=0) {
          free(szXML);
          
          MessageBoxX(
                        TEXT("Invalid dialog template"),
                        TEXT("Dialog error"),
                        mbOk);
          
          return XMLNode::emptyXMLNode;
      }
      XMLNode x=XMLNode::parseString(szXML,tag,pResults);

      free(szXML);
      return x;
    }
  MessageBoxX(
              TEXT("Can't lock resource"),
              TEXT("Dialog error"),
              mbOk);
  return XMLNode::emptyXMLNode;
  
_errmem:
    MessageBoxX(
                TEXT("Can't allocate memory"),
                TEXT("Dialog error"),
                mbOk);
    // unable to allocate memory
    return XMLNode::emptyXMLNode;
}



static XMLNode xmlOpenResourceHelper(const TCHAR *lpszXML, LPCTSTR tag)
{
    XMLResults pResults;

    pResults.error = eXMLErrorNone;
    XMLNode::GlobalError = false;
    XMLNode xnode=xmlLoadFromResource(lpszXML, tag, &pResults);
    if (pResults.error != eXMLErrorNone)
    {
      XMLNode::GlobalError = true;
      TCHAR errortext[100];
      _stprintf(errortext,TEXT("%s %i %i"), XMLNode::getError(pResults.error),
                pResults.nLine, pResults.nColumn);
                
      MessageBoxX(
                  errortext,
                  TEXT("Dialog error"),
                  mbOk);
        // was exit(255);

    }
    return xnode;
}



WndForm *dlgLoadFromXML(CallBackTableEntry_t *LookUpTable, const TCHAR *tfilename, const TCHAR* raw_resource) {

  WndForm *theForm = NULL;
  XMLNode xMainNode;


  // StartupStore(_T("... xmlOpen <%s>\n"),tfilename);

  xMainNode=XMLNode::openFileHelper(tfilename ,TEXT("PMML"));

  //
  // If nothing available in filesystem, load from internal resources
  // This is going to be removed in future LK versions.
  //
  if (xMainNode.isEmpty()) {
    if (raw_resource) {
      xMainNode =xmlOpenResourceHelper(raw_resource, TEXT("PMML"));
    }
  }


  // TODO code: put in error checking here and get rid of exits in xmlParser
  if (xMainNode.isEmpty()) {

    MessageBoxX(
      TEXT("Error in loading XML dialog"),
      TEXT("Dialog error"),
      mbOk);

    return NULL;
  }


  XMLNode xNode=xMainNode.getChildNode(TEXT("WndForm"));

  FontMap[0] = TitleWindowFont;
  FontMap[1] = MapWindowFont;
  FontMap[2] = MapWindowBoldFont;
  FontMap[3] = CDIWindowFont;
  FontMap[4] = CDIWindowFont;
  static_assert(array_size(FontMap)>4, " invalide \"FontMap\" size");

  if (!xNode.isEmpty()){
    int X,Y,Width,Height,Popup,Font;
    TCHAR sTmp[128];
    TCHAR Name[64];


    // fix screen width adjust but do not enlarge it if popup is selected
    GetDefaultWindowControlProps(&xNode, Name, &X, &Y, &Width, &Height, &Popup,
                                 &Font, sTmp);
    if (!Popup) {
      const RECT rc = MainWindow.GetClientRect();
        
      Width=rc.right;
      Height=rc.bottom;
      X=0;
      Y=0;
    }

    theForm = new WndForm(Name, sTmp, X, Y, Width, Height);

    if (Font != -1)
      theForm->SetTitleFont(FontMap[Font]);

    if (Font != -1)
      theForm->SetFont(FontMap[Font]);

    unsigned uBackColor = StringToIntDflt(xNode.getAttribute(TEXT("BackColor")), 0xffffffff);
    if (uBackColor != 0xffffffff){
        theForm->SetBackColor(LKColor((uBackColor>>16)&0xff, (uBackColor>>8)&0xff, (uBackColor>>0)&0xff));
    }

    unsigned uForeColor = StringToIntDflt(xNode.getAttribute(TEXT("ForeColor")), 0xffffffff);
    if (uForeColor != 0xffffffff){
        theForm->SetForeColor(LKColor((uForeColor>>16)&0xff, (uForeColor>>8)&0xff, (uForeColor>>0)&0xff));
    }

    LoadChildsFromXML(theForm, LookUpTable, &xNode, Font);

    if (XMLNode::GlobalError) {
      MessageBoxX(
                 TEXT("Error in loading XML dialog"),
                 TEXT("Dialog error"),
                 mbOk);

      delete theForm;
      return NULL;
    }

  } else {
    MessageBoxX(
      TEXT("Error in loading XML dialog"),
      TEXT("Dialog error"),
      mbOk);

    return NULL;
  }

  return(theForm);

}



void LoadChildsFromXML(WindowControl *Parent, 
                       CallBackTableEntry_t *LookUpTable, 
                       XMLNode *Node, 
                       int ParentFont) {

  int X,Y,Width,Height,Popup,Font;
  TCHAR Caption[128];
  TCHAR Name[64];
  bool Visible;
  int Border;

  int Count = Node->nChildNode();

  for (int i=0; i<Count; i++){

    WindowControl *WC=NULL;

    XMLNode childNode = Node->getChildNode(i);

    GetDefaultWindowControlProps(&childNode, 
                                 Name, 
                                 &X, &Y, 
                                 &Width, &Height, &Popup,
                                 &Font, Caption);

    unsigned uBackColor = StringToIntDflt(childNode.getAttribute(TEXT("BackColor")), 0xffffffff);
    unsigned uForeColor = StringToIntDflt(childNode.getAttribute(TEXT("ForeColor")), 0xffffffff);

    Visible = StringToIntDflt(childNode.getAttribute(TEXT("Visible")), 1) == 1;
    Font = StringToIntDflt(childNode.getAttribute(TEXT("Font")), ParentFont);
    Border = StringToIntDflt(childNode.getAttribute(TEXT("Border")), 0);

    if (_tcscmp(childNode.getName(), TEXT("WndProperty")) == 0){

      WndProperty *W;
      int CaptionWidth;
      TCHAR DataNotifyCallback[128];
      TCHAR OnHelpCallback[128];
      int ReadOnly;
      int MultiLine;

      CaptionWidth = 
        DLGSCALE(StringToIntDflt(childNode.getAttribute(TEXT("CaptionWidth")), 
                        0));
      MultiLine = 
        StringToIntDflt(childNode.getAttribute(TEXT("MultiLine")), 
                        0);
      ReadOnly = \
        StringToIntDflt(childNode.getAttribute(TEXT("ReadOnly")), 
                        0);


      _tcscpy(DataNotifyCallback, 
              StringToStringDflt(childNode.getAttribute(TEXT("OnDataNotify")),
                                 TEXT("")));

      _tcscpy(OnHelpCallback, 
              StringToStringDflt(childNode.getAttribute(TEXT("OnHelp")),
                                 TEXT("")));

	  _tcscpy(Caption, LKGetText(StringToStringDflt(childNode.getAttribute(TEXT("Caption")), TEXT(""))));

      WC = W = 
        new WndProperty(Parent, Name, Caption, X, Y, 
                        Width, Height, CaptionWidth,
                        (WndProperty::DataChangeCallback_t) 
                        CallBackLookup(LookUpTable, DataNotifyCallback), 
                        MultiLine);

      W->SetOnHelpCallback((WindowControl::OnHelpCallback_t) 
                           CallBackLookup(LookUpTable, OnHelpCallback));

      W->SetHelpText(StringToStringDflt(
                     childNode.getAttribute(TEXT("Help")), 
                     TEXT("")));

      Caption[0] = '\0';
      W->SetReadOnly(ReadOnly != 0);

      W->SetUseKeyboard(StringToIntDflt(childNode.getAttribute(TEXT("keyboard")), 0));

      if (childNode.nChildNode(TEXT("DataField")) > 0){

        TCHAR DataType[32];
        TCHAR DisplayFmt[32];
        TCHAR EditFormat[32];
        TCHAR OnDataAccess[64];
        double Min, Max, Step;
	int Fine;

        XMLNode dataFieldNode = 
          childNode.getChildNode(TEXT("DataField"), 0);

        _tcscpy(DataType, 
                StringToStringDflt(dataFieldNode.
                                   getAttribute(TEXT("DataType")), 
                                   TEXT("")));
        _tcscpy(DisplayFmt, 
                StringToStringDflt(dataFieldNode.
                                   getAttribute(TEXT("DisplayFormat")), 
                                   TEXT("")));
        _tcscpy(EditFormat, 
                StringToStringDflt(dataFieldNode.
                                   getAttribute(TEXT("EditFormat")), 
                                   TEXT("")));
        _tcscpy(OnDataAccess, 
                StringToStringDflt(dataFieldNode.
                                   getAttribute(TEXT("OnDataAccess")), 
                                   TEXT("")));
        ReadOnly = StringToIntDflt(dataFieldNode.
                                   getAttribute(TEXT("ReadOnly")), 0);
        Min = StringToIntDflt(dataFieldNode.
                              getAttribute(TEXT("Min")), INT_MIN);
        Max = StringToIntDflt(dataFieldNode.
                              getAttribute(TEXT("Max")), INT_MAX);
        Step = StringToFloatDflt(dataFieldNode.
                                 getAttribute(TEXT("Step")), 1);

	Fine = StringToIntDflt(dataFieldNode.
			       getAttribute(TEXT("Fine")), 0);

        if (_tcsicmp(DataType, TEXT("enum"))==0){
          W->SetDataField(
                          new DataFieldEnum(EditFormat, DisplayFmt, false,
                                            (DataField::DataAccessCallback_t) 
                                            CallBackLookup(LookUpTable, 
                                                           OnDataAccess))
          );
        }
        if (_tcsicmp(DataType, TEXT("filereader"))==0){
          W->SetDataField(
                          new DataFieldFileReader(EditFormat, 
                                                  DisplayFmt,
                                                  (DataField::DataAccessCallback_t) 
                                                  CallBackLookup(LookUpTable, OnDataAccess))
          );
        }
        if (_tcsicmp(DataType, TEXT("boolean"))==0){
          W->SetDataField(
            new DataFieldBoolean(EditFormat, DisplayFmt, false, gettext(TEXT("_@M958_")), gettext(TEXT("_@M959_")), // ON OFF
              (DataField::DataAccessCallback_t) CallBackLookup(LookUpTable, OnDataAccess))
          );
        }
        if (_tcsicmp(DataType, TEXT("double"))==0){
          W->SetDataField(
			  new DataFieldFloat(EditFormat, DisplayFmt, Min, Max, 0, Step, Fine,
              (DataField::DataAccessCallback_t) CallBackLookup(LookUpTable, OnDataAccess))
          );
        }
        if (_tcsicmp(DataType, TEXT("integer"))==0){
          W->SetDataField(
                          new DataFieldInteger(EditFormat, DisplayFmt, (int)Min, (int)Max, (int)0, (int)Step,
              (DataField::DataAccessCallback_t) CallBackLookup(LookUpTable, OnDataAccess))
          );
        }
        if (_tcsicmp(DataType, TEXT("string"))==0){
          W->SetDataField(
            new DataFieldString(EditFormat, DisplayFmt, TEXT(""),
              (DataField::DataAccessCallback_t) CallBackLookup(LookUpTable, OnDataAccess))
          );
        }

      }

    }else

    if (_tcscmp(childNode.getName(), TEXT("WndButton")) == 0){

      TCHAR ClickCallback[128];

       _tcscpy(ClickCallback, StringToStringDflt(childNode.getAttribute(TEXT("OnClickNotify")), TEXT("")));

      WC = new WndButton(Parent, Name, Caption, X, Y, Width, Height,
               (WndButton::ClickNotifyCallback_t) 
                         CallBackLookup(LookUpTable, ClickCallback));

      Caption[0] = '\0';

    }else

    if (_tcscmp(childNode.getName(), TEXT("WndOwnerDrawFrame")) == 0){

      TCHAR PaintCallback[128];

      _tcscpy(PaintCallback, StringToStringDflt(childNode.getAttribute(TEXT("OnPaint")), TEXT("")));

      WC = new WndOwnerDrawFrame(Parent, Name, X, Y, Width, Height,
               (WndOwnerDrawFrame::OnPaintCallback_t) CallBackLookup(LookUpTable, PaintCallback));

    }else

    if (_tcscmp(childNode.getName(), TEXT("WndFrame")) == 0){

      WndFrame *W;

      WC = W = new WndFrame(Parent, Name, X, Y, Width, Height);

      LoadChildsFromXML(W, LookUpTable, &childNode, ParentFont);  // recursivly create dialog

    }else

    if (_tcscmp(childNode.getName(), TEXT("WndListFrame")) == 0){

      TCHAR ListCallback[128];

      _tcscpy(ListCallback, StringToStringDflt(childNode.getAttribute(TEXT("OnListInfo")), TEXT("")));

      WC = new WndListFrame(Parent, Name, X, Y, Width, Height,
               (WndListFrame::OnListCallback_t) CallBackLookup(LookUpTable, ListCallback));

      LoadChildsFromXML(WC, LookUpTable, &childNode, ParentFont);  // recursivly create dialog

    } else {
        LKASSERT(false); // unknow control.
    }

    if (WC != NULL){

      if (Font != -1)
        WC->SetFont(FontMap[Font]);

      if (uBackColor != 0xffffffff){
        WC->SetBackColor(LKColor((uBackColor>>16)&0xff, (uBackColor>>8)&0xff, (uBackColor>>0)&0xff));
      }

      if (uForeColor != 0xffffffff){
        WC->SetForeColor(LKColor((uForeColor>>16)&0xff, (uForeColor>>8)&0xff, (uForeColor>>0)&0xff));
      }

      if (!Visible){
        WC->SetVisible(Visible);
      }

      if (Caption[0] != '\0'){
        WC->SetCaption(LKGetText(Caption));
      }

      if (Border != 0){
        WC->SetBorderKind(Border); 
      }

    }

  }

}

