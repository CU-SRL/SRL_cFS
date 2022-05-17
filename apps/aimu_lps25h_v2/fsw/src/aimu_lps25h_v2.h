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
** File: aimu_lps25h_v2.h
**
** Purpose:
**   This file is main hdr file for the AIMU_LPS25H_V2 application.
**
**
*******************************************************************************/

#ifndef _aimu_lps25h_v2_h_
#define _aimu_lps25h_v2_h_

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>

/***********************************************************************/

#define AIMU_LPS25H_V2_PIPE_DEPTH                     32

/************************************************************************
** Type Definitions
*************************************************************************/

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (Aimu_lps25h_v2Main), these
**       functions are not called from any other source module.
*/
void AIMU_LPS25H_V2Main(void);
void AIMU_LPS25H_V2Init(void);
void AIMU_LPS25H_V2_ProcessCommandPacket(void);
void AIMU_LPS25H_V2_ProcessGroundCommand(void);
void AIMU_LPS25H_V2_ReportHousekeeping(void);
void AIMU_LPS25H_V2_ResetCounters(void);

bool AIMU_LPS25H_V2_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

#endif /* _aimu_lps25h_v2_h_ */
