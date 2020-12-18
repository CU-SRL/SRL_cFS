/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_ModuleAPI_Init, (void))

/*****************************************************************************/
/**
** \brief dummy_function stub function
**
** \par Description
**        This function is used by the OS API function, OS_SymbolLookup, which
**        requires a valid function for which to report the address.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns a user-defined status value.
**
******************************************************************************/
int32 dummy_function(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(dummy_function, 0);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleLoad stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleLoad.  The user can adjust the response by setting
**        the values in the ModuleLoadRtn structure prior to this function
**        being called.  If the value ModuleLoadRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleLoadRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleLoad(osal_id_t *module_id, const char *module_name, const char *filename, uint32 flags)
{
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleLoad), module_id);
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleLoad), module_name);
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleLoad), filename);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ModuleLoad), flags);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleLoad);

    if (status == OS_SUCCESS)
    {
        *module_id = UT_AllocStubObjId(UT_OBJTYPE_MODULE);
    }
    else
    {
        *module_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleUnload stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleUnload.  The user can adjust the response by setting
**        the values in the ModuleUnloadRtn structure prior to this function
**        being called.  If the value ModuleUnloadRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleUnloadRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleUnload(osal_id_t module_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ModuleUnload), module_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleUnload);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_MODULE, module_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleInfo.  The user can adjust the response by setting
**        the values in the ModuleInfoRtn structure prior to this function
**        being called.  If the value ModuleInfoRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleInfoRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleInfo(osal_id_t module_id, OS_module_prop_t *module_info)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ModuleInfo), module_id);
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleInfo), module_info);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ModuleInfo), module_info, sizeof(*module_info)) < sizeof(*module_info))
    {
        memset(module_info, 0, sizeof(*module_info));
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_SymbolLookup stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_SymbolLookup.  The user can adjust the response by setting
**        the values in the SymbolLookupRtn structure prior to this function
**        being called.  If the value SymbolLookupRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value SymbolLookupRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_SymbolLookup(cpuaddr *symbol_address, const char *symbol_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SymbolLookup), symbol_address);
    UT_Stub_RegisterContext(UT_KEY(OS_SymbolLookup), symbol_name);

    int32 status;

    /*
     * Register the context so a hook can do something with the parameters
     */

    status = UT_DEFAULT_IMPL(OS_SymbolLookup);

    if (status != OS_SUCCESS)
    {
        *symbol_address = 0xDEADBEEFU;
    }
    else if (UT_Stub_CopyToLocal(UT_KEY(OS_SymbolLookup), symbol_address, sizeof(*symbol_address)) <
             sizeof(*symbol_address))
    {
        /* return the dummy function when test didn't register anything else */
        *symbol_address = (cpuaddr)&dummy_function;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SymbolTableDump()
 *
 *****************************************************************************/
int32 OS_ModuleSymbolLookup(osal_id_t module_id, cpuaddr *symbol_address, const char *symbol_name)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ModuleSymbolLookup), module_id);
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleSymbolLookup), symbol_address);
    UT_Stub_RegisterContext(UT_KEY(OS_ModuleSymbolLookup), symbol_name);

    int32 status;

    /*
     * Register the context so a hook can do something with the parameters
     */

    status = UT_DEFAULT_IMPL(OS_ModuleSymbolLookup);

    if (status != OS_SUCCESS)
    {
        *symbol_address = 0xDEADBEEFU;
    }
    else if (UT_Stub_CopyToLocal(UT_KEY(OS_ModuleSymbolLookup), symbol_address, sizeof(*symbol_address)) <
             sizeof(*symbol_address))
    {
        /* return the dummy function when test didn't register anything else */
        *symbol_address = (cpuaddr)&dummy_function;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SymbolTableDump()
 *
 *****************************************************************************/
int32 OS_SymbolTableDump(const char *filename, size_t size_limit)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SymbolTableDump), filename);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SymbolTableDump), size_limit);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SymbolTableDump);

    return status;
}
