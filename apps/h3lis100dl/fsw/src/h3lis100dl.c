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
** File: h3lis100dl.c
**
** Purpose:
**   This file contains the source code for the H3LIS100DL App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "h3lis100dl.h"
#include "h3lis100dl_perfids.h"
#include "h3lis100dl_msgids.h"
#include "h3lis100dl_msg.h"
#include "h3lis100dl_events.h"
#include "h3lis100dl_version.h"

#include "i2c_lib.h"

/*
** global data
*/

h3lis100dl_hk_tlm_t       H3LIS100DL_HkTelemetryPkt;
h3lis100dl_data_tlm_t     H3LIS100DL_DataTelemetryPkt;
CFE_SB_PipeId_t              H3LIS100DL_CommandPipe;
CFE_SB_MsgPtr_t              H3LIS100DLMsgPtr;

static CFE_EVS_BinFilter_t  H3LIS100DL_EventFilters[] =
       {  /* Event ID    mask */
          {H3LIS100DL_STARTUP_INF_EID,       0x0000},
          {H3LIS100DL_COMMAND_ERR_EID,       0x0000},
          {H3LIS100DL_COMMANDNOP_INF_EID,    0x0000},
          {H3LIS100DL_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* H3LIS100DL_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void H3LIS100DL_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(H3LIS100DL_PERF_ID);

    H3LIS100DL_AppInit();

    INIT_H3LIS100DL(1, &H3LIS100DL_HkTelemetryPkt);

    //After Initialization
    H3LIS100DL_HkTelemetryPkt.AppStatus = RunStatus;
    /*
    ** H3LIS100DL Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(H3LIS100DL_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&H3LIS100DLMsgPtr, H3LIS100DL_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(H3LIS100DL_PERF_ID);

        // Process Data
        // PROCESS_H3LIS100DL(1, &H3LIS100DL_HkTelemetryPkt, &H3LIS100DL_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            H3LIS100DL_ProcessCommandPacket();
        }

    }
    //After While Loop
    H3LIS100DL_HkTelemetryPkt.AppStatus = RunStatus;

    CFE_ES_ExitApp(RunStatus);

} /* End of H3LIS100DL_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* H3LIS100DL_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void H3LIS100DL_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(H3LIS100DL_EventFilters,
                     sizeof(H3LIS100DL_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&H3LIS100DL_CommandPipe, H3LIS100DL_PIPE_DEPTH,"H3LIS100DL_CMD_PIPE");
    CFE_SB_Subscribe(H3LIS100DL_CMD_MID, H3LIS100DL_CommandPipe);
    CFE_SB_Subscribe(H3LIS100DL_SEND_HK_MID, H3LIS100DL_CommandPipe);
    CFE_SB_Subscribe(H3LIS100DL_SEND_DATA_MID, H3LIS100DL_CommandPipe);

    H3LIS100DL_ResetCounters();

    CFE_SB_InitMsg(&H3LIS100DL_HkTelemetryPkt,
                   H3LIS100DL_HK_TLM_MID,
                   H3LIS100DL_HK_TLM_LNGTH, true);
    
    CFE_SB_InitMsg(&H3LIS100DL_DataTelemetryPkt,
                   H3LIS100DL_DATA_TLM_MID,
                   H3LIS100DL_DATA_TLM_LNGTH, true);

    CFE_EVS_SendEvent (H3LIS100DL_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "H3LIS100DL App Initialized. Version %d.%d.%d.%d\n",
                H3LIS100DL_MAJOR_VERSION,
                H3LIS100DL_MINOR_VERSION, 
                H3LIS100DL_REVISION, 
                H3LIS100DL_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of H3LIS100DL_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  H3LIS100DL_ProcessCommandPacket                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the H3LIS100DL  */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void H3LIS100DL_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(H3LIS100DLMsgPtr);

    switch (MsgId)
    {
        case H3LIS100DL_CMD_MID:
            H3LIS100DL_ProcessGroundCommand();
            break;

        case H3LIS100DL_SEND_HK_MID:
            H3LIS100DL_ReportHousekeeping();
            break;

        case H3LIS100DL_SEND_DATA_MID:
            PROCESS_H3LIS100DL(1, &H3LIS100DL_HkTelemetryPkt, &H3LIS100DL_DataTelemetryPkt);
            break;

        default:
            H3LIS100DL_HkTelemetryPkt.h3lis100dl_command_error_count++;
            CFE_EVS_SendEvent(H3LIS100DL_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"H3LIS100DL: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End H3LIS100DL_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* H3LIS100DL_ProcessGroundCommand() -- H3LIS100DL ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void H3LIS100DL_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(H3LIS100DLMsgPtr);

    /* Process "known" H3LIS100DL app ground commands */
    switch (CommandCode)
    {
        case H3LIS100DL_NOOP_CC:
            H3LIS100DL_HkTelemetryPkt.h3lis100dl_command_count++;
            CFE_EVS_SendEvent(H3LIS100DL_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"H3LIS100DL: NOOP command");
            break;

        case H3LIS100DL_RESET_COUNTERS_CC:
            H3LIS100DL_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case H3LIS100DL_INIT:
            INIT_H3LIS100DL(1, &H3LIS100DL_HkTelemetryPkt);
            break;
        
        case H3LIS100DL_PROCESS:
            PROCESS_H3LIS100DL(1, &H3LIS100DL_HkTelemetryPkt, &H3LIS100DL_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of H3LIS100DL_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  H3LIS100DL_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void H3LIS100DL_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &H3LIS100DL_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &H3LIS100DL_HkTelemetryPkt);
    return;

} /* End of H3LIS100DL_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MAX7502_V2_SendDataPacket                                         */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the ram folder via DS       */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void H3LIS100DL_SendDataPacket(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &H3LIS100DL_DataTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &H3LIS100DL_DataTelemetryPkt);
    return;

} /* End of H3LIS100DL_SendDataPacket() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  H3LIS100DL_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void H3LIS100DL_ResetCounters(void)
{
    /* Status of commands processed by the H3LIS100DL App */
    H3LIS100DL_HkTelemetryPkt.h3lis100dl_command_count       = 0;
    H3LIS100DL_HkTelemetryPkt.h3lis100dl_command_error_count = 0;

    CFE_EVS_SendEvent(H3LIS100DL_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"H3LIS100DL: RESET command");
    return;

} /* End of H3LIS100DL_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* H3LIS100DL_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool H3LIS100DL_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(H3LIS100DL_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        H3LIS100DL_HkTelemetryPkt.h3lis100dl_command_error_count++;
    }

    return(result);

} /* End of H3LIS100DL_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_H3LIS100DL() -- This function initializes the H3LIS100DL according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_H3LIS100DL(int I2CBus, h3lis100dl_hk_tlm_t* H3LIS100DL_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, H3LIS100DL_I2C_ADDR);

    // Accel
    // ODR = 00110 (400 Hz (high performance)); 111 to enable all exes
	if(!I2C_write(file, H3LIS100DL_CTRL1_G, 0x37))
	{
		CFE_EVS_SendEvent(H3LIS100DL_FAILED_ENABLE_AXES, CFE_EVS_EventType_ERROR,
           "Failed to switch enable axes...  ");
        H3LIS100DL_HkTelemetryPkt->h3lis100dl_device_error_count++;

		return false;
	}

    // ODR = 0110 (Normal Mode);  HPc = 64 => 11
	if(!I2C_write(file, H3LIS100DL_CTRL2_G, 0x03))
	{
		CFE_EVS_SendEvent(H3LIS100DL_FAILED_CONFIGURE_HPCFILTER, CFE_EVS_EventType_ERROR,
           "Failed to configure high-pass filter cutoff frequency... ");
        H3LIS100DL_HkTelemetryPkt->h3lis100dl_device_error_count++;

		return false;
	} 

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_H3LIS100DL() -- This function process the H3LIS100DL data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_H3LIS100DL(int i2cbus, h3lis100dl_hk_tlm_t* H3LIS100DL_HkTelemetryPkt, h3lis100dl_data_tlm_t* H3LIS100DL_DataTelemetryPkt)
{

    //define needed variables for data gathering
    

	// Open the I2C Device
	int file = I2C_open(i2cbus, H3LIS100DL_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file, H3LIS100DL_STATUS_REG, 15, H3LIS100DL.status);
	if (H3LIS100DL.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, (H3LIS100DL_OUTX - 1U), 6, H3LIS100DL.buffer))
		{
			CFE_EVS_SendEvent(H3LIS100DL_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	H3LIS100DL_HkTelemetryPkt->h3lis100dl_device_error_count++;

			return;
		}

		/* Process the Data Buffer */

        // Accel
		uint8_t xha, xla, yha, yla, zha, zla;

		xha = H3LIS100DL.buffer[0];
		xla = H3LIS100DL.buffer[1];
		yha = H3LIS100DL.buffer[2];
        yla	= H3LIS100DL.buffer[3];
        zha = H3LIS100DL.buffer[4];
        zla = H3LIS100DL.buffer[5];


        int16_t ax, ay, az;

        ax = ((int16_t)xla * 256) + (int16_t)xha;
        ay = ((int16_t)yla * 256) + (int16_t)yha;
        az = ((int16_t)zla * 256) + (int16_t)zha;

        float accelx, accely, accelz;

        accelx = (float_t)ax / 256.0f * 780.0f; //convert to mg (engineering units)
        accely = (float_t)ay / 256.0f * 780.0f; //convert to mg (engineering units)
        accelz = (float_t)az / 256.0f * 780.0f; //convert to mg (engineering units)
        

		// Store into packet
		H3LIS100DL_DataTelemetryPkt->H3LIS100DL_ACCELERATIONX = accelx;
        H3LIS100DL_DataTelemetryPkt->H3LIS100DL_ACCELERATIONY = accely;
        H3LIS100DL_DataTelemetryPkt->H3LIS100DL_ACCELERATIONZ = accelz;

        H3LIS100DL_SendDataPacket();

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(H3LIS100DL_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Acceleration (x, y, z): %F, %F, %F", accelx, accely, accelz);
	}

	// Close the I2C Buffer
	I2C_close(file);
}