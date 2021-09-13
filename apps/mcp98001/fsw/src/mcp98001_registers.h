/**
 * @file mcp98001_registers.h
 * @brief The main register header file for the MCP98001 Barometer
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _MCP98001_REGISTERS_H_
#define _MCP98001_REGISTERS_H_

//-------------------------------------------------------
//                  MCP98001 Registers
//-------------------------------------------------------

#define MCP98001_1_I2C_ADDR                 0x60 /*! I2C address */
#define MCP98001_2_I2C_ADDR                 0x60 /*! I2C address */
#define MCP98001_AMBIENT_TEMP               0x00 /*! 9 to 12 bit temperature output */
#define MCP98001_CONFIG                     0x01 /*! Configuration Register */
#define MCP98001_HYSTERESIS                 0x02 /*! Temperature lower limit register */
#define MCP98001_LIMITSET                   0x03 /*! Temeprature maximum limit register */

#endif
