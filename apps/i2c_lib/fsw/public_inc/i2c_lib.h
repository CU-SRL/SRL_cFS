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
#define I2C_OPEN_I2C_BUS_ERR_EID      101
#define I2C_OPEN_SLAVE_ERR_EID        102
#define I2C_WRITE_REGISTER_ERR_EID    103
#define I2C_REGISTER_READ_ERR_EID     104

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
int32 I2C_sample_Function( void ); 

int I2C_open(int I2CBus);
void I2C_close(int file);
bool I2C_write(int file,uint8_t slave_addr, uint8_t reg, uint8_t val);
bool I2C_read(int file,uint8_t slave_addr, uint8_t reg, unsigned int byte_count, uint8_t *buffer);

#endif /* _i2c_lib_h_ */

/************************/
/*  End of File Comment */
/************************/
