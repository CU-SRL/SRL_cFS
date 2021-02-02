/*************************************************************************
** File: hs_testrunner.c 
**
** NASA Docket No. GSC-18,476-1, and identified as "Core Flight System 
** (cFS) Health and Safety (HS) Application version 2.3.2” 
**
** Copyright © 2020 United States Government as represented by the 
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
**   This file contains the unit test runner for the HS application.
**
*************************************************************************/

/*
 * Includes
 */

#include "uttest.h"
#include "hs_app_test.h"
#include "hs_cmds_test.h"
#include "hs_custom_test.h"
#include "hs_monitors_test.h"

/*
 * Function Definitions
 */

int main(void)
{   
    HS_App_Test_AddTestCases();
    HS_Cmds_Test_AddTestCases();
    HS_Custom_Test_AddTestCases();
    HS_Monitors_Test_AddTestCases();
    HS_Utils_Test_AddTestCases();

    return(UtTest_Run());
} /* end main */


/************************/
/*  End of File Comment */
/************************/
