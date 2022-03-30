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
** File: aimu_lis3mdl.c
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
          {AIMU_LIS3MDL_COMMANDRST_INF_EID,    0x0000},
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

    INIT_AIMU_LIS3MDL(2, &AIMU_LIS3MDL_HkTelemetryPkt); //initialize device


    //After Initialization
    AIMU_LIS3MDL_HkTelemetryPkt.AppStatus = RunStatus;

    /*
    ** AIMU_LIS3MDL Runloop
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
    //After While Loop
    AIMU_LIS3MDL_HkTelemetryPkt.AppStatus = RunStatus;

    CFE_ES_ExitApp(RunStatus);

} /* End of AIMU_LIS3MDL_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* AIMU_LIS3MDL_AppInit() --  initialization                                     */
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
    CFE_SB_CreatePipe(&AIMU_LIS3MDL_CommandPipe, AIMU_LIS3MDL_PIPE_DEPTH, "LIS3MDL_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LIS3MDL_CMD_MID, AIMU_LIS3MDL_CommandPipe);
    CFE_SB_Subscribe(AIMU_LIS3MDL_SEND_HK_MID, AIMU_LIS3MDL_CommandPipe);
    CFE_SB_Subscribe(AIMU_LIS3MDL_SEND_DATA_MID, AIMU_LIS3MDL_CommandPipe);

    AIMU_LIS3MDL_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LIS3MDL_HkTelemetryPkt,
                   AIMU_LIS3MDL_HK_TLM_MID,
                   AIMU_LIS3MDL_HK_TLM_LNGTH, true);

    CFE_SB_InitMsg(&AIMU_LIS3MDL_DataTelemetryPkt,
                   AIMU_LIS3MDL_DATA_TLM_MID,
                   AIMU_LIS3MDL_DATA_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LIS3MDL_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "AIMU_LIS3MDL App Initialized. Version %d.%d.%d.%d\n",
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

        case AIMU_LIS3MDL_SEND_DATA_MID:
            PROCESS_AIMU_LIS3MDL(2, &AIMU_LIS3MDL_HkTelemetryPkt, &AIMU_LIS3MDL_DataTelemetryPkt);
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
/* AIMU_LIS3MDL_ProcessGroundCommand() -- AIMU_LIS3MDL ground commands              */
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
            AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_count++;
            CFE_EVS_SendEvent(AIMU_LIS3MDL_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"AIMU_LIS3MDL: NOOP command");
            break;

        case AIMU_LIS3MDL_RESET_COUNTERS_CC:
            AIMU_LIS3MDL_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case AIMU_LIS3MDL_INIT:
            INIT_AIMU_LIS3MDL(2, &AIMU_LIS3MDL_HkTelemetryPkt);
            break;
        
        case AIMU_LIS3MDL_PROCESS:
            PROCESS_AIMU_LIS3MDL(2, &AIMU_LIS3MDL_HkTelemetryPkt, &AIMU_LIS3MDL_DataTelemetryPkt);
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
/*  Name:  AIMU_LIS3MDL_SendDataPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the ram folder via DS       */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LIS3MDL_SendDataPacket(void)
{


    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_DataTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_DataTelemetryPkt);
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
void AIMU_LIS3MDL_ResetCounters(void)
{
    /* Status of commands processed by the AIMU_LIS3MDL App */
    AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_count       = 0;
    AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_error_count = 0;

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
        AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_error_count++;
    }

    return(result);

} /* End of AIMU_LIS3MDL_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_AIMU_LIS3MDL() -- This function initializes the LIS3MDL according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_AIMU_LIS3MDL(int I2CBus, aimu_lis3mdl_hk_tlm_t* AIMU_LIS3MDL_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, AIMU_LIS3MDL_I2C_ADDR);
	
	// Place Full Scale +-12 Hz
    if(!I2C_write(file, AIMU_LIS3MDL_CTRL_REG2, 0x40))
    {
        CFE_EVS_SendEvent(AIMU_LIS3MDL_FAILED_FULL_SCALE_CHANGE, CFE_EVS_EventType_ERROR,
           "Failed to place Full Scale +-12 Hz... ");
        AIMU_LIS3MDL_HkTelemetryPkt->aimu_lis3mdl_device_error_count++;
        return false;
    }

    //  Sets UHP mode on the X/Y axes, ODR at 80 Hz and does not activate temperature sensor
    if(!I2C_write(file, AIMU_LIS3MDL_CTRL_REG1, 0x7C))
    {
        CFE_EVS_SendEvent(AIMU_LIS3MDL_FAIL_ACTIVATE_TEMP_EID, CFE_EVS_EventType_ERROR,
           "Failed to activate the temperature sensor...  ");
        AIMU_LIS3MDL_HkTelemetryPkt->aimu_lis3mdl_device_error_count++;

        return false;
    }

    // Sets UHP mode on the Z-axis --> Big Endian data selection
    if(!I2C_write(file, AIMU_LIS3MDL_CTRL_REG4, 0x0C))
    {
        CFE_EVS_SendEvent(AIMU_LIS3MDL_ACTIVE_ZUHP_EID, CFE_EVS_EventType_ERROR,
           "Failed to enable events on the LPS25H...  ");
        AIMU_LIS3MDL_HkTelemetryPkt->aimu_lis3mdl_device_error_count++;

        return false;
    }

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_AIMU_LIS3MDL() -- This function process the AIMU_LIS3MDL data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_AIMU_LIS3MDL(int i2cbus, aimu_lis3mdl_hk_tlm_t* AIMU_LIS3MDL_HkTelemetryPkt, aimu_lis3mdl_data_tlm_t* AIMU_LIS3MDL_DataTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, AIMU_LIS3MDL_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file, AIMU_LIS3MDL_STATUS_REG, 1, AIMU_LIS3MDL.status);
	if (AIMU_LIS3MDL.status[0] != 0) //double check this
	{
        float scale = 2281.0; //scale factor
		// Read the Data Buffer
		if(!I2C_read(file, AIMU_LIS3MDL_OUT_X_L, 6, AIMU_LIS3MDL.buffer))
		{
			CFE_EVS_SendEvent(AIMU_LIS3MDL_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	AIMU_LIS3MDL_HkTelemetryPkt->aimu_lis3mdl_device_error_count++;

			return;
		}

		/* Process the Data Buffer */
			
		// Magnetic readings
		uint8_t xlm, xhm, ylm, yhm, zlm, zhm;

		xlm = AIMU_LIS3MDL.buffer[0];
		xhm = AIMU_LIS3MDL.buffer[1];
		ylm = AIMU_LIS3MDL.buffer[2];
        yhm = AIMU_LIS3MDL.buffer[3];
		zlm = AIMU_LIS3MDL.buffer[4];
		zhm = AIMU_LIS3MDL.buffer[5];	

        int16_t x, y, z; //combine low and high bits
        x = ((int16_t)xhm << 8 | (int16_t)xlm);
        y = ((int16_t)yhm << 8 | (int16_t)ylm);
        z = ((int16_t)zhm << 8 | (int16_t)zlm);

        //read magnetic field
        float magx, magy, magz; //divide by scale factor
        magx = (float)x / scale;
        magy = (float)y / scale;
        magz = (float)z / scale;

		// Store into packet
		AIMU_LIS3MDL_DataTelemetryPkt->AIMU_LIS3MDL_MAGSIGX = magx;
        AIMU_LIS3MDL_DataTelemetryPkt->AIMU_LIS3MDL_MAGSIGY = magy;
        AIMU_LIS3MDL_DataTelemetryPkt->AIMU_LIS3MDL_MAGSIGZ = magz;

        AIMU_LIS3MDL_SendDataPacket(); //send data packet telemetry

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(AIMU_LIS3MDL_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Mag-x: %F Mag-y: %F  Mag-z: %F ", magx, magy, magz);
	}

	// Close the I2C Buffer
	I2C_close(file);
}