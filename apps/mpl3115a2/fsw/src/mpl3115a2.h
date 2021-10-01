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
** File: mpl3115a2.h
**
** Purpose:
**   This file is main hdr file for the MPL3115A2 application.
**
**
*******************************************************************************/

#ifndef _mpl3115a2_h_
#define _mpl3115a2_h_

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
#include <stropts.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "mpl3115a2_msg.h"
#include "mpl3115a2_registers.h"

/***********************************************************************/

#define MPL3115A2_PIPE_DEPTH               32
#define MAX_BUS                            64

// Define the Data Buffer Size using the Datasheet (48 buffers because going to 2f)
#define MPL3115A2_I2C_BUFFER 0x30

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the MPL3115A2
struct MPL3115A2_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[MPL3115A2_I2C_BUFFER];
	
	// Data Variables
	double pressure;
	double temperature;
} MPL3115A2;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (MPL3115A2_AppMain), these
**       functions are not called from any other source module.
*/
void MPL3115A2_AppMain(void);
void MPL3115A2_AppInit(void);
void MPL3115A2_ProcessCommandPacket(void);
void MPL3115A2_ProcessGroundCommand(void);
void MPL3115A2_ReportHousekeeping(void);
void MPL3115A2_ResetCounters(void);

bool MPL3115A2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* MPL3115A2 Barometer */
bool INIT_MPL3115A2(int I2CBus, mpl3115a2_hk_tlm_t* MPL3115A2_HkTelemetryPkt);
void PROCESS_MPL3115A2(int i2cbus, mpl3115a2_hk_tlm_t* MPL3115A2_HkTelemetryPkt, mpl3115a2_data_tlm_t* MPL3115A2_DataTelemetryPkt);

#endif /* _mpl3115a2_h_ */
