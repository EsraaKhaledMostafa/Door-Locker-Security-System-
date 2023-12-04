/*
 *  Module: I2C
 *  File Name: i2c.h
 *  Description: Header file for the I2C Driver
 *  Author: Esraa Khaled
 */

#ifndef I2C_H_
#define I2C_H_

#include "../LIB/std_types.h"

/*type Definitions*/
/*At TWI Prescaler Bits = 00 -> Prescaler Value = 1 & FCPU = 8MHz*/
/*Equation - > SCL Frequency = (FCPU)/(16 + 2*(TWBR)*(4^TWPS))*/
typedef enum
{
	/*The values to be stored in the TWBR Register according to the above equation*/
	BAUD_RATE_500KHZ = 0, BAUD_RATE_400KHZ = 2, BAUD_RATE_250KHZ = 8, BAUD_RATE_200KHZ = 12, BAUD_RATE_125KHZ = 24, BAUD_RATE_100KHZ = 32
}TWI_BaudRate;


typedef struct
{
	uint8 address;
	TWI_BaudRate bit_rate;
}TWI_ConfigType;



/*I2C status bits -> TWSR Register*/

#define TWI_START             0x08 /*Start condition has been sent*/
#define TWI_REPEATED_START    0x10 /*repeated start condition has been sent*/

/*1. Master Transmitter Mode*/
#define TWI_MT_SLA_W_ACK      0x18 /*Master transmits Slave Address + Write Bit and receives Acknowledgment*/
#define TWI_MT_SLA_W_NACK     0x20 /*Master transmits Slave Address + Write Bit and receives NOT Acknowledgment*/
#define TWI_MT_DATA_ACK       0x28 /*Master transmits data and Acknowledgment is received*/
#define TWI_MT_DATA_NACK      0x30 /*Master transmits data and NOT Acknowledgment is received*/

/*2. Master Receiver Mode*/
#define TWI_MR_SLA_R_NACK     0x38 /*Master transmits Slave Address + Read Bit and receives NOT Acknowledgment*/
#define TWI_MR_SLA_R_ACK      0x40 /*Master transmits Slave Address + Read Bit and receives Acknowledgment*/
#define TWI_MR_DATA_ACK       0x50 /*Master receives data and send Acknowledgment to slave*/
#define TWI_MR_DATA_NACK      0x58 /*Master receives data and send NOT Acknowledgment to slave*/



/*Functions Prototypes*/
/*Initialization function of the I2C Driver*/
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*Function to get the status of the I2C from TWSR register*/
uint8 TWI_getStatus(void);

/*Functions to form the frame of the I2C*/

/*Function to send the start bit*/
void TWI_start(void);
/*Function to send the stop bit*/
void TWI_stop(void);
/*Function to send a byte*/
void TWI_writeByte(uint8 data);
/*Function to receive a byte with sending Acknowledgment to the slave*/
uint8 TWI_readByteWithACK(void);
/*Function to receive a byte with sending Negative Acknowledgment to the slave*/
uint8 TWI_readByteWithNACK(void);




#endif /* I2C_H_ */
