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
** File: ms5611_msg.h 
**
** Purpose: 
**  Define MS5611 App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _ms5611_msg_h_
#define _ms5611_msg_h_

/*
** MS5611 App command codes
*/
#define MS5611_NOOP_CC                 0
#define MS5611_RESET_COUNTERS_CC       1

/* Device Command Codes*/
#define MS5611_INIT          10 // Should also be able to be down automatically when flight ready
#define MS5611_SHUTDOWN      11
#define MS5611_RESET         12
#define MS5611_PROCESS       13 // Should also be able to be down automatically when flight ready

/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} ms5611_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (MS5611 App housekeeping)
*/
typedef struct 
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32              AppStatus;
    uint8               ms5611_command_error_count;
    uint8               ms5611_command_count;
    uint8               ms5611_error_count;
    uint8               ms5611_device_count;
    uint8               ms5611_device_error_count;

}   OS_PACK ms5611_hk_tlm_t  ;

typedef struct 
{
    uint8                TlmHeader[CFE_SB_TLM_HDR_SIZE];
    float               MS5611_PRESSURE;
    float               MS5611_TEMPERATURE;
    float               MS5611_ALTITUDE;

}   OS_PACK ms5611_data_tlm_t  ;

#define MS5611_HK_TLM_LNGTH   sizeof ( ms5611_hk_tlm_t )
#define MS5611_DATA_TLM_LNGTH   sizeof ( ms5611_data_tlm_t )

#endif /* _ms5611_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
