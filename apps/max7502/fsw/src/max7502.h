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
** File: max7502.h
**
** Purpose:
**   This file is main hdr file for the MAX7502 application.
**
**
*******************************************************************************/

#ifndef _max7502_h_
#define _max7502_h_

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

#include "max7502_msg.h"
#include "max7502_registers.h"

/***********************************************************************/

#define MAX7502_PIPE_DEPTH               32
#define MAX_BUS                           64

// Define the Data Buffer Size using the Datasheet (4 buffers because going to 03)
#define MAX7502_I2C_BUFFER 0x04

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the MAX7502
struct MAX7502_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[MAX7502_I2C_BUFFER];
	
	// Data Variables
	//float temperature;
} MAX7502;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (MAX7502_AppMain), these
**       functions are not called from any other source module.
*/
void MAX7502_AppMain(void);
void MAX7502_AppInit(void);
void MAX7502_ProcessCommandPacket(void);
void MAX7502_ProcessGroundCommand(void);
void MAX7502_ReportHousekeeping(void);
void MAX7502_ResetCounters(void);

bool MAX7502_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* MAX7502 Barometer */
bool INIT_MAX7502(int I2CBus, max7502_hk_tlm_t* MAX7502_HkTelemetryPkt, uint8 DeviceNumber);
void PROCESS_MAX7502(int i2cbus, max7502_hk_tlm_t* MAX7502_HkTelemetryPkt, max7502_data_tlm_t* MAX7502_DataTelemetryPkt, uint8 DeviceNumber);

#endif /* _max7502_h_ */
