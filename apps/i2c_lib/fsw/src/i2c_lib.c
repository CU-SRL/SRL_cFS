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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int I2C_LibInit(void)
{
    
    OS_printf ("I2C Lib Initialized.  Version %d.%d.%d.%d\n",
                I2C_LIB_MAJOR_VERSION,
                I2C_LIB_MINOR_VERSION, 
                I2C_LIB_REVISION, 
                I2C_LIB_MISSION_REV);
                
    return CFE_SUCCESS;
 
}/* End I2C_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* I2C Lib function                                                */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int I2C_Sample_Function( void ) 
{
   OS_printf ("I2C_Sample_Function called\n");

   return(CFE_SUCCESS);
   
} /* End I2C_sample_Function */


/****************************************/
/*        SINGLE-BYTE I2C DRIVER        */
/****************************************/
// Assuming Linux OS - For a RTOS (RTEMS) we need a different implementation that is RTOS compliant.

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_open() -- This function opens the I2C device, and returns file         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/


int I2C_open(int I2CBus, uint8_t addr)
{
   
   char i2cbuf[MAX_BUS];
   snprintf(i2cbuf, sizeof(i2cbuf), "/dev/i2c-%d", I2CBus);

   int file;

   if ((file = open(i2cbuf, O_RDWR)) < 0) {
        char err[200];
        sprintf(err, "open('%s') in i2c_init", i2cbuf);
        perror(err);
        return -1;
    }

    return file;



}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_close() -- Closes the open i2c file                                    */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void I2C_close(int file)
{
	close(file);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_write() -- This I2C write function assumes that you are                */
/*                  using a one-byte register. Therefore you must first       */
/*                  write the register address and then write the value       */
/*                  for the register.                                         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

// with this update, the other apps would need to change
// instead of writing the file, we need to write their address

bool I2C_write(int file, uint8_t slave_addr, uint8_t reg, uint8_t val)
{
   uint8_t buffer[2];

    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    buffer[0] = reg;
    buffer[1] = val;

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = buffer;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(file, I2C_RDWR, &msgset) < 0) {
      CFE_EVS_SendEvent(I2C_WRITE_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR, 
         "Error failed to write to register %X! ", reg);
      return -1;
    }

    return 0;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_read() -- Reads the specified registers                                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool I2C_read(int file, uint8_t slave_addr, uint8_t reg, unsigned int byte_count, uint8_t *buffer)
{
   if (!buffer)
	{
		return false;
	}

   uint8_t outbuf[1], inbuf[1];
   struct i2c_msg msgs[2];
   struct i2c_rdwr_ioctl_data msgset[1];

   msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = slave_addr;
    msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
    msgs[1].len = 1;
    msgs[1].buf = inbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    outbuf[0] = reg;

    inbuf[0] = 0;

    *buffer = 0;
    if (ioctl(file, I2C_RDWR, &msgset) < 0) {
        CFE_EVS_SendEvent(I2C_REGISTER_READ_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to read from %d registers... ", byte_count);
        //I2C_HkTelemetryPkt.i2c_error_count++;
        return -1;
    }

    *buffer = inbuf[0];
    return 0;
}

/************************/
/*  End of File Comment */
/************************/
