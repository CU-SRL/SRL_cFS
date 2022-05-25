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
** File: sample_app_msg.h 
**
** Purpose: 
**  Define SAMPLE App  Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _h3lis100dl_v2_msg_h_
#define _h3lis100dl_v2_msg_h_

/*
** SAMPLE App command codes
*/
#define H3LIS100DL_V2_NOOP_CC                 0
#define H3LIS100DL_V2_RESET_COUNTERS_CC       1

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} H3LIS100DL_V2_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App housekeeping)
*/
typedef struct 
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              h3lis100dl_v2_command_error_count;
    uint8              h3lis100dl_v2_command_count;
    uint8              spare[2];

}   OS_PACK h3lis100dl_v2_hk_tlm_t  ;

#define H3LIS100DL_V2_HK_TLM_LNGTH   sizeof ( h3lis100dl_v2_hk_tlm_t )

#endif /* _sample_app_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
