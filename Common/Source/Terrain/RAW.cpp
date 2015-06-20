/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id$
*/

#include "externs.h"
#include "RasterTerrain.h"
#include "utils/openzip.h"
#include "OS/Memory.h"

#ifdef __MINGW32__
#define int_fast8_t jas_int_fast8_t
#endif

////// Field access ////////////////////////////////////////////////////


bool RasterMap::Open(const TCHAR* zfilename) {
  ZZIP_FILE *fpTerrain;

  max_field_value = 0;
  terrain_valid = false;

  if (_tcslen(zfilename)<=0) 
    return false;

  StartupStore(_T(". Terrain Open RasterMapRaw <%s>%s"),zfilename,NEWLINE);

  fpTerrain = openzip(zfilename, "rb");
  if (fpTerrain == NULL) {
	StartupStore(_T(". Terrain RasterMapRaw Open failed%s"),NEWLINE);
	return false;
  }

  DWORD dwBytesRead;
  dwBytesRead = zzip_fread(&TerrainInfo, 1, sizeof(TERRAIN_INFO), fpTerrain);
    
  if (dwBytesRead != sizeof(TERRAIN_INFO)) {
	StartupStore(_T("------ Terrain read first failed, invalid header%s"),NEWLINE);
	zzip_fclose(fpTerrain);
	return false;
  }
    
  size_t nsize = TerrainInfo.Rows*TerrainInfo.Columns;
  #ifdef HC_DMALLOC
  StartupStore(_T(". Terrain size is %ld, max hblock %lu %s"),
    (long)nsize*sizeof(short), CheckMaxHeapBlock(), NEWLINE);
  #else
  StartupStore(_T(". Terrain size is %ld%s"),
    (long)nsize*sizeof(short), NEWLINE);
  #endif

  if (CheckFreeRam()>(nsize*sizeof(short)+5000000U)) {
    // make sure there is 5 meg of ram left after allocating space
    TerrainMem = (short*)malloc(sizeof(short)*nsize);
  } else {
    zzip_fclose(fpTerrain);
    StartupStore(_T(".... Load Terrain FAILED: Not enough memory (free=%u need=%u+5M)!\n"),
            (unsigned int)CheckFreeRam(), (unsigned int)(nsize*sizeof(short)));
    TerrainMem = NULL;
    return false;
  }
    
  if (!TerrainMem) {
    OutOfMemory(_T(__FILE__),__LINE__);
    zzip_fclose(fpTerrain);
    terrain_valid = false;
  } else {
    dwBytesRead = zzip_fread(TerrainMem, 1, nsize*sizeof(short), fpTerrain);
    
    for (size_t i=0; i< nsize; ++i) {
      max_field_value = max(TerrainMem[i], max_field_value);
    }
    zzip_fclose(fpTerrain);
    terrain_valid = true;
  }
      
  if (!TerrainInfo.StepSize) {
    StartupStore(_T("... ERROR Terrain StepSize failure\n"));
    terrain_valid = false;
    zzip_fclose(fpTerrain);
    Close();
  }
  return terrain_valid;
}


void RasterMap::Close(void) {
  terrain_valid = false;
  if (TerrainMem) {
    free(TerrainMem); TerrainMem = NULL;
  }
}



