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
** File: aimu_lps25h.h
**
** Purpose:
**   This file is main hdr file for the LPS25H application.
**
** Notes: Naming conventions and device specific variables changed.
*******************************************************************************/

#ifndef _aimu_lps25h_h_
#define _aimu_lps25h_h_

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

#include "aimu_lps25h_msg.h"
#include "aimu_lps25h_registers.h"

/***********************************************************************/

#define AIMU_LPS25H_PIPE_DEPTH           32
#define MAX_BUS                          64

// Define the Data Buffer Size using the Datasheet (59 buffers because going to 3a)
#define AIMU_LPS25H_I2C_BUFFER 0x3B

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the LPS25H
struct AIMU_LPS25H_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[AIMU_LPS25H_I2C_BUFFER];
	
	/* Data Variables
	double acceleration;
	double angular_rate; */
} AIMU_LPS25H;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (AIMU_LPS25H_AppMain), these
**       functions are not called from any other source module.
*/
void AIMU_LPS25H_AppMain(void);
void AIMU_LPS25H_AppInit(void);
void AIMU_LPS25H_ProcessCommandPacket(void);
void AIMU_LPS25H_ProcessGroundCommand(void);
void AIMU_LPS25H_ReportHousekeeping(void);
void AIMU_LPS25H_ResetCounters(void);

bool AIMU_LPS25H_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* LPS25H Accel + Gyro */
bool INIT_AIMU_LPS25H(int I2CBus, aimu_lps25h_hk_tlm_t* AIMU_LPS25H_HkTelemetryPkt);
void PROCESS_AIMU_LPS25H(int i2cbus, aimu_lps25h_hk_tlm_t* AIMU_LPS25H_HkTelemetryPkt, aimu_lps25h_data_tlm_t* AIMU_LPS25H_DataTelemetryPkt);

#endif /* _aimu_lps25h_h_ */
