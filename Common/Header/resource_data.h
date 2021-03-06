/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 * 
 * File:   resource_data.h
 * Author: Bruno de Lacheisserie
 * 
 * Created on 18 septembre 2014, 13:34
 */
#ifndef resource_data_h
#define	resource_data_h

#include "Util/ConstBuffer.hpp"

// for avoid uneeded copy when data is string, all resource are null termined!

#define BIN_DATA(_NAME) \
    extern const unsigned char _NAME##_begin[]; \
    extern const unsigned char _NAME##_end[]; \
    extern const unsigned int _NAME##_size;

// XMLDIALOG
#ifndef WIN32_RESOURCE

BIN_DATA(IDR_XML_AIRSPACE)
BIN_DATA(IDR_XML_AIRSPACECOLOURS)
BIN_DATA(IDR_XML_MULTISELECTLIST)
BIN_DATA(IDR_XML_MULTISELECTLIST_L)
#ifdef HAVE_HATCHED_BRUSH
BIN_DATA(IDR_XML_AIRSPACEPATTERNS)
BIN_DATA(IDR_XML_AIRSPACEPATTERNS_L)
#endif
BIN_DATA(IDR_XML_ANALYSIS)
BIN_DATA(IDR_XML_BASICSETTINGS)
BIN_DATA(IDR_XML_BASICSETTINGS_L)
BIN_DATA(IDR_XML_CHECKLIST)
BIN_DATA(IDR_XML_FONTEDIT)
BIN_DATA(IDR_XML_LOGGERREPLAY)
BIN_DATA(IDR_XML_STARTUP)
BIN_DATA(IDR_XML_DUALPROFILE)
BIN_DATA(IDR_XML_ORACLE)
BIN_DATA(IDR_XML_ORACLE_L)
BIN_DATA(IDR_XML_FLYSIM_P)
BIN_DATA(IDR_XML_FLYSIM_L)
BIN_DATA(IDR_XML_STATUS)
BIN_DATA(IDR_XML_TASKCALCULATOR)
BIN_DATA(IDR_XML_TASKOVERVIEW)
BIN_DATA(IDR_XML_TASKWAYPOINT)
BIN_DATA(IDR_XML_WAYPOINTDETAILS)
BIN_DATA(IDR_XML_WAYPOINTSELECT)
BIN_DATA(IDR_XML_WAYPOINTQUICK)
BIN_DATA(IDR_XML_WAYPOINTQUICK_P)
BIN_DATA(IDR_XML_WINDSETTINGS)
BIN_DATA(IDR_XML_STARTTASK)
BIN_DATA(IDR_XML_TIMEGATES)
BIN_DATA(IDR_XML_TOPOLOGY)
BIN_DATA(IDR_XML_CUSTOMKEYS)
BIN_DATA(IDR_XML_BOTTOMBAR)
BIN_DATA(IDR_XML_INFOPAGES)
BIN_DATA(IDR_XML_WAYPOINTTERRAIN)
BIN_DATA(IDR_XML_LKAIRSPACEWARNING)
BIN_DATA(IDR_XML_HELP)
BIN_DATA(IDR_XML_TEXTENTRY_KEYBOARD)
BIN_DATA(IDR_XML_TEXTENTRY_KEYBOARD_L)
BIN_DATA(IDR_XML_TEAMCODE)
BIN_DATA(IDR_XML_STARTPOINT)
BIN_DATA(IDR_XML_WAYPOINTEDIT)
BIN_DATA(IDR_XML_TARGET)
BIN_DATA(IDR_XML_TASKRULES)
BIN_DATA(IDR_XML_AIRSPACEDETAILS)
BIN_DATA(IDR_XML_LKTRAFFICDETAILS)
BIN_DATA(IDR_XML_THERMALDETAILS)
BIN_DATA(IDR_XML_CHECKLIST_L)
BIN_DATA(IDR_XML_WAYPOINTDETAILS_L)
BIN_DATA(IDR_XML_ANALYSIS_L)
BIN_DATA(IDR_XML_LKAIRSPACEWARNING_L)
BIN_DATA(IDR_XML_HELP_L)
BIN_DATA(IDR_XML_AIRSPACE_L)
BIN_DATA(IDR_XML_AIRSPACECOLOURS_L)
BIN_DATA(IDR_XML_STARTPOINT_L)
BIN_DATA(IDR_XML_WAYPOINTSELECT_L)
BIN_DATA(IDR_XML_TASKOVERVIEW_L)
BIN_DATA(IDR_XML_TASKWAYPOINT_L)
BIN_DATA(IDR_XML_STARTUP_L)
BIN_DATA(IDR_XML_DUALPROFILE_L)
BIN_DATA(IDR_XML_WAYPOINTEDIT_L)
BIN_DATA(IDR_XML_AIRSPACESELECT_P)
BIN_DATA(IDR_XML_AIRSPACESELECT_L)
BIN_DATA(IDR_XML_TARGET_L)
BIN_DATA(IDR_XML_COMBOPICKER_L)
BIN_DATA(IDR_XML_COMBOPICKER)
BIN_DATA(IDR_XML_PROFILES)
BIN_DATA(IDR_XML_AIRSPACEWARNINGPARAMS)
BIN_DATA(IDR_XML_NETWORK_L)
BIN_DATA(IDR_XML_NETWORK_P)

BIN_DATA(IDR_XML_CONFIGURATION_P)
BIN_DATA(IDR_XML_CONFIGURATION_L)
BIN_DATA(IDR_XML_CONFIGAIRCRAFT_P)
BIN_DATA(IDR_XML_CONFIGAIRCRAFT_L)
BIN_DATA(IDR_XML_CONFIGPILOT_P)
BIN_DATA(IDR_XML_CONFIGPILOT_L)
BIN_DATA(IDR_XML_CONFIGDEVICE_P)
BIN_DATA(IDR_XML_CONFIGDEVICE_L)

BIN_DATA(IDR_XML_NUMENTRY_KEYBOARD_L)
BIN_DATA(IDR_XML_NUMENTRY_KEYBOARD)

BIN_DATA(IDR_XML_WAYPOINTEDITUTM)
BIN_DATA(IDR_XML_WAYPOINTEDITUTM_L)

BIN_DATA(IDR_XML_CUSTOMMENU)
BIN_DATA(IDR_XML_MULTIMAPS)

BIN_DATA(IDR_XML_DEVCPROBE)

BIN_DATA(IDR_XML_BLUETOOTH)
BIN_DATA(IDR_XML_BLUETOOTH_L)

BIN_DATA(IDR_XML_IGCFILE)
BIN_DATA(IDR_XML_IGCFILE_L)

BIN_DATA(IDR_XML_BLUEFLYCONFIG_L)
BIN_DATA(IDR_XML_BLUEFLYCONFIG)

BIN_DATA(IDR_XML_PROGRESS_P)
BIN_DATA(IDR_XML_PROGRESS_L)

BIN_DATA(IDR_RASTER_EGM96S)

#endif

#ifndef WIN32
// on win32 platform, Bitmap can't be in unix style resource.
BIN_DATA(IDB_EMPTY)
BIN_DATA(IDB_TOWN)
BIN_DATA(IDB_LKSMALLTOWN)
BIN_DATA(IDB_LKVERYSMALLTOWN)

#ifndef DISABLEAUDIO
   
    BIN_DATA(IDR_WAV_MM0)
    BIN_DATA(IDR_WAV_MM1)
    BIN_DATA(IDR_WAV_MM2)
    BIN_DATA(IDR_WAV_MM3)
    BIN_DATA(IDR_WAV_MM4)
    BIN_DATA(IDR_WAV_MM5)
    BIN_DATA(IDR_WAV_MM6)
    
    BIN_DATA(IDR_WAV_DRIP)
    BIN_DATA(IDR_WAV_CLICK)
    BIN_DATA(IDR_WAV_HIGHCLICK)
    
    BIN_DATA(IDR_WAV_TONE1)
    BIN_DATA(IDR_WAV_TONE2)
    BIN_DATA(IDR_WAV_TONE3)
    BIN_DATA(IDR_WAV_TONE4)
    BIN_DATA(IDR_WAV_TONE7)
    
    BIN_DATA(IDR_WAV_BTONE2)
    BIN_DATA(IDR_WAV_BTONE4)
    BIN_DATA(IDR_WAV_BTONE5)
    BIN_DATA(IDR_WAV_BTONE6)
    BIN_DATA(IDR_WAV_BTONE7)

	BIN_DATA(IDR_WAV_OVERTONE0)
    BIN_DATA(IDR_WAV_OVERTONE1)
    BIN_DATA(IDR_WAV_OVERTONE2)
    BIN_DATA(IDR_WAV_OVERTONE3)
    BIN_DATA(IDR_WAV_OVERTONE4)
    BIN_DATA(IDR_WAV_OVERTONE5)
    BIN_DATA(IDR_WAV_OVERTONE6)
    BIN_DATA(IDR_WAV_OVERTONE7)
#endif
        
#endif

#include <tchar.h>
#include "resource.h"
#define RESSOURCE_ID(_NAME)  #_NAME
#define NAMED_RESSOURCE(_NAME) { _TEXT(RESSOURCE_ID(_NAME)) , ConstBuffer<void>(_NAME##_begin, _NAME##_size) }

static const struct {
    const TCHAR * szName;
    ConstBuffer<void> data;
} named_resources[] = {
#ifndef WIN32_RESOURCE    
    NAMED_RESSOURCE(IDR_XML_AIRSPACE),
    NAMED_RESSOURCE(IDR_XML_AIRSPACECOLOURS),
    NAMED_RESSOURCE(IDR_XML_MULTISELECTLIST),
    NAMED_RESSOURCE(IDR_XML_MULTISELECTLIST_L),
#ifdef HAVE_HATCHED_BRUSH
    NAMED_RESSOURCE(IDR_XML_AIRSPACEPATTERNS),
    NAMED_RESSOURCE(IDR_XML_AIRSPACEPATTERNS_L),
#endif
    NAMED_RESSOURCE(IDR_XML_ANALYSIS),
    NAMED_RESSOURCE(IDR_XML_BASICSETTINGS),
    NAMED_RESSOURCE(IDR_XML_BASICSETTINGS_L),
    NAMED_RESSOURCE(IDR_XML_CHECKLIST),
    NAMED_RESSOURCE(IDR_XML_FONTEDIT),
    NAMED_RESSOURCE(IDR_XML_LOGGERREPLAY),
    NAMED_RESSOURCE(IDR_XML_STARTUP),
    NAMED_RESSOURCE(IDR_XML_DUALPROFILE),
    NAMED_RESSOURCE(IDR_XML_ORACLE),
    NAMED_RESSOURCE(IDR_XML_ORACLE_L),
    NAMED_RESSOURCE(IDR_XML_FLYSIM_P),
    NAMED_RESSOURCE(IDR_XML_FLYSIM_L),
    NAMED_RESSOURCE(IDR_XML_STATUS),
    NAMED_RESSOURCE(IDR_XML_TASKCALCULATOR),
    NAMED_RESSOURCE(IDR_XML_TASKOVERVIEW),
    NAMED_RESSOURCE(IDR_XML_TASKWAYPOINT),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTDETAILS),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTSELECT),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTQUICK),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTQUICK_P),
    NAMED_RESSOURCE(IDR_XML_WINDSETTINGS),
    NAMED_RESSOURCE(IDR_XML_STARTTASK),
    NAMED_RESSOURCE(IDR_XML_TIMEGATES),
    NAMED_RESSOURCE(IDR_XML_TOPOLOGY),
    NAMED_RESSOURCE(IDR_XML_CUSTOMKEYS),
    NAMED_RESSOURCE(IDR_XML_BOTTOMBAR),
    NAMED_RESSOURCE(IDR_XML_INFOPAGES),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTTERRAIN),
    NAMED_RESSOURCE(IDR_XML_LKAIRSPACEWARNING),
    NAMED_RESSOURCE(IDR_XML_HELP),
    NAMED_RESSOURCE(IDR_XML_TEXTENTRY_KEYBOARD),
    NAMED_RESSOURCE(IDR_XML_TEXTENTRY_KEYBOARD_L),
    NAMED_RESSOURCE(IDR_XML_TEAMCODE),
    NAMED_RESSOURCE(IDR_XML_STARTPOINT),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTEDIT),
    NAMED_RESSOURCE(IDR_XML_TARGET),
    NAMED_RESSOURCE(IDR_XML_TASKRULES),
    NAMED_RESSOURCE(IDR_XML_AIRSPACEDETAILS),
    NAMED_RESSOURCE(IDR_XML_LKTRAFFICDETAILS),
    NAMED_RESSOURCE(IDR_XML_THERMALDETAILS),
    NAMED_RESSOURCE(IDR_XML_CHECKLIST_L),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTDETAILS_L),
    NAMED_RESSOURCE(IDR_XML_ANALYSIS_L),
    NAMED_RESSOURCE(IDR_XML_LKAIRSPACEWARNING_L),
    NAMED_RESSOURCE(IDR_XML_HELP_L),
    NAMED_RESSOURCE(IDR_XML_AIRSPACE_L),
    NAMED_RESSOURCE(IDR_XML_AIRSPACECOLOURS_L),
    NAMED_RESSOURCE(IDR_XML_STARTPOINT_L),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTSELECT_L),
    NAMED_RESSOURCE(IDR_XML_TASKOVERVIEW_L),
    NAMED_RESSOURCE(IDR_XML_TASKWAYPOINT_L),
    NAMED_RESSOURCE(IDR_XML_STARTUP_L),
    NAMED_RESSOURCE(IDR_XML_DUALPROFILE_L),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTEDIT_L),
    NAMED_RESSOURCE(IDR_XML_AIRSPACESELECT_P),
    NAMED_RESSOURCE(IDR_XML_AIRSPACESELECT_L),
    NAMED_RESSOURCE(IDR_XML_TARGET_L),
    NAMED_RESSOURCE(IDR_XML_COMBOPICKER_L),
    NAMED_RESSOURCE(IDR_XML_COMBOPICKER),
    NAMED_RESSOURCE(IDR_XML_PROFILES),
    NAMED_RESSOURCE(IDR_XML_AIRSPACEWARNINGPARAMS),
    NAMED_RESSOURCE(IDR_XML_NETWORK_L),
    NAMED_RESSOURCE(IDR_XML_NETWORK_P),

    NAMED_RESSOURCE(IDR_XML_CONFIGURATION_P),
    NAMED_RESSOURCE(IDR_XML_CONFIGURATION_L),
    NAMED_RESSOURCE(IDR_XML_CONFIGAIRCRAFT_P),
    NAMED_RESSOURCE(IDR_XML_CONFIGAIRCRAFT_L),
    NAMED_RESSOURCE(IDR_XML_CONFIGPILOT_P),
    NAMED_RESSOURCE(IDR_XML_CONFIGPILOT_L),
    NAMED_RESSOURCE(IDR_XML_CONFIGDEVICE_P),
    NAMED_RESSOURCE(IDR_XML_CONFIGDEVICE_L),

    NAMED_RESSOURCE(IDR_XML_NUMENTRY_KEYBOARD_L),
    NAMED_RESSOURCE(IDR_XML_NUMENTRY_KEYBOARD),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTEDITUTM),
    NAMED_RESSOURCE(IDR_XML_WAYPOINTEDITUTM_L),
    NAMED_RESSOURCE(IDR_XML_CUSTOMMENU),
    NAMED_RESSOURCE(IDR_XML_MULTIMAPS),
    NAMED_RESSOURCE(IDR_XML_DEVCPROBE),
    NAMED_RESSOURCE(IDR_XML_BLUETOOTH),
    NAMED_RESSOURCE(IDR_XML_BLUETOOTH_L),
    NAMED_RESSOURCE(IDR_XML_IGCFILE),
    NAMED_RESSOURCE(IDR_XML_IGCFILE_L),
    NAMED_RESSOURCE(IDR_XML_BLUEFLYCONFIG_L),
    NAMED_RESSOURCE(IDR_XML_BLUEFLYCONFIG),
    NAMED_RESSOURCE(IDR_XML_PROGRESS_P),
    NAMED_RESSOURCE(IDR_XML_PROGRESS_L),
#endif

#ifndef WIN32
    // on win32 platform, Bitmap can't be in unix style resource.
    NAMED_RESSOURCE(IDB_EMPTY),
    NAMED_RESSOURCE(IDB_TOWN),
    NAMED_RESSOURCE(IDB_LKSMALLTOWN),
    NAMED_RESSOURCE(IDB_LKVERYSMALLTOWN),

#ifndef DISABLEAUDIO
   
    NAMED_RESSOURCE(IDR_WAV_MM0),
    NAMED_RESSOURCE(IDR_WAV_MM1),
    NAMED_RESSOURCE(IDR_WAV_MM2),
    NAMED_RESSOURCE(IDR_WAV_MM3),
    NAMED_RESSOURCE(IDR_WAV_MM4),
    NAMED_RESSOURCE(IDR_WAV_MM5),
    NAMED_RESSOURCE(IDR_WAV_MM6),
    
    NAMED_RESSOURCE(IDR_WAV_DRIP),
    NAMED_RESSOURCE(IDR_WAV_CLICK),
    NAMED_RESSOURCE(IDR_WAV_HIGHCLICK),
    
    NAMED_RESSOURCE(IDR_WAV_TONE1),
    NAMED_RESSOURCE(IDR_WAV_TONE2),
    NAMED_RESSOURCE(IDR_WAV_TONE3),
    NAMED_RESSOURCE(IDR_WAV_TONE4),
    NAMED_RESSOURCE(IDR_WAV_TONE7),
    
    NAMED_RESSOURCE(IDR_WAV_BTONE2),
    NAMED_RESSOURCE(IDR_WAV_BTONE4),
    NAMED_RESSOURCE(IDR_WAV_BTONE5),
    NAMED_RESSOURCE(IDR_WAV_BTONE6),
    NAMED_RESSOURCE(IDR_WAV_BTONE7),

	NAMED_RESSOURCE(IDR_WAV_OVERTONE0),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE1),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE2),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE3),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE4),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE5),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE6),
    NAMED_RESSOURCE(IDR_WAV_OVERTONE7),
#endif

#endif

};

inline ConstBuffer<void> GetNamedResource(const TCHAR* szName) {
    for (auto Resource : named_resources) {
        if (_tcscmp(Resource.szName, szName) == 0) {
            return Resource.data;
        }
    }
    assert(false); // ressource not found;
    return ConstBuffer<void>::Null();
}

inline const char* GetNamedResourceString(const TCHAR* szName) {
    return (const char*)GetNamedResource(szName).data;
}


#endif	/* resource_data_h */

