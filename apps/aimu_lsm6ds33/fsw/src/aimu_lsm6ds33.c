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

    INIT_AIMU_LSM6DS33(2, &AIMU_LSM6DS33_HkTelemetryPkt);

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
    CFE_SB_Subscribe(AIMU_LSM6DS33_SEND_DATA_MID, AIMU_LSM6DS33_CommandPipe);

    AIMU_LSM6DS33_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LSM6DS33_HkTelemetryPkt,
                   AIMU_LSM6DS33_HK_TLM_MID,
                   AIMU_LSM6DS33_HK_TLM_LNGTH, true);

    CFE_SB_InitMsg(&AIMU_LSM6DS33_DataTelemetryPkt,
                   AIMU_LSM6DS33_DATA_TLM_MID,
                   AIMU_LSM6DS33_DATA_TLM_LNGTH, true);

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

        case AIMU_LSM6DS33_SEND_DATA_MID:
            PROCESS_AIMU_LSM6DS33(2, &AIMU_LSM6DS33_HkTelemetryPkt, &AIMU_LSM6DS33_DataTelemetryPkt);
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
/*  Name:  AIMU_LSM6DS33_SendDataPacket                                      */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the ram folder via DS       */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_SendDataPacket(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_DataTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_DataTelemetryPkt);
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
	int file = I2C_open(I2CBus);

    // Accel
    // ODR = 0110 (416 Hz (high performance)); FS_XL = 01 (+/-16 g full scale)
	if(!I2C_write(file, AIMU_LSM6DS33_I2C_ADDR, AIMU_LSM6DS33_CTRL1_XL, 0x64))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch Accel to active...  ");
        AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;

		return false;
	}

	// Gyro
    // ODR = 0110 (416 Hz (high performance)); FS_XL = 11 (2000dps)
	if(!I2C_write(file, AIMU_LSM6DS33_I2C_ADDR, AIMU_LSM6DS33_CTRL2_G, 0x68))
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

    //define needed variables for data gathering
    float gyro_scale = 70.0;
    float accel_scale = 0.488;
    float dps_to_rads = 0.017453293;
    float gravity_standard = 9.80665; //do not use because we want in g's

	// Open the I2C Device
	int file = I2C_open(i2cbus);

	// Check for data in the STATUS register
	I2C_multi_read(file, AIMU_LSM6DS33_I2C_ADDR, AIMU_LSM6DS33_STATUS_REG, 1, AIMU_LSM6DS33.status);
	if (AIMU_LSM6DS33.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_multi_read(file, AIMU_LSM6DS33_I2C_ADDR, AIMU_LSM6DS33_OUTX_L_G, 12, AIMU_LSM6DS33.buffer))
		{
			CFE_EVS_SendEvent(AIMU_LSM6DS33_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	AIMU_LSM6DS33_HkTelemetryPkt->aimu_lsm6ds33_device_error_count++;

			return;
		}

		/* Process the Data Buffer */

        //Temp --> not placed in data packet for now

		// Gyro in radians/second (have to convert from degrees per second)
		uint8_t xlg, xhg, ylg, yhg, zlg, zhg;
		xlg = AIMU_LSM6DS33.buffer[0];
		xhg = AIMU_LSM6DS33.buffer[1];
		ylg = AIMU_LSM6DS33.buffer[2];
        yhg = AIMU_LSM6DS33.buffer[3];
		zlg = AIMU_LSM6DS33.buffer[4];
		zhg = AIMU_LSM6DS33.buffer[5];	

        int16_t gx, gy, gz;
        gx = (xhg << 8) | xlg;
        if(gx & 0x800)
		{
			gx |= 0xF000;
		}

        gy = (yhg << 8) | ylg;
        if(gy & 0x800)
		{
			gy |= 0xF000;
		}

        gz = (zhg << 8) | zlg;
        if(gz & 0x800)
		{
			gz |= 0xF000;
		}

        float gyx, gyy, gyz;
        gyx = (float)gx * gyro_scale * dps_to_rads / 1000.0; //scale in mdps
        gyy = (float)gy * gyro_scale * dps_to_rads / 1000.0;
        gyz = (float)gz * gyro_scale * dps_to_rads / 1000.0;

        // Accel in g
		uint8_t xla, xha, yla, yha, zla, zha;

		xla = AIMU_LSM6DS33.buffer[6];
		xha = AIMU_LSM6DS33.buffer[7];
		yla = AIMU_LSM6DS33.buffer[8];
        yha = AIMU_LSM6DS33.buffer[9];
		zla = AIMU_LSM6DS33.buffer[10];
		zha = AIMU_LSM6DS33.buffer[11];	
        
        //combine high and low bits into twos complement number
        int16_t ax = (xha << 8 | xla);
        if(ax & 0x800)
		{
			ax |= 0xF000;
		}

        int16_t ay = (yha << 8 | yla);
        if(ay & 0x800)
		{
			ay |= 0xF000;
		}

        int16_t az = (zha << 8 | zla);
        if(az & 0x800)
		{
			az |= 0xF000;
		}

        float accelx = (float)ax * accel_scale / 1000.0; //scale in mgauss
        float accely = (float)ay * accel_scale / 1000.0;
        float accelz = (float)az * accel_scale / 1000.0;
        

		// Store into packet
		AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONX = accelx;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONY = accely;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ACCELERATIONZ = accelz;
		AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEX = gyx;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEY = gyy;
        AIMU_LSM6DS33_DataTelemetryPkt->AIMU_LSM6DS33_ANGULAR_RATEZ = gyz;

        AIMU_LSM6DS33_SendDataPacket();

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(AIMU_LSM6DS33_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Acceleration (x, y, z): %F, %F, %F Angular Rate (x, y, z): %F, %F, %F ", accelx, accely, accelz, gyx, gyy, gyz);
	}else{
        if(I2C_single_byte_read(file, AIMU_LSM6DS33_I2C_ADDR, AIMU_LSM6DS33_STATUS_REG, AIMU_LSM6DS33.status)==0){
            printf("Read lsm6ds33 status register with single byte: %d",AIMU_LSM6DS33.status);
        };
    }

	// Close the I2C Buffer
	I2C_close(file);
}