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
** File: ms5611_events.h 
**
** Purpose: 
**  Define MS5611 App Events IDs
**
** Notes: App Events will be the same --> just different names. Device events tbd.
**
**
*************************************************************************/
#ifndef _ms5611_events_h_
#define _ms5611_events_h_


#define MS5611_RESERVED_EID              0
#define MS5611_STARTUP_INF_EID           1 
#define MS5611_COMMAND_ERR_EID           2
#define MS5611_COMMANDNOP_INF_EID        3 
#define MS5611_COMMANDRST_INF_EID        4
#define MS5611_INVALID_MSGID_ERR_EID     5 
#define MS5611_LEN_ERR_EID               6 

/* DEVICE SPECFIC EVENTS */

#define MS5611_DEVICE_INIT_ERR_EID                        20
#define MS5611_FAILED_TO_ACTIVATE_EID                     21
#define MS5611_SAMPLE_RATE_EID                            22
#define MS5611_REGISTERS_READ_ERR_EID                     25
#define MS5611_DATA_DBG_EID                               26


#endif /* _ms5611_events_h_ */

/************************/
/*  End of File Comment */
/************************/
