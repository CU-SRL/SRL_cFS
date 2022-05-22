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
** File: sample_app.h
**
** Purpose:
**   This file is main hdr file for the SAMPLE application.
**
**
*******************************************************************************/

#ifndef _aimu_lsm6ds33_v2_app_h_
#define _aimu_lsm6ds33_v2_app_h_

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>

/***********************************************************************/

#define AIMU_LSM6DS33_V2_PIPE_DEPTH                     32
#define AIMU_LSM6DS33_V2_I2C_BUFFER                     /* fill */

/************************************************************************
** Type Definitions
*************************************************************************/

typedef struct
{
    uint8_t status[1];
    uint8_t buffer[AIMU_LSM6DS33_V2_I2C_BUFFER];
}   AIMU_LSM6DS33_V2_DATA;


/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (SAMPLE_AppMain), these
**       functions are not called from any other source module.
*/
void AIMU_LSM6DS33_V2_AppMain(void);
void AIMU_LSM6DS33_V2_AppInit(void);
void AIMU_LSM6DS33_V2_DeviceInit(int i2cbus, aimu_lsm6ds33_v2_hk_tlm_t* AIMU_LSM6DS33_V2_HkTelemetryPkt);
void AIMU_LSM6DS33_V2_ProcessCommandPacket(void);
void AIMU_LSM6DS33_V2_ProcessGroundCommand(void);
void AIMU_LSM6DS33_V2_ProcessDataPacket(int i2cbus, aimu_lsm6ds33_v2_hk_tlm_t* AIMU_LSM6DS33_V2_HkTelemetryPkt, aimu_lsm6ds33_v2_data_tlm_t* AIMU_LSM6DS33_V2_DataTelemetryPkt);
void AIMU_LSM6DS33_V2_ReportHousekeeping(void);
void AIMU_LSM6DS33_V2_ResetCounters(void);

bool AIMU_LSM6DS33_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

#endif /* _sample_app_h_ */
