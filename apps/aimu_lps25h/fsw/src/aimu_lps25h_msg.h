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
** File: aimu_lps25h_msg.h 
**
** Purpose: 
**  Define LIS3MDL App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _aimu_lps25h_msg_h_
#define _aimu_lps25h_msg_h_

/*
** LIS3MDL App command codes
*/
#define AIMU_LPS25H_NOOP_CC                 0
#define AIMU_LPS25H_RESET_COUNTERS_CC       1

/* Device Command Codes */
#define AIMU_LPS25H_INIT          10 // Should also be able to be down automatically when flight ready
#define AIMU_LPS25H_SHUTDOWN      11
#define AIMU_LPS25H_RESET         12
#define AIMU_LPS25H_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} aimu_lps25h_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (LIS3MDL App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32              AppStatus;
    uint8               aimu_lps25h_command_error_count;
    uint8               aimu_lps25h_command_count;
    uint8               aimu_lps25h_error_count;
    uint8               aimu_lps25h_device_count;
    uint8               aimu_lps25h_device_error_count;

}   OS_PACK aimu_lps25h_hk_tlm_t  ;

typedef struct 
{
    uint8                TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float                AIMU_LPS25H_PRESSURE;
    float                AIMU_LPS25H_TEMPERATURE;

}   OS_PACK aimu_lps25h_data_tlm_t  ;

#define AIMU_LPS25H_HK_TLM_LNGTH   sizeof ( aimu_lps25h_hk_tlm_t )
#define AIMU_LPS25H_DATA_TLM_LNGTH   sizeof ( aimu_lps25h_data_tlm_t )

#endif /* _aimu_lps25h_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
