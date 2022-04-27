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
** File: sample_app.c
**
** Purpose:
**   This file contains the source code for the Sample App.
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

/*
** global data
*/

aimu_lsm6ds33_v2_hk_tlm_t    AIMU_LSM6DS33_V2_HkTelemetryPkt;
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
/* SAMPLE_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LSM6DS33_V2_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LSM6DS33_V2_PERF_ID);

    AIMU_LSM6DS33_V2_AppInit();

    /*
    ** SAMPLE Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LSM6DS33_V2_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LSM6DS33_V2MsgPtr, AIMU_LSM6DS33_V2_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(AIMU_LSM6DS33_V2_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            AIMU_LSM6DS33_V2_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of SAMPLE_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* SAMPLE_AppInit() --  initialization                                       */
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

    AIMU_LSM6DS33_V2_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LSM6DS33_V2_HkTelemetryPkt,
                   AIMU_LSM6DS33_V2_HK_TLM_MID,
                   AIMU_LSM6DS33_V2_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (AIMU_LSM6DS33_V2_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "AIMU_LSM6DS33_V2 App Initialized. Version %d.%d.%d.%d",
                AIMU_LSM6DS33_V2_MAJOR_VERSION,
                AIMU_LSM6DS33_V2_MINOR_VERSION, 
                AIMU_LSM6DS33_V2_REVISION, 
                AIMU_LSM6DS33_V2_MISSION_REV);
				
} /* End of SAMPLE_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_ProcessCommandPacket                                        */
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

        default:
            AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_error_count++;
            CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"AIMU_LSM6DS33_V2: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End SAMPLE_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_ProcessGroundCommand() -- SAMPLE ground commands                    */
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

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of SAMPLE_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_ReportHousekeeping                                              */
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

} /* End of SAMPLE_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LSM6DS33_V2_ResetCounters(void)
{
    /* Status of commands processed by the SAMPLE App */
    AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_count       = 0;
    AIMU_LSM6DS33_V2_HkTelemetryPkt.aimu_lsm6ds33_v2_command_error_count = 0;

    CFE_EVS_SendEvent(AIMU_LSM6DS33_V2_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LSM6DS33_V2: RESET command");
    return;

} /* End of SAMPLE_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_VerifyCmdLength() -- Verify command packet length                   */
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

} /* End of SAMPLE_VerifyCmdLength() */

