/**
 * @file h3lis100dl_registers.h
 * @brief The main register header file for the H3LIS100DL IMU
 * 
 * All registers are declared and defined here for use with I2C and/or any other comms interface
 * 
 */

#ifndef _H3LIS100DL_REGISTERS_H_
#define _H3LIS100DL_REGISTERS_H_

//-------------------------------------------------------
//                  H3LIS100DL Registers
//        Registers can be found in Datasheets.
//-------------------------------------------------------

#define H3LIS100DL_I2C_ADDR             0x18 /*! I2C address */
#define H3LIS100DL_WHO_AM_I             0x0F /*! Who I Am ID */
#define H3LIS100DL_CTRL1_G              0x20 /*! Control Register 1 */
#define H3LIS100DL_CTRL2_G              0x21 /*! Control Register 2 */
#define H3LIS100DL_CTRL3_G              0x22 /*! Control Register 3 [interrupt CTRL register]*/
#define H3LIS100DL_CTRL4_C              0x23 /*! Control Register 4 */
#define H3LIS100DL_CTRL5_C              0x24 /*! Control Register 5 */
#define H3LIS100DL_HP_FILTER_RESET      0x25 /*! Dummy register */
#define H3LIS100DL_REFERENCE            0x26 /*! Sets the acceleration value taken as a reference for the high-pass filter output */
#define H3LIS100DL_STATUS_REG           0x27 /*! Status Register */
#define H3LIS100DL_OUTX                 0x29 /*! X-axis acceleration data */
#define H3LIS100DL_OUTY                 0x2B /*! Y-axis acceleration data*/
#define H3LIS100DL_OUTZ                 0x2D /*! Z-axis acceleration data */
#define H3LIS100DL_INT1_CFG             0x30 /*! Configuration register for interrupt 1 source */
#define H3LIS100DL_INT1_SRC             0x31 /*! Interrupt 1 source register */
#define H3LIS100DL_INT1_THS             0x32 /*! Interrupt 1 threshold */
#define H3LIS100DL_INT1_DURATION        0x33 /*! Duration value */
#define H3LIS100DL_INT2_CFG             0x34 /*! Configuration register for interrupt 2 source */
#define H3LIS100DL_INT2_SRC             0x35 /*! Interrupt 2 source register */
#define H3LIS100DL_INT2_THS             0x36 /*! Interrupt 2 threshold */
#define H3LIS100DL_INT2_DURATION        0x37 /*! Duration value */



#endif /* _h3lis100dl_registers_h_ */

/************************/
/*  End of File Comment */
/************************/