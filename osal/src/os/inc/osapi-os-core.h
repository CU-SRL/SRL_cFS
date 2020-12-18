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

/*
 * File: osapi-os-core.h
 *
 * Author:  Ezra Yeheksli -Code 582/Raytheon
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, Core OS module
 */

#ifndef _osapi_core_
#define _osapi_core_

#include <stdarg.h> /* for va_list */

/* Defines constants for making object ID's unique */
#define OS_OBJECT_INDEX_MASK 0xFFFF /**< @brief Object index mask */
#define OS_OBJECT_TYPE_SHIFT 16     /**< @brief Object type shift */

/** @defgroup OSObjectTypes OSAL Object Type Defines
 * @{
 */
#define OS_OBJECT_TYPE_UNDEFINED   0x00 /**< @brief Object type undefined */
#define OS_OBJECT_TYPE_OS_TASK     0x01 /**< @brief Object task type */
#define OS_OBJECT_TYPE_OS_QUEUE    0x02 /**< @brief Object queue type */
#define OS_OBJECT_TYPE_OS_COUNTSEM 0x03 /**< @brief Object counting semaphore type */
#define OS_OBJECT_TYPE_OS_BINSEM   0x04 /**< @brief Object binary semaphore type */
#define OS_OBJECT_TYPE_OS_MUTEX    0x05 /**< @brief Object mutex type */
#define OS_OBJECT_TYPE_OS_STREAM   0x06 /**< @brief Object stream type */
#define OS_OBJECT_TYPE_OS_DIR      0x07 /**< @brief Object directory type */
#define OS_OBJECT_TYPE_OS_TIMEBASE 0x08 /**< @brief Object timebase type */
#define OS_OBJECT_TYPE_OS_TIMECB   0x09 /**< @brief Object timer callback type */
#define OS_OBJECT_TYPE_OS_MODULE   0x0A /**< @brief Object module type */
#define OS_OBJECT_TYPE_OS_FILESYS  0x0B /**< @brief Object file system type */
#define OS_OBJECT_TYPE_OS_CONSOLE  0x0C /**< @brief Object console type */
#define OS_OBJECT_TYPE_USER        0x10 /**< @brief Object user type */
/**@}*/

/** @brief Upper limit for OSAL task priorities */
#define OS_MAX_TASK_PRIORITY 255

/**
 * @brief Initializer for the osal_id_t type which will not match any valid value
 */
#define OS_OBJECT_ID_UNDEFINED ((osal_id_t) {0})

/**
 * @brief Constant that may be passed to OS_ForEachObject()/OS_ForEachObjectOfType() to match any
 * creator (i.e. get all objects)
 */
#define OS_OBJECT_CREATOR_ANY OS_OBJECT_ID_UNDEFINED

/** @defgroup OSSemaphoreStates OSAL Semaphore State Defines
 * @{
 */
#define OS_SEM_FULL  1 /**< @brief Semaphore full state */
#define OS_SEM_EMPTY 0 /**< @brief Semaphore empty state */
/**@}*/

/** @brief Floating point enabled state for a task */
#define OS_FP_ENABLED 1

/** @brief Error string name length
 *
 * The sizes of strings in OSAL functions are built with this limit in mind.
 * Always check the uses of os_err_name_t when changing this value.
 */
#define OS_ERROR_NAME_LENGTH 35

/**
 * @brief Type to be used for OSAL task priorities.
 *
 * OSAL priorities are in reverse order, and range
 * from 0 (highest; will preempt all other tasks) to
 * 255 (lowest; will not preempt any other task).
 */
typedef uint8_t osal_priority_t;

#define OSAL_PRIORITY_C(X) ((osal_priority_t) {X})

/**
 * @brief Type to be used for OSAL stack pointer.
 */
typedef void *osal_stackptr_t;

#define OSAL_STACKPTR_C(X)       ((osal_stackptr_t) {X})
#define OSAL_TASK_STACK_ALLOCATE OSAL_STACKPTR_C(NULL)

/*  Object property structures */

/** @brief OSAL task properties */
typedef struct
{
    char            name[OS_MAX_API_NAME];
    osal_id_t       creator;
    size_t          stack_size;
    osal_priority_t priority;
} OS_task_prop_t;

/** @brief OSAL queue properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_queue_prop_t;

/** @brief OSAL binary semaphore properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
    int32     value;
} OS_bin_sem_prop_t;

/** @brief OSAL counting semaphore properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
    int32     value;
} OS_count_sem_prop_t;

/** @brief OSAL mutexe properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_mut_sem_prop_t;

/** @brief OSAL time */
typedef struct
{
    uint32 seconds;
    uint32 microsecs;
} OS_time_t;

/** @brief OSAL heap properties
 *
 * @sa OS_HeapGetInfo()
 */
typedef struct
{
    size_t            free_bytes;
    osal_blockcount_t free_blocks;
    size_t            largest_free_block;
} OS_heap_prop_t;

/**
 * @brief An abstract structure capable of holding several OSAL IDs
 *
 * This is part of the select API and is manipulated using the
 * related API calls.  It should not be modified directly by applications.
 *
 * @sa OS_SelectFdZero(), OS_SelectFdAdd(), OS_SelectFdClear(), OS_SelectFdIsSet()
 */
typedef struct
{
    uint8 object_ids[(OS_MAX_NUM_OPEN_FILES + 7) / 8];
} OS_FdSet;

/**
 * @brief For the OS_SelectSingle() function's in/out StateFlags parameter,
 * the state(s) of the stream and the result of the select is a combination
 * of one or more of these states.
 *
 * @sa OS_SelectSingle()
 */
typedef enum
{
    OS_STREAM_STATE_BOUND     = 0x01, /**< @brief whether the stream is bound     */
    OS_STREAM_STATE_CONNECTED = 0x02, /**< @brief whether the stream is connected */
    OS_STREAM_STATE_READABLE  = 0x04, /**< @brief whether the stream is readable  */
    OS_STREAM_STATE_WRITABLE  = 0x08, /**< @brief whether the stream is writable  */
} OS_StreamState_t;

/**
 * @brief A set of events that can be used with event callback routines
 */
typedef enum
{
    OS_EVENT_RESERVED = 0, /**< no-op/reserved event id value */

    /**
     * resource/id has been newly allocated but not yet created.
     *
     * This event is invoked from WITHIN the locked region, in
     * the context of the task which is allocating the resource.
     *
     * If the handler returns non-success, the error will be returned
     * to the caller and the creation process is aborted.
     */
    OS_EVENT_RESOURCE_ALLOCATED,

    /**
     * resource/id has been fully created/finalized.
     *
     * Invoked outside locked region, in the context
     * of the task which created the resource.
     *
     * Data object is not used, passed as NULL.
     *
     * Return value is ignored - this is for information purposes only.
     */
    OS_EVENT_RESOURCE_CREATED,

    /**
     * resource/id has been deleted.
     *
     * Invoked outside locked region, in the context
     * of the task which deleted the resource.
     *
     * Data object is not used, passed as NULL.
     *
     * Return value is ignored - this is for information purposes only.
     */
    OS_EVENT_RESOURCE_DELETED,

    /**
     * New task is starting.
     *
     * Invoked outside locked region, in the context
     * of the task which is currently starting, before
     * the entry point is called.
     *
     * Data object is not used, passed as NULL.
     *
     * If the handler returns non-success, task startup is aborted
     * and the entry point is not called.
     */
    OS_EVENT_TASK_STARTUP,

    OS_EVENT_MAX /**< placeholder for end of enum, not used */
} OS_Event_t;

/**
 * @brief A callback routine for event handling.
 *
 * @param[in]    event      The event that occurred
 * @param[in]    object_id  The associated object_id, or 0 if not associated with an object
 * @param[inout] data       An abstract data/context object associated with the event, or NULL.
 * @return status Execution status, see @ref OSReturnCodes.
 */
typedef int32 (*OS_EventHandler_t)(OS_Event_t event, osal_id_t object_id, void *data);

/**
 * @brief For the @ref OS_GetErrorName() function, to ensure
 * everyone is making an array of the same length.
 *
 * Implementation note for developers:
 *
 * The sizes of strings in OSAL functions are built with this
 * #OS_ERROR_NAME_LENGTH limit in mind.  Always check the uses of os_err_name_t
 * when changing this value.
 */
typedef char os_err_name_t[OS_ERROR_NAME_LENGTH];

/*
** These typedefs are for the task entry point
*/
typedef void osal_task;                      /**< @brief For task entry point */
typedef osal_task((*osal_task_entry)(void)); /**< @brief For task entry point */

/**
 * @brief General purpose OSAL callback function
 *
 * This may be used by multiple APIS
 */
typedef void (*OS_ArgCallback_t)(osal_id_t object_id, void *arg);

/** @defgroup OSAPICore OSAL Core Operation APIs
 *
 * These are for OSAL core operations for startup/initialization, running, and shutdown.
 * Typically only used in bsps, unit tests, psps, etc.
 *
 * Not intended for user application use
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Application startup
 *
 * Startup abstraction such that the same BSP can be used for operations and testing.
 */
void OS_Application_Startup(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Application run
 *
 * Run abstraction such that the same BSP can be used for operations and testing.
 */
void OS_Application_Run(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initialization of API
 *
 * This function returns initializes the internal data structures of the OS
 * Abstraction Layer. It must be called in the application startup code before
 * calling any other OS routines.
 *
 * @return Execution status, see @ref OSReturnCodes. Any error code (negative)
 *         means the OSAL can not be initialized.  Typical platform specific response
 *         is to abort since additional OSAL calls will have undefined behavior.
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR   @copybrief OS_ERROR
 */
int32 OS_API_Init(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Background thread implementation - waits forever for events to occur.
 *
 * This should be called from the BSP main routine or initial thread after all other
 * board and application initialization has taken place and all other tasks are running.
 *
 * Typically just waits forever until "OS_shutdown" flag becomes true.
 */
void OS_IdleLoop(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief delete all resources created in OSAL.
 *
 * provides a means to clean up all resources allocated by this
 * instance of OSAL.  It would typically be used during an orderly
 * shutdown but may also be helpful for testing purposes.
 */
void OS_DeleteAllObjects(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initiate orderly shutdown
 *
 * Indicates that the OSAL application should perform an orderly shutdown
 * of ALL tasks, clean up all resources, and exit the application.
 *
 * This allows the task currently blocked in OS_IdleLoop() to wake up, and
 * for that function to return to its caller.
 *
 * This is preferred over e.g. OS_ApplicationExit() which exits immediately and
 * does not provide for any means to clean up first.
 *
 * @param[in] flag set to true to initiate shutdown, false to cancel
 */
void OS_ApplicationShutdown(uint8 flag);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exit/Abort the application
 *
 * Indicates that the OSAL application should exit and return control to the OS
 * This is intended for e.g. scripted unit testing where the test needs to end
 * without user intervention.
 *
 * This function does not return.  Production code typically should not ever call this.
 *
 * @note This exits the entire process including tasks that have been created.
 */
void OS_ApplicationExit(int32 Status);
/**@}*/

/** @defgroup OSAPIObjUtil OSAL Object Utility APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain an integer value corresponding to an object ID
 *
 * Obtains an integer representation of an object id, generally
 * for the purpose of printing to the console or system logs.
 *
 * The returned value is of the type "unsigned long" for direct use with
 * printf-style functions. It is recommended to use the "%lx" conversion
 * specifier as the hexidecimal encoding clearly delineates the internal fields.
 *
 * @note This provides the raw integer value and is _not_ suitable for use
 * as an array index, as the result is not zero-based.  See the
 * OS_ConvertToArrayIndex() to obtain a zero-based index value.
 *
 * @param[in]   object_id The object ID
 * @returns     integer value representation of object ID
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline unsigned long OS_ObjectIdToInteger(osal_id_t object_id)
{
    return object_id;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain an osal ID corresponding to an integer value
 *
 * Provides the inverse of OS_ObjectIdToInteger().  Reconstitutes the original
 * osal_id_t type from an integer representation.
 *
 * @param[in]   value   The integer representation of an OSAL ID
 * @returns     The ID value converted to an osal_id_t
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline osal_id_t OS_ObjectIdFromInteger(unsigned long value)
{
    return value;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check two OSAL object ID values for equality
 *
 * The OSAL ID values should be treated as abstract values by applications, and not
 * directly manipulated using standard C operators.
 *
 * This checks two values for equality, replacing the "==" operator.
 *
 * @param[in]   object_id1 The first object ID
 * @param[in]   object_id2 The second object ID
 * @returns     true if the object IDs are equal
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline bool OS_ObjectIdEqual(osal_id_t object_id1, osal_id_t object_id2)
{
    return (object_id1 == object_id2);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an object ID is defined.
 *
 * The OSAL ID values should be treated as abstract values by applications, and not
 * directly manipulated using standard C operators.
 *
 * This returns false if the ID is NOT a defined resource (i.e. free/empty/invalid).
 *
 * @note OS_ObjectIdDefined(OS_OBJECT_ID_UNDEFINED) is always guaranteed to be false.
 *
 * @param[in]   object_id The first object ID
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline bool OS_ObjectIdDefined(osal_id_t object_id)
{
    return (object_id != 0);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the name of an object given an arbitrary object ID
 *
 * All OSAL resources generally have a name associated with them.  This
 * allows application code to retrieve the name of any valid OSAL object ID.
 *
 * @param[in]  object_id The object ID to operate on
 * @param[out] buffer Buffer in which to store the name
 * @param[in]  buffer_size Size of the output storage buffer
 *
 * @returns #OS_SUCCESS if successful
 *          #OS_ERR_INVALID_ID if the passed-in ID is not a valid OSAL ID
 *          #OS_INVALID_POINTER if the passed-in buffer is invalid
 *          #OS_ERR_NAME_TOO_LONG if the name will not fit in the buffer provided
 */
int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the type of an object given an arbitrary object ID
 *
 * Given an arbitrary object ID, get the type of the object
 *
 * @param[in] object_id The object ID to operate on
 *
 * @return The object type portion of the object_id, see @ref OSObjectTypes for
 *         expected values
 */
osal_objtype_t OS_IdentifyObject(osal_id_t object_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Converts an abstract ID into a number suitable for use as an array index.
 *
 * This will return a unique zero-based integer number in the range of [0,MAX) for
 * any valid object ID.  This may be used by application code as an array index
 * for indexing into local tables.
 *
 * @note This does NOT verify the validity of the ID, that is left to the caller.
 * This is only the conversion logic.
 *
 * This routine accepts any object type, and returns a value based on the
 * maximum number of objects for that type.  This is equivalent to invoking
 * OS_ObjectIdToArrayIndex() with the idtype set to OS_OBJECT_TYPE_UNDEFINED.
 *
 * @sa OS_ObjectIdToArrayIndex
 *
 * @param[in]  object_id    The object ID to operate on
 * @param[out] *ArrayIndex  The Index to return
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS                 @copybrief OS_SUCCESS
 * @retval #OS_ERR_INCORRECT_OBJ_TYPE  @copybrief OS_ERR_INCORRECT_OBJ_TYPE
 */
int32 OS_ConvertToArrayIndex(osal_id_t object_id, osal_index_t *ArrayIndex);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Converts an abstract ID into a number suitable for use as an array index.
 *
 * This will return a unique zero-based integer number in the range of [0,MAX) for
 * any valid object ID.  This may be used by application code as an array index
 * for indexing into local tables.
 *
 * This routine operates on a specific object type, and returns a value based on the
 * maximum number of objects for that type.
 *
 * If the idtype is passed as #OS_OBJECT_TYPE_UNDEFINED, then object type verification
 * is skipped and any object ID will be accepted and converted to an index.  In this
 * mode, the range of the output depends on the actual passed-in object type.
 *
 * If the idtype is passed as any other value, the passed-in ID value is first
 * confirmed to be the correct type.  This check will guarantee that the output
 * is within an expected range; for instance, if the type is passed as
 * #OS_OBJECT_TYPE_OS_TASK, then the output index is guaranteed to be between 0 and
 * #OS_MAX_TASKS-1 after successful conversion.
 *
 * @param[in]  idtype       The object type to convert
 * @param[in]  object_id    The object ID to operate on
 * @param[out] *ArrayIndex  The Index to return
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS                 @copybrief OS_SUCCESS
 * @retval #OS_ERR_INCORRECT_OBJ_TYPE  @copybrief OS_ERR_INCORRECT_OBJ_TYPE
 * */
int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t object_id, osal_index_t *ArrayIndex);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief call the supplied callback function for all valid object IDs
 *
 * Loops through all defined OSAL objects of all types and calls callback_ptr on each one
 * If creator_id is nonzero then only objects with matching creator id are processed.
 *
 * @param[in]  creator_id   Filter objects to those created by a specific task
 *                          This may be passed as OS_OBJECT_CREATOR_ANY to return all objects
 * @param[in]  callback_ptr Function to invoke for each matching object ID
 * @param[in]  callback_arg Opaque Argument to pass to callback function
 */
void OS_ForEachObject(osal_id_t creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief call the supplied callback function for valid object IDs of a specific type
 *
 * Loops through all defined OSAL objects of a specific type and calls callback_ptr on each one
 * If creator_id is nonzero then only objects with matching creator id are processed.
 *
 * @param[in]  objtype      The type of objects to iterate
 * @param[in]  creator_id   Filter objects to those created by a specific task
 *                          This may be passed as OS_OBJECT_CREATOR_ANY to return all objects
 * @param[in]  callback_ptr Function to invoke for each matching object ID
 * @param[in]  callback_arg Opaque Argument to pass to callback function
 */
void OS_ForEachObjectOfType(osal_objtype_t objtype, osal_id_t creator_id, OS_ArgCallback_t callback_ptr,
                            void *callback_arg);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Callback routine registration
 *
 * This hook enables the application code to perform extra platform-specific
 * operations on various system events such as resource creation/deletion.
 *
 * @note Some events are invoked while the resource is "locked" and therefore
 * application-defined handlers for these events should not block or attempt
 * to access other OSAL resources.
 *
 * @param[in] handler The application-provided event handler
 * @return Execution status, see @ref OSReturnCodes.
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR   @copybrief OS_ERROR
 */
int32 OS_RegisterEventHandler(OS_EventHandler_t handler);

/**@}*/

/** @defgroup OSAPITask OSAL Task APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a task and starts running it.
 *
 * Creates a task and passes back the id of the task created. Task names must be unique;
 * if the name already exists this function fails. Names cannot be NULL.
 *
 * @param[out]  task_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   task_name the name of the new resource to create
 * @param[in]   function_pointer the entry point of the new task
 * @param[in]   stack_pointer pointer to the stack for the task, or NULL
 *              to allocate a stack from the system memory heap
 * @param[in]   stack_size the size of the stack, or 0 to use a default stack size.
 * @param[in]   priority initial priority of the new task
 * @param[in]   flags initial options for the new task
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if any of the necessary pointers are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_INVALID_PRIORITY if the priority is bad
 * @retval #OS_ERR_NO_FREE_IDS if there can be no more tasks created
 * @retval #OS_ERR_NAME_TAKEN if the name specified is already used by a task
 * @retval #OS_ERROR if an unspecified/other error occurs
 */
int32 OS_TaskCreate(osal_id_t *task_id, const char *task_name, osal_task_entry function_pointer,
                    osal_stackptr_t stack_pointer, size_t stack_size, osal_priority_t priority, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Task
 *
 * The task will be removed from the local tables. and the OS will
 * be configured to stop executing the task at the next opportunity.
 *
 * @param[in] task_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID given to it is invalid
 * @retval #OS_ERROR if the OS delete call fails
 */
int32 OS_TaskDelete(osal_id_t task_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exits the calling task
 *
 * The calling thread is terminated.  This function does not return.
 */
void OS_TaskExit(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Installs a handler for when the task is deleted.
 *
 * This function is used to install a callback that is called when the task is deleted.
 * The callback is called when OS_TaskDelete is called with the task ID. A task delete
 * handler is useful for cleaning up resources that a task creates, before the task is
 * removed from the system.
 *
 * @param[in]   function_pointer function to be called when task exits
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Delay a task for specified amount of milliseconds
 *
 * Causes the current thread to be suspended from execution for the period of millisecond.
 *
 * @param[in]   millisecond    Amount of time to delay
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if sleep fails or millisecond = 0
 */
int32 OS_TaskDelay(uint32 millisecond);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the given task to a new priority
 *
 * @param[in] task_id        The object ID to operate on
 *
 * @param[in] new_priority   Set the new priority
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID passed to it is invalid
 * @retval #OS_ERR_INVALID_PRIORITY if the priority is greater than the max allowed
 * @retval #OS_ERROR if the OS call to change the priority fails
 */
int32 OS_TaskSetPriority(osal_id_t task_id, osal_priority_t new_priority);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obsolete
 * @deprecated Explicit registration call no longer needed
 *
 * Obsolete function retained for compatibility purposes.
 * Does Nothing in the current implementation.
 *
 * @return #OS_SUCCESS (always), see @ref OSReturnCodes
 */
int32 OS_TaskRegister(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the task id of the calling task
 *
 * This function returns the task id of the calling task
 *
 * @return Task ID, or zero if the operation failed (zero is never a valid task ID)
 */
osal_id_t OS_TaskGetId(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing task ID by name
 *
 * This function tries to find a task Id given the name of a task
 *
 * @param[out]  task_id will be set to the ID of the existing resource
 * @param[in]   task_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if the pointers passed in are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name wasn't found in the table
 */
int32 OS_TaskGetIdByName(osal_id_t *task_id, const char *task_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (creator, stack size, priority, name) about the
 * specified task.
 *
 * @param[in]   task_id The object ID to operate on
 * @param[out]  task_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID passed to it is invalid
 * @retval #OS_INVALID_POINTER if the task_prop pointer is NULL
 */
int32 OS_TaskGetInfo(osal_id_t task_id, OS_task_prop_t *task_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Reverse-lookup the OSAL task ID from an operating system ID
 *
 * This provides a method by which an external entity may find the OSAL task
 * ID corresponding to a system-defined identifier (e.g. TASK_ID, pthread_t, rtems_id, etc).
 *
 * Normally OSAL does not expose the underlying OS-specific values to the application,
 * but in some circumstances, such as exception handling, the OS may provide this information
 * directly to handler outside of the normal OSAL API.
 *
 * @param[out]  task_id         The buffer where the task id output is stored
 * @param[in]   sysdata         Pointer to the system-provided identification data
 * @param[in]   sysdata_size    Size of the system-provided identification data
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 */
int32 OS_TaskFindIdBySystemData(osal_id_t *task_id, const void *sysdata, size_t sysdata_size);

/**@}*/

/** @defgroup OSAPIMsgQueue OSAL Message Queue APIs
 * @{
 */

/**
 * @brief Create a message queue
 *
 * This is the function used to create a queue in the operating system.
 * Depending on the underlying operating system, the memory for the queue
 * will be allocated automatically or allocated by the code that sets up
 * the queue. Queue names must be unique; if the name already exists this
 * function fails. Names cannot be NULL.
 *
 *
 * @param[out]  queue_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   queue_name the name of the new resource to create
 * @param[in]   queue_depth the maximum depth of the queue
 * @param[in]   data_size the size of each entry in the queue
 * @param[in]   flags options for the queue (reserved for future use, pass as 0)
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if a pointer passed in is NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are already the max queues created
 * @retval #OS_ERR_NAME_TAKEN if the name is already being used on another queue
 * @retval #OS_QUEUE_INVALID_SIZE if the queue depth exceeds the limit
 * @retval #OS_ERROR if the OS create call fails
 */
int32 OS_QueueCreate(osal_id_t *queue_id, const char *queue_name, osal_blockcount_t queue_depth, size_t data_size,
                     uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified message queue.
 *
 * This is the function used to delete a queue in the operating system.
 * This also frees the respective queue_id to be used again when another queue is created.
 *
 * @note If There are messages on the queue, they will be lost and any subsequent
 * calls to QueueGet or QueuePut to this queue will result in errors
 *
 * @param[in] queue_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in does not exist
 * @retval #OS_ERROR if the OS call to delete the queue fails
 */
int32 OS_QueueDelete(osal_id_t queue_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Receive a message on a message queue
 *
 * If a message is pending, it is returned immediately.  Otherwise the calling task
 * will block until a message arrives or the timeout expires.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  data The buffer to store the received message
 * @param[in]   size The size of the data buffer
 * @param[out]  size_copied Set to the actual size of the message
 * @param[in]   timeout The maximum amount of time to block, or OS_PEND to wait forever
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the given ID does not exist
 * @retval #OS_INVALID_POINTER if a pointer passed in is NULL
 * @retval #OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
 * @retval #OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
 * @retval #OS_QUEUE_INVALID_SIZE if the size copied from the queue was not correct
 */
int32 OS_QueueGet(osal_id_t queue_id, void *data, size_t size, size_t *size_copied, int32 timeout);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Put a message on a message queue.
 *
 * @param[in]  queue_id The object ID to operate on
 * @param[in]  data The buffer containing the message to put
 * @param[in]  size The size of the data buffer
 * @param[in]  flags Currently reserved/unused, should be passed as 0
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the queue id passed in is not a valid queue
 * @retval #OS_INVALID_POINTER if the data pointer is NULL
 * @retval #OS_QUEUE_FULL if the queue cannot accept another message
 * @retval #OS_ERROR if the OS call returns an error
 */
int32 OS_QueuePut(osal_id_t queue_id, const void *data, size_t size, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing queue ID by name
 *
 * This function tries to find a queue Id given the name of the queue. The
 * id of the queue is passed back in queue_id.
 *
 * @param[out]  queue_id will be set to the ID of the existing resource
 * @param[in]   queue_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if the name or id pointers are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND the name was not found in the table
 */
int32 OS_QueueGetIdByName(osal_id_t *queue_id, const char *queue_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (name and creator) about the specified queue.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  queue_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if queue_prop is NULL
 * @retval #OS_ERR_INVALID_ID if the ID given is not  a valid queue
 */
int32 OS_QueueGetInfo(osal_id_t queue_id, OS_queue_prop_t *queue_prop);
/**@}*/

/** @defgroup OSAPISem OSAL Semaphore APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a binary semaphore
 *
 * Creates a binary semaphore with initial value specified by
 * sem_initial_value and name specified by sem_name. sem_id will be
 * returned to the caller
 *
 * @param[out]  sem_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   sem_initial_value the initial value of the binary semaphore
 * @param[in]   options Reserved for future use, should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sen name or sem_id are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
 * @retval #OS_ERR_NAME_TAKEN if this is already the name of a binary semaphore
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_BinSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unblock all tasks pending on the specified semaphore
 *
 * The function unblocks all tasks pending on the specified semaphore. However,
 * this function does not change the state of the semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
 * @retval #OS_SEM_FAILURE if an unspecified failure occurs
 */
int32 OS_BinSemFlush(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Increment the semaphore value
 *
 * The function  unlocks the semaphore referenced by sem_id by performing
 * a semaphore unlock operation on that semaphore.  If the semaphore value
 * resulting from this operation is positive, then no threads were blocked
 * waiting for the semaphore to become unlocked; the semaphore value is
 * simply incremented for this semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
 */
int32 OS_BinSemGive(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value
 *
 * The locks the semaphore referenced by sem_id by performing a
 * semaphore lock operation on that semaphore.  If the semaphore value
 * is currently zero, then the calling thread shall not return from
 * the call until it either locks the semaphore or the call is
 * interrupted.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the Id passed in is not a valid binary semaphore
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_BinSemTake(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value with a timeout
 *
 * The function locks the semaphore referenced by sem_id. However,
 * if the semaphore cannot be locked without waiting for another process
 * or thread to unlock the semaphore, this wait shall be terminated when
 * the specified timeout, msecs, expires.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[in] msecs  The maximum amount of time to block, in milliseconds
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_TIMEOUT if semaphore was not relinquished in time
 * @retval #OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
 */
int32 OS_BinSemTimedWait(osal_id_t sem_id, uint32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Binary Semaphore
 *
 * This is the function used to delete a binary semaphore in the operating system.
 * This also frees the respective sem_id to be used again when another semaphore is created.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
 * @retval #OS_SEM_FAILURE the OS call failed
 */
int32 OS_BinSemDelete(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing semaphore ID by name
 *
 * This function tries to find a binary sem Id given the name of a bin_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_BinSemGetIdByName(osal_id_t *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified binary
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  bin_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the bin_prop pointer is null
 */
int32 OS_BinSemGetInfo(osal_id_t sem_id, OS_bin_sem_prop_t *bin_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a counting semaphore
 *
 * Creates a counting semaphore with initial value specified by
 * sem_initial_value and name specified by sem_name. sem_id will be
 * returned to the caller
 *
 * @param[out]  sem_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   sem_initial_value the initial value of the counting semaphore
 * @param[in]   options Reserved for future use, should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sen name or sem_id are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
 * @retval #OS_ERR_NAME_TAKEN if this is already the name of a counting semaphore
 * @retval #OS_SEM_FAILURE if the OS call failed
 * @retval #OS_INVALID_SEM_VALUE if the semaphore value is too high
 */
int32 OS_CountSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Increment the semaphore value
 *
 * The function  unlocks the semaphore referenced by sem_id by performing
 * a semaphore unlock operation on that semaphore.  If the semaphore value
 * resulting from this operation is positive, then no threads were blocked
 * waiting for the semaphore to become unlocked; the semaphore value is
 * simply incremented for this semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
 * in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore
 */
int32 OS_CountSemGive(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value
 *
 * The locks the semaphore referenced by sem_id by performing a
 * semaphore lock operation on that semaphore.  If the semaphore value
 * is currently zero, then the calling thread shall not return from
 * the call until it either locks the semaphore or the call is
 * interrupted.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the Id passed in is not a valid counting semaphore
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_CountSemTake(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value with timeout
 *
 * The function locks the semaphore referenced by sem_id. However,
 * if the semaphore cannot be locked without waiting for another process
 * or thread to unlock the semaphore, this wait shall be terminated when
 * the specified timeout, msecs, expires.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[in] msecs  The maximum amount of time to block, in milliseconds
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_TIMEOUT if semaphore was not relinquished in time
 * @retval #OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
 */
int32 OS_CountSemTimedWait(osal_id_t sem_id, uint32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified counting Semaphore.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore
 * @retval #OS_SEM_FAILURE the OS call failed
 */
int32 OS_CountSemDelete(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing semaphore ID by name
 *
 * This function tries to find a counting sem Id given the name of a count_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_CountSemGetIdByName(osal_id_t *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified counting
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  count_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the count_prop pointer is null
 */
int32 OS_CountSemGetInfo(osal_id_t sem_id, OS_count_sem_prop_t *count_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a mutex semaphore
 *
 * Mutex semaphores are always created in the unlocked (full) state.
 *
 * @param[out]  sem_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   options reserved for future use.  Should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sem_id or sem_name are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are no more free mutex Ids
 * @retval #OS_ERR_NAME_TAKEN if there is already a mutex with the same name
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Releases the mutex object referenced by sem_id.
 *
 * If there are threads blocked on the mutex object referenced by
 * mutex when this function is called, resulting in the mutex becoming
 * available, the scheduling policy shall determine which thread shall
 * acquire the mutex.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * @retval #OS_SEM_FAILURE if an unspecified error occurs
 */
int32 OS_MutSemGive(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Acquire the mutex object referenced by sem_id.
 *
 * If the mutex is already locked, the calling thread shall
 * block until the mutex becomes available. This operation shall
 * return with the mutex object referenced by mutex in the locked state
 * with the calling thread as its owner.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_FAILURE if the semaphore was not previously initialized or is
 * not in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid mutex
 */
int32 OS_MutSemTake(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Mutex Semaphore.
 *
 * Delete the semaphore.  This also frees the respective sem_id such that it can be
 * used again when another is created.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemDelete(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing mutex ID by name
 *
 * This function tries to find a mutex sem Id given the name of a mut_sem.
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_MutSemGetIdByName(osal_id_t *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified mutex
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  mut_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the mut_prop pointer is null
 */
int32 OS_MutSemGetInfo(osal_id_t sem_id, OS_mut_sem_prop_t *mut_prop);
/**@}*/

/** @defgroup OSAPITime OSAL Time APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the local time
 *
 * This function gets the local time from the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Service
 *
 * @param[out]  time_struct An OS_time_t that will be set to the current time
 *
 * @return Get local time status, see @ref OSReturnCodes
 */
int32 OS_GetLocalTime(OS_time_t *time_struct);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the local time
 *
 * This function sets the local time on the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Services
 *
 * @param[in]  time_struct An OS_time_t containing the current time
 *
 * @return Set local time status, see @ref OSReturnCodes
 */
int32 OS_SetLocalTime(OS_time_t *time_struct);
/**@}*/

/** @defgroup OSAPIHeap OSAL Heap APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Return current info on the heap
 *
 * @param[out] heap_prop Storage buffer for heap info
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop);
/**@}*/

/** @defgroup OSAPIError OSAL Error Info APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Convert an error number to a string
 *
 * @param[in] error_num Error number to convert
 * @param[out] err_name Buffer to store error string
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_GetErrorName(int32 error_num, os_err_name_t *err_name);
/**@}*/

/** @defgroup OSAPISelect OSAL Select APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events across multiple file handles
 *
 * Wait for any of the given sets of IDs to be become readable or writable
 *
 * This function will block until any of the following occurs:
 *  - At least one OSAL ID in the ReadSet is readable
 *  - At least one OSAL ID in the WriteSet is writable
 *  - The timeout has elapsed
 *
 * The sets are input/output parameters.  On entry, these indicate the
 * file handle(s) to wait for.  On exit, these are set to the actual
 * file handle(s) that have activity.
 *
 * If the timeout occurs this returns an error code and all output sets
 * should be empty.
 *
 * @note This does not lock or otherwise protect the file handles in the
 * given sets.  If a filehandle supplied via one of the FdSet arguments
 * is closed or modified by another while this function is in progress,
 * the results are undefined.  Because of this limitation, it is recommended
 * to use OS_SelectSingle() whenever possible.
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events on a single file handle
 *
 * Wait for a single OSAL filehandle to change state
 *
 * This function can be used to wait for a single OSAL stream ID
 * to become readable or writable.   On entry, the "StateFlags"
 * parameter should be set to the desired state (OS_STREAM_STATE_READABLE
 * and/or OS_STREAM_STATE_WRITABLE) and upon return the flags
 * will be set to the state actually detected.
 *
 * As this operates on a single ID, the filehandle is protected
 * during this call, such that another thread accessing the same
 * handle will return an error.  However, it is important to note that
 * once the call returns then other threads may then also read/write
 * and affect the state before the current thread can service it.
 *
 * To mitigate this risk the application may prefer to use
 * the OS_TimedRead/OS_TimedWrite calls.
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_SelectSingle(osal_id_t objid, uint32 *StateFlags, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear a FdSet structure
 *
 * After this call the set will contain no OSAL IDs
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_SelectFdZero(OS_FdSet *Set);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Add an ID to an FdSet structure
 *
 * After this call the set will contain the given OSAL ID
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_SelectFdAdd(OS_FdSet *Set, osal_id_t objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear an ID from an FdSet structure
 *
 * After this call the set will no longer contain the given OSAL ID
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_SelectFdClear(OS_FdSet *Set, osal_id_t objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an FdSet structure contains a given ID
 *
 * @return Boolean set status
 * @retval true  FdSet structure contains ID
 * @retval false FDSet structure does not contain ID
 */
bool OS_SelectFdIsSet(OS_FdSet *Set, osal_id_t objid);
/**@}*/

/** @defgroup OSAPIPrintf OSAL Printf APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Abstraction for the system printf() call
 *
 * This function abstracts out the printf type statements. This is
 * useful for using OS- specific thats that will allow non-polled
 * print statements for the real time systems.
 *
 * Operates in a manner similar to the printf() call defined by the standard C
 * library and takes all the parameters and formatting options of printf.
 * This abstraction may implement additional buffering, if necessary,
 * to improve the real-time performance of the call.
 *
 * Strings (including terminator) longer than #OS_BUFFER_SIZE will be truncated.
 *
 * The output of this routine also may be dynamically enabled or disabled by
 * the OS_printf_enable() and OS_printf_disable() calls, respectively.
 *
 * @param[in] string Format string, followed by additional arguments
 */
void OS_printf(const char *string, ...) OS_PRINTF(1, 2);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function disables the output from OS_printf.
 */
void OS_printf_disable(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function enables the output from OS_printf.
 *
 */
void OS_printf_enable(void);
/**@}*/

/****************************************************************************************
                    BSP LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   Function: OS_BSP_GetArgC

    Purpose: Obtain the number of boot arguments passed from the bootloader
             or shell if supported by the platform

    Returns: The number of boot arguments, or 0 if no arguments were passed
             or not supported by the BSP.
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetArgC(void);

/*----------------------------------------------------------------
   Function: OS_BSP_GetArgV

    Purpose: Obtain an array of boot argument strings passed from the bootloader
             or shell if supported by the platform

    Returns: Pointer to char* array containing the argument strings, or NULL if
             no arguments are available or not supported by the BSP.

             The array is sized according to OS_BSP_GetArgC()
 ------------------------------------------------------------------*/
char *const *OS_BSP_GetArgV(void);

/*----------------------------------------------------------------
   Function: OS_BSP_SetExitCode

    Purpose: Sets the status to be returned to the shell or bootloader
             if supported by the platform.  The value is an integer with
             platform and application-defined meaning, but BSP's should
             attempt to provide consistent meaning for the following values

             OS_SUCCESS: normal status (default)
             OS_ERROR: any abnormal status

             Other more specific status values may be passed, with
             implementation-defined behavior.  Depending on the system
             capabilities, the BSP implementation may either pass the
             value through as-is, translate it to defined value, or
             ignore it.

             Note this does NOT cause the application to exit, it only
             sets the state that will be returned if/when the application
             exits itself at a future time.

 ------------------------------------------------------------------*/
void OS_BSP_SetExitCode(int32 code);

#endif
