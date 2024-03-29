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
** File: to_sub_table.h
**
** Purpose: 
**  Define TO Lab CPU specific subscription table 
**
** Notes:
**
*************************************************************************/

/* 
** Add the proper include file for the message IDs below
*/
#include "cfe_msgids.h"

/*
** Common CFS app includes below are commented out
*/

#include "ci_lab_msgids.h"

//#include "sample_app_msgids.h"

#include "hs_msgids.h"
#include "fm_msgids.h"
#include "sc_msgids.h"
#include "ds_msgids.h"
#include "cs_msgids.h"
#include "hk_msgids.h"
#include "aimu_lis3mdl_msgids.h"
#include "aimu_lps25h_msgids.h"
#include "aimu_lsm6ds33_msgids.h"
/* #include "lc_msgids.h" */


static TO_subscription_t  TO_SubTable[] =
{
            /* CFS App Subscriptions */
            {TO_LAB_HK_TLM_MID,     {0,0},  4},
            {TO_LAB_DATA_TYPES_MID, {0,0},  4},
            {CI_LAB_HK_TLM_MID,     {0,0},  4},
            //{SAMPLE_APP_HK_TLM_MID, {0,0},  4},

	        {HS_HK_TLM_MID,         {0,0},  4},
            {FM_HK_TLM_MID,         {0,0},  4},
            {SC_HK_TLM_MID,         {0,0},  4},
            {DS_HK_TLM_MID,         {0,0},  4},
            {CS_HK_TLM_MID,         {0,0},  4},
            {HK_HK_TLM_MID,         {0,0},  4},

            /* cFE Core subscriptions */
            {CFE_ES_HK_TLM_MID,          {0,0},  4},
            {CFE_EVS_HK_TLM_MID,         {0,0},  4},
            {CFE_SB_HK_TLM_MID,          {0,0},  4},
            {CFE_TBL_HK_TLM_MID,         {0,0},  4},
            {CFE_TIME_HK_TLM_MID,        {0,0},  4},
            {CFE_TIME_DIAG_TLM_MID,      {0,0},  4},
            {CFE_SB_STATS_TLM_MID,       {0,0},  4},
            {CFE_TBL_REG_TLM_MID,        {0,0},  4},
            {CFE_EVS_LONG_EVENT_MSG_MID, {0,0}, 32},
            {CFE_ES_SHELL_TLM_MID,       {0,0}, 32},
            {CFE_ES_APP_TLM_MID,         {0,0},  4},
            {CFE_ES_MEMSTATS_TLM_MID,    {0,0},  4},

	    /* cFS Extra Telemetry */
            {DS_DIAG_TLM_MID,        {0,0},  4},
            {FM_FILE_INFO_TLM_MID,   {0,0},  4},
            {FM_DIR_LIST_TLM_MID,    {0,0},  4},
	        {FM_OPEN_FILES_TLM_MID,  {0,0},  4},
	        {FM_FREE_SPACE_TLM_MID,  {0,0},  4},
            {FM_FREE_SPACE_TLM_MID,  {0,0},  4},
            {AIMU_LIS3MDL_HK_TLM_MID,   {0,0},  4},
            {AIMU_LIS3MDL_DATA_TLM_MID, {0,0},  4},
            {AIMU_LPS25H_HK_TLM_MID,   {0,0},  4},
            {AIMU_LPS25H_DATA_TLM_MID, {0,0},  4},
            {AIMU_LSM6DS33_HK_TLM_MID,   {0,0},  4},
            {AIMU_LSM6DS33_DATA_TLM_MID, {0,0},  4},
            {TO_UNUSED,              {0, 0}, 0} //end of valid MIDs
};

/************************
** End of File Comment ** 
************************/
