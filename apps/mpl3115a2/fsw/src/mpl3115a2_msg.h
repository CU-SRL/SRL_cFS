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
** File: mpl3115a2_msg.h 
**
** Purpose: 
**  Define MPL3115A2 App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _mpl3115a2_msg_h_
#define _mpl3115a2_msg_h_

/*
** MPL3115A2 App command codes
*/
#define MPL3115A2_NOOP_CC                 0
#define MPL3115A2_RESET_COUNTERS_CC       1

// Device Command Codes
#define MPL3115A2_INIT          10 // Should also be able to be down automatically when flight ready
#define MPL3115A2_SHUTDOWN      11
#define MPL3115A2_RESET         12
#define MPL3115A2_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} mpl3115a2_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (MPL3115A2 App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8               mpl3115a2_command_error_count;
    uint8               mpl3115a2_command_count;
    uint8               mpl3115a2_error_count;
    uint8               mpl3115a2_device_count;
    uint8               mpl3115a2_device_error_count;

}   OS_PACK mpl3115a2_hk_tlm_t  ;

typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float               MPL3115A2_ALTITUDE;
    float               MPL3115A2_TEMPERATURE;

}   OS_PACK mpl3115a2_data_tlm_t  ;

#define MPL3115A2_HK_TLM_LNGTH   sizeof ( mpl3115a2_hk_tlm_t )
#define MPL3115A2_DATA_TLM_LNGTH   sizeof ( mpl3115a2_data_tlm_t )

#endif /* _mpl3115a2_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
