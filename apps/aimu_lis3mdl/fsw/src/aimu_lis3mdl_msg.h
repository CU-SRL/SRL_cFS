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
** File: aimu_lis3mdl_msg.h 
**
** Purpose: 
**  Define LIS3MDL App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _aimu_lis3mdl_msg_h_
#define _aimu_lis3mdl_msg_h_

/*
** LIS3MDL App command codes
*/
#define AIMU_LIS3MDL_NOOP_CC                 0
#define AIMU_LIS3MDL_RESET_COUNTERS_CC       1

/* Device Command Codes */
#define AIMU_LIS3MDL_INIT          10 // Should also be able to be down automatically when flight ready
#define AIMU_LIS3MDL_SHUTDOWN      11
#define AIMU_LIS3MDL_RESET         12
#define AIMU_LIS3MDL_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} aimu_lis3mdl_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (LIS3MDL App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8               aimu_lis3mdl_command_error_count;
    uint8               aimu_lis3mdl_command_count;
    uint8               aimu_lis3mdl_error_count;
    uint8               aimu_lis3mdl_device_count;
    uint8               aimu_lis3mdl_device_error_count;

}   OS_PACK aimu_lis3mdl_hk_tlm_t  ;

typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint16              AIMU_LIS3MDL_MAGSIGX;
    uint16              AIMU_LIS3MDL_MAGSIGY;
    uint16              AIMU_LIS3MDL_MAGSIGZ;

}   OS_PACK aimu_lis3mdl_data_tlm_t  ;

#define AIMU_LIS3MDL_HK_TLM_LNGTH   sizeof ( aimu_lis3mdl_hk_tlm_t )
#define AIMU_LIS3MDL_DATA_TLM_LNGTH   sizeof ( aimu_lis3mdl_data_tlm_t )

#endif /* _aimu_lis3mdl_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
