/**
 * @file aimu_lis3mdl_registers.h
 * @brief The main register header file for the AIMU_LIS3MDL IMU
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _AIMU_LIS3MDL_REGISTERS_H_
#define _AIMU_LIS3MDL_REGISTERS_H_

//-------------------------------------------------------
//                  LIS3MDL Registers
//        Registers can be found in Datasheets.
//-------------------------------------------------------

#define AIMU_LIS3MDL_I2C_ADDR             0x1e /*! I2C address Read*/
#define AIMU_LIS3MDL_WHO_AM_I             0x0F /*! Device identification register */
#define AIMU_LIS3MDL_CTRL_REG1            0x20 /*! Control Register */
#define AIMU_LIS3MDL_CTRL_REG2            0x21 /*! Control Register */
#define AIMU_LIS3MDL_CTRL_REG3            0x22 /*! Control Register */
#define AIMU_LIS3MDL_CTRL_REG4            0x23 /*! Control Register */
#define AIMU_LIS3MDL_CTRL_REG5            0x24 /*! Control Register */
#define AIMU_LIS3MDL_STATUS_REG           0x27 /*! Status Register */
#define AIMU_LIS3MDL_OUT_X_L              0x28 /*! X-axis data output. The value of magnetic field is expressed as two’s complement */
#define AIMU_LIS3MDL_OUT_X_H              0x29 /*! X-axis data output. The value of magnetic field is expressed as two’s complement */
#define AIMU_LIS3MDL_OUT_Y_L              0x2A /*! Y-axis data output. The value of magnetic field is expressed as two’s complement. */
#define AIMU_LIS3MDL_OUT_Y_H              0x2B /*! Y-axis data output. The value of magnetic field is expressed as two’s complement. */
#define AIMU_LIS3MDL_OUT_Z_L              0x2C /*! Z-axis data output. The value of magnetic field is expressed as two’s complement. */
#define AIMU_LIS3MDL_OUT_Z_H              0x2D /*! Z-axis data output. The value of magnetic field is expressed as two’s complement. */
#define AIMU_LIS3MDL_TEMP_OUT_L           0x2E /*! Temperature sensor data. The value of temperature is expressed as two’s complement. */
#define AIMU_LIS3MDL_TEMP_OUT_H           0x2F /*! Temperature sensor data. The value of temperature is expressed as two’s complement. */
#define AIMU_LIS3MDL_INT_CFG              0x30 /*! Interrupt Configuration */
#define AIMU_LIS3MDL_INT_SRC              0x31 /*! Register holds positive threshold exceeded values for each axis */
#define AIMU_LIS3MDL_INT_THS_L            0x32 /*! Interrupt threshold */
#define AIMU_LIS3MDL_INT_THS_H            0x33 /*! Interrupt threshold */



#endif /* _aimu_lis3mdl_registers_h_ */

/************************/
/*  End of File Comment */
/************************/