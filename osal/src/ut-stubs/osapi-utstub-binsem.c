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

UT_DEFAULT_STUB(OS_BinSemAPI_Init, (void))

/*****************************************************************************/
/**
** \brief OS_BinSemTake stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTake.  The user can adjust the response by setting
**        the value of UT_BinSemFail, causing it to return a failure
**        indication (-1) on the first or second call to the function.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either -1 or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTake(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemTake), sem_id);

    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_BinSemTake);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemFlush stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemFlush.  The variable OSBinSemFlushRtn.value is set to the
**        value passed to the function, reset_type, and the variable
**        OSBinSemFlushRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemFlush(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemFlush), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemFlush);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemCreate.  The user can adjust the response by setting the
**        values in the OS_BinSemCreateRtn structure prior to this function
**        being called.  If the value OS_BinSemCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value
**        OS_BinSemCreateRtn.value.  Alternately, the user can cause the
**        function to return a failure result, OS_ERROR, by setting the value
**        of UT_OS_Fail to OS_SEMCREATE_FAIL prior to this function being
**        called.  OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    UT_Stub_RegisterContext(UT_KEY(OS_BinSemCreate), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_BinSemCreate), sem_name);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemCreate), sem_initial_value);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemCreate), options);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_BINSEM);
    }
    else
    {
        *sem_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGive stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGive.  The user can adjust the response by setting the value
**        of UT_BinSemFail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns the user-defined value UT_BinSemFail.
**
******************************************************************************/
int32 OS_BinSemGive(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemGive), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGive);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGetInfo.  It sets the binary semaphore structure variables
**        to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemGetInfo(osal_id_t sem_id, OS_bin_sem_prop_t *bin_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemGetInfo), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_BinSemGetInfo), bin_prop);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGetInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetInfo), bin_prop, sizeof(*bin_prop)) < sizeof(*bin_prop))
    {
        UT_ObjIdCompose(1, UT_OBJTYPE_TASK, &bin_prop->creator);
        strncpy(bin_prop->name, "Name", OS_MAX_API_NAME - 1);
        bin_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemDelete.  The user can adjust the response by setting
**        the values in the BinSemDelRtn structure prior to this function
**        being called.  If the value BinSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value BinSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemDelete(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemDelete), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_BINSEM, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemTimedWait stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTimedWait.  The variable OSBinSemTimedWaitRtn.value is set
**        to the value passed to the function, reset_type, and the variable
**        OSBinSemTimedWaitRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTimedWait(osal_id_t sem_id, uint32 msecs)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemTimedWait), sem_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_BinSemTimedWait), msecs);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemTimedWait);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_BinSemGetIdByName()
 *
 *****************************************************************************/
int32 OS_BinSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_BinSemGetIdByName), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_BinSemGetIdByName), sem_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        UT_ObjIdCompose(1, UT_OBJTYPE_BINSEM, sem_id);
    }

    return status;
}
