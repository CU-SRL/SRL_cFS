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

UT_DEFAULT_STUB(OS_TimerCbAPI_Init, (void))

/*****************************************************************************
 *
 * Stub function for OS_TimerAdd()
 *
 *****************************************************************************/
int32 OS_TimerAdd(osal_id_t *timer_id, const char *timer_name, osal_id_t timebase_id, OS_ArgCallback_t callback_ptr,
                  void *callback_arg)
{
    UT_Stub_RegisterContext(UT_KEY(OS_TimerAdd), timer_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerAdd), timer_name);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerAdd), timebase_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerAdd), callback_ptr);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerAdd), callback_arg);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerAdd);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerCreate()
 *
 *****************************************************************************/
int32 OS_TimerCreate(osal_id_t *timer_id, const char *timer_name, uint32 *clock_accuracy,
                     OS_TimerCallback_t callback_ptr)
{
    UT_Stub_RegisterContext(UT_KEY(OS_TimerCreate), timer_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerCreate), timer_name);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerCreate), clock_accuracy);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerCreate), callback_ptr);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerCreate);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerSet()
 *
 *****************************************************************************/
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerSet), timer_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerSet), start_time);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerSet), interval_time);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerSet);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TimerDelete stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_TimerDelete.  It always returns OS_ERR_INVALID_ID.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_ERR_INVALID_ID.
**
******************************************************************************/
int32 OS_TimerDelete(osal_id_t timer_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerDelete), timer_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TIMECB, timer_id);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerGetIdByName()
 *
 *****************************************************************************/
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_TimerGetIdByName), timer_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerGetIdByName), timer_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetIdByName), timer_id, sizeof(*timer_id)) < sizeof(*timer_id))
    {
        UT_ObjIdCompose(1, UT_OBJTYPE_TIMECB, timer_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TimerGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TimerGetInfo.  The user can adjust the response by setting
**        the values in the OSTimerGetInfoRtn structure prior to this function
**        being called.  If the value OSTimerGetInfoRtn.count is greater than
**        zero then the counter is decremented and the timer creator value is
**        set to the user-defined value OSTimerGetInfoRtn.value.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimerGetInfo), timer_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimerGetInfo), timer_prop);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerGetInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetInfo), timer_prop, sizeof(*timer_prop)) < sizeof(*timer_prop))
    {
        memset(timer_prop, 0, sizeof(*timer_prop));
        UT_ObjIdCompose(1, UT_OBJTYPE_TASK, &timer_prop->creator);
    }

    return status;
}
