/**
 * @file ms5611_registers.h
 * @brief The main register header file for the MS5611 IMU
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _MS5611_REGISTERS_H_
#define _MS5611_REGISTERS_H_

//-------------------------------------------------------
//                  MS5611 Registers
//        Registers can be found in Datasheets.
//-------------------------------------------------------

#define MS5611_I2C_ADDR             0x77 /*! I2C address */
#define MS5611_ADC_READ             0x00 /*! Read ADC */
#define MS5611_RESET                0x1E /*! Reset command */
#define MS5611_CONV_D1              0x40 /*! conversion d1 */
#define MS5611_CONV_D2              0x50 /*! conversion d2 */
#define MS5611_WHO_AM_I             0x0F /*! Contains the device ID */
#define MS5611_READ_PROM            0xA2 /*! PROM for values */



#endif /* _ms5611_registers_h_ */

/************************/
/*  End of File Comment */
/************************/