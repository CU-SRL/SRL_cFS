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
** File: mcp98001.h
**
** Purpose:
**   This file is main hdr file for the MCP98001 application.
**
**
*******************************************************************************/

#ifndef _mcp98001_h_
#define _mcp98001_h_

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

#include "mcp98001_msg.h"
#include "mcp98001_registers.h"

/***********************************************************************/

#define MCP98001_PIPE_DEPTH               32
#define MAX_BUS                           64

// Define the Data Buffer Size using the Datasheet (4 buffers because going to 03)
#define MCP98001_I2C_BUFFER 0x04

/************************************************************************
** Type Definitions
*************************************************************************/

// Define the Data Struct to hold the data from the MCP98001
struct MCP98001_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[MCP98001_I2C_BUFFER];
	
	// Data Variables
	//float temperature;
} MCP98001;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (MCP98001_AppMain), these
**       functions are not called from any other source module.
*/
void MCP98001_AppMain(void);
void MCP98001_AppInit(void);
void MCP98001_ProcessCommandPacket(void);
void MCP98001_ProcessGroundCommand(void);
void MCP98001_ReportHousekeeping(void);
void MCP98001_ResetCounters(void);

bool MCP98001_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* MCP98001 Barometer */
bool INIT_MCP98001(int I2CBus, mcp98001_hk_tlm_t* MCP98001_HkTelemetryPkt, uint8 DeviceNumber);
void PROCESS_MCP98001(int i2cbus, mcp98001_hk_tlm_t* MCP98001_HkTelemetryPkt, mcp98001_data_tlm_t* MCP98001_DataTelemetryPkt, uint8 DeviceNumber);

#endif /* _mcp98001_h_ */
