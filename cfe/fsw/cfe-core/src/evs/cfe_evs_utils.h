/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
**  Filename: cfe_evs_utils.h
**
**  Title:    Event Services Task and API - Utility functions.
**
**  Purpose:
**            Unit specification for the event services utility functions.
**
**  Contents:
**       I.  macro and constant type definitions
**      II.  EVS utility internal structures
**     III.  function prototypes
**
**  Design Notes:
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**
*/

#ifndef _cfe_evs_utils_
#define _cfe_evs_utils_

/********************* Include Files  ************************/

#include "cfe_evs_task.h"        /* EVS internal definitions */

/* ==============   Section I: Macro and Constant Type Definitions   =========== */

/* ==============   Section II: Internal Structures ============ */     

/* ==============   Section III: Function Prototypes =========== */

/**
 * @brief Obtain the EVS app record for the given ID
 *
 * This only obtains a pointer to where the record should be, it does
 * not check/confirm that the record actually is for the given AppID.
 * Use EVS_AppDataIsMatch() to determine if the record is valid.
 *
 * @sa EVS_AppDataIsMatch()
 *
 * @param[in]   AppID   AppID to find
 * @returns Pointer to app table entry, or NULL if ID is invalid.
 */
EVS_AppData_t *EVS_GetAppDataByID (CFE_ES_ResourceID_t AppID);

/**
 * @brief Obtain the context information for the currently running app
 *
 * Obtains both the AppData record (pointer) and AppID for the current context.
 *
 * @param[out]   AppDataOut     Location to store App Data record pointer
 * @param[out]   AppIDOut       Location to store AppID
 * @returns CFE_SUCCESS if successful, or relevant error code.
 */
int32 EVS_GetCurrentContext (EVS_AppData_t **AppDataOut, CFE_ES_ResourceID_t *AppIDOut);


/**
 * @brief Check if an EVS app record is in use or free/empty
 *
 * This routine checks if the EVS app entry is in use or if it is free
 *
 * As this dereferences fields within the record, global data must be
 * locked prior to invoking this function.
 *
 * @param[in]   AppDataPtr   pointer to app table entry
 * @returns true if the entry is in use/configured, or false if it is free/empty
 */
static inline bool EVS_AppDataIsUsed(EVS_AppData_t *AppDataPtr)
{
    return CFE_ES_ResourceID_IsDefined(AppDataPtr->AppID);
}

/**
 * @brief Get the ID value from an EVS table entry
 *
 * This routine converts the table entry back to an abstract ID.
 *
 * @param[in]   AppDataPtr   pointer to app table entry
 * @returns AppID of entry
 */
static inline CFE_ES_ResourceID_t EVS_AppDataGetID(EVS_AppData_t *AppDataPtr)
{
    /*
     * The initial implementation does not store the ID in the entry;
     * the ID is simply the zero-based index into the table.
     */
    return (AppDataPtr->AppID);
}

/**
 * @brief Marks an EVS table entry as used (not free)
 *
 * This sets the internal field(s) within this entry, and marks
 * it as being associated with the given app ID.
 *
 * As this dereferences fields within the record, global data must be
 * locked prior to invoking this function.
 *
 * @param[in]   AppDataPtr   pointer to app table entry
 * @param[in]   AppID       the app ID of this entry
 */
static inline void EVS_AppDataSetUsed(EVS_AppData_t *AppDataPtr, CFE_ES_ResourceID_t AppID)
{
    AppDataPtr->AppID = AppID;
}

/**
 * @brief Set an EVS table entry free (not used)
 *
 * This clears the internal field(s) within this entry, and allows the
 * memory to be re-used in the future.
 *
 * @param[in]   AppDataPtr   pointer to app table entry
 */
static inline void EVS_AppDataSetFree(EVS_AppData_t *AppDataPtr)
{
    AppDataPtr->AppID = CFE_ES_RESOURCEID_UNDEFINED;
}

/**
 * @brief Check if an EVS record is a match for the given AppID
 *
 * This routine confirms that the previously-located record is valid
 * and matches the expected app ID.
 *
 * @param[in]   AppDataPtr   pointer to app table entry
 * @param[in]   AppID       expected app ID
 * @returns true if the entry matches the given app ID
 */
static inline bool EVS_AppDataIsMatch(EVS_AppData_t *AppDataPtr, CFE_ES_ResourceID_t AppID)
{
    return (AppDataPtr != NULL && CFE_ES_ResourceID_Equal(AppDataPtr->AppID, AppID));
}



int32 EVS_GetApplicationInfo(EVS_AppData_t **AppDataOut, const char *pAppName);

int32 EVS_NotRegistered (EVS_AppData_t *AppDataPtr, CFE_ES_ResourceID_t CallerID);

bool EVS_IsFiltered(EVS_AppData_t *AppDataPtr, uint16 EventID, uint16 EventType);

EVS_BinFilter_t *EVS_FindEventID(int16 EventID, EVS_BinFilter_t *FilterArray);

void EVS_EnableTypes (EVS_AppData_t *AppDataPtr, uint8 BitMask);
void EVS_DisableTypes (EVS_AppData_t *AppDataPtr, uint8 BitMask);

void EVS_GenerateEventTelemetry(EVS_AppData_t *AppDataPtr, uint16 EventID, uint16 EventType,
        const CFE_TIME_SysTime_t *Time, const char *MsgSpec, va_list ArgPtr);

int32 EVS_SendEvent (uint16 EventID, uint16 EventType, const char *Spec, ... );

#endif  /* _cfe_evs_utils_ */
