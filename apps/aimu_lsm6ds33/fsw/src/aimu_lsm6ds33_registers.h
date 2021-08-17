/**
 * @file aimu_lsm6ds33_registers.h
 * @brief The main register header file for the AIMU_LSM6DS33 IMU
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _AIMU_LSM6DS33_REGISTERS_H_
#define _AIMU_LSM6DS33_REGISTERS_H_

//-------------------------------------------------------
//                  LSM6DS33 Registers
//        Registers can be found in Datasheets.
//-------------------------------------------------------

#define AIMU_LSM6DS33_I2C_ADDR             0x6A /*! I2C address */
#define AIMU_LSM6DS33_FUNC_CFG_ACCESS      0x01 /*! Enable embedded functions register */
#define AIMU_LSM6DS33_FIFO_CTRL1           0x06 /*! FIFO control register */
#define AIMU_LSM6DS33_FIFO_CTRL2           0x07 /*! FIFO control register */
#define AIMU_LSM6DS33_FIFO_CTRL3           0x08 /*! FIFO control register */
#define AIMU_LSM6DS33_FIFO_CTRL4           0x09 /*! FIFO control register */
#define AIMU_LSM6DS33_FIFO_CTRL5           0x0A /*! FIFO control register */
#define AIMU_LSM6DS33_ORIENT_CFG_G         0x0B /*! Angular rate sensor sign and orientation register */
#define AIMU_LSM6DS33_INT1_CTRL            0x0D /*! INT1 pad control register */
#define AIMU_LSM6DS33_INT2_CTRL            0x0E /*! INT1 pad control register */
#define AIMU_LSM6DS33_WHO_AM_I             0x0F /*! Who I Am ID */
#define AIMU_LSM6DS33_CTRL1_XL             0x10 /*! Linear acceleration sensor control register 1 */
#define AIMU_LSM6DS33_CTRL2_G              0x11 /*! Angular rate sensor control register 2 */
#define AIMU_LSM6DS33_CTRL3_C              0x12 /*! Control Register 3 */
#define AIMU_LSM6DS33_CTRL4_C              0x13 /*! Control Register 3 */
#define AIMU_LSM6DS33_CTRL5_C              0x14 /*! Control Register 3 */
#define AIMU_LSM6DS33_CTRL6_C              0x15 /*! Angular rate sensor control register 6 */
#define AIMU_LSM6DS33_CTRL7_G              0x16 /*! Angular rate sensor control register 7 */
#define AIMU_LSM6DS33_CTRL8_XL             0x17 /*! Linear acceleration sensor control register 8 */
#define AIMU_LSM6DS33_CTRL9_XL             0x18 /*! Linear acceleration sensor control register 9 */
#define AIMU_LSM6DS33_CTRL9_C              0x19 /*! Control register 10 */
#define AIMU_LSM6DS33_WAKE_UP_SRC          0x1B /*! Wake up interrupt source register */
#define AIMU_LSM6DS33_TAP_SRC              0x1C /*! Tap source register */
#define AIMU_LSM6DS33_D6D_SRC              0x1D /*! Portrait, landscape, face-up and face-down source register */
#define AIMU_LSM6DS33_STATUS_REG           0x1E /*! Status Data Register */
#define AIMU_LSM6DS33_OUT_TEMP_L           0x20 /*! Temperature data output register */
#define AIMU_LSM6DS33_OUT_TEMP_R           0x21 /*! Temperature data output register */
#define AIMU_LSM6DS33_OUTX_L_G             0x22 /*! Angular rate sensor pitch axis (X) angular rate output register */
#define AIMU_LSM6DS33_OUTX_H_G             0x23 /*! Angular rate sensor pitch axis (X) angular rate output register */
#define AIMU_LSM6DS33_OUTY_L_G             0x24 /*! Angular rate sensor roll axis (Y) angular rate output register */
#define AIMU_LSM6DS33_OUTY_H_G             0x25 /*! Angular rate sensor roll axis (Y) angular rate output register */
#define AIMU_LSM6DS33_OUTZ_L_G             0x26 /*! Angular rate sensor yaw axis (Z) angular rate output register */
#define AIMU_LSM6DS33_OUTZ_H_G             0x27 /*! Angular rate sensor yaw axis (Z) angular rate output register */
#define AIMU_LSM6DS33_OUTX_L_XL            0x28 /*! Linear acceleration sensor X-axis output register */
#define AIMU_LSM6DS33_OUTX_H_XL            0x29 /*! Linear acceleration sensor X-axis output register */
#define AIMU_LSM6DS33_OUTY_L_XL            0x2A /*! Linear acceleration sensor Y-axis output register */
#define AIMU_LSM6DS33_OUTY_H_XL            0x2B /*! Linear acceleration sensor Y-axis output register */
#define AIMU_LSM6DS33_OUTZ_L_XL            0x2C /*! Linear acceleration sensor Z-axis output register */
#define AIMU_LSM6DS33_OUTZ_H_XL            0x2D /*! Linear acceleration sensor Z-axis output register */
#define AIMU_LSM6DS33_FIFO_STATUS1         0x3A /*! FIFO status control register */
#define AIMU_LSM6DS33_FIFO_STATUS2         0x3B /*! FIFO status control register */
#define AIMU_LSM6DS33_FIFO_STATUS3         0x3C /*! FIFO status control register */
#define AIMU_LSM6DS33_FIFO_STATUS4         0x3D /*! FIFO status control register */
#define AIMU_LSM6DS33_FIFO_DATA_OUT_L      0x3E /*! FIFO data output register */
#define AIMU_LSM6DS33_FIFO_DATA_OUT_H      0x3F /*! FIFO data output register */
#define AIMU_LSM6DS33_TIMESTAMP0_REG       0x40 /*! Time stamp first byte data output register */
#define AIMU_LSM6DS33_TIMESTAMP1_REG       0x41 /*! Time stamp second byte data output register */
#define AIMU_LSM6DS33_TIMESTAMP2_REG       0x42 /*! Time stamp third byte data output register */
#define AIMU_LSM6DS33_STEP_TIMESTAMP_L     0x49 /*! Step counter timestamp information register */
#define AIMU_LSM6DS33_STEP_TIMESTAMP_H     0x4A /*! Step counter timestamp information register */
#define AIMU_LSM6DS33_STEP_COUNTER_L       0x4B /*! Step counter output register */
#define AIMU_LSM6DS33_STEP_COUNTER_H       0x4C /*! Step counter output register */
#define AIMU_LSM6DS33_FUNC_SRC             0x53 /*! Significant motion, tilt, step detector interrupt source register */
#define AIMU_LSM6DS33_TAP_CFG              0x58 /*! Time stamp, pedometer, tilt, filtering, and tap recognition functions configuration register */
#define AIMU_LSM6DS33_TAP_THIS_6D          0x59 /*! Portrait/landscape position and tap function threshold register */
#define AIMU_LSM6DS33_INT_DUR2             0x5A /*! Tap recognition function setting register */
#define AIMU_LSM6DS33_WAKE_UP_THS          0x5B /*! Single and double-tap function threshold register */
#define AIMU_LSM6DS33_WAKE_UP_DUR          0x5C /*! Free-fall, wakeup, time stamp and sleep mode functions duration setting register */
#define AIMU_LSM6DS33_FREE_FALL            0x5D /*! Free-fall function duration setting register */
#define AIMU_LSM6DS33_MD1_CFG              0x5E /*! Functions routing on INT1 register */
#define AIMU_LSM6DS33_MD2_CFG              0x5F /*! Functions routing on INT1 register */


#endif /* _aimu_lsm6ds33_registers_h_ */

/************************/
/*  End of File Comment */
/************************/