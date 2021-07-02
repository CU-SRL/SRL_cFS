#ifndef _MPL3115A2_H_
#define _MPL3115A2_H_

// Define the Data Buffer Size using the Datasheet (48 buffers because going to 2f)
#define MPL3115A2_I2C_BUFFER 0x30

// Define the Data Struct to hold the data from the MPL3115A2
struct MPL3115A2_data {
	// Status Buffer
	uint8_t status[1];

	// Data Buffer
	uint8_t buffer[MPL3115A2_I2C_BUFFER];
	
	// Data Variables
	double pressure;
	double temperature;
} MPL3115A2;

#endif
