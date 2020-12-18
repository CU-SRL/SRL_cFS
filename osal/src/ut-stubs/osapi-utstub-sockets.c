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
 * \file   osapi-utstub-sockets.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_SocketAPI_Init, (void))

/*****************************************************************************
 *
 * Stub function for OS_SocketOpen()
 *
 *****************************************************************************/
int32 OS_SocketOpen(osal_id_t *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketOpen), sock_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketOpen), Domain);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketOpen), Type);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketOpen);

    if (status == OS_SUCCESS)
    {
        *sock_id = UT_AllocStubObjId(UT_OBJTYPE_SOCKET);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketBind()
 *
 *****************************************************************************/
int32 OS_SocketBind(osal_id_t sock_id, const OS_SockAddr_t *Addr)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketBind), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketBind), Addr);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketBind);

    return status;
}
/*****************************************************************************
 *
 * Stub function for OS_SocketAccept()
 *
 *****************************************************************************/
int32 OS_SocketAccept(osal_id_t sock_id, osal_id_t *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketAccept), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAccept), connsock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAccept), Addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketAccept), timeout);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAccept);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketConnect()
 *
 *****************************************************************************/
int32 OS_SocketConnect(osal_id_t sock_id, const OS_SockAddr_t *Addr, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketConnect), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketConnect), Addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketConnect), timeout);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketConnect);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketRecvFrom()
 *
 *****************************************************************************/
int32 OS_SocketRecvFrom(osal_id_t sock_id, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketRecvFrom), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketRecvFrom), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketRecvFrom), buflen);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketRecvFrom), RemoteAddr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketRecvFrom), timeout);

    int32  status;
    size_t CopySize;

    status = UT_DEFAULT_IMPL(OS_SocketRecvFrom);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketRecvFrom), buffer, buflen);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, buflen);
            status = buflen;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketSendTo()
 *
 *****************************************************************************/
int32 OS_SocketSendTo(osal_id_t sock_id, const void *buffer, size_t buflen, const OS_SockAddr_t *RemoteAddr)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketSendTo), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketSendTo), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketSendTo), buflen);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketSendTo), RemoteAddr);

    int32  status;
    size_t CopySize;

    status = UT_DEFAULT_IMPL_RC(OS_SocketSendTo, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyFromLocal(UT_KEY(OS_SocketSendTo), buffer, buflen);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = buflen;
        }
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketGetIdByName()
 *
 *****************************************************************************/
int32 OS_SocketGetIdByName(osal_id_t *sock_id, const char *sock_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketGetIdByName), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketGetIdByName), sock_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetIdByName), sock_id, sizeof(*sock_id)) < sizeof(*sock_id))
    {
        UT_ObjIdCompose(1, UT_OBJTYPE_SOCKET, sock_id);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_SocketGetInfo(,sock_id)
 *
 *****************************************************************************/
int32 OS_SocketGetInfo(osal_id_t sock_id, OS_socket_prop_t *sock_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketGetInfo), sock_id);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketGetInfo), sock_prop);

    int32  status;
    size_t CopySize;

    status = UT_DEFAULT_IMPL(OS_SocketGetInfo);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetInfo), sock_prop, sizeof(*sock_prop));
        if (CopySize < sizeof(*sock_prop))
        {
            UT_ObjIdCompose(1, UT_OBJTYPE_TASK, &sock_prop->creator);
            strncpy(sock_prop->name, "ut", sizeof(sock_prop->name));
        }
    }

    return status;
}

int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrInit), Addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketAddrInit), Domain);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrInit);

    if (status == OS_SUCCESS && UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrInit), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }

    return status;
}

int32 OS_SocketAddrToString(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrToString), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketAddrToString), buflen);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrToString), Addr);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrToString);

    if (status == OS_SUCCESS && buflen > 0 && UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrToString), buffer, buflen) == 0)
    {
        strncpy(buffer, "UT-addr", buflen - 1);
        buffer[buflen - 1] = 0;
    }

    return status;
}

int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrFromString), Addr);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrFromString), string);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrFromString);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrFromString), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }

    return status;
}

int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrGetPort), PortNum);
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrGetPort), Addr);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrGetPort);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrGetPort), PortNum, sizeof(*PortNum)) < sizeof(*PortNum))
    {
        *PortNum = 0;
    }

    return status;
}

int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
{
    UT_Stub_RegisterContext(UT_KEY(OS_SocketAddrSetPort), Addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SocketAddrSetPort), PortNum);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_SocketAddrSetPort);

    return status;
}
