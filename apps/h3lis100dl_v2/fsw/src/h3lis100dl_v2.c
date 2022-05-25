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

#include "h3lis100dl_v2.h"
#include "../mission_inc/h3lis100dl_v2_perfids.h"
#include "../platform_inc/h3lis100dl_v2_msgids.h"
#include "h3lis100dl_v2_msg.h"
#include "h3lis100dl_v2_events.h"
#include "h3lis100dl_v2_version.h"

/*
** global data
*/

h3lis100dl_v2_hk_tlm_t    H3LIS100DL_V2_HkTelemetryPkt;
CFE_SB_PipeId_t    H3LIS100DL_V2_CommandPipe;
CFE_SB_MsgPtr_t    H3LIS100DL_V2MsgPtr;

static CFE_EVS_BinFilter_t  H3LIS100DL_V2_EventFilters[] =
       {  /* Event ID    mask */
          {H3LIS100DL_V2_STARTUP_INF_EID,       0x0000},
          {H3LIS100DL_V2_COMMAND_ERR_EID,       0x0000},
          {H3LIS100DL_V2_COMMANDNOP_INF_EID,    0x0000},
          {H3LIS100DL_V2_COMMANDRST_INF_EID,    0x0000},
       };

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* SAMPLE_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void H3LIS100DL_V2_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_ES_PerfLogEntry(H3LIS100DL_V2_PERF_ID);

    H3LIS100DL_V2_AppInit();

    /*
    ** SAMPLE Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        CFE_ES_PerfLogExit(H3LIS100DL_V2_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&H3LIS100DL_V2MsgPtr, H3LIS100DL_V2_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(H3LIS100DL_V2_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            H3LIS100DL_V2_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of SAMPLE_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* SAMPLE_AppInit() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void H3LIS100DL_V2_AppInit(void)
{
    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(H3LIS100DL_V2_EventFilters,
                     sizeof(H3LIS100DL_V2_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_EventFilter_BINARY);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&H3LIS100DL_V2_CommandPipe, H3LIS100DL_V2_PIPE_DEPTH,"SAMPLE_CMD_PIPE");
    CFE_SB_Subscribe(H3LIS100DL_V2_CMD_MID, H3LIS100DL_V2_CommandPipe);
    CFE_SB_Subscribe(H3LIS100DL_V2_SEND_HK_MID, H3LIS100DL_V2_CommandPipe);

    H3LIS100DL_V2_ResetCounters();

    CFE_SB_InitMsg(&H3LIS100DL_V2_HkTelemetryPkt,
                   H3LIS100DL_V2_HK_TLM_MID,
                   H3LIS100DL_V2_HK_TLM_LNGTH, true);

    CFE_EVS_SendEvent (H3LIS100DL_V2_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
               "H3LIS100DL_V2 Initialized. Version %d.%d.%d.%d",
                H3LIS100DL_V2_MAJOR_VERSION,
                H3LIS100DL_V2_MINOR_VERSION, 
                H3LIS100DL_V2_REVISION, 
                H3LIS100DL_V2_MISSION_REV);
				
} /* End of SAMPLE_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the SAMPLE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void H3LIS100DL_V2_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(H3LIS100DL_V2MsgPtr);

    switch (MsgId)
    {
        case H3LIS100DL_V2_CMD_MID:
            H3LIS100DL_V2_ProcessGroundCommand();
            break;

        case H3LIS100DL_V2_SEND_HK_MID:
            H3LIS100DL_V2_ReportHousekeeping();
            break;

        default:
            H3LIS100DL_V2_HkTelemetryPkt.h3lis100dl_v2_command_error_count++;
            CFE_EVS_SendEvent(H3LIS100DL_V2_COMMAND_ERR_EID,CFE_EVS_EventType_ERROR,
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

void H3LIS100DL_V2_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(H3LIS100DL_V2MsgPtr);

    /* Process "known" SAMPLE app ground commands */
    switch (CommandCode)
    {
        case H3LIS100DL_V2_NOOP_CC:
            H3LIS100DL_V2_HkTelemetryPkt.h3lis100dl_v2_command_count++;
            CFE_EVS_SendEvent(H3LIS100DL_V2_COMMANDNOP_INF_EID,
                        CFE_EVS_EventType_INFORMATION,
			"H3LIS100DL_V2: NOOP command");
            break;

        case H3LIS100DL_V2_RESET_COUNTERS_CC:
            H3LIS100DL_V2_ResetCounters();
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
void H3LIS100DL_V2_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &H3LIS100DL_V2_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &H3LIS100DL_V2_HkTelemetryPkt);
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
void H3LIS100DL_V2_ResetCounters(void)
{
    /* Status of commands processed by the SAMPLE App */
    H3LIS100DL_V2_HkTelemetryPkt.h3lis100dl_v2_command_count       = 0;
    H3LIS100DL_V2_HkTelemetryPkt.h3lis100dl_v2_command_error_count = 0;

    CFE_EVS_SendEvent(H3LIS100DL_V2_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
		"H3LIS100DL_V2: RESET command");
    return;

} /* End of SAMPLE_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool H3LIS100DL_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
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

        CFE_EVS_SendEvent(H3LIS100DL_V2_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = false;
        H3LIS100DL_V2_HkTelemetryPkt.h3lis100dl_v2_command_error_count++;
    }

    return(result);

} /* End of SAMPLE_VerifyCmdLength() */

