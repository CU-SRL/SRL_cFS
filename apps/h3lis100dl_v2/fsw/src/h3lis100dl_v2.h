/**
 * @file h3lis100dl_v2.h
 * @brief H3LIS100DL_V2 App Header File
 * @version 0.1
 * @date 2022-05-31
 */

#ifndef _h3lis100dl_v2_h_
#define _h3lis100dl_v2_h_

/* Required header files. */
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>

/***********************************************************************/

#define H3LIS100DL_V2_NUM_TABLES        2       /* Number of tables used by the app */

#define H3LIS100DL_V2_PIPE_DEPTH        32      /* Depth of the command cipe for the app */
#define H3LIS100DL_V2_LIMIT_HK          2       /* Limit of housekeeping requests on pipe for the app */
#define H3LIS100DL_V2_LIMIT_CMD         4       /* Limit of commands on pipe for application */

#define H3LIS100DL_V2_COMMAND_TIMEOUT_LIMIT     500     /* Command packet timeout limit in ms */

/* Filenames of default data images for tables */
#define H3LIS100DL_V2_FIRST_TBL_DEFAULT_FILE    "RAM:/FirstTblDef.dat"      /* First Table */
#define H3LIS100DL_V2_SECOND_TBL_DEFAULT_FILE   "RAM:/SecondTblDef.dat"     /* Second Table */

/* Define specific app error code numbers for validating table data */
#define H3LIS100DL_V2_TBL_1_ELEMENT_OUT_OF_RANGE_ERR_CODE   1
#define H3LIS100DL_V2_TBL_2_ELEMENT_OUT_OF_RANGE_ERR_CODE   -1

#define H3LIS100DL_V2_TBL_ELEMENT_1_MAX                     10
#define H3LIS100DL_V2_TBL_ELEMENT_3_MAX                     20

/********************/
/* Type Definitions */
/********************/

/* Definition of Table Data Structures */
typedef struct
{
    uint32      TblElement1;
    uint16      TblElement2;
    uint8       TblElement3;
} H3LIS100DL_V2_MyFirstTable_t;

typedef struct
{
    int32       TblElement1;
    int16       TblElement2;
    int8        TblElement3;
} H3LIS100DL_V2_MySecondTable_t;

/* Type Definition Critical Data Store */
typedef struct
{
    uint32      DataPtOne;
    uint32      DataPtTwo;
    uint32      DataPtThree;
    uint32      DataPtFour;
    uint32      DataPtFive;
} H3LIS100DL_V2_CDSDataType_t;

/* No-Arguments Command */
typedef struct
{
    uint8       CmdHeader[CFE_SB_CMD_HDR_SIZE];
} H3LIS100DL_V2_NoArgsCmd_t;

/* Housekeeping Packet */
typedef struct
{
    uint8       TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /* Command interface counter */
    uint8       CmdCounter;
    uint8       ErrCounter;
} H3LIS100DL_V2_HKPacket_t;

/* H3LIS100DL_V2 App Global Data */
typedef struct
{
    /* Command interface counters */
    uint8                       CmdCounter;
    uint8                       ErrCounter;

    /* Housekeeping telemetry packet */
    H3LIS100DL_V2_HKPacket_t    HKPacket;

    /* Operational data (not reported in housekeeping) */
    CFE_SB_MsgPtr_t             MsgPtr;
    CFE_SB_PipeId_t             CmdPipe;

    /* Run Status variable used in main processing loop */
    uint32                      RunStatus;

    /* Operational data (not reported in housekeeping) */
    H3LIS100DL_V2_CDSDataType_t WorkingCriticalData; /* Define a copy of the critical data used during Application execution */

    CFE_ES_CDSHandle_t          CDSHandle;  /* Handle to CDS Memory block */

    /* Initialization data (not reported in housekeeping) */
    char                        PipeName[16];
    uint16                      PipeDepth;

    uint8                       LimitHK;
    uint8                       LimitCmd;

    CFE_EVS_BinFilter_t         EventFilters[H3LIS100DL_V2_EVT_COUNT]
    CFE_TBL_Handle_t            TblHandles[H3LIS100DL_V2_NUM_TABLES];

} H3LIS100DL_V2_AppData_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (H3LIS100DL_V2_AppMain), these
**       functions are not called from any other source module.
*/
void H3LIS100DL_V2_AppMain(void);
void H3LIS100DL_V2_AppInit(void);
void H3LIS100DL_V2_AppPipe(CFE_SB_MsgPtr_t msg);

void H3LIS100DL_V2_HousekeepingCmd(CFE_SB_MsgPtr_t msg);

void H3LIS100DL_V2_NoopCmd(CFE_SB_MsgPtr_t msg);
void H3LIS100DL_V2_ResetCmd(CFE_SB_MsgPtr_t msg);
void H3LIS100DL_V2_RoutineProcessingCmd(CFE_SB_MsgPtr_t msg);

boolean H3LIS100DL_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

int32 H3LIS100DL_V2_FirstTblValidationFunc(void *TblData);
int32 H3LIS100DL_V2_SecondTblValidationFunc(void *TblData);

#endif /* _sample_app_h_ */
