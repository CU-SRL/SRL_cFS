/*******************************************************************************
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
** File: i2c_msg.h 
**
** Purpose: 
**  Define I2C App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _i2c_msg_h_
#define _i2c_msg_h_

/*
** I2C App command codes
*/
#define I2C_NOOP_CC                 0
#define I2C_RESET_COUNTERS_CC       1

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} I2C_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (I2C App housekeeping)
*/
typedef struct 
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              i2c_command_error_count;
    uint8              i2c_command_count;
    uint8              i2c_active_device_count;
    uint8              i2c_error_count;
    uint8              spare[2];

}   OS_PACK i2c_hk_tlm_t  ;

#define I2C_HK_TLM_LNGTH   sizeof ( i2c_hk_tlm_t )

#endif /* _i2c_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
