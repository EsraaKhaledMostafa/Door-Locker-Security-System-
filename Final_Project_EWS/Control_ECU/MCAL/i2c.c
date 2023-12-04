/*
 *  Module: I2C
 *  File Name: i2c.c
 *  Description: Source file for the I2C Driver
 *  Author: Esraa Khaled
 */



#include "i2c.h"
#include "../LIB/atmega32_registers.h"

/*Functions Definitions*/
/*Initialization function of the I2C Driver*/
void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	/*Setting the Baud rate by choosing prescaler = 1 and Configuring the TWBR register as wanted*/
	TWBR.byte = Config_Ptr->bit_rate;
	TWSR.bits.TWPS0 = 0;
	TWSR.bits.TWPS1 = 0;

	/*My address*/
	TWAR.byte = Config_Ptr->address;

	/*Enable the TWI */
	TWCR.bits.TWEN;
}

/*Function to send the start bit*/
void TWI_start(void)
{
	/*Clearing the Flag before sending the start bit, Send teh start bit, Enable teh TWI*/
	/*TWINT -> bit 7, TWSTA -> bit 5, TWEN -> bit 2*/
	TWCR.byte = (1<<7) | (1<<5) | (1<<2);

	/*Wait till the Flag is set indicating the start bit is sent successfully*/
	while(TWCR.bits.TWINT == 0);
}

/*Function to send the stop bit*/
void TWI_stop(void)
{
	/*Clearing the Flag before sending the stop bit, send the stop and enable the TWI*/
	/*TWINT -> bit 7, TWSTO -> bit 4, TWEN -> bit 2*/
	TWCR.byte = (1<<7) | (1<<4) | (1<<2);
}

/*Function to send a byte*/
void TWI_writeByte(uint8 data)
{
	/*Put the data needed to be sent in the TWI Data Register*/
	TWDR.byte = data;

	/*Clear the flag before sending the data, and enable the TWI*/
	/*TWINT -> bit 7, TWEN -> bit 2*/
	TWCR.byte = (1<<7) | (1<<2);

	/*Wait till the Flag is set indicating the data is sent successfully*/
	while(TWCR.bits.TWINT == 0);
}


/*Function to receive a byte with sending Acknowledgment to the slave*/
uint8 TWI_readByteWithACK(void)
{
	/*Clear the Flag before reading the received byte, enable sending the ACK, and enable the TWI*/
	/*TWINT -> bit 7, TWEA -> bit 6, TWEN -> bit 2*/
	TWCR.byte = (1<<7) | (1<<6)| (1<<2);

	/*Wait till the Flag is set indicating the data is received successfully*/
	while(TWCR.bits.TWINT == 0);

	/*read the received data*/
	return TWDR.byte;
}

/*Function to receive a byte with sending Negative Acknowledgment to the slave*/
uint8 TWI_readByteWithNACK(void)
{
	/*Clear the Flag before reading the received byte, and enable the TWI*/
	/*TWINT -> bit 7, TWEN -> bit 2*/
	TWCR.byte = (1<<7) | (1<<2);

	/*Wait till the Flag is set indicating the data is received successfully*/
	while(TWCR.bits.TWINT == 0);

	/*read the received data*/
	return TWDR.byte;
}

/*Function to get the status of the I2C from TWSR register*/
uint8 TWI_getStatus(void)
{
	uint8 status;
	/*From the Data Sheet: The application designer should mask the prescaler bits to zero when checking the Status bits*/
	status = TWSR.byte & 0xF8;
	return status;
}

