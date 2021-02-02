/*************************************************************************
** File: hs_monitors.h 
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
**
** Purpose:
**   Specification for the CFS Health and Safety (HS) routines that
**   handle application and event monitoring
**
**
**************************************************************************/
#ifndef _hs_monitors_h_
#define _hs_monitors_h_

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

/*************************************************************************
** Exported Functions
*************************************************************************/
/************************************************************************/
/** \brief Check execution status of each app in AppMon table
**
**  \par Description
**       Cycles through the Application Monitor Table checking the current
**       execution count for each monitored application. If the count fails
**       to increment for the table specified duration, the table specified
**       action is taken.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void HS_MonitorApplications(void);

/************************************************************************/
/** \brief Search the EventMon table for matches to the incoming event
**
**  \par Description
**       Searches the Event Monitor Table for matches to the incoming
**       event message. If a match is found, the table specified action is
**       taken.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message
**
*************************************************************************/
void HS_MonitorEvent(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Monitor the utilization tracker counter
**
**  \par Description
**       Monitors the utilization tracker counter incremented by the Idle
**       Task, converting it into an estimated CPU Utilization for the
**       previous cycle. If the utilization is over a certain theshold
**       for a certain amount of time, an event is output.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void HS_MonitorUtilization(void);

/************************************************************************/
/** \brief Validate application monitor table
**
**  \par Description
**       This function is called by table services when a validation of
**       the application monitor table is required
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   *TableData     Pointer to the table data to validate
**
**  \returns
**  \retcode #CFE_SUCCESS            \retdesc \copydoc CFE_SUCCESS            \endcode
**  \retcode #HS_AMTVAL_ERR_ACT      \retdesc \copydoc HS_AMTVAL_ERR_ACT      \endcode
**  \retcode #HS_AMTVAL_ERR_NUL      \retdesc \copydoc HS_AMTVAL_ERR_NUL      \endcode
**  \endreturns
**
**  \sa #HS_ValidateEMTable, #HS_ValidateXCTable, #HS_ValidateMATable
**
*************************************************************************/
int32 HS_ValidateAMTable(void *TableData);

/************************************************************************/
/** \brief Validate event monitor table
**
**  \par Description
**       This function is called by table services when a validation of
**       the event monitor table is required
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   *TableData     Pointer to the table data to validate
**
**  \returns
**  \retcode #CFE_SUCCESS            \retdesc \copydoc CFE_SUCCESS            \endcode
**  \retcode #HS_EMTVAL_ERR_ACT      \retdesc \copydoc HS_EMTVAL_ERR_ACT      \endcode
**  \retcode #HS_EMTVAL_ERR_NUL      \retdesc \copydoc HS_EMTVAL_ERR_NUL      \endcode
**  \endreturns
**
**  \sa #HS_ValidateAMTable, #HS_ValidateXCTable, #HS_ValidateMATable
**
*************************************************************************/
int32 HS_ValidateEMTable(void *TableData);

/************************************************************************/
/** \brief Validate execution counter table
**
**  \par Description
**       This function is called by table services when a validation of
**       the execution counter table is required
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   *TableData     Pointer to the table data to validate
**
**  \returns
**  \retcode #CFE_SUCCESS            \retdesc \copydoc CFE_SUCCESS            \endcode
**  \retcode #HS_XCTVAL_ERR_TYPE     \retdesc \copydoc HS_XCTVAL_ERR_TYPE     \endcode
**  \retcode #HS_XCTVAL_ERR_NUL      \retdesc \copydoc HS_XCTVAL_ERR_NUL      \endcode
**  \endreturns
**
**  \sa #HS_ValidateAMTable, #HS_ValidateEMTable, #HS_ValidateMATable
**
*************************************************************************/
int32 HS_ValidateXCTable(void *TableData);

/************************************************************************/
/** \brief Validate message actions table
**
**  \par Description
**       This function is called by table services when a validation of
**       the message actions table is required
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   *TableData     Pointer to the table data to validate
**
**  \returns
**  \retcode #CFE_SUCCESS            \retdesc \copydoc CFE_SUCCESS            \endcode
**  \retcode #HS_MATVAL_ERR_ID       \retdesc \copydoc HS_MATVAL_ERR_ID       \endcode
**  \retcode #HS_MATVAL_ERR_LEN      \retdesc \copydoc HS_MATVAL_ERR_LEN      \endcode
**  \retcode #HS_MATVAL_ERR_ENA      \retdesc \copydoc HS_MATVAL_ERR_ENA      \endcode
**  \endreturns
**
**  \sa #HS_ValidateAMTable, #HS_ValidateEMTable, #HS_ValidateXCTable
**
*************************************************************************/
int32 HS_ValidateMATable(void *TableData);

/************************************************************************/
/** \brief Update and store CDS data
**
**  \par Description
**       This function is called to update and then store the data in the
**       critical data store.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   ResetsPerformed     Number of HS caused processor resets
**  \param [in]   MaxResets           Max number of resets allowed
**
*************************************************************************/
void HS_SetCDSData(uint16 ResetsPerformed, uint16 MaxResets);

#endif /* _hs_monitors_h_ */

/************************/
/*  End of File Comment */
/************************/
