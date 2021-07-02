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
bool INIT_MPL3115A2(int I2CBus)
{
	// Open I2C for the device address
	int file = I2C_open(I2CBus, MPL3115_I2C_ADDR);
	
	// Place MPL3115A2 into standby mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0))
	{
		printf("Failed to place MPL3115A2 into Standby Mode... \n");
		return false;
	}

	// Set the MPL3115A2 sample rate to 34ms
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0x98))
	{
		printf("Failed to switch output on MPL3115A2 to 34ms... \n");
		return false;
	}

	// Switch the MPL3115A2 to active, set altimeter mode, set polling mode
	if(!I2C_write(file, MPL3115_CTRL_REG1, 0xB9))
	{
		printf("Failed to switch MPL3115A2 to active...");
		return false;
	}

	// Enable Events on the MPL3115A2
	if(!I2C_write(file, MPL3115_PT_DATA_CFG, 0x07))
	{
		printf("Failed to enable events on the MPL3115A2...");
		return false;
	}

	// Close the I2C file
	I2C_close(file);

	return true;
}

/*int main()
{
	// Set the I2C bus
	int i2cbus = 1;
	
	// Initialize the MPL3115A2
	//init_mpl3115a2_old(i2cbus);
	//init_mpl3115a2(i2cbus);

	// Initialize LOOP
	// Ctrl-C to quit
	while(true)
	{
		// Open the I2C Device
		int file = i2c_open(i2cbus, MPL3115_I2C_ADDR);

		// Check for data in the STATUS register
		i2c_read(file, MPL3115_STATUS, 1, mpl3115a2.status);
		if (mpl3115a2.status[0] != 0)
		{
			// Read the Data Buffer
			if(!i2c_read(file, MPL3115_OUT_P_MSB, 5, mpl3115a2.buffer))
			{
				printf("Failed to read data buffers... \n");
				return;
			}

			for (int i = 0; i < 5; i++)
			{
				printf("Buffer: %X ", i);
				printf("Data: %X \n", mpl3115a2.buffer[i]);
			}

			// Process the Data Buffer
			
			// Altitude
			int32_t alt;

			alt = ((uint32_t)mpl3115a2.buffer[0]) << 24;
			alt |= ((uint32_t)mpl3115a2.buffer[1]) << 16;
			alt |= ((uint32_t)mpl3115a2.buffer[2]) << 8;

			float altitude = alt;
			altitude /= 65536.0;		

			// Temperature
			int16_t t;

			t = mpl3115a2.buffer[3];
			t <<= 8;
			t |= mpl3115a2.buffer[4];
			t >>= 4;

			if(t & 0x800)
			{
				t |= 0xF000;
			}

			float temp = t;
			temp /= 16.0;

			// Print Processed Values
			printf("Altitude: %F \n", altitude);
			printf("Temperature: %F \n\n", temp);
		} else
		{
			// printf("Data not ready... %X \n", mpl3115a2.status[0]);
		}
		// Close the I2C Buffer
		i2c_close(file);
	}
	
	// Return program
	return;
}*/
