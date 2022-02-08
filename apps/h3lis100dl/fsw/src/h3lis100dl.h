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
** File: h3lis100dl.h
**
** Purpose:
**   This file is main hdr file for the H3LIS100DL application.
**
** Notes: Naming conventions and device specific variables changed.
*******************************************************************************/

#ifndef _h3lis100dl_h_
#define _h3lis100dl_h_

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

#include "h3lis100dl_msg.h"
#include "h3lis100dl_registers.h"

/***********************************************************************/

#define H3LIS100DL_PIPE_DEPTH           32
#define MAX_BUS                            64

// Define the Data Buffer Size using the Datasheet (55 buffers because going to 0x37)
#define H3LIS100DL_I2C_BUFFER 0x38

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the H3LIS100DL
struct H3LIS100DL_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[H3LIS100DL_I2C_BUFFER];
	
	/* Data Variables
	double acceleration;
	double angular_rate; */
} H3LIS100DL;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (H3LIS100DL_AppMain), these
**       functions are not called from any other source module.
*/
void H3LIS100DL_AppMain(void);
void H3LIS100DL_AppInit(void);
void H3LIS100DL_ProcessCommandPacket(void);
void H3LIS100DL_ProcessGroundCommand(void);
void H3LIS100DL_ReportHousekeeping(void);
void H3LIS100DL_ResetCounters(void);

bool H3LIS100DL_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* H3LIS100DL Accel + Gyro */
bool INIT_H3LIS100DL(int I2CBus, h3lis100dl_hk_tlm_t* H3LIS100DL_HkTelemetryPkt);
void PROCESS_H3LIS100DL(int i2cbus, h3lis100dl_hk_tlm_t* H3LIS100DL_HkTelemetryPkt, h3lis100dl_data_tlm_t* H3LIS100DL_DataTelemetryPkt);

#endif /* _h3lis100dl_h_ */
