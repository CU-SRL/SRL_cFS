/*************************************************************************
** File: hs_tbldefs.h
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
**   Specification for the CFS Health and Safety (HS) table related
**   constant definitions.
**
** Notes:
**   These Macro definitions have been put in this file (instead of
**   hs_tbl.h) so this file can be included directly into ASIST build
**   test scripts. ASIST RDL files can accept C language #defines but
**   can't handle type definitions. As a result: DO NOT PUT ANY
**   TYPEDEFS OR STRUCTURE DEFINITIONS IN THIS FILE!
**   ADD THEM TO hs_tbl.h IF NEEDED!
**
**
*************************************************************************/
#ifndef _hs_tbldefs_h_
#define _hs_tbldefs_h_

/************************************************************************
** Macro Definitions
*************************************************************************/
/**
** \name HS Table Name Strings */
/** \{ */
#define HS_AMT_TABLENAME    "AppMon_Tbl"
#define HS_EMT_TABLENAME    "EventMon_Tbl"
#define HS_XCT_TABLENAME    "ExeCount_Tbl"
#define HS_MAT_TABLENAME    "MsgActs_Tbl"
/** \} */

/**
** \name Application Monitor Table (AMT) Action Types */
/** \{ */
#define HS_AMT_ACT_NOACT          0    /**< \brief No action is taken */
#define HS_AMT_ACT_PROC_RESET     1    /**< \brief Generates Processor Reset on failure */
#define HS_AMT_ACT_APP_RESTART    2    /**< \brief Attempts to restart application on failure */
#define HS_AMT_ACT_EVENT          3    /**< \brief Generates event message on failure */
#define HS_AMT_ACT_LAST_NONMSG    3    /**< \brief Index for finding end of non-message actions */
/** \} */

/**
** \name Event Monitor Table (EMT) Action Types */
/** \{ */
#define HS_EMT_ACT_NOACT          0    /**< \brief No action is taken */
#define HS_EMT_ACT_PROC_RESET     1    /**< \brief Generates Processor Reset on detection */
#define HS_EMT_ACT_APP_RESTART    2    /**< \brief Attempts to restart application on detection */
#define HS_EMT_ACT_APP_DELETE     3    /**< \brief Deletes application on detection */
#define HS_EMT_ACT_LAST_NONMSG    3    /**< \brief Index for finding end of non-message actions */
/** \} */

/**
** \name Execution Counters Table (XCT) Resource Types */
/** \{ */
#define HS_XCT_TYPE_NOTYPE        0    /**< \brief No type */
#define HS_XCT_TYPE_APP_MAIN      1    /**< \brief Counter for Application Main task */
#define HS_XCT_TYPE_APP_CHILD     2    /**< \brief Counter for Application Child task */
#define HS_XCT_TYPE_DEVICE        3    /**< \brief Counter for Device Driver */
#define HS_XCT_TYPE_ISR           4    /**< \brief Counter for Interrupt Service Routine */
/** \} */

/**
** \name Message Actions Table (MAT) Enable State */
/** \{ */
#define HS_MAT_STATE_DISABLED     0    /**< \brief Message Actions are Disabled */
#define HS_MAT_STATE_ENABLED      1    /**< \brief Message Actions are Enabled  */
#define HS_MAT_STATE_NOEVENT      2    /**< \brief Message Actions are Enabled but produce no events */
/** \} */

/**
** \name Application Monitor Table (AMT) Validation Error Enumerated Types */
/** \{ */
#define HS_AMTVAL_NO_ERR          0    /**< \brief No error                          */
#define HS_AMTVAL_ERR_ACT         -1   /**< \brief Invalid ActionType specified      */
#define HS_AMTVAL_ERR_NUL         -2   /**< \brief Null Safety Buffer not Null       */
/** \} */

/**
** \name Event Monitor Table (EMT) Validation Error Enumerated Types */
/** \{ */
#define HS_EMTVAL_NO_ERR          0    /**< \brief No error                          */
#define HS_EMTVAL_ERR_ACT         -1   /**< \brief Invalid ActionType specified      */
#define HS_EMTVAL_ERR_NUL         -2   /**< \brief Null Safety Buffer not Null       */
/** \} */

/**
** \name Event Counter Table (XCT) Validation Error Enumerated Types */
/** \{ */
#define HS_XCTVAL_NO_ERR          0    /**< \brief No error                          */
#define HS_XCTVAL_ERR_TYPE        -1   /**< \brief Invalid Counter Type specified    */
#define HS_XCTVAL_ERR_NUL         -2   /**< \brief Null Safety Buffer not Null       */
/** \} */

/**
** \name Message Actions Table (MAT) Validation Error Enumerated Types */
/** \{ */
#define HS_MATVAL_NO_ERR          0    /**< \brief No error                          */
#define HS_MATVAL_ERR_ID          -1   /**< \brief Invalid Message ID specified      */
#define HS_MATVAL_ERR_LEN         -2   /**< \brief Invalid Length specified          */
#define HS_MATVAL_ERR_ENA         -3   /**< \brief Invalid Enable State specified    */
/** \} */

#endif /*_hs_tbldefs_h_*/

/************************/
/*  End of File Comment */
/************************/
