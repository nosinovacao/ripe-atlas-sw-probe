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

#ifndef  _COSA_APIS_NOSECOWIFI_H
#define  _COSA_APIS_NOSECOWIFI_H

#include "slap_definitions.h"

BOOL
AtlasRipe_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
AtlasRipe_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParamName,
        BOOL                        bValue
    );

int
AtlasRipe_GetParamStringValue
	(
		ANSC_HANDLE                 hInsContext,
		char*                       pParamName,
		char*                       pValue,
		ULONG*                      pUlSize
	);

ULONG
AtlasRipe_Commit
	(
		ANSC_HANDLE                 hInsContext
	);

ULONG
AtlasRipe_Rollback
	(
		ANSC_HANDLE                 hInsContext
	);

#endif
