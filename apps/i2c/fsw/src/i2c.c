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
** File: i2c.c
**
** Purpose:
**   This file contains the source code for the I2C App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "i2c.h"
#include "i2c_perfids.h"
#include "i2c_msgids.h"
#include "i2c_msg.h"
#include "i2c_events.h"
#include "i2c_version.h"

/*
** global data
*/

i2c_hk_tlm_t    I2C_HkTelemetryPkt;
CFE_SB_PipeId_t    I2C_CommandPipe;
CFE_SB_MsgPtr_t    I2CMsgPtr;

static CFE_EVS_BinFilter_t  I2C_EventFilters[] =
       {  /* Event ID    mask */
          {I2C_STARTUP_INF_EID,       0x0000},
          {I2C_COMMAND_ERR_EID,       0x0000},
          {I2C_COMMANDNOP_INF_EID,    0x0000},
          {I2C_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* I2C_AppMain() -- Application entry point and main process loop             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void I2C_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(I2C_PERF_ID);

    I2C_AppInit();

    /*
    ** I2C Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(I2C_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&I2CMsgPtr, I2C_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(I2C_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            I2C_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of I2C_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* I2C_AppInit() --  initialization                                           */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void I2C_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(I2C_EventFilters,
                     sizeof(I2C_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&I2C_CommandPipe, I2C_PIPE_DEPTH,"I2C_CMD_PIPE");
    CFE_SB_Subscribe(I2C_CMD_MID, I2C_CommandPipe);
    CFE_SB_Subscribe(I2C_SEND_HK_MID, I2C_CommandPipe);

    I2C_ResetCounters();

    CFE_SB_InitMsg(&I2C_HkTelemetryPkt,
                   I2C_HK_TLM_MID,
                   I2C_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (I2C_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "I2C App Initialized. Version %d.%d.%d.%d",
                I2C_MAJOR_VERSION,
                I2C_MINOR_VERSION, 
                I2C_REVISION, 
                I2C_MISSION_REV);
				
} /* End of I2C_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  I2C_ProcessCommandPacket                                           */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the I2C       */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void I2C_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(I2CMsgPtr);

    switch (MsgId)
    {
        case I2C_CMD_MID:
            I2C_ProcessGroundCommand();
            break;

        case I2C_SEND_HK_MID:
            I2C_ReportHousekeeping();
            break;

        default:
            I2C_HkTelemetryPkt.i2c_command_error_count++;
            CFE_EVS_SendEvent(I2C_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"I2C: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End I2C_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_ProcessGroundCommand() -- I2C ground commands                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void I2C_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(I2CMsgPtr);

    /* Process "known" I2C app ground commands */
    switch (CommandCode)
    {
        case I2C_NOOP_CC:
            I2C_HkTelemetryPkt.i2c_command_count++;
            CFE_EVS_SendEvent(I2C_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"I2C: NOOP command");
            break;

        case I2C_RESET_COUNTERS_CC:
            I2C_ResetCounters();
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of I2C_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  I2C_ReportHousekeeping                                              */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void I2C_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &I2C_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &I2C_HkTelemetryPkt);
    return;

} /* End of I2C_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  I2C_ResetCounters                                                  */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void I2C_ResetCounters(void)
{
    /* Status of commands processed by the I2C App */
    I2C_HkTelemetryPkt.i2c_command_count       = 0;
    I2C_HkTelemetryPkt.i2c_command_error_count = 0;

    CFE_EVS_SendEvent(I2C_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"I2C: RESET command");
    return;

} /* End of I2C_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_VerifyCmdLength() -- Verify command packet length                      */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool I2C_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
{     
    bool result = true;

    uint16 ActualLength = CFE_SB_GetTotalMsgLength(msg);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID   = CFE_SB_GetMsgId(msg);
        uint16         CommandCode = CFE_SB_GetCmdCode(msg);

        CFE_EVS_SendEvent(I2C_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        I2C_HkTelemetryPkt.i2c_command_error_count++;
    }

    return(result);

} /* End of I2C_VerifyCmdLength() */


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
        I2C_HkTelemetryPkt.i2c_error_count++;

        return -1;
	}

	// Open IO operation
	if(ioctl(file, I2C_SLAVE, addr) < 0)
	{
        CFE_EVS_SendEvent(I2C_OPEN_SLAVE_ERR_EID, CFE_EVS_EventType_ERROR,
           "I2C_SLAVE address %X failed... ", addr);
        I2C_HkTelemetryPkt.i2c_error_count++;
        
        return -1;
	}

	// Return the file if successful
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
bool I2C_write(int file, uint8_t reg, uint8_t val)
{
	uint8_t write_buf[2] = { reg, val };
	if (write(file, write_buf, 2) != 2)
	{
        CFE_EVS_SendEvent(I2C_WRITE_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
           "Error failed to write to register %X! ", reg);
        I2C_HkTelemetryPkt.i2c_error_count++;
        
		return false;
	}

	// If not, return succeeded
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* I2C_read() -- Reads the specified registers                                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool I2C_read(int file, uint8_t reg, unsigned int byte_count, uint8_t *buffer)
{
	// Make sure the buffer is declared
	if (!buffer)
	{
		return false;
	}

	// Write to the register we want to read from
	if(!I2C_write(file, reg, 1))
	{
        CFE_EVS_SendEvent(I2C_WRITE_REGISTER_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to write to register %X for reading... ", reg);
        I2C_HkTelemetryPkt.i2c_error_count++;

		return false;
	}

	// Read the specific number of bytes
	if(read(file, buffer, byte_count) != byte_count)
	{
        CFE_EVS_SendEvent(I2C_REGISTER_READ_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to read from %d registers... ", byte_count);
        I2C_HkTelemetryPkt.i2c_error_count++;

		return false;
	}

	// Return true if succeeded
	return true;
}

