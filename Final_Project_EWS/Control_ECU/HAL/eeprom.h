/*
 *  Module: EEPROM
 *  File Name: eeprom.h
 *  Description: Header file for the EEPROM Driver
 *  Author: Esraa Khaled
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "../LIB/std_types.h"

/*Definitions*/
#define ERROR    0
#define SUCCESS  1


/*Functions Prototypes*/
/*Function to store a byte in the memory*/
uint8 EEPROM_writeByte(uint16 memory_address, uint8 data);

/*Function to read a byte from the memory*/
uint8 EEPROM_readByte(uint16 memory_address, uint8* data);

#endif /* EEPROM_H_ */
