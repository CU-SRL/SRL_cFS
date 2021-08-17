/**
 * @file aimu_lps25h_registers.h
 * @brief The main register header file for the AIMU_LPS25H IMU
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _AIMU_LPS25H_REGISTERS_H_
#define _AIMU_LPS25H_REGISTERS_H_

//-------------------------------------------------------
//                  LPS25H Registers
//        Registers can be found in Datasheets.
//-------------------------------------------------------

#define AIMU_LPS25H_I2C_ADDR             0x5D /*! I2C address */
#define AIMU_LPS25H_REF_P_XL             0x08 /*! The REF_P_XL register contains the lowest part of the reference pressure value that is sum to the sensor output pressure */
#define AIMU_LPS25H_REF_P_L              0x09 /*! The REF_P_L register contains the middle part of the reference pressure value that is sum to the sensor output pressure */
#define AIMU_LPS25H_REF_P_H              0x0A /*! The REF_P_H register contains the highest part of the reference pressure value that is sum to the sensor output pressure. */
#define AIMU_LPS25H_WHO_AM_I             0x0F /*! Contains the device ID */
#define AIMU_LPS25H_RES_CONF             0x10 /*! Pressure and Temperature internal average configuration. */
#define AIMU_LPS25H_CTL_REG1             0x20 /*! Control Register */
#define AIMU_LPS25H_CTL_REG2             0x21 /*! Control register. */
#define AIMU_LPS25H_CTL_REG3             0x22 /*! Control register. */
#define AIMU_LPS25H_CTL_REG4             0x23 /*! Control register. */
#define AIMU_LPS25H_INT_CFG              0x24 /*! Interrupt differential configuration register. */
#define AIMU_LPS25H_INT_SOURCE           0x25 /*! INT_SOURCE register is cleared by reading it */
#define AIMU_LPS25H_STATUS_REG           0x27 /*! Status register */
#define AIMU_LPS25H_PRESS_POUT_XL        0x28 /*! The PRESS_OUT_XL register contains the lowest part of the pressure output value */
#define AIMU_LPS25H_PRESS_OUT_L          0x29 /*! The PRESS_OUT_L register contains the middle part of the pressure output value. */
#define AIMU_LPS25H_PRESS_OUT_H          0x2A /*! The PRESS_OUT_H register contains the highest part of the pressure output value. */
#define AIMU_LPS25H_TEMP_OUT_L           0x2B /*! The TEMP_OUT_L register contains the low part of the temperature output value. */
#define AIMU_LPS25H_TEMP_OUT_H           0x2C /*! The TEMP_OUT_H register contains the high part of the temperature outputvalue. */
#define AIMU_LPS25H_FIFO_CTRL            0x2E /*! The FIFO_CTRL registers allows to control the FIFO functionality. */
#define AIMU_LPS25H_FIFO_STATUS          0x2F /*! FIFO_status */
#define AIMU_LPS25H_THS_P_L              0x30 /*! This register contains the low part of threshold value for pressure interrupt generation. */
#define AIMU_LPS25H_THS_P_H              0x31 /*! This register contains the high part of threshold value for pressure interrupt generation. */
#define AIMU_LPS25H_RPDS_L               0x39 /*! This register contains the low part of the pressure offset value after soldering,for differential pressure computing. */
#define AIMU_LPS25H_RPDS_H               0x3A /*! This register contains the high part of the pressure offset value after soldering */



#endif /* _aimu_lps25h_registers_h_ */

/************************/
/*  End of File Comment */
/************************/