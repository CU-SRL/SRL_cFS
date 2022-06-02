/*************************************************************************
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
** File: i2c_lib.h
**
** Purpose: 
**   Specification for the i2c library functions.
**
*************************************************************************/
#ifndef _i2c_lib_h_
#define _i2c_lib_h_

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
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
#include <ctype.h>

/************************************************************************
** Type Definitions
*************************************************************************/


/************************************************************************
** Error ID Definitions
*************************************************************************/
#define I2C_OPEN_I2C_BUS_ERR_EID        101
#define I2C_SET_SLAVE_ERR_EID           102
#define I2C_NULL_BUF_ERR_EID            103
#define I2C_SPECIFY_REGISTER_ERR_EID    104
#define I2C_READ_FROM_REGISTER_ERR_EID  105
#define I2C_WRITE_TO_REGISTER_ERR_EID   106

/*************************************************************************
** Exported Functions
*************************************************************************/
/************************************************************************/
/** \brief i2c Sample Lib Function 
**  
**  \par Description
**        This is a i2c sample function
**
**  \par Assumptions, External Events, and Notes:
**        None
**       
**  \returns
**  \retstmt Returns #CFE_SUCCESS \endcode
**  \endreturns
** 
*************************************************************************/
int32 I2C_Sample_Function( void ); 

int I2C_Open(int I2CBus);
void I2C_Close(int file);
bool I2C_Read(int file, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool I2C_Write(int file, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif /* _i2c_lib_h_ */

/************************/
/*  End of File Comment */
/************************/
