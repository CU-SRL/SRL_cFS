/*
** Filename: fm_cmds.h 
**
** NASA Docket No. GSC-18,475-1, identified as “Core Flight Software System (CFS)
** File Manager Application Version 2.5.3
**
** Copyright © 2020 United States Government as represented by the Administrator of
** the National Aeronautics and Space Administration. All Rights Reserved. 
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
**  
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
*
** Title: CFS File Manager (FM) Application Ground Commands Header File
**
** Purpose: Specification for the CFS FM ground commands.
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**
*/

#ifndef _fm_cmds_h_
#define _fm_cmds_h_

#include "cfe.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler function prototypes                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
**  \brief Move File Command Handler Function
**
**  \par Description
**       This function generates an event that displays the application version
**       numbers.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_NOOP_CC, #FM_Noop, #FM_NoopCmd_t
**/
bool FM_NoopCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Reset Counters Command Handler Function
**
**  \par Description
**       This function resets the FM housekeeping telemetry counters.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_RESET_CC, #FM_Reset, #FM_ResetCmd_t
**/
bool FM_ResetCountersCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Copy File Command Handler Function
**
**  \par Description
**       This function copies the command specified source file to the command
**       specified target file.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       copying the file will be performed by a lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_COPY_CC, #FM_Copy, #FM_CopyFileCmd_t
**/
bool FM_CopyFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Move File Command Handler Function
**
**  \par Description
**       This function moves the command specified source file to the command
**       specified target filename.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_MOVE_CC, #FM_Move, #FM_MoveFileCmd_t
**/
bool FM_MoveFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Rename File Command Handler Function
**
**  \par Description
**       This function renames the command specified source file to the command
**       specified target filename.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_RENAME_CC, #FM_Rename, #FM_RenameFileCmd_t
**/
bool FM_RenameFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Delete File Command Handler Function
**
**  \par Description
**       This function deletes the command specified file.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_DELETE_CC, #FM_Delete, #FM_DeleteFileCmd_t
**/
bool FM_DeleteFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Delete All Files Command Handler Function
**
**  \par Description
**       This function deletes all files from the command specified directory.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       reading the directory and deleting each file will be performed by a
**       lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_DELETE_ALL_CC, #FM_DeleteAll, #FM_DeleteAllCmd_t
**/
bool FM_DeleteAllFilesCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Decompress Files Command Handler Function
**
**  \par Description
**       This function decompresses the command specified source file into the
**       command specified target file.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       decompressing the source file into the target file will be performed by
**       a lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_DECOMPRESS_CC, #FM_Decompress, #FM_DecompressCmd_t
**/
bool FM_DecompressFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Concatenate Files Command Handler Function
**
**  \par Description
**       This function concatenates two command specified source files into the
**       command specified target file.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       copying the first source file to the target file and then appending the
**       second source file to the target file will be performed by a lower priority
**       child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_CONCAT_CC, #FM_Concat, #FM_ConcatCmd_t
**/
bool FM_ConcatFilesCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Get File Information Command Handler Function
**
**  \par Description
**       This function creates a telemetry packet and populates the packet with
**       the current information regarding the command specified file.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       collecting the status data and calculating the CRC will be performed by
**       a lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_GET_FILE_INFO_CC, #FM_GetFileInfo, #FM_GetFileInfoCmd_t, #FM_FileInfoPkt_t
**/
bool FM_GetFileInfoCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Get Open Files List Command Handler Function
**
**  \par Description
**       This function creates a telemetry packet and populates it with a list of
**       the current open files.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_GET_OPEN_FILES_CC, #FM_GetOpenFiles, #FM_GetOpenFilesCmd_t, #FM_OpenFilesPkt_t
**/
bool FM_GetOpenFilesCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Create Directory Command Handler Function
**
**  \par Description
**       This function creates the command specified directory.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_CREATE_DIR_CC, #FM_CreateDir, #FM_CreateDirCmd_t
**/
bool FM_CreateDirectoryCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Delete Directory Command Handler Function
**
**  \par Description
**       This function deletes the command specified directory.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_DELETE_DIR_CC, #FM_DeleteDir, #FM_DeleteDirCmd_t
**/
bool FM_DeleteDirectoryCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Get Directory List to Packet Command Handler Function
**
**  \par Description
**       This function creates an output file and writes a listing of the command
**       specified directory to the file.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       reading the directory will be performed by a lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_GET_DIR_FILE_CC, #FM_GetDirFile, #FM_GetDirFileCmd_t,
        #FM_DirListFileStats_t, FM_DirListEntry_t
**/
bool FM_GetDirListFileCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Get Directory List to Packet Command Handler Function
**
**  \par Description
**       This function creates a telemetry packet and populates the packet with
**       the directory listing data for the command specified directory, starting
**       at the command specified directory entry.
**
**       Because of the possibility that this command might take a very long time
**       to complete, command argument validation will be done immediately but
**       reading the directory will be performed by a lower priority child task.
**       As such, the return value for this function only refers to the result
**       of command argument verification and being able to place the command on
**       the child task interface queue.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_GET_DIR_PKT_CC, #FM_GetDirPkt, #FM_GetDirPktCmd_t, #FM_DirListPkt_t
**/
bool FM_GetDirListPktCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Get Free Space Command Handler Function
**
**  \par Description
**       This function creates a telemetry packet and populates the packet with
**       free space data for each file system listed in the FM File System Free
**       Space Table.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_GET_FREE_SPACE_CC, #FM_GetFreeSpace, #FM_GetFreeSpaceCmd_t, #FM_FreeSpacePkt_t
**/
bool FM_GetFreeSpaceCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Set Table Entry State Command Handler Function
**
**  \par Description
**       This function modifies the enable/disable state for a single entry in
**       the File System Free Space Table.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_SET_TABLE_STATE_CC, #FM_SetTableState, #FM_SetTableStateCmd_t, #FM_TableEntry_t
**/
bool FM_SetTableStateCmd(CFE_MSG_Message_t* MessagePtr);


/**
**  \brief Set File Permissions of a file
**
**  \par Description
**       This function is a direct call to OS_chmod to set the file access
**
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \returns
**  \retcode #true   \retdesc \copydoc true    \endcode
**  \retcode #false  \retdesc \copydoc false   \endcode
**  \retstmt Boolean true indicates command success  \endcode
**  \endreturns
**
**  \sa #FM_SET_PERM_CC, #FM_SetPermCmd_t, #FM_SET_PERM_CMD_EID, #FM_SET_PERM_ERR_EID
**/
bool FM_SetPermissionsCmd(CFE_MSG_Message_t* MessagePtr);

#endif /* _fm_cmds_h_ */

/************************/
/*  End of File Comment */
/************************/

