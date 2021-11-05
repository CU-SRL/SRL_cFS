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
** File: aimu_lsm6ds33_msg.h 
**
** Purpose: 
**  Define LSM6DS33 App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _aimu_lsm6ds33_msg_h_
#define _aimu_lsm6ds33_msg_h_

/*
** LSM6DS33 App command codes
*/
#define AIMU_LSM6DS33_NOOP_CC                 0
#define AIMU_LSM6DS33_RESET_COUNTERS_CC       1

//Device Command Codes 
#define AIMU_LSM6DS33_INIT          10 // Should also be able to be down automatically when flight ready
#define AIMU_LSM6DS33_SHUTDOWN      11
#define AIMU_LSM6DS33_RESET         12
#define AIMU_LSM6DS33_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} aimu_lsm6ds33_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (LSM6DS33 App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32              AppStatus;
    uint8               aimu_lsm6ds33_command_error_count;
    uint8               aimu_lsm6ds33_command_count;
    uint8               aimu_lsm6ds33_error_count;
    uint8               aimu_lsm6ds33_device_count;
    uint8               aimu_lsm6ds33_device_error_count;

}   OS_PACK aimu_lsm6ds33_hk_tlm_t  ;

typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float              AIMU_LSM6DS33_ACCELERATIONX;
    float              AIMU_LSM6DS33_ACCELERATIONY;
    float              AIMU_LSM6DS33_ACCELERATIONZ;
    float              AIMU_LSM6DS33_ANGULAR_RATEX;
    float              AIMU_LSM6DS33_ANGULAR_RATEY;
    float              AIMU_LSM6DS33_ANGULAR_RATEZ;

}   OS_PACK aimu_lsm6ds33_data_tlm_t  ;

#define AIMU_LSM6DS33_HK_TLM_LNGTH   sizeof ( aimu_lsm6ds33_hk_tlm_t )
#define AIMU_LSM6DS33_DATA_TLM_LNGTH   sizeof ( aimu_lsm6ds33_data_tlm_t )

#endif /* _aimu_lsm6ds33_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
