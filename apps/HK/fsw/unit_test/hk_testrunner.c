/*************************************************************************
** File: hk_testrunner.c
**
** NASA Docket No. GSC-18449-1, and identified as "Core Flight System (cFS)
** Housekeeping (HK) Application version 2.4.3” 
**
** Copyright © 2019 United States Government as represented by the Administrator of
** the National Aeronautics and Space Administration.  All Rights Reserved. 
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
**   This file contains the unit test runner for the HK application.
**
** References:
**   Flight Software Branch C Coding Standard Version 1.2
**   CFS Development Standards Document
**
** Notes:
**
*************************************************************************/

/*
 * Includes
 */

#include "uttest.h"
#include "hk_app_test.h"
#include "hk_utils_test.h"

/*
 * Function Definitions
 */

int main(void)
{   
    HK_App_Test_AddTestCases();
    HK_Utils_Test_AddTestCases();

    return(UtTest_Run());
} /* end main */

/************************/
/*  End of File Comment */
/************************/
