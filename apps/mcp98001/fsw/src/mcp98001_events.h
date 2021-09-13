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
** File: mcp98001_events.h 
**
** Purpose: 
**  Define MCP98001 App Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _mcp98001_events_h_
#define _mcp98001_events_h_


#define MCP98001_RESERVED_EID              0
#define MCP98001_STARTUP_INF_EID           1 
#define MCP98001_COMMAND_ERR_EID           2
#define MCP98001_COMMANDNOP_INF_EID        3 
#define MCP98001_COMMANDRST_INF_EID        4
#define MCP98001_INVALID_MSGID_ERR_EID     5 
#define MCP98001_LEN_ERR_EID               6 

/* DEVICE SPECFIC EVENTS */

#define MCP98001_FAILED_TO_CONFIGURE                        20
#define MCP98001_REGISTERS_READ_ERR_EID                     25
#define MCP98001_DATA_DBG_EID                               26


#endif /* _mcp98001_events_h_ */

/************************/
/*  End of File Comment */
/************************/
