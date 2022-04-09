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

#include "aimu_lis3mdl.h"
#include "aimu_lis3mdl_perfids.h"
#include "aimu_lis3mdl_msgids.h"
#include "aimu_lis3mdl_msg.h"
#include "aimu_lis3mdl_events.h"
#include "aimu_lis3mdl_version.h"

/*
** global data
*/

aimu_lis3mdl_hk_tlm_t    AIMU_LIS3MDL_HkTelemetryPkt;
CFE_SB_PipeId_t    AIMU_LIS3MDL_CommandPipe;
CFE_SB_MsgPtr_t    AIMU_LIS3MDMsgPtr;

static CFE_EVS_BinFilter_t  AIMU_LIS3MDL_EventFilters[] =
       {  /* Event ID    mask */
          {aimu_lis3mdl_STARTUP_INF_EID,       0x0000},
          {aimu_lis3mdl_COMMAND_ERR_EID,       0x0000},
          {aimu_lis3mdl_COMMANDNOP_INF_EID,    0x0000},
          {aimu_lis3mdl_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* SAMPLE_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void AIMU_LIS3MDL_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(AIMU_LIS3MDL_PERF_ID);

    AIMU_LIS3MDL_AppInit();

    /*
    ** SAMPLE Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(AIMU_LIS3MDL_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&AIMU_LIS3MDMsgPtr, AIMU_LIS3MDL_CommandPipe, INT32_MAX); 
        
        CFE_ES_PerfLogEntry(AIMU_LIS3MDL_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            AIMU_LIS3MDL_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of SAMPLE_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* SAMPLE_AppInit() --  initialization                                       */
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
    CFE_SB_CreatePipe(&AIMU_LIS3MDL_CommandPipe, AIMU_LIS3MDL_PIPE_DEPTH,"LIS3MDL_CMD_PIPE");
    CFE_SB_Subscribe(AIMU_LIS3MDL_CMD_MID, AIMU_LIS3MDL_CommandPipe);
    CFE_SB_Subscribe(AIMU_LIS3MDL_SEND_HK_MID, AIMU_LIS3MDL_CommandPipe);

    AIMU_LIS3MDL_ResetCounters();

    CFE_SB_InitMsg(&AIMU_LIS3MDL_HkTelemetryPkt,
                   AIMU_LIS3MDL_HK_TLM_MID,
                   AIMU_LIS3MDL_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (aimu_lis3mdl_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "aimu_lis3mdl App Initialized. Version %d.%d.%d.%d",
                aimu_lis3mdl_MAJOR_VERSION,
                aimu_lis3mdl_MINOR_VERSION, 
                aimu_lis3mdl_REVISION, 
                aimu_lis3mdl_MISSION_REV);
				
} /* End of SAMPLE_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the SAMPLE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void AIMU_LIS3MDL_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(AIMU_LIS3MDMsgPtr);

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
            CFE_EVS_SendEvent(aimu_lis3mdl_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
			"SAMPLE: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End SAMPLE_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_ProcessGroundCommand() -- SAMPLE ground commands                    */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void AIMU_LIS3MDL_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(AIMU_LIS3MDMsgPtr);

    /* Process "known" SAMPLE app ground commands */
    switch (CommandCode)
    {
        case AIMU_LIS3MDL_NOOP_CC:
            AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_count++;
            CFE_EVS_SendEvent(aimu_lis3mdl_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"SAMPLE: NOOP command");
            break;

        case AIMU_LIS3MDL_RESET_COUNTERS_CC:
            AIMU_LIS3MDL_ResetCounters();
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
void AIMU_LIS3MDL_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &AIMU_LIS3MDL_HkTelemetryPkt);
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
void AIMU_LIS3MDL_ResetCounters(void)
{
    /* Status of commands processed by the SAMPLE App */
    AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_count       = 0;
    AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_error_count = 0;

    CFE_EVS_SendEvent(aimu_lis3mdl_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"AIMU_LIS3MDL: RESET command");
    return;

} /* End of SAMPLE_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_VerifyCmdLength() -- Verify command packet length                   */
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

        CFE_EVS_SendEvent(aimu_lis3mdl_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        AIMU_LIS3MDL_HkTelemetryPkt.aimu_lis3mdl_command_error_count++;
    }

    return(result);

} /* End of SAMPLE_VerifyCmdLength() */

