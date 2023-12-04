/*
 *  Module: UART
 *  File Name: uart.c
 *  Description: Header file for the UART driver
 *  Author: Esraa Khaled
 */

#include "uart.h"
#include "../LIB/atmega32_registers.h"

/*Functions Definitions*/

/*Initialization function of the UART Driver*/
void UART_init(UART_ConfigType* Config_Ptr)
{
	/*value to be written in UBRR Register according to baud rate*/
	uint16 ubrr_value = 0;

	UCSRA.bits.U2X = 1; /*Asynchronous with double the UART transmission speed*/
	UCSRB.bits.TXEN = 1; /*Tx enable*/
	UCSRB.bits.RXEN = 1; /*Rx enable*/

	/*URSEL should  = 1 to access UCSRC*/
	UCSRC.bits.URSEL = 1;
	/*Choose the Parity --> UPM0 & UPM1*/
	UCSRC.byte = (UCSRC.byte & 0xCF) | (Config_Ptr->parity << 4);

	/*Choose the Stop Bits*/
	UCSRC.bits.USBS = Config_Ptr->stop;

	/*Choose the data length*/
	switch(Config_Ptr->data)
	{
		case (FIVE_BITS):
			UCSRC.bits.UCSZ0 = 0;
			UCSRC.bits.UCSZ1 = 0;
			UCSRB.bits.UCSZ2 = 0;
			break;
		case (SIX_BITS):
			UCSRC.bits.UCSZ0 = 1;
			UCSRC.bits.UCSZ1 = 0;
			UCSRB.bits.UCSZ2 = 0;
			break;
		case (SEVEN_BITS):
			UCSRC.bits.UCSZ0 = 0;
			UCSRC.bits.UCSZ1 = 1;
			UCSRB.bits.UCSZ2 = 0;
			break;
		case (EIGHT_BITS):
			UCSRC.bits.UCSZ0 = 1;
			UCSRC.bits.UCSZ1 = 1;
			UCSRB.bits.UCSZ2 = 0;
			break;
		case (NINE_BITS):
			UCSRC.bits.UCSZ0 = 1;
			UCSRC.bits.UCSZ1 = 1;
			UCSRB.bits.UCSZ2 = 1;
			break;
	}

	/*Calculate the value to be written in UBRR Register according to baud rate*/
	ubrr_value = (uint16)(((F_CPU/(8UL * Config_Ptr->baudrate))) - 1);

	/*URSEL should = 0 to access UBRRH*/
	UBRRH.byte = (ubrr_value>>8);
	UBRRL.byte = ubrr_value;

}

/*Function responsible for send byte to another UART device.*/
void UART_sendByte(const uint8 data)
{
	/*wait for the UDRE Flag to be set indicating that the UDR register is empty and previous data was transmitted to avoid overwriting*/
	while(UCSRA.bits.UDRE == 0);

	/*Put the required data to be sent in the UDR Regsiter*/
	UDR.byte = data;
}

/*Functional responsible for receive byte from another UART device.*/
uint8 UART_receiveByte(void)
{
	/*wait for the RXC Flag to be set indicating that the UART finished receiving the data*/
	while(UCSRA.bits.RXC == 0);

	/*read the data received from the UDR Register*/
	return UDR.byte;
}

/*Send the required string through UART to the other UART device.*/
void UART_sendString(const uint8* str)
{
	/*Send the whole string byte by byte*/
	while(*str != '\0')
	{
		UART_sendByte(*str);
		str++;
	}
}

/*Receive the required string until the '#' symbol through UART from the other UART device.*/
void UART_receiveString(uint8* str)
{
	/*counter*/
	uint8 i = 0;

	/*Receive first byte*/
	str[i] = UART_receiveByte();

	while(str[i] != '#') /*# is the key for sending and receiving*/
	{
		/*receive new byte from the string in the next free location*/
		i++;
		str[i] = UART_receiveByte();
	}

	str[i] = '\0'; /*put the null terminator for the string*/
}

