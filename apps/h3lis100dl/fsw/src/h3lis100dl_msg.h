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
** File: h3lis100dl_msg.h 
**
** Purpose: 
**  Define H3LIS100DL App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _h3lis100dl_msg_h_
#define _h3lis100dl_msg_h_

/*
** H3LIS100DL App command codes
*/
#define H3LIS100DL_NOOP_CC                 0
#define H3LIS100DL_RESET_COUNTERS_CC       1

//Device Command Codes 
#define H3LIS100DL_INIT          10 // Should also be able to be down automatically when flight ready
#define H3LIS100DL_SHUTDOWN      11
#define H3LIS100DL_RESET         12
#define H3LIS100DL_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} h3lis100dl_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (H3LIS100DL App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32              AppStatus;
    uint8               h3lis100dl_command_error_count;
    uint8               h3lis100dl_command_count;
    uint8               h3lis100dl_error_count;
    uint8               h3lis100dl_device_count;
    uint8               h3lis100dl_device_error_count;

}   OS_PACK h3lis100dl_hk_tlm_t  ;

typedef struct 
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float              H3LIS100DL_ACCELERATIONX;
    float              H3LIS100DL_ACCELERATIONY;
    float              H3LIS100DL_ACCELERATIONZ;

}   OS_PACK h3lis100dl_data_tlm_t  ;

#define H3LIS100DL_HK_TLM_LNGTH   sizeof ( h3lis100dl_hk_tlm_t )
#define H3LIS100DL_DATA_TLM_LNGTH   sizeof ( h3lis100dl_data_tlm_t )

#endif /* _h3lis100dl_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
