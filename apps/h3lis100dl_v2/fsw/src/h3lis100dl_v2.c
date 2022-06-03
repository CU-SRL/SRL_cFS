/**
 * @file h3lis100dl_v2.c
 * @brief Source code for the H3LIS100DL_V2 App.
 * @version 0.1
 * @date 2022-05-31
 * 
 */

/* Include Files */
#include "h3lis100dl_v2.h"
#include "../mission_inc/h3lis100dl_v2_perfids.h"
#include "../platform_inc/h3lis100dl_v2_msgids.h"
#include "h3lis100dl_v2_msg.h"
#include "h3lis100dl_v2_events.h"
#include "h3lis100dl_v2_version.h"
#include "h3lis100dl_v2_registers.h"

/* Global Data */
H3LIS100DL_V2_AppData_t     H3LIS100DL_V2_AppData;

/**
 * @brief Application entry point and main process loop
 * 
 */
void H3LIS100DL_V2_AppMain(void)
{
    int32 Status;

    /* Register application with executive services */
    CFE_ES_RegisterApp()

    /* Create first performance log entry */
    CFE_ES_PerfLogEntry(H3LIS100DL_V2_PERF_ID);

    /* Initialize the app */
    Status = H3LIS100DL_V2_AppInit()

    if (Status != CFE_SUCCESS)
    {
        H3LIS100DL_V2_AppData.RunStatus = CFE_ES_APP_ERROR;
    }

    /* App Runloop */
    while (CFE_ES_RunLoop(&H3LIS100DL_V2_AppData.RunStatus) == true)
    {
        /* Performance log exit stamp */
        CFE_ES_PerfLogExit(H3LIS100DL_V2_PERF_ID);

        /* Pend on receipt of command packet */
        Status = CFE_SB_RcvMsg(&H3LIS100DL_V2_AppData.MsgPtr, H3LIS100DL_V2_AppData.CommandPipe, H3LIS100DL_V2_COMMAND_TIMEOUT_LIMIT);
        
        /* Performance log entry stamp */
        CFE_ES_PerfLogEntry(H3LIS100DL_V2_PERF_ID);

        /* Check the return status from the software bus */
        if (Status == CFE_SUCCESS)
        {
            /* Process software bus message */
            H3LIS100DL_V2_AppPipe(H3LIS100DL_V2_AppData.MsgPtr);

            /* Update critical data store */
            CFE_ES_CopyToCDS(H3LIS100DL_V2_AppData.CDSHandle, &H3LIS100DL_V2_AppData.WorkingCriticalData);
        }
        else
        {
            CFE_EVS_SendEvent(&H3LIS100DL_V2_PIPE_ERR_EID, CFE_EVS_Error, "H3LIS100DL: Software Bus Pipe Read Error");
            H3LIS100DL_V2_AppData.RunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Performance log exit stamp */
    CFE_ES_PerfLogExit(H3LIS100DL_V2_PERF_ID);

    /* Exit app */
    CFE_ES_ExitApp(H3LIS100DL_V2_AppData.RunStatus);

} /* End of H3LIS100DL_V2_AppMain() */

/**
 * @brief H3LIS100DL app initialization      
 * 
 */
void H3LIS100DL_V2_AppInit(void)
{
    int32 Status;
    int32 ResetType;
    uint32 ResetSubType;

    ResetType = CFE_ES_GetResetType(&ResetSubType);

    /* For a PowerOn reset, initialize the critical variables, and for processor reset, restored later from critical data store */
    if (ResetType == CFE_ES_POWER_ON)
    {
        H3LIS100DL_V2_AppData.RunStatus = CFE_ES_APP_ERROR;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtOne = 1;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtTwo = 2;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtThree = 3;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFour = 4;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFive = 5;
    }
    
    H3LIS100DL_V2_AppData.RunStatus = CFE_ES_APP_RUN;

    /* Initialize app command execution counters */
    H3LIS100DL_V2_AppData.CmdCounter = 0;
    H3LIS100DL_V2_AppData.ErrCounter = 0;
    
    /* Initialize app configuration data */
    strcpy(H3LIS100DL_V2_AppData.PipeName, "H3LIS100DL_V2_CMD_PIPE")

    H3LIS100DL_V2_AppData.PipeDepth = H3LIS100DL_V2_PIPE_DEPTH;
    H3LIS100DL_V2_AppData.LimitHK = H3LIS100DL_V2_LIMIT_HK;
    H3LIS100DL_V2_AppData.LimitCmd = H3LIS100DL_V2_LIMIT_CMD;

    /* Initialize event filter table */
    H3LIS100DL_V2_AppData.EventFilters[0].EventID = H3LIS100DL_V2_PROCESS_INF_EID;
    H3LIS100DL_V2_AppData.EventFilters[0].Mask = CFE_EVS_EVERY_FOURTH_TIME;
    H3LIS100DL_V2_AppData.EventFilters[1].EventID = H3LIS100DL_V2_RESET_INF_EID;
    H3LIS100DL_V2_AppData.EventFilters[1].Mask = CFE_EVS_NO_FILTER;
    H3LIS100DL_V2_AppData.EventFilters[2].EventID = H3LIS100DL_V2_CC1_INF_EID;
    H3LIS100DL_V2_AppData.EventFilters[2].Mask = CFE_EVS_EVERY_OTHER_TWO;
    H3LIS100DL_V2_AppData.EventFilters[3].EventID = H3LIS100DL_V2_LEN_ERR_EID;
    H3LIS100DL_V2_AppData.EventFilters[3].Mask = CFE_EVS_FIRST_8_STOP;


    /* Register the events */ 
    Status = CFE_EVS_Register(H3LIS100DL_V2_AppData.EventFilters,
                                4,
                                CFE_EVS_EventFilter_BINARY);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Error Registering Events, RC = 0x%08X\n", Status);
        return Status;
    }

    /* Create the software bus command pipe */
    Status = CFE_SB_CreatePipe(&H3LIS100DL_V2_AppData.CmdPipe,
                                H3LIS100DL_V2_AppData.PipeDepth,
                                H3LIS100DL_V2_AppData.PipeName);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Error Creating Software Bus Pipe, RC = 0x%08X\n", Status);
        return Status;
    }

    /* Subscribe to housekeeping request commands */
    Status = CFE_SB_Subscribe(H3LIS100DL_V2_SEND_HK_MID, H3LIS100DL_V2_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Error Subscribing to HK Request, RC = 0x%08X\n", Status);
        return Status;
    }

    /* Subscribe to H3LIS100DL_V2 ground command packets */
    Status = CFE_SB_Subscribe(H3LIS100DL_V2_CMD_MID, H3LIS100DL_V2_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Error Subscribing to H3LIS100DL_V2 Command, RC = 0x%08X\n", Status);
        return Status;
    }

    /* Register tables with cFE and load default data */
    Status = CFE_TBL_Register(&H3LIS100DL_V2_AppData.TblHandles[0],
                                "MyFirstTableTODO",
                                sizeof(H3LIS100DL_V2_MyFirstTable_t),
                                CFE_TBL_OPT_DEFAULT,
                                H3LIS100DL_V2_FirstTblValidationFunc);
    if (Status != CFE_SUCCESS){
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Error Registering Table 1, RC = 0x%08X\n", Status);
        return Status;
    }
    else{
        Status = CFE_TBL_Load(H3LIS100DL_V2_AppData.TblHandles[0], CFE_TBL_SRC_FILE, H3LIS100DL_V2_FIRST_TBL_DEFAULT_FILE);
    }

    /* Create and manage the Critical Data Store */
    Status = CFE_ES_RegisterCDS(&H3LIS100DL_V2_AppData.CDSHandle, sizeof(H3LIS100DL_V2_CDSDataType_t), H3LIS100DL_V2_CDS_NAME);
    if (Status == CFE_SUCCESS){
        CFE_ES_CopyToCDS(H3LIS100DL_V2_AppData.CDSHandle, &H3LIS100DL_V2_AppData.WorkingCriticalData);
    }
    else if (Status == CFE_ES_CDS_ALREADY_EXISTS)
    {
        /* Get a copy of current contents to see if existing data is useful */
        Status = CFE_ES_RestoreFromCDS(&H3LIS100DL_V2_AppData.WorkingCriticalData, H3LIS100DL_V2_AppData.CDSHandle);
        if (Status == CFE_SUCCESS)
        {
            // Perform logical verifications, Data verification/validation
            CFE_ES_WriteToSysLog("H3LIS100DL_V2 App CDS data preserved\n");
        }
        else{
            /* Perform baseline initialization */
            H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtOne = 1;
            H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtTwo = 2;
            H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtThree = 3;
            H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFour = 4;
            H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFive = 5;
            CFE_ES_WriteToSysLog("Failed to Restore CDS. Re-Initialized CDS Data.\n");
        }
    }
    else{
        /* Error Creating critical data store */
        CFE_ES_WriteToSysLog("H3LIS100DL_V2 App: Failed to create CDS, RC = 0x%08X\n", Status);
        return Status;
    }

    /* Application startup event message */
    CFE_EVS_SendEvent(H3LIS100DL_V2_INIT_INF_EID, //H3LIS100DL_V2_STARTUP_INF_EI
                        CFE_EVS_INFORMATION, //CFE_EVS_EventType_INFORMATION
                        "H3LIS100DL_V2 App: Application Initialized, Version %d.%d.%d.%d",
                        H3LIS100DL_V2_MAJOR_VERSION,
                        H3LIS100DL_V2_MINOR_VERSION, 
                        H3LIS100DL_V2_REVISION, 
                        H3LIS100DL_V2_MISSION_REV);

    return CFE_SUCCESS;
				
} /* End of H3LIS100DL_V2_AppInit() */

/**
 * @brief Process any packet that is received on the H3LIS100DL_V2 command pipe.        
 * 
 * @param msg Pointer to the software bus message
 */
void H3LIS100DL_V2_AppPipe(CFE_SB_MsgPtr_t msg)
{
    CFE_SB_MsgId_t MessageID;
    uint16 CommandCode;

    MessageID = CFE_SB_GetMsgId(msg);
    switch MessageID
    {
        /* Housekeeping telemetry request */
        case H3LIS100DL_V2_SEND_HK_MID:
            H3LIS100DL_V2_HousekeepingCmd(msg);
            break;
        
        /* H3LIS100DL_V2 ground command */
        case H3LIS100DL_V2_CMD_MID:
            CommandCode = CFE_SB_GetCmdCode(msg);
            switch CommandCode
            {
                case H3LIS100DL_V2_NOOP_CC:
                    H3LIS100DL_V2_NoopCmd(msg);
                    break;
                
                case H3LIS100DL_V2_RESET_CC:
                    H3LIS110DL_V2_ResetCmd(msg);
                    break;

                case H3LIS100DL_V2_PROCESS_CC:
                    H3LIS100DL_V2_RoutineProcessingCmd(msg);
                    break;
                
                default:
                    CFE_EVS_SendEvent(H3LIS100DL_V2_CC1_ERR_EID, CFE_EVS_ERROR,
                                        "Invalid ground command code: ID = 0x%X, CC = %d",
                                        MessageID, CommandCode);
                    break;
            }
            break;

        /* Invalid message ID */
        default:
            CFE_EVS_SendEvent(H3LIS100DL_V2_MID_ERR_EID, CFE_EVS_ERROR,
                                "Invalid command pipe message ID: 0x%X",
                                MessageID);
            break;
    }

    return;

} /* End of H3LIS100DL_V2_AppPipe() */

/**
 * @brief Housekeeping Command
 * 
 * @param msg
 *
 * This function is triggered in response to a task telemetry request from the
 * housekeeping task. This function will gather the app's telemetry, packetize
 * it and send it to the housekeeping task via the software bus.         
 */
void H3LIS100DL_V2_HousekeepingCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(H3LIS100DL_V2_NoArgsCmd_t);
    uint16 i;

    /* Verify command packet length */
    if (H3LIS100DL_V2_VerifyCmdLength(msg, ExpectedLength))
    {
        /* Get command execution counters */
        H3LIS100DL_V2_AppData.HKPacket.CmdCounter = H3LIS100DL_V2_AppData.CmdCounter;
        H3LIS100DL_V2_AppData.HKPacket.ErrCounter = H3LIS100DL_V2_AppData.ErrCounter;

        /* Send housekeeping telemetry packet */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &H3LIS100DL_V2_AppData.HKPacket);
        CFE_SB_SendMsg((CFE_SB_Msg_t *) &H3LIS100DL_V2_AppData.HKPacket);

        /* Manage any pending table loads, validations, etc. */
        for (i = 0; i < H3LIS100DL_V2_NUM_TABLES; i++)
        {
            CFE_TBL_Manage(H3LIS100DL_V2_AppData.TblHandles[i]);
        }

        /* This command does not affect the command execution counter */
    }

    return;

} /* End of H3LIS100DL_V2_HousekeepingCmd() */

/**
 * @brief H3LIS100DL_V2 ground command (NO-OP)
 *
 * @param msg Pointer to the software bus message
 */
void H3LIS100DL_V2_NoopCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(H3LIS100DL_V2_NoArgsCmd_t);

    /* Verify command packet length */
    if (H3LIS100DL_V2_VerifyCmdLength(msg, ExpectedLength))
    {
        H3LIS100DL_V2_AppData.CmdCounter++;

        CFE_EVS_SendEvent(H3LIS100DL_V2_NOOP_INF_EID, CFE_EVS_INFORMATION,
                            "No-op command");
    }

    return;
} /* End of H3LIS100DL_V2_NoopCmd() */

/**
 * @brief H3LIS100DL_V2 ground command (Reset Counters)
 *
 * @param msg Pointer to the software bus message
 */
 void H3LIS100DL_V2_ResetCmd(CFE_SB_MsgPtr_t msg)
 {
    uint16 ExpectedLength = sizeof(H3LIS100DL_V2_NoArgsCmd_t);

    /* Verify command packet length */
    if (H3LIS100DL_V2_VerifyCmdLength(msg, ExpectedLength))
    {
        H3LIS100DL_V2_AppData.CmdCounter = 0;
        H3LIS100DL_V2_AppData.ErrCounter = 0;

        CFE_EVS_SendEvent(H3LIS100DL_V2_RESET_INF_EID, CFE_EVS_INFORMATION,
                            "Reset Counters command");
    }

    return;

 } /* End of H3LIS100DL_V2_ResetCmd() */

 /**
  * @brief H3LIS100DL_V2 ground command (Process command)
  *
  * @param msg Pointer to the software bus message
  */
void H3LIS100DL_V2_ProcessCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(H3LIS100DL_V2_NoArgsCmd_t);
    H3LIS100DL_V2_MyFirstTableTODO_t *MyFirstTblPtr;

    /* Verify command packet length */
    if (H3LIS100DL_V2_VerifyCmdLength(msg, ExpectedLength))
    {
        /* Obtain access to table data adresses */
        CFE_TBL_GetAddress((void *) &MyFirstTblPtr,
                            H3LIS100DL_V2_AppData.TblHandles[0]);
        CFE_TBL_GetAddress((void *) &MySecondTblPtr,
                            H3LIS100DL_V2_AppData.TblHandles[1]);

        /* Process data accessing the tables with pointers */

        /* Release table pointer addresses */
        CFE_TBL_ReleaseAddress(H3LIS100DL_V2_AppData.TblHandles[0]);
        CFE_TBL_ReleaseAddress(H3LIS100DL_V2_AppData.TblHandles[1]);

        /* Update critical variables. These variables will be saved in the critical data store */
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtOne++;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtTwo++;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtThree++;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFour++;
        H3LIS100DL_V2_AppData.WorkingCriticalData.DataPtFive++;

        CFE_EVS_SendEvent(H3LIS100DL_V2_PROCESS_INF_EID, CFE_EVS_INFORMATION,
                            "H3LIS100DL: Routine Processing Command");
    }

    return;

} /* End of H3LIS100DL_V2_ProcessCmd() */

/**
 * @brief Verifies H3LIS100DL_V2 command packet length
 *
 * @param msg Pointer to the software bus message
 * @param ExpectedLength Expected number of bytes (or in bits?) of the command packet
 */
 boolean H3LIS100DL_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
 {
    boolean result = true;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(msg);

    /* Verify the command packet length */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID = CFE_SB_GetMsgId(msg);
        uint16 CommandCode = CFE_SB_GetCmdCode(msg);

        CFE_EVS_SendEvent(H3LIS100DL_V2_LEN_ERR_EID, CFE_EVS_ERROR,
                            "Invalid cmd pkt: ID = 0x%X, CC = %d, Len = %d",
                            MessageID, CommandCode, ActualLength);
        result = false;
        H3LIS100DL_V2_AppData.ErrCounter++;
    }

    return result;

 } /* End of H3LIS100DL_V2_VerifyCmdLength() */

/**
 * @brief Verify contents of First Table buffer contents
 *
 * @param
 */
int32 H3LIS100DL_V2_FirstTblValidationFunc(void *TblData)
{
    int32 ReturnCode = CFE_SUCCESS;
    H3LIS100DL_V2_MyFirstTable_t *TblDataPtr = (H3LIS100DL_V2_MyFirstTable_t *) TblData;

    if (TblDataPtr->TblElement1 > H3LIS100DL_V2_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range; return an appropriate error code */
        ReturnCode = H3LIS100DL_V2_TBL_1_ELEMENT_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/**
 * @brief Verify contents of Second Table buffer contents
 *
 * @param
 */
int32 H3LIS100DL_V2_SecondTblValidationFunc(void *TblData)
{
    int32 ReturnCode = CFE_SUCCESS;
    H3LIS100DL_V2_MySecondTable_t *TblDataPtr = (H3LIS100DL_V2_MySecondTable_t *) TblData;

    if (TblDataPtr->TblElement3 > H3LIS100DL_V2_TBL_ELEMENT_3_MAX)
    {
        /* Third element is out of range; return an appropriate error code */
        ReturnCode = H3LIS100DL_V2_TBL_2_ELEMENT_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}
