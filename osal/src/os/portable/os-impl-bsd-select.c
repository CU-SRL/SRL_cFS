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
 * \file   os-impl-bsd-select.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains wrappers around the select() system call
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   FD_SET/FD_CLR/FD_ISSET macros and fd_set typedef
 *   select()
 *   clock_gettime() - for computing select timeouts
 */
#include <string.h>
#include <errno.h>
#include <time.h>

#include <osapi.h>
#include "os-impl-select.h"
#include "os-shared-select.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/****************************************************************************************
                                LOCAL FUNCTIONS
 ***************************************************************************************/

/*----------------------------------------------------------------
 * Function: OS_FdSet_ConvertIn_Impl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *          Convert an OS_FdSet (OSAL) structure into an fd_set (POSIX)
 *          which can then be passed to the POSIX select function.
 *
 * returns: Highest numbered file descriptor in the output fd_set
 *-----------------------------------------------------------------*/
static int OS_FdSet_ConvertIn_Impl(fd_set *os_set, OS_FdSet *OSAL_set)
{
    size_t       offset;
    size_t       bit;
    osal_index_t id;
    uint8        objids;
    int          osfd;
    int          maxfd;

    maxfd = -1;
    for (offset = 0; offset < sizeof(OSAL_set->object_ids); ++offset)
    {
        objids = OSAL_set->object_ids[offset];
        bit    = 0;
        while (objids != 0)
        {
            if (objids & 0x01)
            {
                id   = OSAL_INDEX_C((offset * 8) + bit);
                osfd = OS_impl_filehandle_table[id].fd;
                if (osfd >= 0 && OS_impl_filehandle_table[id].selectable)
                {
                    FD_SET(osfd, os_set);
                    if (osfd > maxfd)
                    {
                        maxfd = osfd;
                    }
                }
            }
            ++bit;
            objids >>= 1;
        }
    }

    return maxfd;
} /* end OS_FdSet_ConvertIn_Impl */

/*----------------------------------------------------------------
 * Function: OS_FdSet_ConvertOut_Impl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *          Convert a POSIX fd_set structure into an OSAL OS_FdSet
 *          which can then be returned back to the application.
 *
 *          This actually un-sets any bits in the "Input" parameter
 *          which are also set in the "output" parameter.
 *-----------------------------------------------------------------*/
static void OS_FdSet_ConvertOut_Impl(fd_set *output, OS_FdSet *Input)
{
    size_t       offset;
    size_t       bit;
    osal_index_t id;
    uint8        objids;
    int          osfd;

    for (offset = 0; offset < sizeof(Input->object_ids); ++offset)
    {
        objids = Input->object_ids[offset];
        bit    = 0;
        while (objids != 0)
        {
            if (objids & 0x01)
            {
                id   = OSAL_INDEX_C((offset * 8) + bit);
                osfd = OS_impl_filehandle_table[id].fd;
                if (osfd < 0 || !FD_ISSET(osfd, output))
                {
                    Input->object_ids[offset] &= ~(1 << bit);
                }
            }
            ++bit;
            objids >>= 1;
        }
    }
} /* end OS_FdSet_ConvertOut_Impl */

/*----------------------------------------------------------------
 * Function: OS_DoSelect
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *          Actual implementation of select() call
 *          Used by SelectSingle and SelectMultiple implementations (below)
 *-----------------------------------------------------------------*/
static int32 OS_DoSelect(int maxfd, fd_set *rd_set, fd_set *wr_set, int32 msecs)
{
    int             os_status;
    int32           return_code;
    struct timeval  tv;
    struct timeval *tvptr;
    struct timespec ts_now;
    struct timespec ts_end;

    if (msecs > 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &ts_now);
        ts_end.tv_sec  = ts_now.tv_sec + (msecs / 1000);
        ts_end.tv_nsec = ts_now.tv_nsec + (1000000 * (msecs % 1000));
        if (ts_end.tv_nsec >= 1000000000)
        {
            ++ts_end.tv_sec;
            ts_end.tv_nsec -= 1000000000;
        }
    }
    else
    {
        /* eliminates a false warning about possibly uninitialized use */
        memset(&ts_end, 0, sizeof(ts_end));
    }

    do
    {
        if (msecs < 0)
        {
            tvptr = NULL;
        }
        else if (msecs == 0)
        {
            tvptr      = &tv;
            tv.tv_sec  = 0;
            tv.tv_usec = 0;
        }
        else
        {
            tvptr = &tv;

            clock_gettime(CLOCK_MONOTONIC, &ts_now);

            /* note that the tv_sec and tv_usec/tv_nsec values are all signed longs, so OK to subtract */
            tv.tv_sec  = ts_end.tv_sec - ts_now.tv_sec;
            tv.tv_usec = (ts_end.tv_nsec - ts_now.tv_nsec) / 1000;

            if (tv.tv_sec < 0 || (tv.tv_sec == 0 && tv.tv_usec < 0))
            {
                os_status = 0;
                break;
            }

            if (tv.tv_usec < 0)
            {
                tv.tv_usec += 1000000;
                ++tv.tv_sec;
            }
        }

        os_status = select(maxfd + 1, rd_set, wr_set, NULL, tvptr);
    } while (os_status < 0 && errno == EINTR);

    if (os_status < 0)
    {
        return_code = OS_ERROR;
    }
    else if (os_status == 0)
    {
        return_code = OS_ERROR_TIMEOUT;
    }
    else
    {
        return_code = OS_SUCCESS;
    }

    return return_code;
} /* end OS_DoSelect */

/****************************************************************************************
                                SELECT API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_SelectSingle_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, int32 msecs)
{
    int32                           return_code;
    fd_set                          wr_set;
    fd_set                          rd_set;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    /*
     * If called on a stream_id which does not support this
     * operation, return immediately and do not invoke the system call
     */
    if (!impl->selectable)
    {
        return OS_ERR_OPERATION_NOT_SUPPORTED;
    }

    if (*SelectFlags != 0)
    {
        FD_ZERO(&wr_set);
        FD_ZERO(&rd_set);
        if (*SelectFlags & OS_STREAM_STATE_READABLE)
        {
            FD_SET(impl->fd, &rd_set);
        }
        if (*SelectFlags & OS_STREAM_STATE_WRITABLE)
        {
            FD_SET(impl->fd, &wr_set);
        }

        return_code = OS_DoSelect(impl->fd, &rd_set, &wr_set, msecs);

        if (return_code == OS_SUCCESS)
        {
            if (!FD_ISSET(impl->fd, &rd_set))
            {
                *SelectFlags &= ~OS_STREAM_STATE_READABLE;
            }
            if (!FD_ISSET(impl->fd, &wr_set))
            {
                *SelectFlags &= ~OS_STREAM_STATE_WRITABLE;
            }
        }
        else
        {
            *SelectFlags = 0;
        }
    }
    else
    {
        /* Nothing to check for, return immediately. */
        return_code = OS_SUCCESS;
    }

    return return_code;
} /* end OS_SelectSingle_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_SelectMultiple_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    fd_set wr_set;
    fd_set rd_set;
    int    osfd;
    int    maxfd;
    int32  return_code;

    /*
     * This return code will be used if the set(s) do not
     * contain any file handles capable of select().  It
     * will be overwritten with the real result of the
     * select call, if selectable file handles were specified.
     */
    return_code = OS_ERR_OPERATION_NOT_SUPPORTED;
    FD_ZERO(&rd_set);
    FD_ZERO(&wr_set);
    maxfd = -1;
    if (ReadSet != NULL)
    {
        osfd = OS_FdSet_ConvertIn_Impl(&rd_set, ReadSet);
        if (osfd > maxfd)
        {
            maxfd = osfd;
        }
    }
    if (WriteSet != NULL)
    {
        osfd = OS_FdSet_ConvertIn_Impl(&wr_set, WriteSet);
        if (osfd > maxfd)
        {
            maxfd = osfd;
        }
    }

    if (maxfd >= 0)
    {
        return_code = OS_DoSelect(maxfd, &rd_set, &wr_set, msecs);
    }

    if (return_code == OS_SUCCESS)
    {
        if (ReadSet != NULL)
        {
            OS_FdSet_ConvertOut_Impl(&rd_set, ReadSet);
        }
        if (WriteSet != NULL)
        {
            OS_FdSet_ConvertOut_Impl(&wr_set, WriteSet);
        }
    }

    return return_code;
} /* end OS_SelectMultiple_Impl */
