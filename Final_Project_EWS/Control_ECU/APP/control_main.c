/*
 *  File Name: control_main.c
 *  Description: Application for uC2
 *  Author: Esraa Khaled
 */

#include "control.h"
#include "../HAL/buzzer.h"
#include "../HAL/dc_motor.h"
#include "../HAL/eeprom.h"
#include "../MCAL/uart.h"
#include "../MCAL/timer1.h"
#include "../MCAL/i2c.h"
#include "../LIB/atmega32_registers.h"
#include <util/delay.h>

                                         /*Global Variables*/

/*array to save password 1st entered*/
uint8 firstPasswordEntry[PASSWORD_LENGTH];

/*array to save password 2nd entered*/
uint8 secondPasswordEntry[PASSWORD_LENGTH];

/*variable to count the timer ticks on interrupt occurring -> 1 tick =  1 sec*/
uint8 volatile timerTicks = 0;

/*Timer Configuration*/
/*FCPU = 8 MHz -> Ftimer = FCPU/N -> N = 265 -> Ftimer = 31250 Hz*/
/*Tcompare = Comparevalue * Ttimer*/
/*Ttimer = 1/Ftimer = 32us -> Tcompare = 1 sec -> Interrupt will occur every 1 sec -> 1 tick = 1 sec*/
/*Therefore Compare value = 31250*/
Timer1_ConfigType Timer1_Configurations = {0,7812,CLK_1024, COMPARE_A_MODE};


                                                     /*MAIN*/

int main (void)
{
	SREG.bits.I_BIT = 1; /*enable the global interrupts*/

	Buzzer_init(); /*Initializing the buzzer driver*/
	DcMotor_Init(); /*Initializing the DC motor driver*/

	/*UART Configuration with Baud rate = 9600, eight bits data, disabling parity and one stop bit*/
	UART_ConfigType UART_Configurations = {9600,EIGHT_BITS,DISABLE,ONE_BIT};
	UART_init(&UART_Configurations);

	/*I2C Configuratiosn with frequency 400 KHz and address 0x01*/
	TWI_ConfigType TWI_Configurations = {0x01,BAUD_RATE_400KHZ};
	TWI_init(&TWI_Configurations);

	Timer1_setCallBack(countTimerTicks); /*set the callback function of the timer*/

	while(1)
	{
		/*Check for the received byte from hmi ECU and take an action accordinglys*/
		switch(UART_receiveByte())
		{
			case (MC1_READY):
			{
				UART_sendByte(MC2_READY);
				break;
			}
			case (MC1_FIRST_PASSWORD):
			{
				receivePassword(firstPasswordEntry); /*store first password*/
				break;
			}
			case (MC1_SECOND_PASSWORD):
			{
				receivePassword(secondPasswordEntry); /*store second password*/
				/*check if 2 passwords match or not*/
				if (checkPassword(firstPasswordEntry,secondPasswordEntry) == TRUE)
				{
					UART_sendByte(MC2_PASSWORDS_MATCH); /*send to the hmi ECU that they are the same*/
					passwordSaving(firstPasswordEntry); /*save the password in the EEPROM sicne 2 pass are matched*/
				}
				else if(checkPassword(firstPasswordEntry,secondPasswordEntry) == FALSE)
				{
					UART_sendByte(MC2_PASSWORDS_DONOT_MATCH); /*send to the hmi ECU that they are not the same*/
				}
				break;
			}
			case(MC1_OPEN_DOOR):
			{
				while(UART_receiveByte() != MC1_FIRST_PASSWORD);
				receivePassword(firstPasswordEntry); /*receive the password*/
				if(checkPasswordInMemory() == TRUE) /*check if it the same password stored in the memory*/
				{
					UART_sendByte(MC2_PASSWORDS_MATCH); /*send that they are matched*/

					/*open the door for 15 sec*/
					UART_sendByte(MC2_DOOR_UNLOCKING);
					Timer1_init(&Timer1_Configurations);
					DcMotor_Rotate(CW,90);
					while(timerTicks != 15);
					Timer1_deInit();
					timerTicks = 0;

					/*hold the door for 3 sec then*/
					Timer1_init(&Timer1_Configurations);
					DcMotor_Rotate(STOP,0);
					while(timerTicks != 3);
					Timer1_deInit();
					timerTicks = 0;

					/*close the door again for 15 sec*/
					UART_sendByte(MC2_DOOR_LOCKING);
					Timer1_init(&Timer1_Configurations);
					DcMotor_Rotate(ACW,90);
					while(timerTicks != 15);
					Timer1_deInit();
					timerTicks = 0;

					/*stop the motion of the motor since the program will be resetted to the menu option*/
					DcMotor_Rotate(STOP,0);

					UART_sendByte(MC2_RESET_PROGRAM);

				}
				else
				{
					UART_sendByte(MC2_PASSWORDS_DONOT_MATCH); /*if the password did not match the one stored in the memory*/
				}
				break;
			}
			case(MC1_CHANGE_PASSWORD):
			{
				while(UART_receiveByte() != MC1_FIRST_PASSWORD);
				receivePassword(firstPasswordEntry); /*receive the password*/
				if(checkPasswordInMemory() == TRUE)  /*check if it the same password stored in the memory*/
				{
					UART_sendByte(MC2_PASSWORDS_MATCH); /*send they are the same and thus the program will be resetted to create the password again*/
				}
				else
				{
					UART_sendByte(MC2_PASSWORDS_DONOT_MATCH); /*if the pass did not match the one stored in the memory*/
				}
				break;
			}
			case(MC1_SET_ALARM):
			{
				/*set alarm for 1 min then reset the program to retrun back to the menu option*/
				Timer1_init(&Timer1_Configurations);
				Buzzer_on();
				while(timerTicks != 60);
				Timer1_deInit();
				Buzzer_off(); /*turn off the buzzer after 1 min*/
				timerTicks = 0;
				UART_sendByte(MC2_RESET_PROGRAM);
				break;
			}
		}
	}
}


												/*Functions Protoypes*/

/*Function to check the password if it matches the one stored in the memory*/
uint8 checkPasswordInMemory(void)
{
	uint8 i = 0, memoryLocationStart = 99; /*password is stored in a random location inside the EEPROM to avoid hacking*/
	for(i = 0; i < PASSWORD_LENGTH; i++)
	{
		EEPROM_readByte(memoryLocationStart,&secondPasswordEntry[i]); /*read the password stored in the memory and save it temporarily*/
		memoryLocationStart++;
	}
	return checkPassword(firstPasswordEntry,secondPasswordEntry); /*check if the 2 passwords match or not*/
}


/*Function to check 2 given passwords if they match or not*/
uint8 checkPassword(uint8* firstPasswordEntry, uint8* secondPasswordEntry)
{
	uint8 i = 0, PasswordMatchFlag = 0;
	for(i = 0; i < PASSWORD_LENGTH; i++)
	{
		if (firstPasswordEntry[i] == secondPasswordEntry[i]) /*check each location corresponding to the other one if they are equal*/
		{
			PasswordMatchFlag = TRUE;
		}
		else
		{
			PasswordMatchFlag = FALSE; /*if ay digit is not the same break from the loop since the 2 passwords do not match*/
			break;
		}
	}
	return PasswordMatchFlag;
}

/*Function to receive more than one byte password length and store it in the array*/
void receivePassword(uint8* password)
{
	uint8 i = 0;
	for(i = 0; i < PASSWORD_LENGTH;i++)
	{
		password[i] = UART_receiveByte(); /*store byte by byte*/
		_delay_ms(15);
	}
}

/*Function to save the password in the EEPROM through the I2C*/
void passwordSaving(uint8* password)
{
	uint8 i = 0, memoryLocationStart = 99; /*store it in a random location inside the EEPROM to avoid hacking*/
	for(i = 0; i < PASSWORD_LENGTH; i++)
	{
		EEPROM_writeByte(memoryLocationStart++,password[i]); /*write byte by byte to teh memory*/
	}
}

/*It is the call back function of the timer*/
void countTimerTicks(void)
{
	timerTicks++; /*increment the timer ticks by 1 upon each interrupt*/
}
