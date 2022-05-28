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
** File: aimu_lsm6ds33_v2.c
**
** Purpose:
**   This file contains the source code for the LSM6DS33_V2 app.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "aimu_lsm6ds33_v2.h"
#include "aimu_lsm6ds33_v2_perfids.h"
#include "aimu_lsm6ds33_v2_msgids.h"
#include "aimu_lsm6ds33_v2_msg.h"
#include "aimu_lsm6ds33_v2_events.h"
#include "aimu_lsm6ds33_v2_version.h"

#include "i2c_lib.h"

/**
 * change bit 7 based on SDO/SA0 pin
 * if SAD[0] = SA0, 0, else if 1
*/
#define AIMU_LSM6DS33_V2_SAD_READ       0xD7 /* this is conncted to supply voltage (default), if connected to ground: D5 */
#define AIMU_LSM6DS33_V2_SAD_WRITE      0xD6 /* this is conncted to supply voltage (default), if connected to ground: D4 */
#define CTRL1_XL                        0x10 /* accel activation control register */
#define CTRL2_G                         0x11 /* gyro activation control register */
#define STATUS_REG                      0x1E /* Status data register */
#define OUT_TEMP_L                      0x20 /* first register to be read (temperature) */

/*
** global data
*/

int bus = 2;

aimu_lsm6ds33_v2_hk_tlm_t    AIMU_LSM6DS33_V2_HkTelemetryPkt;
aimu_lsm6ds33_v2_data_tlm_t  AIMU_LSM6DS33_V2_DataTelemetryPkt;
CFE_SB_PipeId_t    AIMU_LSM6DS33_V2_CommandPipe;
CFE_SB_MsgPtr_t    AIMU_LSM6DS33_V2MsgPtr;

static CFE_EVS_BinFilter_t  AIMU_LSM6DS33_V2_EventFilters[] =
       {  /* Event ID    mask */
          {AIMU_LSM6DS33_V2_STARTUP_INF_EID,       0x0000},
          {AIMU_LSM6DS33_V2_COMMAND_ERR_EID,       0x0000},
          {AIMU_LSM6DS33_V2_COMMANDNOP_INF_EID,    0x0000},
          {AIMU_LSM6DS33_V2_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* AIMU_LSM6DS33_V2_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LSM6DS33_V2_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LSM6DS33_V2_PERF_ID);

    AIMU_LSM6DS33_V2_AppInit();

    //initializes lsm6ds33 device
    AIMU_LSM6DS33_V2_DeviceInit(bus, &AIMU_LSM6DS33_V2_HkTelemetryPkt);

    /*
    ** LSM6DS_V2 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LSM6DS33_V2_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LSM6DS33_V2MsgPtr, AIMU_LSM6DS33_V2_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(AIMU_LSM6DS33_V2_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            // process data function called here
            AIMU_LSM6DS33_V2_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of AIMU_LSM6DS33_V2_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* AIMU_LSM6DS33_V2_AppInit() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void AIMU_LSM6DS33_V2_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(AIMU_LSM6DS33_V2_EventFilters,
                     sizeof(AIMU_LSM6DS33_V2_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&AIMU_LSM6DS33_V2_CommandPipe, AIMU_LSM6DS33_V2_PIPE_DEPTH,"AIMU_LSM6DS33_V2_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LSM6DS33_V2_CMD_MID, AIMU_LSM6DS33_V2_CommandPipe);
    CFE_SB_Subscribe(AIMU_LSM6DS33_V2_SEND_HK_MID, AIMU_LSM6DS33_V2_CommandPipe);
    CFE_SB_Subscribe(AIMU_LSM6DS33_V2_SEND_DATA_MID, AIMU_LSM6DS33_V2_CommandPipe);

    AIMU_LSM6DS33_V2_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LSM6DS33_V2_HkTelemetryPkt,
                   AIMU_LSM6DS33_V2_HK_TLM_MID,
                   AIMU_LSM6DS33_V2_HK_TLM_LNGTH, true);

    CFE_SB_InitMsg(&AIMU_LSM6DS33_V2_DataTelemetryPkt,
                   AIMU_LSM6DS33_V2_DATA_TLM_MID,
                   AIMU_LSM6DS33_V2_DATA_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LSM6DS33_V2_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "AIMU_LSM6DS33_V2 App Initialized. Version %d.%d.%d.%d",
                AIMU_LSM6DS33_V2_MAJOR_VERSION,
                AIMU_LSM6DS33_V2_MINOR_VERSION, 
                AIMU_LSM6DS33_V2_REVISION, 
                AIMU_LSM6DS33_V2_MISSION_REV);
				
} /* End of AIMU_LSM6DS33_V2_AppInit() */

/**
 * @brief initilizes lsm6ds33 device, opening a file and writing to a register
 * 
 * @param i2cbus used for specifying path (name) of i2c buffer
 * @param AIMU_LSM6DS33_V2_HkTelemetryPkt empty housekeeping telemetry packet to be updated
 */

void AIMU_LSM6DS33_V2_DeviceInit(int i2cbus, aimu_lsm6ds33_v2_hk_tlm_t* AIMU_LSM6DS33_V2_HkTelemetryPkt)
{

	int file = I2C_open(i2cbus);

    // 01000100 - power on modes for accelerometer (104Hz ODR, 16g full-scale, 200Hz Bandwidth)
    // control register for activation: CTRL1_XL (11h)

	if(!I2C_write(file, AIMU_LSM6DS33_V2_SAD_WRITE, CTRL1_XL, 0x44))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_ACTIVATION_FAILURE_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch Accel to active...  ");
        AIMU_LSM6DS33_V2_HkTelemetryPkt->aimu_lsm6ds33_v2_device_error_count++;

		return false;
	}

    // 01000100 - power on modes for gyroscope (104Hz ODR, 500 dps full-scale)
    // control register for for activation: CTRL2_G (11h)

	if(!I2C_write(file, AIMU_LSM6DS33_V2_SAD_WRITE, CTRL2_G, 0x44))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_ACTIVATION_FAILURE_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch Gyro to active...  ");
        AIMU_LSM6DS33_V2_HkTelemetryPkt->aimu_lsm6ds33_v2_device_error_count++;

		return false;
	}

    /**
     * Auto-incrementation of register addresses enabled by default,
        so no need to configure in CTRL3_C 
     * 
     */

	I2C_close(file);

	return true;
}

/**
 * @brief reads from relevant registers, and sends data down software bus
 * 
 * @param i2cbus used for specifying path (name) of i2c buffer
 * @param AIMU_LSM6DS33_V2_HkTelemetryPkt housekeeping telemetry path to be updated
 * @param AIMU_LSM6DS33_V2_DataTelemetryPkt data telemetry path to be updated
 */

void AIMU_LSM6DS33_V2_ProcessDataPacket(int i2cbus, aimu_lsm6ds33_v2_hk_tlm_t* AIMU_LSM6DS33_V2_HkTelemetryPkt, 
    aimu_lsm6ds33_v2_data_tlm_t* AIMU_LSM6DS33_V2_DataTelemetryPkt)
{
	
    int file = I2C_open(i2cbus);

    // stores status from register in data struct status
    I2C_read(file, AIMU_LSM6DS33_V2_SAD_READ, STATUS_REG, AIMU_LSM6DS33_V2_DATA.status);

    // checking to see if there is any data that needs reading
    if (AIMU_LSM6DS33_V2_DATA.status[0] == 0) {
        return;
    }

    if(!I2C_multi_read(file, AIMU_LSM6DS33_V2_SAD_READ, OUT_TEMP_L, 14, AIMU_LSM6DS33_V2_DATA.buffer))
	{
		CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
    	AIMU_LSM6DS33_V2_HkTelemetryPkt->aimu_lsm6ds33_v2_device_error_count++;

		return;
	}

    // temperature output data register
    uint8_t tempL, tempH;
    tempL = AIMU_LSM6DS33_V2_DATA.buffer[0];
    tempH = AIMU_LSM6DS33_V2_DATA.buffer[1];

    uint16_t temp;
    temp = (tempH << 8) | tempL;

    // gyroscope output register
    uint8_t gxL, gxH, gyL, gyH, gzL, gzH;
    gxL = AIMU_LSM6DS33_V2_DATA.buffer[2];
    gxH = AIMU_LSM6DS33_V2_DATA.buffer[3];
    gyL = AIMU_LSM6DS33_V2_DATA.buffer[4];
    gyH = AIMU_LSM6DS33_V2_DATA.buffer[5];
    gzL = AIMU_LSM6DS33_V2_DATA.buffer[6];
    gzH = AIMU_LSM6DS33_V2_DATA.buffer[7];

    uint16_t gx, gy, gz;
    gx = (gxH << 8) | gxL;
    gy = (gyH << 8) | gyL;
    gz = (gzH << 8) | gzL;

    // accelerometer output register
    uint8_t axL, axH, ayL, ayH, azL, azH;
    axL = AIMU_LSM6DS33_V2_DATA.buffer[8];
    axH = AIMU_LSM6DS33_V2_DATA.buffer[9];
    ayL = AIMU_LSM6DS33_V2_DATA.buffer[10];
    ayH = AIMU_LSM6DS33_V2_DATA.buffer[11];
    azL = AIMU_LSM6DS33_V2_DATA.buffer[12];
    azH = AIMU_LSM6DS33_V2_DATA.buffer[13];

    uint16_t ax, ay, az;
    ax = (axH << 8) | axL;
    ay = (ayH << 8) | ayL;
    az = (azH << 8) | azL;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_V2_DataTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_V2_DataTelemetryPkt);

	// Close the I2C Buffer
	I2C_close(file);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_V2_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the SAMPLE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_V2_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(AIMU_LSM6DS33_V2MsgPtr);

    switch (MsgId)
    {
        case AIMU_LSM6DS33_V2_CMD_MID:
            AIMU_LSM6DS33_V2_ProcessGroundCommand();
            break;

        case AIMU_LSM6DS33_V2_SEND_HK_MID:
            AIMU_LSM6DS33_V2_ReportHousekeeping();
            break;

        case AIMU_LSM6DS33_V2_SEND_DATA_MID:
            AIMU_LSM6DS33_V2_ProcessData(bus, &AIMU_LSM6DS33_V2_HkTelemetryPkt, &AIMU_LSM6DS33_V2_DataTelemetryPkt);
            break;

        default:
            AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_error_count++;
            CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"AIMU_LSM6DS33_V2: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End AIMU_LSM6DS33_V2_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LSM6DS33_V2_ProcessGroundCommand() -- SAMPLE ground commands                    */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void AIMU_LSM6DS33_V2_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(AIMU_LSM6DS33_V2MsgPtr);

    /* Process "known" SAMPLE app ground commands */
    switch (CommandCode)
    {
        case AIMU_LSM6DS33_V2_NOOP_CC:
            AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_count++;
            CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"AIMU_LSM6DS33_V2: NOOP command");
            break;

        case AIMU_LSM6DS33_V2_RESET_COUNTERS_CC:
            AIMU_LSM6DS33_V2_ResetCounters();
            break;

        case AIMU_LSM6DS33_V2_INIT:
            AIMU_LSM6DS33_V2_DeviceInit(bus, &AIMU_LSM6DS33_V2_HkTelemetryPkt);
            break;
        
        case AIMU_LSM6DS33_V2_PROCESS:
            AIMU_LSM6DS33_V2_ProcessData(bus, &AIMU_LSM6DS33_V2_HkTelemetryPkt, &AIMU_LSM6DS33_V2_DataTelemetryPkt);
            break;
        

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of AIMU_LSM6DS33_V2_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_V2_ReportHousekeeping                                              */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_V2_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_V2_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LSM6DS33_V2_HkTelemetryPkt);
    return;

} /* End of AIMU_LSM6DS33_V2_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  AIMU_LSM6DS33_V2_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_V2_ResetCounters(void)
{
    /* Status of commands processed by the AIMU_LSM6DS33_V2 */
    AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_count       = 0;
    AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_error_count = 0;

    CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LSM6DS33_V2: RESET command");
    return;

} /* End of AIMU_LSM6DS33_V2_ResetCounters() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* AIMU_LSM6DS33_V2_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool AIMU_LSM6DS33_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_error_count++;
    }

    return(result);

} /* End of AIMU_LSM6DS33_V2_VerifyCmdLength() */
