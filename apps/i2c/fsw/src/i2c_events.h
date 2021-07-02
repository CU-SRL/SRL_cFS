/************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.7"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: i2c_events.h 
**
** Purpose: 
**  Define I2C App Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _i2c_events_h_
#define _i2c_events_h_


#define I2C_RESERVED_EID              0
#define I2C_STARTUP_INF_EID           1 
#define I2C_COMMAND_ERR_EID           2
#define I2C_COMMANDNOP_INF_EID        3 
#define I2C_COMMANDRST_INF_EID        4
#define I2C_INVALID_MSGID_ERR_EID     5 
#define I2C_LEN_ERR_EID               6 

#endif /* _i2c_events_h_ */

/************************/
/*  End of File Comment */
/************************/
