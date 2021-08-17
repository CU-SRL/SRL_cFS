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
** File: aimu_lsm6ds33_events.h 
**
** Purpose: 
**  Define LSM6DS33 App Events IDs
**
** Notes: App Events will be the same --> just different names. Device events tbd.
**
**
*************************************************************************/
#ifndef _aimu_lsm6ds33_events_h_
#define _aimu_lsm6ds33_events_h_


#define AIMU_LSM6DS33_RESERVED_EID              0
#define AIMU_LSM6DS33_STARTUP_INF_EID           1 
#define AIMU_LSM6DS33_COMMAND_ERR_EID           2
#define AIMU_LSM6DS33_COMMANDNOP_INF_EID        3 
#define AIMU_LSM6DS33_COMMANDRST_INF_EID        4
#define AIMU_LSM6DS33_INVALID_MSGID_ERR_EID     5 
#define AIMU_LSM6DS33_LEN_ERR_EID               6 

/* DEVICE SPECFIC EVENTS --> Need to update for device

#define MPL3115A2_DEVICE_INIT_ERR_EID                        20
#define MPL3115A2_FAILED_CHANGE_TO_STANDBY_MODE_ERR_EID      21
#define MPL3115A2_RATE_SWITCH_ERR_EID                        22
#define MPL3115A2_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID       23
#define MPL3115A2_ENABLE_EVENTS_ERR_EID                      24
#define MPL3115A2_REGISTERS_READ_ERR_EID                     25
#define MPL3115A2_DATA_DBG_EID                               26
*/

#endif /* _aimu_lsm6ds33_events_h_ */

/************************/
/*  End of File Comment */
/************************/
