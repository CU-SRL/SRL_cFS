/**
 * @file h3lis100dl_v2_registers.h
 * @brief Register definitions for the H3LIS100DL accelerometer
 * @version 0.1
 * @date 2022-05-30
 * 
 * Registers marked as Reserved must not be changed as they contain the factory calibration values.
 * Their content is automatically restored when the device is powered up.
 * Writing to those registers may change calibration data and thus lead to improper functioning of the device.
 */

 #ifndef _H3LIS100DL_V2_REGISTERS_H_
 #define _H3LIS100DL_V2_REGISTERS_H_

//                                  0x00-0x0E   /*! Reserved (do not modify). */
#define H3LIS100DL_WHO_AM_I         0x0F        /*! Read. Device identification register. */
//                                  0x10-0x1F   /*! Reserved (do not modify). */
#define H3LIS100DL_CTRL_REG1        0x20        /*! Read-Write. Control Register 1: Power mode, data rate, and x-y-z axes configuration. */
#define H3LIS100DL_CTRL_REG2        0x21        /*! Read-Write. Control Register 2: Reboot bit and high-pass filter configuration. */
#define H3LIS100DL_CTRL_REG3        0x22        /*! Read-Write. Control Register 3: Interrupt control register. */
#define H3LIS100DL_CTRL_REG4        0x23        /*! Read-Write. Control Register 4: SPI serial interface mode configuration. */
#define H3LIS100DL_CTRL_REG5        0x24        /*! Read-Write. Control Register 5: Sleep-to-wake configuration. */
#define H3LIS100DL_HP_FILTER_RESET  0x25        /*! Read. Dummy Register: Instantaneously zeroes internal high-pass filter. */
#define H3LIS100DL_REFERENCE        0x26        /*! Read-Write. Sets acceleration reference value for high-pass filter output. */
#define H3LIS100DL_STATUS_REG       0x27        /*! Read. X-y-z axes data status register. */
//                                  0x28        /*! Reserved (do not modify). */
#define H3LIS100DL_OUT_X            0x29        /*! Read. X-axis acceleration data expressed as two's complement. */
//                                  0x2A        /*! Reserved (do not modify). */
#define H3LIS100DL_OUT_Y            0x2B        /*! Read. Y-axis acceleration data expressed as two's complement. */
//                                  0x2C        /*! Reserved (do not modify). */
#define H3LIS100DL_OUT_Z            0x2D        /*! Read. Z-axis acceleration data expressed as two's complement. */
//                                  0x2E-0x2F   /*! Reserved (do not modify). */
#define H3LIS100DL_INT1_CFG         0x30        /*! Read-Write. Interrupt 1 configuration register. */
#define H3LIS100DL_INT1_SRC         0x31        /*! Read. Interrupt 1 source register. */
#define H3LIS100DL_INT1_THS         0x32        /*! Read-Write. Interrupt 1 threshold. */
#define H3LIS100DL_INT1_DURATION    0x33        /*! Read-Write. Interrupt 1 event minimum duration. */
#define H3LIS100DL_INT2_CFG         0x34        /*! Read-Write. Interrupt 2 configuration register. */
#define H3LIS100DL_INT2_SRC         0x35        /*! Read. Interrupt 2 source register. */
#define H3LIS100DL_INT2_THS         0x36        /*! Read-Write. Interrupt 2 threshold. */
#define H3LIS100DL_INT2_DURATION    0x37        /*! Read-Write. Interrupt 2 event minimum duration. */
//                                  0x38-0x3F   /*! Reserved (do not modify). */

#endif