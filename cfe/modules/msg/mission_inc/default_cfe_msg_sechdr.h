/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
 * Define cFS standard secondary header
 *  - Avoid direct access for portability, use APIs
 *  - Used to construct message structures
 */

#ifndef _cfe_msg_sechdr_
#define _cfe_msg_sechdr_

/*
 * Include Files
 */

#include "common_types.h"
#include "cfe_mission_cfg.h"

/*
 * Defines
 */

/*
 * Type Definitions
 */

/**********************************************************************
 * Structure definitions for secondary headers
 *
 * These are based on historically supported definitions and not
 * an open source standard.
 */

/**
 * \brief cFS command secondary header
 */
typedef struct
{

    uint8 FunctionCode; /**< \brief Command Function Code */
                        /* bits shift ---------description-------- */
                        /* 0x7F  0    Command function code        */
                        /* 0x80  7    Reserved                     */

    uint8 Checksum; /**< \brief Command checksum  (all bits, 0xFF)      */

} CFE_MSG_CommandSecondaryHeader_t;

/**
 * \brief cFS telemetry secondary header
 */
typedef struct
{

    uint8 Time[6]; /**< \brief Time, big endian: 4 byte seconds, 2 byte subseconds */

} CFE_MSG_TelemetrySecondaryHeader_t;

#endif /* _cfe_msg_sechdr_ */
