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
 * \file     osapi-file-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-file.h"
#include "os-shared-dir.h"

/*
 * File API abstraction layer
 */

UT_DEFAULT_STUB(OS_FileOpen_Impl, (const OS_object_token_t *token, const char *local_path, int32 flags, int32 access))
UT_DEFAULT_STUB(OS_FileStat_Impl, (const char *local_path, os_fstat_t *filestat))
UT_DEFAULT_STUB(OS_FileRemove_Impl, (const char *local_path))
UT_DEFAULT_STUB(OS_FileRename_Impl, (const char *old_path, const char *new_path))
UT_DEFAULT_STUB(OS_FileChmod_Impl, (const char *local_path, uint32 access))
UT_DEFAULT_STUB(OS_ShellOutputToFile_Impl, (const OS_object_token_t *token, const char *Cmd))

/*
 * Directory API abstraction layer
 */
UT_DEFAULT_STUB(OS_DirCreate_Impl, (const char *local_path, uint32 access))
UT_DEFAULT_STUB(OS_DirOpen_Impl, (const OS_object_token_t *token, const char *local_path))
UT_DEFAULT_STUB(OS_DirClose_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_DirRead_Impl, (const OS_object_token_t *token, os_dirent_t *dirent))
UT_DEFAULT_STUB(OS_DirRewind_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_DirRemove_Impl, (const char *local_path))

/*
 * Stream abstraction layer (applies to sockets and files)
 */
int32 OS_GenericRead_Impl(const OS_object_token_t *token, void *buffer, size_t nbytes, int32 timeout)
{
    int32 Status = UT_DEFAULT_IMPL(OS_GenericRead_Impl);

    if (Status == OS_SUCCESS)
    {
        Status = UT_Stub_CopyToLocal(UT_KEY(OS_GenericRead_Impl), buffer, nbytes);
    }

    return Status;
}

int32 OS_GenericWrite_Impl(const OS_object_token_t *token, const void *buffer, size_t nbytes, int32 timeout)
{
    int32 Status = UT_DEFAULT_IMPL(OS_GenericWrite_Impl);

    if (Status == OS_SUCCESS)
    {
        Status = UT_Stub_CopyFromLocal(UT_KEY(OS_GenericWrite_Impl), (const uint8 *)buffer, nbytes);
    }

    return Status;
}

UT_DEFAULT_STUB(OS_GenericSeek_Impl, (const OS_object_token_t *token, int32 offset, uint32 whence))
UT_DEFAULT_STUB(OS_GenericClose_Impl, (const OS_object_token_t *token))
