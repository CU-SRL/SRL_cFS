#include "i2c.h"
#include "i2c_perfids.h"
#include "i2c_msgids.h"
#include "i2c_msg.h"
#include "i2c_events.h"
#include "i2c_version.h"

#include "mpl3115a2.h"
#include "mpl3115a2_registers.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* INIT_MPL3115A2() -- This function initializes the MPL3115A2 according	  */
/*					to the datasheet.										  */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool INIT_MPL3115A2(int I2CBus, i2c_hk_tlm_t* I2C_HkTelemetryPkt)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, MPL3115_I2C_ADDR);
	
	// Place MPL3115A2 into standby mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0))
	{
		CFE_EVS_SendEvent(I2C_MPL3115A2_FAILED_CHANGE_TO_STANDBY_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to place MPL3115A2 into Standby Mode... ");
        I2C_HkTelemetryPkt->i2c_device_error_count++;
		return false;
	}

	// Set the MPL3115A2 sample rate to 34ms
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0x98))
	{
		CFE_EVS_SendEvent(I2C_MPL3115A2_RATE_SWITCH_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch output on MPL3115A2 to 34ms...  ");
        I2C_HkTelemetryPkt->i2c_device_error_count++;

		return false;
	}

	// Switch the MPL3115A2 to active, set altimeter mode, set polling mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0xB9))
	{
		CFE_EVS_SendEvent(I2C_MPL3115A2_FAILED_CHANGE_TO_ACTIVE_MODE_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to switch MPL3115A2 to active...  ");
        I2C_HkTelemetryPkt->i2c_device_error_count++;

		return false;
	}

	// Enable Events on the MPL3115A2
	if(!I2C_write(file, MPL3115_PT_DATA_CFG, 0x07))
	{
		CFE_EVS_SendEvent(I2C_MPL3115A2_ENABLE_EVENTS_ERR_EID, CFE_EVS_EventType_ERROR,
           "Failed to enable events on the MPL3115A2...  ");
        I2C_HkTelemetryPkt->i2c_device_error_count++;

		return false;
	}

	// Close the I2C file
	I2C_close(file);

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* PROCESS_MPL3115A2() -- This function process the MPL3115A2 data according  */
/*					to the datasheet.										  */
/*                                                                            */
/*  NOTE: Realistically the data acquistion and processing should happen in   */
/*			different functions. For now it was paired in one for quick		  */
/*			turn around...		                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void PROCESS_MPL3115A(int i2cbus, i2c_hk_tlm_t* I2C_HkTelemetryPkt)
{
	// Open the I2C Device
	int file = I2C_open(i2cbus, MPL3115_I2C_ADDR);

	// Check for data in the STATUS register
	I2C_read(file, MPL3115_STATUS, 1, MPL3115A2.status);
	if (MPL3115A2.status[0] != 0)
	{
		// Read the Data Buffer
		if(!I2C_read(file, MPL3115_OUT_P_MSB, 5, MPL3115A2.buffer))
		{
			CFE_EVS_SendEvent(I2C_MPL3115A2_REGISTERS_READ_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to read data buffers... ");
        	I2C_HkTelemetryPkt->i2c_device_error_count++;

			return;
		}

		/* Process the Data Buffer */
			
		// Altitude
		int32_t alt;

		alt = ((uint32_t)MPL3115A2.buffer[0]) << 24;
		alt |= ((uint32_t)MPL3115A2.buffer[1]) << 16;
		alt |= ((uint32_t)MPL3115A2.buffer[2]) << 8;

		float altitude = alt;
		altitude /= 65536.0;		

		// Temperature
		int16_t t;

		t = MPL3115A2.buffer[3];
		t <<= 8;
		t |= MPL3115A2.buffer[4];
		t >>= 4;

		if(t & 0x800)
		{
			t |= 0xF000;
		}

		float temp = t;
		temp /= 16.0;

		// Print Processed Values if the debug flag is enabled for this app
		CFE_EVS_SendEvent(I2C_MPL3115A2_DATA_DBG_EID, CFE_EVS_EventType_DEBUG, "Altitude: %F Temperature: %F ", altitude, temp);
	}

	// Close the I2C Buffer
	I2C_close(file);
}