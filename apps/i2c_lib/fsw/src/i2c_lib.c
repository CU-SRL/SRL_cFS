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
** File: i2c_lib.c
**
** Purpose: 
**   I2C CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "i2c_lib.h"
#include "i2c_lib_version.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
#define I2C_PIPE_DEPTH      32
#define MAX_BUS             64

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int I2C_LibInit(void);

/*************************************************************************
** Functions
*************************************************************************/

/**
 * @brief CFE init function for the library
 * 
 * @return int
 */
int I2C_LibInit(void)
{
   OS_printf("I2C Lib Initialized. Version %d.%d.%d.%d\n",
      I2C_LIB_MAJOR_VERSION,
      I2C_LIB_MINOR_VERSION, 
      I2C_LIB_REVISION, 
      I2C_LIB_MISSION_REV);

   return CFE_SUCCESS;

} /* End I2C_LibInit() */


/**
 * @brief Sample I2C device function
 * 
 * @return int 
 */
int I2C_Sample_Function(void) 
{
   OS_printf("I2C_Sample_Function called\n");

   return(CFE_SUCCESS);
   
} /* End I2C_Sample_Function() */


/****************************************/
/*               I2C DRIVER             */
/****************************************/


/**
 * @brief Open I2C device and returns the file descriptor for it
 * 
 * @param I2CBus Logical bus the I2C device is on
 *
 * @return int File descriptor for the opened I2C device
 */
int I2C_Open(int I2CBus)
{
   /* File descriptor variable for the I2C device */
   int file;

	/* I2C device name character array buffer */
	char i2cbuf[MAX_BUS];

	/* Assign I2C device bus name to the buffer */
	snprintf(i2cbuf, sizeof(i2cbuf), "/dev/i2c-%d", I2CBus);

	/* Open the I2C device */
	if ((file = open(i2cbuf, O_RDWR)) < 0)
	{
      /* Error handling */
      CFE_EVS_SendEvent(I2C_OPEN_I2C_BUS_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed to open I2C BUS %d", I2CBus);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
	}

	/* Return the file descriptor for the opened I2C device */
	return file;

} /* End of I2C_Open() */


/**
 * @brief Close a given I2C device file descriptor
 * 
 * @param file I2C device file descriptor to close
 */
void I2C_Close(int file)
{
	close(file);

} /* End of I2C_Close() */


/**
 * @brief Reads the specified number of bytes from an I2C slave register
 *
 * @param file I2C device file descriptor
 * @param addr I2C slave address (7 bit, MSB is 0)
 * @param reg Address of register to be read from
 * @param len Number of bytes to be read into the buffer (also the minimum required size of the buffer)
 * @param buf Buffer that will store the data read in
 *
 * @return CFE_SUCCESS Successfully read data
 * @return -1 Failed reading data
 */
int I2C_Read(int file, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
   /* Check if buffer is declared */
   if (!buf)
   {
      CFE_EVS_SendEvent(I2C_NULL_BUF_ERR_EID, CFE_EVS_EventType_ERROR,
         "No buffer declared to read to, Slave = 0x%x, Reg = 0x%x", addr, reg);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Specify which slave to communicate with */
   if (ioctl(file, I2C_SLAVE, addr) < 0)
   {
      /* Failed to set slave address with ioctl call */
      CFE_EVS_SendEvent(I2C_SET_SLAVE_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed setting slave address 0x%x, errno = %d" addr, errno);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Write to the bus to indicate the register being used */
   buf[0] = reg;
   if (write(file, buf, 1) != 1)
   {
      /* Failed to write the byte containing the register information */
      CFE_EVS_SendEvent(I2C_SPECIFY_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed to write register address 0x%x to I2C Bus %d", reg, file);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Read bytes from the register specified above into the buffer */
   if (read(file, buf, len) != len)
   {
      /* Failed to read all of the bytes */
      CFE_EVS_SendEvent(I2C_READ_FROM_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed to read %d bytes, Slave = 0x%x, Reg = 0x%x", len, addr, reg);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   return CFE_SUCCESS;

} /* End of I2C_Read() */


/**
 * @brief Writes the data in buffer to an I2C slave register
 *
 * @param file I2C device file descriptor
 * @param addr I2C slave address (7 bit, MSB is 0)
 * @param reg Address of register that will be wrote to
 * @param len Number of bytes in the buffer (also the minimum required size of the buffer)
 * @param buf Buffer containing data to write
 *
 * @return CFE_SUCCESS Successfully wrote data
 * @return -1 Failed writing data
 */
bool I2C_Write(int file, uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
   /* Check if buffer is declared */
   if (!buf)
   {
      CFE_EVS_SendEvent(I2C_NULL_BUF_ERR_EID, CFE_EVS_EventType_ERROR,
         "No buffer declared to write to, Slave = 0x%x, Reg = 0x%x", addr, reg);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Specify which slave to communicate with */
   if (ioctl(file, I2C_SLAVE, addr) < 0)
   {
      /* Failed to set slave address with ioctl call */
      CFE_EVS_SendEvent(I2C_SET_SLAVE_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed setting slave address 0x%x, errno = %d" addr, errno);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Write to the bus to indicate the register being used */
   if (write(file, &reg, 1) != 1)
   {
      /* Failed to write the byte containing the register information */
      CFE_EVS_SendEvent(I2C_SPECIFY_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed to write register address 0x%x to I2C Bus %d", reg, file);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   /* Write bytes from the buffer to the register specified above */
   if (write(file, buf, len) != len)
   {
      /* Failed to write all of the bytes */
      CFE_EVS_SendEvent(I2C_WRITE_TO_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
         "Failed to write all %d bytes, Slave = 0x%x, Reg = 0x%x", len, addr, reg);
      /* TODO: Increase error count for I2C housekeeping packet */
      return -1;
   }

   return CFE_SUCCESS;

} /* End of I2C_Write() */

/************************/
/*  End of File Comment */
/************************/
