/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2017 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "ansc_platform.h"
#include "cosa_apis_ripe_atlas.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"
#include <syscfg/syscfg.h>

#include "ripe_atlas.h"

BOOL AtlasRipe_GetParamBoolValue(ANSC_HANDLE hInsContext,
                                  char* pParamName,
                                  BOOL* pBool) {
	debug_trace();

	char buffer[8] = {0};

	if ( AnscEqualString(pParamName, "Enable", TRUE) ) 
	{
		*pBool = ripeatlas_get_enable();
		return TRUE;
	}

	return FALSE;
}

BOOL AtlasRipe_SetParamBoolValue(ANSC_HANDLE hInsContext,
                                  char* pParamName,
                                  BOOL  bValue) {

	debug_printf("%d\n", bValue);

	char buffer[8] = {0};
	bool new_value = (bValue == TRUE ? true : false);
	bool old_value = ripeatlas_get_enable();

	if (AnscEqualString(pParamName, "Enable", TRUE)) {
		if (new_value != old_value)
			ripeatlas_set_enable(new_value);
		return TRUE;
	}
	return FALSE;
}

/**
  AtlasRipe_GetParamStringValue

  return:     0 = SUCCESS; -1 = FAILURE; 1 = NEW_SIZE;

**********************************************************************/
int AtlasRipe_GetParamStringValue	( 	ANSC_HANDLE                 hInsContext,
									  	char*                       pParamName,
										char*                       pValue,
										ULONG*                      pUlSize )
{
	if( AnscEqualString(pParamName, "Key", TRUE)) {
		ripeatlas_get_key(pValue, *pUlSize);
		return 0;
	}
	else if( AnscEqualString(pParamName, "Status", TRUE)) {
		ripeatlas_get_status(pValue, *pUlSize);
		return 0;
	}
	return -1;
}


ULONG AtlasRipe_Commit (ANSC_HANDLE hInsContext) {
	debug_trace();
	return 0;
}

ULONG AtlasRipe_Rollback (ANSC_HANDLE hInsContext) {
	debug_trace();
	return 0;
}
