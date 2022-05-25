/**
 * @file max7502_registers.h
 * @brief The main register header file for the MAX7502 Barometer
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _MAX7502_REGISTERS_H_
#define _MAX7502_REGISTERS_H_

//-------------------------------------------------------
//                  MAX7502 Registers
//-------------------------------------------------------

/* 01001000 */
#define MAX7502_1_I2C_ADDR                 0x48 /*! I2C address */
/* 01001010 */
#define MAX7502_2_I2C_ADDR                 0x4A /*! I2C address */
#define MAX7502_AMBIENT_TEMP               0x00 /*! 9 to 12 bit temperature output */
#define MAX7502_CONFIG                     0x01 /*! Configuration Register */
#define MAX7502_HYSTERESIS                 0x02 /*! Temperature lower limit register */
#define MAX7502_OS                         0x03 /*! Temeprature maximum limit register */

#endif
