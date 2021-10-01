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
** File: mcp98001.c
**
** Purpose:
**   This file contains the source code for the MCP98001 App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "mcp98001.h"
#include "mcp98001_perfids.h"
#include "mcp98001_msgids.h"
#include "mcp98001_msg.h"
#include "mcp98001_events.h"
#include "mcp98001_version.h"

#include "i2c_lib.h"

/*
** global data
*/

mcp98001_hk_tlm_t       MCP98001_HkTelemetryPkt;
mcp98001_data_tlm_t     MCP98001_DataTelemetryPkt;
CFE_SB_PipeId_t          MCP98001_CommandPipe;
CFE_SB_MsgPtr_t          MCP98001MsgPtr;

static CFE_EVS_BinFilter_t  MCP98001_EventFilters[] =
       {  /* Event ID    mask */
          {MCP98001_STARTUP_INF_EID,       0x0000},
          {MCP98001_COMMAND_ERR_EID,       0x0000},
          {MCP98001_COMMANDNOP_INF_EID,    0x0000},
          {MCP98001_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* MCP98001_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void MCP98001_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(MCP98001_PERF_ID);

    MCP98001_AppInit();

    /*
    ** MCP98001 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(MCP98001_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&MCP98001MsgPtr, MCP98001_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(MCP98001_PERF_ID);

        // Process Data
        // PROCESS_MCP98001(1, &MCP98001_HkTelemetryPkt, &MCP98001_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            MCP98001_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of MCP98001_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* MCP98001_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void MCP98001_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(MCP98001_EventFilters,
                     sizeof(MCP98001_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&MCP98001_CommandPipe, MCP98001_PIPE_DEPTH,"MCP98001_CMD_PIPE");
    CFE_SB_Subscribe(MCP98001_CMD_MID, MCP98001_CommandPipe);
    CFE_SB_Subscribe(MCP98001_SEND_HK_MID, MCP98001_CommandPipe);

    MCP98001_ResetCounters();

    CFE_SB_InitMsg(&MCP98001_HkTelemetryPkt,
                   MCP98001_HK_TLM_MID,
                   MCP98001_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (MCP98001_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "MCP98001 App Initialized. Version %d.%d.%d.%d\n",
                MCP98001_MAJOR_VERSION,
                MCP98001_MINOR_VERSION, 
                MCP98001_REVISION, 
                MCP98001_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of MCP98001_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MCP98001_ProcessCommandPacket                                     */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the MCP98001 */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MCP98001_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(MCP98001MsgPtr);

    switch (MsgId)
    {
        case MCP98001_CMD_MID:
            MCP98001_ProcessGroundCommand();
            break;

        case MCP98001_SEND_HK_MID:
            MCP98001_ReportHousekeeping();
            break;

        default:
            MCP98001_HkTelemetryPkt.mcp98001_command_error_count++;
            CFE_EVS_SendEvent(MCP98001_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"MCP98001: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End MCP98001_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MCP98001_ProcessGroundCommand() -- MCP98001 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void MCP98001_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(MCP98001MsgPtr);

    /* Process "known" MCP98001 app ground commands */
    switch (CommandCode)
    {
        case MCP98001_NOOP_CC:
            MCP98001_HkTelemetryPkt.mcp98001_command_count++;
            CFE_EVS_SendEvent(MCP98001_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"MCP98001: NOOP command");
            break;

        case MCP98001_RESET_COUNTERS_CC:
            MCP98001_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case MCP98001_INIT:
            INIT_MCP98001(1, &MCP98001_HkTelemetryPkt);
            break;
        
        case MCP98001_PROCESS:
            PROCESS_MCP98001(1, &MCP98001_HkTelemetryPkt, &MCP98001_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of MCP98001_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MCP98001_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MCP98001_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &MCP98001_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MCP98001_HkTelemetryPkt);
    return;

} /* End of MCP98001_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MCP98001_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MCP98001_ResetCounters(void)
{
    /* Status of commands processed by the MCP98001 App */
    MCP98001_HkTelemetryPkt.mcp98001_command_count       = 0;
    MCP98001_HkTelemetryPkt.mcp98001_command_error_count = 0;

    CFE_EVS_SendEvent(MCP98001_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"MCP98001: RESET command");
    return;

} /* End of MCP98001_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MCP98001_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool MCP98001_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(MCP98001_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        MCP98001_HkTelemetryPkt.mcp98001_command_error_count++;
    }

    return(result);

} /* End of MCP98001_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_MCP98001() -- This function initializes the MCP98001 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_MCP98001(int I2CBus, mcp98001_hk_tlm_t* MCP98001_HkTelemetryPkt)
{
	//Device 2
    // Open I2C for the first device address
	int file1 = I2C_open(I2CBus, MCP98001_1_I2C_ADDR);

	
	// Configure resolution to 12 bits
	if(!I2C_write(file1, MCP98001_CONFIG, 0x60))
	{
		CFE_EVS_SendEvent(MCP98001_FAILED_TO_CONFIGURE, CFE_EVS_EventType_ERROR,
           "Failed to place MCP98001 resolution to 12 bits... ");
        MCP98001_HkTelemetryPkt->mcp98001_device_error_count++;
		return false;
	}

	// Close the I2C file
	I2C_close(file1);

    //Device 2
    // Open I2C for the second device address
	int file2 = I2C_open(I2CBus, MCP98001_2_I2C_ADDR);

   // Configure resolution to 12 bits
	if(!I2C_write(file2, MCP98001_CONFIG, 0x60))
	{
		CFE_EVS_SendEvent(MCP98001_FAILED_TO_CONFIGURE, CFE_EVS_EventType_ERROR,
           "Failed to place MCP98001 resolution to 12 bits... ");
        MCP98001_HkTelemetryPkt->mcp98001_device_error_count++;
		return false;
	}

    // Close the I2C file
	I2C_close(file2);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_MCP98001() -- This function process the MCP98001 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_MCP98001(int i2cbus, mcp98001_hk_tlm_t* MCP98001_HkTelemetryPkt, mcp98001_data_tlm_t* MCP98001_DataTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, MPL3115_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file1, MPL3115_STATUS, 1, MCP98001.status);
	if (MCP98001.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, MCP98001_AMBIENT_TEMP, 2, MCP98001.buffer))
		{
			CFE_EVS_SendEvent(MCP98001_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	MCP98001_HkTelemetryPkt->mcp98001_device_error_count++;

			return;
		}

		/* Process the Data Buffer */	

		// Temperature
		int16_t t;

		t = MCP98001.buffer[0];
		t <<= 8;
		t |= MCP98001.buffer[1];
		t >>= 4;

		if(t & 0x800)
		{
			t |= 0xF000;
		}

		float temp = t;
		temp /= 16.0;


		MCP98001_DataTelemetryPkt->MCP98001_TEMPERATURE = temp;

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(MCP98001_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Temperature: %F ", temp);
	}

	// Close the I2C Buffer
	I2C_close(file);
}