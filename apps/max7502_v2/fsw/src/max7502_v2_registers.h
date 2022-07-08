/*
max7503_v2 register file as according to the data sheet
*/

#ifndef _MAX7502_V2_REGISTERS_H_        
#define _MAX7502_V2_REGISTERS_H_        

#define MAX7502_V2_TEMPERATURE          0x00 //Temperature register. Read only register. Data format is 9 bits 2's compliment. Comes out in 2 bytes.
#define MAX7502_V2_CONFIGURATION        0x01 //Read and write 8 bit confiuration register that sets the fault queue, OS polarity, shutdown control, and whether the OS output functions in comparator or interrupt mode
#define MAX7502_V2_HYSTERESIS           0x02 // Read and write register. Lower limit temperature register that filters signals so that the output reacts less rapidly so as to not have minute readings that can deteriorate the controller.
#define MAX7502_V2_OS                   0x03 // Read and write register. Upper limit temperature register, similar to that of HYSTERESIS. Depending on the mode according to the CONFIGURATION register OS can act as a thermostat, turn on cooling fan, initiate shut dowm, or reduce system clock speed

#endif