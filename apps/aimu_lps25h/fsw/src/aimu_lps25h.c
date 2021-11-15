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
** File: aimu_lps25h.c
**
** Purpose:
**   This file contains the source code for the LPS25H App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "aimu_lps25h.h"
#include "aimu_lps25h_perfids.h"
#include "aimu_lps25h_msgids.h"
#include "aimu_lps25h_msg.h"
#include "aimu_lps25h_events.h"
#include "aimu_lps25h_version.h"

#include "i2c_lib.h"

/*
** global data
*/

aimu_lps25h_hk_tlm_t         AIMU_LPS25H_HkTelemetryPkt;
aimu_lps25h_data_tlm_t       AIMU_LPS25H_DataTelemetryPkt;
CFE_SB_PipeId_t              AIMU_LPS25H_CommandPipe;
CFE_SB_MsgPtr_t              AIMU_LPS25HMsgPtr;

static CFE_EVS_BinFilter_t  AIMU_LPS25H_EventFilters[] =
       {  /* Event ID    mask */
          {AIMU_LPS25H_STARTUP_INF_EID,       0x0000},
          {AIMU_LPS25H_COMMAND_ERR_EID,       0x0000},
          {AIMU_LPS25H_COMMANDNOP_INF_EID,    0x0000},
          {AIMU_LPS25H_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* AIMU_LPS25H_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LPS25H_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LPS25H_PERF_ID);
    AIMU_LPS25H_AppInit();

    //After Initialization
    AIMU_LPS25H_HkTelemetryPkt.AppStatus = RunStatus;
    /*
    ** LPS25H Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LPS25H_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LPS25HMsgPtr, AIMU_LPS25H_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(AIMU_LPS25H_PERF_ID);

        // Process Data
        // PROCESS_AIMU_LPS25H(1, &AIMU_LPS25H_HkTelemetryPkt, &AIMU_LPS25H_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            AIMU_LPS25H_ProcessCommandPacket();
        }

    }
    //After While Loop
    AIMU_LPS25H_HkTelemetryPkt.AppStatus = RunStatus;

    CFE_ES_ExitApp(RunStatus);

} /* End of AIMU_LPS25H_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/*AIMU_LPS25H_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void AIMU_LPS25H_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(AIMU_LPS25H_EventFilters,
                     sizeof(AIMU_LPS25H_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&AIMU_LPS25H_CommandPipe, AIMU_LPS25H_PIPE_DEPTH,"LPS25H_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LPS25H_CMD_MID, AIMU_LPS25H_CommandPipe);
    CFE_SB_Subscribe(AIMU_LPS25H_SEND_HK_MID, AIMU_LPS25H_CommandPipe);

    AIMU_LPS25H_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LPS25H_HkTelemetryPkt,
                   AIMU_LPS25H_HK_TLM_MID,
                   AIMU_LPS25H_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LPS25H_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "LPS25H App Initialized. Version %d.%d.%d.%d\n",
                AIMU_LPS25H_MAJOR_VERSION,
                AIMU_LPS25H_MINOR_VERSION, 
                AIMU_LPS25H_REVISION, 
                AIMU_LPS25H_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of AIMU_LPS25H_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LPS25H_ProcessCommandPacket                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the LPS25H  */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LPS25H_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(AIMU_LPS25HMsgPtr);

    switch (MsgId)
    {
        case AIMU_LPS25H_CMD_MID:
            AIMU_LPS25H_ProcessGroundCommand();
            break;

        case AIMU_LPS25H_SEND_HK_MID:
            AIMU_LPS25H_ReportHousekeeping();
            break;

        default:
            AIMU_LPS25H_HkTelemetryPkt.aimu_lps25h_command_error_count++;
            CFE_EVS_SendEvent(AIMU_LPS25H_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"AIMU_LPS25H: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End AIMU_LPS25H_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*AIMU_LPS25H_ProcessGroundCommand() -- LPS25H ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void AIMU_LPS25H_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(AIMU_LPS25HMsgPtr);

    /* Process "known" LPS25H app ground commands */
    switch (CommandCode)
    {
        case AIMU_LPS25H_NOOP_CC:
            AIMU_LPS25H_HkTelemetryPkt.aimu_lps25h_command_count++;
            CFE_EVS_SendEvent(AIMU_LPS25H_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"AIMU_LPS25H: NOOP command");
            break;

        case AIMU_LPS25H_RESET_COUNTERS_CC:
            AIMU_LPS25H_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case AIMU_LPS25H_INIT:
            INIT_AIMU_LPS25H(1, &AIMU_LPS25H_HkTelemetryPkt);
            break;
        
        case AIMU_LPS25H_PROCESS:
            PROCESS_AIMU_LPS25H(1, &AIMU_LPS25H_HkTelemetryPkt, &AIMU_LPS25H_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of AIMU_LPS25H_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LPS25H_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LPS25H_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LPS25H_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LPS25H_HkTelemetryPkt);
    return;

} /* End of AIMU_LPS25H_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LPS25H_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LPS25H_ResetCounters(void)
{
    /* Status of commands processed by the LPS25H App */
    AIMU_LPS25H_HkTelemetryPkt.aimu_lps25h_command_count       = 0;
    AIMU_LPS25H_HkTelemetryPkt.aimu_lps25h_command_error_count = 0;

    CFE_EVS_SendEvent(AIMU_LPS25H_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LPS25H: RESET command");
    return;

} /* End of AIMU_LPS25H_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LPS25H_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool AIMU_LPS25H_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(AIMU_LPS25H_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        AIMU_LPS25H_HkTelemetryPkt.aimu_lps25h_command_error_count++;
    }

    return(result);

} /* End of AIMU_LPS25H_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_AIMU_LPS25H() -- This function initializes the LPS25H according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_AIMU_LPS25H(int I2CBus, aimu_lps25h_hk_tlm_t* AIMU_LPS25H_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, AIMU_LPS25H_I2C_ADDR_W);
	
	// Set Device to Continuous mode and to 12.5 Hz
	if(!I2C_write(file, AIMU_LPS25H_CTRL_REG1, 0x58))
	{
		CFE_EVS_SendEvent(AIMU_LPS25H_FAILED_TO_ACTIVATE_EID, CFE_EVS_EventType_ERROR,
           "Failed to activate the sensor properly... ");
        AIMU_LPS25H_HkTelemetryPkt->aimu_lps25h_device_error_count++;
		return false;
	}

	// Sets sample rate per measurement cycle to 8
	if(!I2C_write(file, AIMU_LPS25H_RES_CONF, 0x00))
	{
		CFE_EVS_SendEvent(AIMU_LPS25H_SAMPLE_RATE_EID, CFE_EVS_EventType_ERROR,
           "Failed to set sample rate correctly...  ");
        AIMU_LPS25H_HkTelemetryPkt->aimu_lps25h_device_error_count++;

		return false;
	}

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_LPS25H() -- This function process the LPS25H data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_LPS25H(int i2cbus, aimu_lps25h_hk_tlm_t* AIMU_LPS25H_HkTelemetryPkt, aimu_lps25h_data_tlm_t* AIMU_LPS25H_DataTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, AIMU_LPS25H_I2C_ADDR_R);

	// Check for data in the STATUS register
	I2C_read(file, AIMU_LPS25H_STATUS_REG, 1, AIMU_LPS25H.status);
	if (AIMU_LPS25H.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, AIMU_LPS25H_PRESS_POUT_XL, 5, AIMU_LPS25H.buffer))
		{
			CFE_EVS_SendEvent(AIMU_LPS25H_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	AIMU_LPS25H_HkTelemetryPkt->aimu_lps25h_device_error_count++;

			return;
		}

		/* Process the Data Buffer */
			
		// Pressure
		int32_t press;

		press = AIMU_LPS25H.buffer[0] << 24;
		press |= AIMU_LPS25H.buffer[1] << 16;
		press |= AIMU_LPS25H.buffer[2] << 8;

		float pressure = press;		

		// Temperature
		int16_t t;

		t = AIMU_LPS25H.buffer[3];
		t <<= 8;
		t |= AIMU_LPS25H.buffer[4];
		t >>= 4;

		if(t & 0x800)
		{
			t |= 0xF000;
		}

		float temp = t;
		temp /= 16.0;

		// Store into packet
		AIMU_LPS25H_DataTelemetryPkt->AIMU_LPS25H_PRESSURE = pressure;
		AIMU_LPS25H_DataTelemetryPkt->AIMU_LPS25H_TEMPERATURE = temp;

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(AIMU_LPS25H_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Pressure: %F Temperature: %F ", pressure, temp);
	}

	// Close the I2C Buffer
	I2C_close(file);
}