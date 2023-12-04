/*
 *  Module: EEPROM
 *  File Name: eeprom.c
 *  Description: Source file for the EEPROM Driver
 *  Author: Esraa Khaled
 */

#include "eeprom.h"
#include "../MCAL/i2c.h"

/*Functions Definitions*/
/*Function to store a byte in the memory*/
uint8 EEPROM_writeByte(uint16 memory_address, uint8 data)
{
	/*Send the start byte*/
	TWI_start();
	/*Check for errors*/
	if(TWI_getStatus() != TWI_START)
		return ERROR;

	/*send the device address starting with constant ob1010 and then A8, A9, A10 from the memory address as they are part of the device address and finally followed by the R/W bit*/
	/*R/W -> 0 since we need to write a byte in memory*/
	TWI_writeByte((uint8)(0xA0 | ((memory_address & 0x0700)>>7)));
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/*Send the required memory location address*/
	TWI_writeByte((uint8)memory_address);
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/*write the data to the EEPROM*/
	TWI_writeByte(data);
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/*Send the Stop bit*/
	TWI_stop();

	return SUCCESS;
}

/*Function to read a byte from the memory*/
uint8 EEPROM_readByte(uint16 memory_address, uint8* data)
{
	/*Send the start byte*/
	TWI_start();
	/*Check for errors*/
	if(TWI_getStatus() != TWI_START)
		return ERROR;

	/*send the device address starting with constant ob1010 and then A8, A9, A10 from the memory address as they are part of the device address and finally followed by the R/W bit*/
	/*R/W -> 0 since we need to write a byte in memory*/
	TWI_writeByte((uint8)(0xA0 | ((memory_address & 0x0700)>>7)));
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/*Send the required memory location address*/
	TWI_writeByte((uint8)memory_address);
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/*Send the repeated start bit to start the reading frame*/
	TWI_start();
	/*Check for errors*/
	if(TWI_getStatus() != TWI_REPEATED_START)
		return ERROR;

	/*send the device address starting with constant ob1010 and then A8, A9, A10 from the memory address as they are part of the device address and finally followed by the R/W bit*/
	/*R/W -> 1 since we need to read a byte in memory*/
	TWI_writeByte((uint8)(0xA0 | ((memory_address & 0x0700)>>7) | 1));
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MR_SLA_R_ACK)
		return ERROR;

	/*Read the byte needed from gthe specific sent memory location*/
	*data = TWI_readByteWithNACK();
	/*Check for errors*/
	if(TWI_getStatus() != TWI_MR_SLA_R_NACK)
		return ERROR;

	/*Send the Stop bit*/
	TWI_stop();

	return SUCCESS;
}

