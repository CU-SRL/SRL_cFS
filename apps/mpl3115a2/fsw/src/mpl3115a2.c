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
** File: mpl3115a2.c
**
** Purpose:
**   This file contains the source code for the MPL3115A2 App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "mpl3115a2.h"
#include "mpl3115a2_perfids.h"
#include "mpl3115a2_msgids.h"
#include "mpl3115a2_msg.h"
#include "mpl3115a2_events.h"
#include "mpl3115a2_version.h"

#include "i2c_lib.h"

/*
** global data
*/

mpl3115a2_hk_tlm_t       MPL3115A2_HkTelemetryPkt;
mpl3115a2_data_tlm_t     MPL3115A2_DataTelemetryPkt;
CFE_SB_PipeId_t          MPL3115A2_CommandPipe;
CFE_SB_MsgPtr_t          MPL3115A2MsgPtr;

static CFE_EVS_BinFilter_t  MPL3115A2_EventFilters[] =
       {  /* Event ID    mask */
          {MPL3115A2_STARTUP_INF_EID,       0x0000},
          {MPL3115A2_COMMAND_ERR_EID,       0x0000},
          {MPL3115A2_COMMANDNOP_INF_EID,    0x0000},
          {MPL3115A2_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* MPL3115A2_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void MPL3115A2_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(MPL3115A2_PERF_ID);

    MPL3115A2_AppInit();

    /*
    ** MPL3115A2 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(MPL3115A2_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&MPL3115A2MsgPtr, MPL3115A2_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(MPL3115A2_PERF_ID);

        // Process Data
        // PROCESS_MPL3115A2(1, &MPL3115A2_HkTelemetryPkt, &MPL3115A2_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            MPL3115A2_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of MPL3115A2_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* MPL3115A2_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void MPL3115A2_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(MPL3115A2_EventFilters,
                     sizeof(MPL3115A2_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&MPL3115A2_CommandPipe, MPL3115A2_PIPE_DEPTH,"MPL3115A2_CMD_PIPE");
    CFE_SB_Subscribe(MPL3115A2_CMD_MID, MPL3115A2_CommandPipe);
    CFE_SB_Subscribe(MPL3115A2_SEND_HK_MID, MPL3115A2_CommandPipe);

    MPL3115A2_ResetCounters();

    CFE_SB_InitMsg(&MPL3115A2_HkTelemetryPkt,
                   MPL3115A2_HK_TLM_MID,
                   MPL3115A2_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (MPL3115A2_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "MPL3115A2 App Initialized. Version %d.%d.%d.%d\n",
                MPL3115A2_MAJOR_VERSION,
                MPL3115A2_MINOR_VERSION, 
                MPL3115A2_REVISION, 
                MPL3115A2_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of MPL3115A2_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MPL3115A2_ProcessCommandPacket                                     */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the MPL3115A2 */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MPL3115A2_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(MPL3115A2MsgPtr);

    switch (MsgId)
    {
        case MPL3115A2_CMD_MID:
            MPL3115A2_ProcessGroundCommand();
            break;

        case MPL3115A2_SEND_HK_MID:
            MPL3115A2_ReportHousekeeping();
            break;

        default:
            MPL3115A2_HkTelemetryPkt.mpl3115a2_command_error_count++;
            CFE_EVS_SendEvent(MPL3115A2_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"MPL3115A2: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End MPL3115A2_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MPL3115A2_ProcessGroundCommand() -- MPL3115A2 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void MPL3115A2_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(MPL3115A2MsgPtr);

    /* Process "known" MPL3115A2 app ground commands */
    switch (CommandCode)
    {
        case MPL3115A2_NOOP_CC:
            MPL3115A2_HkTelemetryPkt.mpl3115a2_command_count++;
            CFE_EVS_SendEvent(MPL3115A2_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"MPL3115A2: NOOP command");
            break;

        case MPL3115A2_RESET_COUNTERS_CC:
            MPL3115A2_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case MPL3115A2_INIT:
            INIT_MPL3115A2(1, &MPL3115A2_HkTelemetryPkt);
            break;
        
        case MPL3115A2_PROCESS:
            PROCESS_MPL3115A2(1, &MPL3115A2_HkTelemetryPkt, &MPL3115A2_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of MPL3115A2_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MPL3115A2_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MPL3115A2_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &MPL3115A2_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MPL3115A2_HkTelemetryPkt);
    return;

} /* End of MPL3115A2_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MPL3115A2_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MPL3115A2_ResetCounters(void)
{
    /* Status of commands processed by the MPL3115A2 App */
    MPL3115A2_HkTelemetryPkt.mpl3115a2_command_count       = 0;
    MPL3115A2_HkTelemetryPkt.mpl3115a2_command_error_count = 0;

    CFE_EVS_SendEvent(MPL3115A2_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"MPL3115A2: RESET command");
    return;

} /* End of MPL3115A2_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MPL3115A2_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool MPL3115A2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(MPL3115A2_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        MPL3115A2_HkTelemetryPkt.mpl3115a2_command_error_count++;
    }

    return(result);

} /* End of MPL3115A2_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_MPL3115A2() -- This function initializes the MPL3115A2 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_MPL3115A2(int I2CBus, mpl3115a2_hk_tlm_t* MPL3115A2_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, MPL3115_I2C_ADDR);
	
	// Place MPL3115A2 into standby mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0))
	{
		CFE_EVS_SendEvent(MPL3115A2_FAILED_CHANGE_TO_STANDBY_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to place MPL3115A2 into Standby Mode... ");
        MPL3115A2_HkTelemetryPkt->mpl3115a2_device_error_count++;
		return false;
	}

	// Set the MPL3115A2 sample rate to 34ms
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0x98))
	{
		CFE_EVS_SendEvent(MPL3115A2_RATE_SWITCH_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch output on MPL3115A2 to 34ms...  ");
        MPL3115A2_HkTelemetryPkt->mpl3115a2_device_error_count++;

		return false;
	}

	// Switch the MPL3115A2 to active, set altimeter mode, set polling mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0xB9))
	{
		CFE_EVS_SendEvent(MPL3115A2_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch MPL3115A2 to active...  ");
        MPL3115A2_HkTelemetryPkt->mpl3115a2_device_error_count++;

		return false;
	}

	// Enable Events on the MPL3115A2
	if(!I2C_write(file, MPL3115_PT_DATA_CFG, 0x07))
	{
		CFE_EVS_SendEvent(MPL3115A2_ENABLE_EVENTS_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to enable events on the MPL3115A2...  ");
        MPL3115A2_HkTelemetryPkt->mpl3115a2_device_error_count++;

		return false;
	}

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_MPL3115A2() -- This function process the MPL3115A2 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_MPL3115A2(int i2cbus, mpl3115a2_hk_tlm_t* MPL3115A2_HkTelemetryPkt, mpl3115a2_data_tlm_t* MPL3115A2_DataTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, MPL3115_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file, MPL3115_STATUS, 1, MPL3115A2.status);
	if (MPL3115A2.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, MPL3115_OUT_P_MSB, 5, MPL3115A2.buffer))
		{
			CFE_EVS_SendEvent(MPL3115A2_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	MPL3115A2_HkTelemetryPkt->mpl3115a2_device_error_count++;

			return;
		}

		/* Process the Data Buffer */
			
		// Altitude
		int32_t alt;

		alt = ((uint32_t)MPL3115A2.buffer[0]) << 24;
		alt |= ((uint32_t)MPL3115A2.buffer[1]) << 16;
		alt |= ((uint32_t)MPL3115A2.buffer[2]) << 8;

		float altitude = alt;
		altitude /= 65536.0;		

		// Temperature
		int16_t t;

		t = MPL3115A2.buffer[3];
		t <<= 8;
		t |= MPL3115A2.buffer[4];
		t >>= 4;

		if(t & 0x800)
		{
			t |= 0xF000;
		}

		float temp = t;
		temp /= 16.0;

		// Store into packet
		MPL3115A2_DataTelemetryPkt->MPL3115A2_ALTITUDE = altitude;
		MPL3115A2_DataTelemetryPkt->MPL3115A2_TEMPERATURE = temp;

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(MPL3115A2_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Altitude: %F Temperature: %F ", altitude, temp);
	}

	// Close the I2C Buffer
	I2C_close(file);
}