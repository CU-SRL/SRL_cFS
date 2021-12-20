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
** File: aimu_lsm6ds33.c
**
** Purpose:
**   This file contains the source code for the AIMU_LSM6DS33 App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "aimu_lsm6ds33.h"
#include "aimu_lsm6ds33_perfids.h"
#include "aimu_lsm6ds33_msgids.h"
#include "aimu_lsm6ds33_msg.h"
#include "aimu_lsm6ds33_events.h"
#include "aimu_lsm6ds33_version.h"

#include "i2c_lib.h"

/*
** global data
*/

aimu_lsm6ds33_hk_tlm_t       AIMU_LSM6DS33_HkTelemetryPkt;
aimu_lsm6ds33_data_tlm_t     AIMU_LSM6DS33_DataTelemetryPkt;
CFE_SB_PipeId_t              AIMU_LSM6DS33_CommandPipe;
CFE_SB_MsgPtr_t              AIMU_LSM6DS33MsgPtr;

static CFE_EVS_BinFilter_t  AIMU_LSM6DS33_EventFilters[] =
       {  /* Event ID    mask */
          {AIMU_LSM6DS33_STARTUP_INF_EID,       0x0000},
          {AIMU_LSM6DS33_COMMAND_ERR_EID,       0x0000},
          {AIMU_LSM6DS33_COMMANDNOP_INF_EID,    0x0000},
          {AIMU_LSM6DS33_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* AIMU_LSM6DS33_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LSM6DS33_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LSM6DS33_PERF_ID);

    AIMU_LSM6DS33_AppInit();

    //After Initialization
    AIMU_LSM6DS33_HkTelemetryPkt.AppStatus = RunStatus;
    /*
    ** AIMU_LSM6DS33 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LSM6DS33_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LSM6DS33MsgPtr, AIMU_LSM6DS33_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(AIMU_LSM6DS33_PERF_ID);

        // Process Data
        // PROCESS_AIMU_LSM6DS33(1, &AIMU_LSM6DS33_HkTelemetryPkt, &AIMU_LSM6DS33_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            AIMU_LSM6DS33_ProcessCommandPacket();
        }

    }
    //After While Loop
    AIMU_LSM6DS33_HkTelemetryPkt.AppStatus = RunStatus;

    CFE_ES_ExitApp(RunStatus);

} /* End of AIMU_LSM6DS33_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* AIMU_LSM6DS33_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void AIMU_LSM6DS33_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(AIMU_LSM6DS33_EventFilters,
                     sizeof(AIMU_LSM6DS33_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&AIMU_LSM6DS33_CommandPipe, AIMU_LSM6DS33_PIPE_DEPTH,"LSM6DS33_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LSM6DS33_CMD_MID, AIMU_LSM6DS33_CommandPipe);
    CFE_SB_Subscribe(AIMU_LSM6DS33_SEND_HK_MID, AIMU_LSM6DS33_CommandPipe);

    AIMU_LSM6DS33_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LSM6DS33_HkTelemetryPkt,
                   AIMU_LSM6DS33_HK_TLM_MID,
                   AIMU_LSM6DS33_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LSM6DS33_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "AIMU_LSM6DS33 App Initialized. Version %d.%d.%d.%d\n",
                AIMU_LSM6DS33_MAJOR_VERSION,
                AIMU_LSM6DS33_MINOR_VERSION, 
                AIMU_LSM6DS33_REVISION, 
                AIMU_LSM6DS33_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of AIMU_LSM6DS33_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_ProcessCommandPacket                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the LSM6DS33  */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(AIMU_LSM6DS33MsgPtr);

    switch (MsgId)
    {
        case AIMU_LSM6DS33_CMD_MID:
            AIMU_LSM6DS33_ProcessGroundCommand();
            break;

        case AIMU_LSM6DS33_SEND_HK_MID:
            AIMU_LSM6DS33_ReportHousekeeping();
            break;

        default:
            AIMU_LSM6DS33_HkTelemetryPkt.aimu_lsm6ds33_command_error_count++;
            CFE_EVS_SendEvent(AIMU_LSM6DS33_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"AIMU_LSM6DS33: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End AIMU_LSM6DS33_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LSM6DS33_ProcessGroundCommand() -- AIMU_LSM6DS33 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void AIMU_LSM6DS33_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(AIMU_LSM6DS33MsgPtr);

    /* Process "known" LSM6DS33 app ground commands */
    switch (CommandCode)
    {
        case AIMU_LSM6DS33_NOOP_CC:
            AIMU_LSM6DS33_HkTelemetryPkt.aimu_lsm6ds33_command_count++;
            CFE_EVS_SendEvent(AIMU_LSM6DS33_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"AIMU_LSM6DS33: NOOP command");
            break;

        case AIMU_LSM6DS33_RESET_COUNTERS_CC:
            AIMU_LSM6DS33_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case AIMU_LSM6DS33_INIT:
            INIT_AIMU_LSM6DS33(2, &AIMU_LSM6DS33_HkTelemetryPkt);
            break;
        
        case AIMU_LSM6DS33_PROCESS:
            PROCESS_AIMU_LSM6DS33(2, &AIMU_LSM6DS33_HkTelemetryPkt, &AIMU_LSM6DS33_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of AIMU_LSM6DS33_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_HkTelemetryPkt);
    return;

} /* End of AIMU_LSM6DS33_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_ResetCounters(void)
{
    /* Status of commands processed by the AIMU_LSM6DS33 App */
    AIMU_LSM6DS33_HkTelemetryPkt.aimu_lsm6ds33_command_count       = 0;
    AIMU_LSM6DS33_HkTelemetryPkt.aimu_lsm6ds33_command_error_count = 0;

    CFE_EVS_SendEvent(AIMU_LSM6DS33_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LSM6DS33: RESET command");
    return;

} /* End of AIMU_LSM6DS33_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LSM6DS33_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool AIMU_LSM6DS33_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(AIMU_LSM6DS33_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        AIMU_LSM6DS33_HkTelemetryPkt.aimu_lsm6ds33_command_error_count++;
    }

    return(result);

} /* End of AIMU_LSM6DS33_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_AIMU_LSM6DS33() -- This function initializes the LSM6DS33 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_AIMU_LSM6DS33(int I2CBus, aimu_lsm6ds33_hk_tlm_t* AIMU_LSM6DS33_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, AIMU_LSM6DS33_I2C_ADDR);
	
	// Accelerometer Axes Enabled
	if(!I2C_write(file, AIMU_LSM6DS33_CTRL9_XL, 0x38))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_FAILED_ENABLE_AXES, CFE_EVS_EventType_ERROR,
           "Failed to enable AIMU_LSM6DS33 Accel Axes... ");
        AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;
		return false;
	}

    // Switch the Accel to active, set mode to High performance (416 Hz)
	if(!I2C_write(file, AIMU_LSM6DS33_CTRL1_XL, 0x60))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch Accel to active...  ");
        AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;

		return false;
	}

	// Gyro Axes Enabled
	if(!I2C_write(file, AIMU_LSM6DS33_CTRL10_C, 0x38))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_FAILED_ENABLE_AXES, CFE_EVS_EventType_ERROR,
           "Failed to enable AIMU_LSM6DS33 Gyro Axes... ");
        AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;
		return false;
	}

    // Switch the Gyro to active, set mode to High performance (416 Hz)
	if(!I2C_write(file, AIMU_LSM6DS33_CTRL2_G, 0x60))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch Gyro to active...  ");
        AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;

		return false;
	} 

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_AIMU_LSM6DS33() -- This function process the AIMU_LSM6DS33 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_AIMU_LSM6DS33(int i2cbus, aimu_lsm6ds33_hk_tlm_t* AIMU_LSM6DS33_HkTelemetryPkt, aimu_lsm6ds33_data_tlm_t* AIMU_LSM6DS33_DataTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, AIMU_LSM6DS33_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file, AIMU_LSM6DS33_STATUS_REG, 1, AIMU_LSM6DS33.status);
	if (AIMU_LSM6DS33.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, AIMU_LSM6DS33_OUTX_L_G, 12, AIMU_LSM6DS33.buffer))
		{
			CFE_EVS_SendEvent(AIMU_LSM6DS33_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;

			return;
		}

		/* Process the Data Buffer */
			
		// Altitude
		uint8_t xla, xha, yla, yha, zla, zha;
        float accx, accy, accz;

		xla = AIMU_LSM6DS33.buffer[0];
		xha = AIMU_LSM6DS33.buffer[1];
		yla = AIMU_LSM6DS33.buffer[2];
        yha = AIMU_LSM6DS33.buffer[3];
		zla = AIMU_LSM6DS33.buffer[4];
		zha = AIMU_LSM6DS33.buffer[5];	

        accx = (int16_t)(xha << 8 | xla);
        accy = (int16_t)(yha << 8 | yla);
        accz = (int16_t)(zha << 8 | zla);

        float mag = sqrt(((accx**2) + (accy**2) + (accz**2)))

        accx /= mag;
        accy /= mag;
        accz /= mag;

		// Gyro
		uint8_t xlg, xhg, ylg, yhg, zlg, zhg;
        float gyx, gyy, gyz;

		xlg = AIMU_LSM6DS33.buffer[6];
		xhg = AIMU_LSM6DS33.buffer[7];
		ylg = AIMU_LSM6DS33.buffer[8];
        yhg = AIMU_LSM6DS33.buffer[9];
		zlg = AIMU_LSM6DS33.buffer[10];
		zhg = AIMU_LSM6DS33.buffer[11];	

        gyx = (int16_t)(xhg << 8 | xlg);
        gyy = (int16_t)(yhg << 8 | ylg);
        gyz = (int16_t)(zhg << 8 | zlg);

        mag = sqrt(((gyx**2) + (gyy**2) + (gyz**2)))

        gyx /= mag;
        gyy /= mag;
        gyz /= mag;

		// Store into packet
		AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONX = accx;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONY = accy;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONZ = accz;
		AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEX = gyx;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEY = gyy;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEZ = gyz;

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(AIMU_LSM6DS33_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Acceleration (x, y, z): %F, %F, %F Angular Rate (x, y, z): %F, %F, %F ", accx, accy, accz, gyx, gyy, gyz);
	}

	// Close the I2C Buffer
	I2C_close(file);
}