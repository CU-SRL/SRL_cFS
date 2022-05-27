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
** File: max7502.c
**
** Purpose:
**   This file contains the source code for the MAX7502 App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "max7502.h"
#include "max7502_perfids.h"
#include "max7502_msgids.h"
#include "max7502_msg.h"
#include "max7502_events.h"
#include "max7502_version.h"

#include "i2c_lib.h"

/*
** global data
*/

max7502_hk_tlm_t       MAX7502_HkTelemetryPkt;
max7502_data_tlm_t     MAX7502_DataTelemetryPkt;
CFE_SB_PipeId_t          MAX7502_CommandPipe;
CFE_SB_MsgPtr_t          MAX7502MsgPtr;

static CFE_EVS_BinFilter_t  MAX7502_EventFilters[] =
       {  /* Event ID    mask */
          {MAX7502_STARTUP_INF_EID,       0x0000},
          {MAX7502_COMMAND_ERR_EID,       0x0000},
          {MAX7502_COMMANDNOP_INF_EID,    0x0000},
          {MAX7502_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* MAX7502_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void MAX7502_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(MAX7502_PERF_ID);

    MAX7502_AppInit();

    INIT_MAX7502(2, &MAX7502_HkTelemetryPkt, 1);
    INIT_MAX7502(2, &MAX7502_HkTelemetryPkt, 2);

    MAX7502_HkTelemetryPkt.AppStatus = RunStatus;

    /*
    ** MAX7502 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(MAX7502_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&MAX7502MsgPtr, MAX7502_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(MAX7502_PERF_ID);

        // Process Data
        // PROCESS_MAX7502(1, &MAX7502_HkTelemetryPkt, &MAX7502_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            MAX7502_ProcessCommandPacket();
        }
        
        MAX7502_HkTelemetryPkt.AppStatus = RunStatus;
    }

    CFE_ES_ExitApp(RunStatus);

} /* End of MAX7502_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* MAX7502_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void MAX7502_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(MAX7502_EventFilters,
                     sizeof(MAX7502_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&MAX7502_CommandPipe, MAX7502_PIPE_DEPTH,"MAX7502_CMD_PIPE");
    CFE_SB_Subscribe(MAX7502_CMD_MID, MAX7502_CommandPipe);
    CFE_SB_Subscribe(MAX7502_SEND_HK_MID, MAX7502_CommandPipe);
    CFE_SB_Subscribe(MAX7502_SEND_DATA_MID, MAX7502_CommandPipe);

    MAX7502_ResetCounters();

    CFE_SB_InitMsg(&MAX7502_HkTelemetryPkt,
                   MAX7502_HK_TLM_MID,
                   MAX7502_HK_TLM_LNGTH, true);
    
    CFE_SB_InitMsg(&MAX7502_DataTelemetryPkt,
                   MAX7502_DATA_TLM_MID,
                   MAX7502_DATA_TLM_LNGTH, true);

    CFE_EVS_SendEvent (MAX7502_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "MAX7502 App Initialized. Version %d.%d.%d.%d\n",
                MAX7502_MAJOR_VERSION,
                MAX7502_MINOR_VERSION, 
                MAX7502_REVISION, 
                MAX7502_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of MAX7502_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MAX7502_ProcessCommandPacket                                     */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the MAX7502 */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MAX7502_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(MAX7502MsgPtr);

    switch (MsgId)
    {
        case MAX7502_CMD_MID:
            MAX7502_ProcessGroundCommand();
            break;

        case MAX7502_SEND_HK_MID:
            MAX7502_ReportHousekeeping();
            break;

        case MAX7502_SEND_DATA_MID:
            PROCESS_MAX7502(2, &MAX7502_HkTelemetryPkt, &MAX7502_DataTelemetryPkt, 1);
            PROCESS_MAX7502(2, &MAX7502_HkTelemetryPkt, &MAX7502_DataTelemetryPkt, 2);
            break;

        default:
            MAX7502_HkTelemetryPkt.max7502_command_error_count++;
            CFE_EVS_SendEvent(MAX7502_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"MAX7502: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End MAX7502_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MAX7502_ProcessGroundCommand() -- MAX7502 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void MAX7502_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(MAX7502MsgPtr);

    /* Process "known" MAX7502 app ground commands */
    switch (CommandCode)
    {
        case MAX7502_NOOP_CC:
            MAX7502_HkTelemetryPkt.max7502_command_count++;
            CFE_EVS_SendEvent(MAX7502_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"MAX7502: NOOP command");
            break;

        case MAX7502_RESET_COUNTERS_CC:
            MAX7502_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case MAX7502_INIT:
            INIT_MAX7502(2, &MAX7502_HkTelemetryPkt, 1);
            INIT_MAX7502(2, &MAX7502_HkTelemetryPkt, 2);
            break;
        
        case MAX7502_PROCESS:
            PROCESS_MAX7502(2, &MAX7502_HkTelemetryPkt, &MAX7502_DataTelemetryPkt, 1);
            PROCESS_MAX7502(2, &MAX7502_HkTelemetryPkt, &MAX7502_DataTelemetryPkt, 2);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of MAX7502_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MAX7502_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MAX7502_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &MAX7502_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MAX7502_HkTelemetryPkt);
    return;

} /* End of MAX7502_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MAX7502_SendDataPacket                                         */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the ram folder via DS       */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MAX7502_SendDataPacket(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &MAX7502_DataTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MAX7502_DataTelemetryPkt);
    return;

} /* End of MAX7502_SendDataPacket() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MAX7502_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MAX7502_ResetCounters(void)
{
    /* Status of commands processed by the MAX7502 App */
    MAX7502_HkTelemetryPkt.max7502_command_count       = 0;
    MAX7502_HkTelemetryPkt.max7502_command_error_count = 0;

    CFE_EVS_SendEvent(MAX7502_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"MAX7502: RESET command");
    return;

} /* End of MAX7502_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MAX7502_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool MAX7502_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(MAX7502_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        MAX7502_HkTelemetryPkt.max7502_command_error_count++;
    }

    return(result);

} /* End of MAX7502_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_MAX7502() -- This function initializes the MAX7502 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_MAX7502(int I2CBus, max7502_hk_tlm_t* MAX7502_HkTelemetryPkt, uint8 DeviceNumber)
{
    int file;
	//Device
    // Open I2C 
	if(DeviceNumber==1){
        file = I2C_open(I2CBus, MAX7502_1_I2C_ADDR);
    }
    else{
        file = I2C_open(I2CBus, MAX7502_2_I2C_ADDR);
    }
	
	// Configure fault queue to 4 faults needed to trigger OS
	if(!I2C_write(file, MAX7502_CONFIG, 0x14))
	{
		CFE_EVS_SendEvent(MAX7502_FAILED_TO_CONFIGURE, CFE_EVS_EventType_ERROR,
           "Failed to place configure device %ld ... ", DeviceNumber);
        MAX7502_HkTelemetryPkt->max7502_device_error_count++;
		return false;
	}

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_MAX7502() -- This function process the MAX7502 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_MAX7502(int i2cbus, max7502_hk_tlm_t* MAX7502_HkTelemetryPkt, max7502_data_tlm_t* MAX7502_DataTelemetryPkt, uint8 DeviceNumber)
{
    int file;

	// Open the I2C Device
	if(DeviceNumber==1){
        file = I2C_open(i2cbus, MAX7502_1_I2C_ADDR);
    }
    else{
        file = I2C_open(i2cbus, MAX7502_2_I2C_ADDR);
    }

    MAX7502_DataTelemetryPkt->DeviceNumber = DeviceNumber; //set device number so telemetry packets can be disinguished

	// Check for data in the STATUS register
	I2C_read(file, MAX7502_CONFIG, 1, MAX7502.status);
	if (MAX7502.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, MAX7502_AMBIENT_TEMP, 2, MAX7502.buffer))
		{
			CFE_EVS_SendEvent(MAX7502_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	MAX7502_HkTelemetryPkt->max7502_device_error_count++;

			return;
		}

		/* Process the Data Buffer */	

		// Temperature
		int16_t t;

		t = MAX7502.buffer[0];
		t <<= 8;
		t |= MAX7502.buffer[1];
		t >>= 7;

		if(t & 0x800)
		{
			t |= 0xF000;
		}

		float temp = t;
        temp /= 16.0;


		MAX7502_DataTelemetryPkt->MAX7502_TEMPERATURE = temp;

        MAX7502_SendDataPacket();

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(MAX7502_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Temperature: %F ", temp);
	}

	// Close the I2C Buffer
	I2C_close(file);
}