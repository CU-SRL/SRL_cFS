/************************************************************************
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
** File: H3LIS100DL_V2_events.h 
**
** Purpose: 
**  Define H3LIS100DL_V2 App Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _h3lis100dl_v2_events_h_
#define _h3lis100dl_v2_events_h_

/* Event IDs */

/* Reserved */
#define H3LIS100DL_V2_RESERVED_EID              0       /* Reserved */

/* Start Up Message: INF - "Informational" */
#define H3LIS100DL_V2_INIT_INF_EID              1       /* Startup/

/* Processed Command: INF - "Informational" */
#define H3LIS100DL_V2_NOOP_INF_EID              2       /* NO-OP */
#define H3LIS100DL_V2_RESET_INF_EID             3       /* Reset Counters */
#define H3LIS100DL_V2_PROCESS_INF_EID           4       /* Process */

/* Invalid Command Packet: ERR - "Error" */
#define H3LIS100DL_V2_MID_ERR_EID               5       /* Invalid Message ID */
#define H3LIS100DL_V2_CC1_ERR_EID               6       /* Invalid Command Code */
#define H3LIS100DL_V2_LEN_ERR_EID               7       /* Unexpected Command Packet Length */
#define H3LIS100DL_V2_PIPE_ERR_EID              8       /* SB Bus Pipe Read Error */


#define H3LIS100DL_V2_EVT_COUNT                 8       /* Total Count of Event Message IDs */

#endif /* _sample_app_events_h_ */

/************************/
/*  End of File Comment */
/************************/
