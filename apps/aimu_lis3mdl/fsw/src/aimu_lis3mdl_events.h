/************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.7"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: aimu_lis3mdl_events.h 
**
** Purpose: 
**  Define LIS3MDL App Events IDs
**
** Notes: App Events will be the same --> just different names. Device events tbd.
**
**
*************************************************************************/
#ifndef _aimu_lis3mdl_events_h_
#define _aimu_lis3mdl_events_h_


#define AIMU_LIS3MDL_RESERVED_EID              0
#define AIMU_LIS3MDL_STARTUP_INF_EID           1 
#define AIMU_LIS3MDL_COMMAND_ERR_EID           2
#define AIMU_LIS3MDL_COMMANDNOP_INF_EID        3 
#define AIMU_LIS3MDL_COMMANDRST_INF_EID        4
#define AIMU_LIS3MDL_INVALID_MSGID_ERR_EID     5 
#define AIMU_LIS3MDL_LEN_ERR_EID               6 

/* DEVICE SPECFIC EVENTS */

#define AIMU_LIS3MDL_FAILED_FULL_SCALE_CHANGE                   20
#define AIMU_LIS3MDL_FAIL_ACTIVATE_TEMP_EID                     21
#define AIMU_LIS3MDL_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID       22
#define AIMU_LIS3MDL_ACTIVE_ZUHP_EID                            23
#define AIMU_LIS3MDL_REGISTERS_READ_ERR_EID                     25
#define AIMU_LIS3MDL_DATA_DBG_EID                               26

#endif /* _aimu_lis3mdl_events_h_ */

/************************/
/*  End of File Comment */
/************************/
