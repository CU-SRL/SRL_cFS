/*************************************************************************
** File: hs_msgids.h 
**
** NASA Docket No. GSC-18,476-1, and identified as "Core Flight System 
** (cFS) Health and Safety (HS) Application version 2.3.2"
**
** Copyright � 2020 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration.  
** All Rights Reserved. 
** 
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
*
** Purpose: 
**   CFS Health and Safety (HS) Application Message IDs
**
** Notes:
**
** 
*************************************************************************/
#ifndef _hs_msgids_h_
#define _hs_msgids_h_

/*************************************************************************
** Macro Definitions
*************************************************************************/
/**
** \name HS Command Message IDs */
/** \{ */
#define HS_CMD_MID           0x18AE    /**< \brief Msg ID for cmds to HS                */
#define HS_SEND_HK_MID       0x18AF    /**< \brief Msg ID to request HS housekeeping    */
#define HS_WAKEUP_MID        0x18B0    /**< \brief Msg ID to wake up HS                 */
/** \} */

/**
** \name HS Telemetry Message IDs */
/** \{ */
#define HS_HK_TLM_MID        0x08AD    /**< \brief HS Housekeeping Telemetry            */
/** \} */

#endif /*_hs_msgids_h_*/

/************************/
/*  End of File Comment */
/************************/
