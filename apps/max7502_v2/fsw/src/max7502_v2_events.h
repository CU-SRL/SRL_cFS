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
** File: max7502_v2_events.h 
**
** Purpose: 
**  Define MAX7502_V2 App Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _max7502_v2_events_h_
#define _max7502_v2_events_h_


#define MAX7502_V2_RESERVED_EID              0
#define MAX7502_V2_STARTUP_INF_EID           1 
#define MAX7502_V2_COMMAND_ERR_EID           2
#define MAX7502_V2_COMMANDNOP_INF_EID        3 
#define MAX7502_V2_COMMANDRST_INF_EID        4
#define MAX7502_V2_INVALID_MSGID_ERR_EID     5 
#define MAX7502_V2_LEN_ERR_EID               6 

//need to device specfic events. the data sheet is kinda lacking tbh

#endif /* _max7502_v2_events_h_ */

/************************/
/*  End of File Comment */
/************************/
