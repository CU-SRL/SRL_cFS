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
** File: lis3mdl.c
**
** Purpose:
**   This file contains the source code for the LIS3MDL App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "aimu_lis3mdl.h"
#include "aimu_lis3mdl_perfids.h"
#include "aimu_lis3mdl_msgids.h"
#include "aimu_lis3mdl_msg.h"
#include "aimu_lis3mdl_events.h"
#include "aimu_lis3mdl_version.h"

#include "i2c_lib.h"

/*
** global data
*/

aimu_lis3mdl_hk_tlm_t       AIMU_LIS3MDL_HkTelemetryPkt;
aimu_lis3mdl_data_tlm_t     AIMU_LIS3MDL_DataTelemetryPkt;
CFE_SB_PipeId_t              AIMU_LIS3MDL_CommandPipe;
CFE_SB_MsgPtr_t              AIMU_LIS3MDLMsgPtr;

static CFE_EVS_BinFilter_t  AIMU_LIS3MDL_EventFilters[] =
       {  /* Event ID    mask */
          {AIMU_LIS3MDL_STARTUP_INF_EID,       0x0000},
          {AIMU_LIS3MDL_COMMAND_ERR_EID,       0x0000},
          {AIMU_LIS3MDL_COMMANDNOP_INF_EID,    0x0000},
          {AIMU_LIS3MDL2_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* AIMU_LIS3MDL_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LIS3MDL_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LIS3MDL_PERF_ID);

    AIMU_LIS3MDL_AppInit();

    /*
    ** MPL3115A2 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LIS3MDL_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LIS3MDLMsgPtr, AIMU_LIS3MDL_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(AIMU_LIS3MDL_PERF_ID);

        // Process Data
        // PROCESS_AIMU_LIS3MDL(1, &AIMU_LIS3MDL_HkTelemetryPkt, &AIMU_LIS3MDL_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            AIMU_LIS3MDL_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of AIMU_LIS3MDL_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* MPL3115A2_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void AIMU_LIS3MDL_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(AIMU_LIS3MDL_EventFilters,
                     sizeof(AIMU_LIS3MDL_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&AIMU_LIS3MDL_CommandPipe, AIMU_LIS3MDL_PIPE_DEPTH,"AIMU_LIS3MDL_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LIS3MDL_CMD_MID, AIMU_LIS3MDL_CommandPipe);
    CFE_SB_Subscribe(AIMU_LIS3MDL_SEND_HK_MID, AIMU_LIS3MDL_CommandPipe);

    AIMU_LIS3MDL_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LIS3MDL_HkTelemetryPkt,
                   AIMU_LIS3MDL_HK_TLM_MID,
                   AIMU_LIS3MDL_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LIS3MDL_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "MPL3115A2 App Initialized. Version %d.%d.%d.%d\n",
                AIMU_LIS3MDL_MAJOR_VERSION,
                AIMU_LIS3MDL_MINOR_VERSION, 
                AIMU_LIS3MDL_REVISION, 
                AIMU_LIS3MDL_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of AIMU_LIS3MDL_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LIS3MDL_ProcessCommandPacket                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the LIS3MDL  */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LIS3MDL_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(AIMU_LIS3MDLMsgPtr);

    switch (MsgId)
    {
        case AIMU_LIS3MDL_CMD_MID:
            AIMU_LIS3MDL_ProcessGroundCommand();
            break;

        case AIMU_LIS3MDL_SEND_HK_MID:
            AIMU_LIS3MDL_ReportHousekeeping();
            break;

        default:
            AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_error_count++;
            CFE_EVS_SendEvent(AIMU_LIS3MDL_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"AIMU_LIS3MDL: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End AIMU_LIS3MDL_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MPL3115A2_ProcessGroundCommand() -- MPL3115A2 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void AIMU_LIS3MDL_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(AIMU_LIS3MDLMsgPtr);

    /* Process "known" LIS3MDL app ground commands */
    switch (CommandCode)
    {
        case AIMU_LIS3MDL_NOOP_CC:
            AIMU_LIS3MDL_HkTelemetryPkt.mpl3115a2_command_count++;
            CFE_EVS_SendEvent(AIMU_LIS3MDL_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"AIMU_LIS3MDL: NOOP command");
            break;

        case AIMU_LIS3MDL_RESET_COUNTERS_CC:
            AIMU_LIS3MDL_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case AIMU_LIS3MDL_INIT:
            INIT_AIMU_LIS3MDL(1, &AIMU_LIS3MDL_HkTelemetryPkt);
            break;
        
        case AIMU_LIS3MDL_PROCESS:
            PROCESS_AIMU_LIS3MDL(1, &AIMU_LIS3MDL_HkTelemetryPkt, &AIMU_LIS3MDL_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of AIMU_LIS3MDL_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LIS3MDL_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LIS3MDL_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_HkTelemetryPkt);
    return;

} /* End of AIMU_LIS3MDL_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LIS3MDL_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MPL3115A2_ResetCounters(void)
{
    /* Status of commands processed by the MPL3115A2 App */
    AIMU_LIS3MDL_HkTelemetryPkt.mpl3115a2_command_count       = 0;
    AIMU_LIS3MDL_HkTelemetryPkt.mpl3115a2_command_error_count = 0;

    CFE_EVS_SendEvent(AIMU_LIS3MDL_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LIS3MDL: RESET command");
    return;

} /* End of AIMU_LIS3MDL_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LIS3MDL_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool AIMU_LIS3MDL_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(AIMU_LIS3MDL_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        AIMU_LIS3MDL_HkTelemetryPkt.mpl3115a2_command_error_count++;
    }

    return(result);

} /* End of AIMU_LIS3MDL_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_AIMU_LIS3MDL() -- This function initializes the LIS3MDL according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_AIMU_LIS3MDL(int I2CBus, mpl3115a2_hk_tlm_t* AIMU_LIS3MDL_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, AIMU_LIS3MDL_I2C_ADDR);
	
	// Place LIS3MDL into standby mode
	if(!I2C_write(file, AIMU_LIS3MDL_CTRL_REG1, 0))
	{
		CFE_EVS_SendEvent(AIMU_LIS3MDL_FAILED_CHANGE_TO_STANDBY_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to place MPL3115A2 into Standby Mode... ");
        AIMU_LIS3MDL_HkTelemetryPkt->mpl3115a2_device_error_count++;
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