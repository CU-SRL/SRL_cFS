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
** File: i2c.h
**
** Purpose:
**   This file is main hdr file for the I2C application.
**
**
*******************************************************************************/

#ifndef _i2c_h_
#define _i2c_h_

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

#include "i2c_msg.h"

/***********************************************************************/

#define I2C_PIPE_DEPTH                     32
#define MAX_BUS 64

/************************************************************************
** Type Definitions
*************************************************************************/

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (I2C_AppMain), these
**       functions are not called from any other source module.
*/
void I2C_AppMain(void);
void I2C_AppInit(void);
void I2C_ProcessCommandPacket(void);
void I2C_ProcessGroundCommand(void);
void I2C_ReportHousekeeping(void);
void I2C_ResetCounters(void);

bool I2C_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

/****************************************************************************/
/*
** I2C function prototypes.
**
*/
int I2C_open(int I2CBus, uint8_t addr);
void I2C_close(int file);
bool I2C_write(int file, uint8_t reg, uint8_t val);
bool I2C_read(int file, uint8_t reg, unsigned int byte_count, uint8_t *buffer);

/****************************************************************************/
/*
** Device function prototypes.
**
*/

/* MPL3115A2 Barometer */
bool INIT_MPL3115A2(int I2CBus, i2c_hk_tlm_t* I2C_HkTelemetryPkt);
void PROCESS_MPL3115A(int i2cbus, i2c_hk_tlm_t* I2C_HkTelemetryPkt);

#endif /* _i2c_h_ */
