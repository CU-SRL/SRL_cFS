#include <unistd.h>

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
** File: ms5611.c
**
** Purpose:
**   This file contains the source code for the MS5611 App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "ms5611.h"
#include "ms5611_perfids.h"
#include "ms5611_msgids.h"
#include "ms5611_msg.h"
#include "ms5611_events.h"
#include "ms5611_version.h"

#include "i2c_lib.h"

/*
** global data
*/

ms5611_hk_tlm_t         MS5611_HkTelemetryPkt;
ms5611_data_tlm_t       MS5611_DataTelemetryPkt;
CFE_SB_PipeId_t              MS5611_CommandPipe;
CFE_SB_MsgPtr_t              MS5611MsgPtr;

static CFE_EVS_BinFilter_t  MS5611_EventFilters[] =
       {  /* Event ID    mask */
          {MS5611_STARTUP_INF_EID,       0x0000},
          {MS5611_COMMAND_ERR_EID,       0x0000},
          {MS5611_COMMANDNOP_INF_EID,    0x0000},
          {MS5611_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* MS5611_AppMain() -- Application entry point and main process loop       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void MS5611_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(MS5611_PERF_ID);
    MS5611_AppInit();

    //After Initialization
    MS5611_HkTelemetryPkt.AppStatus = RunStatus;
    /*
    ** MS5611 Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(MS5611_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&MS5611MsgPtr, MS5611_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(MS5611_PERF_ID);

        // Process Data
        // PROCESS_MS5611(1, &MS5611_HkTelemetryPkt, &MS5611_DataTelemetryPkt);

        if (status == CFE_SUCCESS)
        {
            MS5611_ProcessCommandPacket();
        }

    }
    //After While Loop
    MS5611_HkTelemetryPkt.AppStatus = RunStatus;

    CFE_ES_ExitApp(RunStatus);

} /* End of MS5611_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/*MS5611_AppInit() --  initialization                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void MS5611_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(MS5611_EventFilters,
                     sizeof(MS5611_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&MS5611_CommandPipe, MS5611_PIPE_DEPTH,"MS5611_CMD_PIPE");
    CFE_SB_Subscribe(MS5611_CMD_MID, MS5611_CommandPipe);
    CFE_SB_Subscribe(MS5611_SEND_HK_MID, MS5611_CommandPipe);

    MS5611_ResetCounters();

    CFE_SB_InitMsg(&MS5611_HkTelemetryPkt,
                   MS5611_HK_TLM_MID,
                   MS5611_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (MS5611_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "MS5611 App Initialized. Version %d.%d.%d.%d\n",
                MS5611_MAJOR_VERSION,
                MS5611_MINOR_VERSION, 
                MS5611_REVISION, 
                MS5611_MISSION_REV);

    /**********************/
    /*    DEVICE INIT     */
    /**********************/
    
    /* WHEN FLIGHT READY, PUT THE DEVICE INITIALIZATIONS IN HERE */
				
} /* End of MS5611_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MS5611_ProcessCommandPacket                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the MS5611  */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MS5611_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(MS5611MsgPtr);

    switch (MsgId)
    {
        case MS5611_CMD_MID:
            MS5611_ProcessGroundCommand();
            break;

        case MS5611_SEND_HK_MID:
            MS5611_ReportHousekeeping();
            break;

        default:
            MS5611_HkTelemetryPkt.ms5611_command_error_count++;
            CFE_EVS_SendEvent(MS5611_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"MS5611: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End MS5611_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*MS5611_ProcessGroundCommand() -- MS5611 ground commands              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void MS5611_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(MS5611MsgPtr);

    /* Process "known" MS5611 app ground commands */
    switch (CommandCode)
    {
        case MS5611_NOOP_CC:
            MS5611_HkTelemetryPkt.ms5611_command_count++;
            CFE_EVS_SendEvent(MS5611_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"MS5611: NOOP command");
            break;

        case MS5611_RESET_COUNTERS_CC:
            MS5611_ResetCounters();
            break;

        // DEVICE SPECIFIC COMMANDS
        case MS5611_INIT:
            INIT_MS5611(1, &MS5611_HkTelemetryPkt);
            break;
        
        case MS5611_PROCESS:
            PROCESS_MS5611(1, &MS5611_HkTelemetryPkt, &MS5611_DataTelemetryPkt);
            break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of MS5611_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MS5611_ReportHousekeeping                                       */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MS5611_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &MS5611_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MS5611_HkTelemetryPkt);
    return;

} /* End of MS5611_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  MS5611_ResetCounters                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MS5611_ResetCounters(void)
{
    /* Status of commands processed by the MS5611 App */
    MS5611_HkTelemetryPkt.ms5611_command_count       = 0;
    MS5611_HkTelemetryPkt.ms5611_command_error_count = 0;

    CFE_EVS_SendEvent(MS5611_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"MS5611: RESET command");
    return;

} /* End of MS5611_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MS5611_VerifyCmdLength() -- Verify command packet length                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool MS5611_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(MS5611_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        MS5611_HkTelemetryPkt.ms5611_command_error_count++;
    }

    return(result);

} /* End of MS5611_VerifyCmdLength() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_MS5611() -- This function initializes the MS5611 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_MS5611(int I2CBus, ms5611_hk_tlm_t* MS5611_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, MS5611_I2C_ADDR);
	
    //read PROM for everything needed in calculations
    for(int i = 0; i < 6; i++){
        if(!I2C_read(file, MS5611_READ_PROM + i*2, 1, MS5611.prom[i]))
		{
			CFE_EVS_SendEvent(MS5611_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read PROM with offset %d...", i);
            MS5611_HkTelemetryPkt->ms5611_device_error_count++;

			return;
		}
        CFE_EVS_SendEvent(MS5611_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "PROM(%d): %d", i, MS5611.prom[i]);
        
    }

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_MS5611() -- This function process the MS5611 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_MS5611(int i2cbus, ms5611_hk_tlm_t* MS5611_HkTelemetryPkt, ms5611_data_tlm_t* MS5611_DataTelemetryPkt)
{
    //define variables needed for data calculations
    double sealevelP = 101325;

	// Open the I2C Device
	int file = I2C_open(i2cbus, MS5611_I2C_ADDR); 

    /* Process the Data Buffer */
    uint32_t D1, D2;

    //Pressure
    D1 = readRawData(file, MS5611_CONV_D1);
    //Temperature
    D2 = readRawData(file, MS5611_CONV_D2);

    int32_t dT = D2 - (uint32_t)MS5611.prom[4] * 256;

    //below calculations are taken from Arduino Code for sensor and must be checked
    //need to decide of ADC is reading for temperature or pressure
    int64_t OFF = (int64_t)MS5611.prom[1] * 65536 + (int64_t)MS5611.prom[3] * dT / 128;
    int64_t SENS = (int64_t)MS5611.prom[0] * 32768 + (int64_t)MS5611.prom[2] * dT / 256;

    //calculate pressure
    float pressure = (D1 * SENS / 2097152 - OFF) / 32768;		

    // Temperature (in C)
    float temp = (2000 + ((int64_t) dT * MS5611.prom[5]) / 8388608) / 100.0;

    //calculate altitude
    float altitude = (44330.0f * (1.0f - pow((double)pressure / sealevelP, 0.1902949f)));

    // Store into packet
    MS5611_DataTelemetryPkt->MS5611_PRESSURE = pressure;
    MS5611_DataTelemetryPkt->MS5611_TEMPERATURE = temp;
    MS5611_DataTelemetryPkt->MS5611_ALTITUDE = altitude;

    // Print Processed Values if the debug flag is enabled for this app
    CFE_EVS_SendEvent(MS5611_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Pressure: %F Temperature: %F , Altitude: %F", pressure, temp, altitude);


	// Close the I2C Buffer
	I2C_close(file);
}

uint32_t readRawData(int file, uint8_t reg){
    //conversion D1 to get pressure data, must have some sort of delay after
        if(!I2C_write(file, MS5611_I2C_ADDR, (reg + 0x08)))
        {
            CFE_EVS_SendEvent(MS5611_FAILED_TO_ACTIVATE_EID, CFE_EVS_EventType_ERROR,
            "Failed to configure conversion ... ");
            MS5611_HkTelemetryPkt.ms5611_device_error_count++;
            return;
        }
        
        delay(10);


        // Read the Data Buffer
		if(!I2C_read(file, MS5611_ADC_READ, 3, MS5611.buffer))
		{
			CFE_EVS_SendEvent(MS5611_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
            MS5611_HkTelemetryPkt.ms5611_device_error_count++;

			return;
		}


		// Pressure
        uint8_t vx, vh, vl;
        vx = MS5611.buffer[0];
        vh = MS5611.buffer[1];
        vl = MS5611.buffer[2];
        uint32_t D = ((int32_t)vx << 16) | ((int32_t)vh << 8) | vl;

        return D;
}