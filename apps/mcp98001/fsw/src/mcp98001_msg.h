/*******************************************************************************
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
** File: mcp98001_msg.h 
**
** Purpose: 
**  Define MCP98001 App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _mcp98001_msg_h_
#define _mcp98001_msg_h_

/*
** MCP98001 App command codes
*/
#define MCP98001_NOOP_CC                 0
#define MCP98001_RESET_COUNTERS_CC       1

// Device Command Codes
#define MCP98001_INIT          10 // Should also be able to be down automatically when flight ready
#define MCP98001_SHUTDOWN      11
#define MCP98001_RESET         12
#define MCP98001_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} mcp98001_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (MCP98001 App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8               mcp98001_command_error_count;
    uint8               mcp98001_command_count;
    uint8               mcp98001_error_count;
    uint8               mcp98001_device_count;
    uint8               mcp98001_device_error_count;

}   OS_PACK mcp98001_hk_tlm_t  ;

typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float               MCP98001_TEMPERATURE;

}   OS_PACK mcp98001_data_tlm_t  ;

#define MCP98001_HK_TLM_LNGTH   sizeof ( mcp98001_hk_tlm_t )
#define MCP98001_DATA_TLM_LNGTH   sizeof ( mcp98001_data_tlm_t )

#endif /* _mcp98001_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
