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


/**
 * @brief CFE init function for the library
 * 
 * @return int 
 */
int I2C_LibInit(void)
{
    
    OS_printf ("I2C Lib Initialized.  Version %d.%d.%d.%d\n",
                I2C_LIB_MAJOR_VERSION,
                I2C_LIB_MINOR_VERSION, 
                I2C_LIB_REVISION, 
                I2C_LIB_MISSION_REV);
                
    return CFE_SUCCESS;
 
}/* End I2C_LibInit */

/**
 * @brief sample i2c device function
 * 
 * @return int 
 */

int I2C_Sample_Function( void ) 
{
   OS_printf ("I2C_Sample_Function called\n");

   return(CFE_SUCCESS);
   
} /* End I2C_sample_Function */


/****************************************/
/*        SINGLE-BYTE I2C DRIVER        */
/****************************************/
// Assuming Linux OS

/**
 * @brief Open i2c device and returns the fd for it
 * 
 * @param I2CBus logical bus the i2c device is on
 * @param addr slave addr for the i2c device
 * @return int fd for the opened device
 */
int I2C_open(int I2CBus)
{
	// Declare I2C device name char array
	char i2cbuf[MAX_BUS];

	// Assign I2C device bus name
	snprintf(i2cbuf, sizeof(i2cbuf), "/dev/i2c-%d", I2CBus);

	// Declare File Variable
	int file;

	// Open the I2C Device
	if ((file = open(i2cbuf, O_RDWR)) < 0)
	{
        CFE_EVS_SendEvent(I2C_OPEN_I2C_BUS_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to open I2C BUS %d", I2CBus);
        //I2C_HkTelemetryPkt.i2c_error_count++;

        return -1;
	}

	// Return the file if successful
	return file;
}

/**
 * @brief close a given fd
 * 
 * @param file file descriptor for the file to close
 */

void I2C_close(int file)
{
	close(file);
}

/**
 * @brief This I2C write function assumes that you are using a one-byte register. Therefore you must first 
 *        write the register address and then write the value for the register
 *       follows figure 8 of https://www.ti.com/lit/an/slva704/slva704.pdf
 * 
 * @param file file descriptor for the device
 * @param slave_addr i2c slave addr for the device
 * @param reg desired register to write
 * @param val value write
 * @return true success
 * @return false failure
 */

bool I2C_write(int file, uint8_t slave_addr, uint8_t reg, uint8_t val)
{
    uint8_t buff[2];
    struct i2c_msg msg[1];
    struct i2c_rdwr_ioctl_data msgset[1];

   //set register value into the buffer
    buff[0]=reg;
    buff[1]=val;

   //load the message to be sent
    msg[0].addr=slave_addr;
    msg[0].flags=0;
    msg[0].len=2;
    msg[0].buf=buff;
   //load the data
    msgset[0].msgs=msg;
    msgset[0].nmsgs=1;

   //make sure it was written properly
    if(ioctl(file, I2C_RDWR, &msgset)<0){
        CFE_EVS_SendEvent(I2C_WRITE_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
           "Error failed to write to register %X! ", reg);

        return false;
    }

    return true;
}

/**
 * @brief reads the specfied number of bytes starting at a specific register
 * 
 * @param file file descriptor for device
 * @param slave_addr address of slave device on i2cbus
 * @param start_addr address of register to start reading from
 * @param length number of bytes wanted to be read
 * @param buffer buffer where read data is to be stored
 * @return true successfully read data
 * @return false failed reading data
 */

bool I2C_multi_read(int file, uint8_t slave_addr, uint8_t start_addr, uint8_t length, uint8_t *buffer) {
   uint8_t out[1];
   struct i2c_msg msg[2];
   struct i2c_rdwr_ioctl_data msgset[1];

   
   // Make sure the buffer is declared
	if (!buffer)
	{
		return false;
	}
   //set the first message data to the register
   out[0] = start_addr;

   //set the first message to be sent to the register of the device to read
   msg[0].addr    = slave_addr;
   msg[0].flags   = 0;
   msg[0].len     = 1;
   msg[0].buf     = out;

   //set the second message to be the read message
   msg[1].addr    = slave_addr;
   msg[1].flags   = I2C_M_RD;
   msg[1].len     = length;
   msg[1].buf     = buffer;

   //load the messages into the data
   msgset[0].msgs=msg;
   //set the number of messages to 2
   msgset[0].nmsgs=2;

   //send off the messages to the device and ensure that it works properly
   if(ioctl(file,I2C_RDWR,&msgset)<0){
        CFE_EVS_SendEvent(I2C_REGISTER_READ_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to read from %d registers... ", length);
        //I2C_HkTelemetryPkt.i2c_error_count++;

		return false;
   }

	// Return true if succeeded
	return true;
}

/************************/
/*  End of File Comment */
/************************/
