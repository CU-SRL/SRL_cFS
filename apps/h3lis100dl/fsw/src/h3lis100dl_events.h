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
** File: h3lis100dl_events.h 
**
** Purpose: 
**  Define H3LIS100DL App Events IDs
**
** Notes: App Events will be the same --> just different names. Device events tbd.
**
**
*************************************************************************/
#ifndef _h3lis100dl_events_h_
#define _h3lis100dl_events_h_


#define H3LIS100DL_RESERVED_EID              0
#define H3LIS100DL_STARTUP_INF_EID           1 
#define H3LIS100DL_COMMAND_ERR_EID           2
#define H3LIS100DL_COMMANDNOP_INF_EID        3 
#define H3LIS100DL_COMMANDRST_INF_EID        4
#define H3LIS100DL_INVALID_MSGID_ERR_EID     5 
#define H3LIS100DL_LEN_ERR_EID               6 

/* DEVICE SPECFIC EVENTS */

#define H3LIS100DL_DEVICE_INIT_ERR_EID                        20
#define H3LIS100DL_FAILED_ENABLE_AXES                         21
#define H3LIS100DL_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID       22
#define H3LIS100DL_REGISTERS_READ_ERR_EID                     23
#define H3LIS100DL_DATA_DBG_EID                               24


#endif /* _h3lis100dl_events_h_ */

/************************/
/*  End of File Comment */
/************************/
