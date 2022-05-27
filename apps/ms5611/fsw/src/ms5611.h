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
** File: ms5611.h
**
** Purpose:
**   This file is main hdr file for the MS5611 application.
**
** Notes: Naming conventions and device specific variables changed.
*******************************************************************************/

#ifndef _ms5611_h_
#define _ms5611_h_

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

#include "ms5611_msg.h"
#include "ms5611_registers.h"

/***********************************************************************/

#define MS5611_PIPE_DEPTH           32
#define MAX_BUS                     64

// Define the Data Buffer Size using the Datasheet (162 buffers because going to a2)
#define MS5611_I2C_BUFFER 0xA2

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the MS5611
struct MS5611_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[MS5611_I2C_BUFFER];
	
	/* Data Variables */
	uint16_t prom[6];
} MS5611;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (MS5611_AppMain), these
**       functions are not called from any other source module.
*/
void MS5611_AppMain(void);
void MS5611_AppInit(void);
void MS5611_ProcessCommandPacket(void);
void MS5611_ProcessGroundCommand(void);
void MS5611_ReportHousekeeping(void);
void MS5611_ResetCounters(void);

bool MS5611_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* MS5611 Accel */
bool INIT_MS5611(int I2CBus, ms5611_hk_tlm_t* MS5611_HkTelemetryPkt);
void PROCESS_MS5611(int i2cbus, ms5611_hk_tlm_t* MS5611_HkTelemetryPkt, ms5611_data_tlm_t* MS5611_DataTelemetryPkt);
uint32_t readRawData(int file, uint8_t reg);

#endif /* _ms5611_h_ */
