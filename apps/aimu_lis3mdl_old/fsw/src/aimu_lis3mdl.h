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
** File: aimu_lis3mdl.h
**
** Purpose:
**   This file is main hdr file for the LIS3MDL application.
**
** Notes: Naming conventions and device specific variables changed.
*******************************************************************************/

#ifndef _aimu_lis3mdl_h_
#define _aimu_lis3mdl_h_

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

#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
//#include <stropts.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "aimu_lis3mdl_msg.h"
#include "aimu_lis3mdl_registers.h"

/***********************************************************************/

#define AIMU_LIS3MDL_PIPE_DEPTH           32
#define MAX_BUS                            64

// Define the Data Buffer Size using the Datasheet (96 buffers because going to 5f)
#define AIMU_LIS3MDL_I2C_BUFFER 0x60

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the LIS3MDL
struct AIMU_LIS3MDL_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[AIMU_LIS3MDL_I2C_BUFFER];
	
} AIMU_LIS3MDL;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (AIMU_LIS3MDL_AppMain), these
**       functions are not called from any other source module.
*/
void AIMU_LIS3MDL_AppMain(void);
void AIMU_LIS3MDL_AppInit(void);
void AIMU_LIS3MDL_ProcessCommandPacket(void);
void AIMU_LIS3MDL_ProcessGroundCommand(void);
void AIMU_LIS3MDL_ReportHousekeeping(void);
void AIMU_LIS3MDL_ResetCounters(void);

bool AIMU_LIS3MDL_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* LIS3MDL Accel + Gyro */
bool INIT_AIMU_LIS3MDL(int I2CBus, aimu_lis3mdl_hk_tlm_t* AIMU_LIS3MDL_HkTelemetryPkt);
void PROCESS_AIMU_LIS3MDL(int i2cbus, aimu_lis3mdl_hk_tlm_t* AIMU_LIS3MDL_HkTelemetryPkt, aimu_lis3mdl_data_tlm_t* AIMU_LIS3MDL_DataTelemetryPkt);

#endif /* _aimu_lis3mdl_h_ */
