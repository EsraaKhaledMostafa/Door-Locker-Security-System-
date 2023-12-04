/*
 *  File Name: hmi_main.c
 *  Description: Application for uC1
 *  Author: Esraa Khaled
 */

#include "hmi.h"
#include "../HAL/keypad.h"
#include "../HAL/lcd.h"
#include "../MCAL/timer1.h"
#include "../MCAL/uart.h"
#include "../LIB/atmega32_registers.h"
#include <util/delay.h>


                                         /*Global Variables*/

/*array to save password 1st entered*/
uint8 firstPasswordEntry[PASSWORD_LENGTH];

/*array to save password 2nd entered*/
uint8 secondPasswordEntry[PASSWORD_LENGTH];

/*Flag to know if the password are matched or not*/
uint8 passwordsMatchingStatus = FALSE;

/*Flag to know if passsword is set and stored in the EEPROM*/
uint8 passwordIsSet = FALSE;

/*Flag to reset program after error and alarm*/
uint8 reset = FALSE;

/*variable to count the timer ticks on interrupt occurring -> 1 tick =  1 sec*/
uint8 volatile timerTicks = 0;

/*Timer Configurations*/
/*FCPU = 8 MHz -> Ftimer = FCPU/N -> N = 265 -> Ftimer = 31250 Hz*/
/*Tcompare = Comparevalue * Ttimer*/
/*Ttimer = 1/Ftimer = 32us -> Tcompare = 1 sec -> Interrupt will occur every 1 sec -> 1 tick = 1 sec*/
/*Therefore Compare value = 31250*/
Timer1_ConfigType Timer1_Configurations = {0,7812,CLK_1024, COMPARE_A_MODE};


                                                   /*MAIN*/
int main (void)
{
	SREG.bits.I_BIT = 1; /*enable the global interrupts*/
	LCD_init(); /*Initialize the LCD Driver*/

	Timer1_setCallBack(countTimerTicks); /*set the callback function of the timer*/

	/*UART Configuration with Baud rate = 9600, eight bits data, disabling parity and one stop bit*/
	UART_ConfigType UART_Configurations = {9600,EIGHT_BITS,DISABLE,ONE_BIT};
	UART_init(&UART_Configurations); /*Initialize the UART Driver with the required configuration*/

	/*Super Loop*/
	while(1)
	{
		/*Check if it is the first time to create the password or the password already was created and stored thus we need the menu*/
		/*if the password is set earlier or the program is resetted after 1 min of alarm then we need to loop back into the menu options*/
		if(passwordIsSet && reset == TRUE)
		{
			MenuOptions(); /*call the function that displays the menu and choose what needed action from it*/
		}
		else
		{
			/*if the password is not set then we nead to create password first*/
			creatingPassword();

			/*check the 2 entered passwords if they match or not*/
			passwordsMatchingStatus = UART_receiveByte();
			if(passwordsMatchingStatus == MC2_PASSWORDS_MATCH)
			{
				/*if the 2 passwords are the same then set the flag to 1 and call the menu function*/
				passwordIsSet = TRUE;
				MenuOptions();
			}
			else
			{
				/*if the 2 passwords are not the same then we will set the flag to 0 in order to create passsword again*/
				passwordIsSet = FALSE;
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,1,"Pass not match");
			}
		}
		_delay_ms(1000);
	}
}

                                                 /*Functions Protoypes*/

/*Function to create password by taking 2 values from the user and sending them to the control ECU to check their equality*/
void creatingPassword(void)
{
	/*take the 2 passwords from teh user*/
	passwordEntering(firstPasswordEntry,MC1_FIRST_PASSWORD);
	passwordEntering(secondPasswordEntry,MC1_SECOND_PASSWORD);

	/*check that control ECU is ready to receive data from the hmi ECU*/
	UART_sendByte(MC1_READY);
	while(UART_receiveByte()!= MC2_READY);

	/*send first pass to the control ECU through the UART*/
	UART_sendByte(MC1_FIRST_PASSWORD);
	sendPassword(firstPasswordEntry);

	/*send second pass to the control ECU through the UART*/
	UART_sendByte(MC1_SECOND_PASSWORD);
	sendPassword(secondPasswordEntry);
}


/*Function to diplay the menu options and choose from it what to do*/
void MenuOptions(void)
{
	uint8 userChoice, passswordEntryNumbers = 0;
	passwordsMatchingStatus = 0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	passwordsMatchingStatus = 0;
	userChoice = KEYPAD_getPressedKey(); /*get the choice from the user*/

	/*The only condition to leave the loop is that passwords are matched if any choice is chosen and password is entered*/
	while(passwordsMatchingStatus != MC2_PASSWORDS_MATCH)
	{
		passswordEntryNumbers++;
		switch(userChoice)
		{
			case '+':
			{
				UART_sendByte(MC1_OPEN_DOOR); /*notifying the control ECU which choice the user wants*/
				passwordEntering(firstPasswordEntry,MC1_FIRST_PASSWORD); /*taking the password from the user*/
				UART_sendByte(MC1_FIRST_PASSWORD);
				sendPassword(firstPasswordEntry);  /*sending the password to be checked*/
				passwordsMatchingStatus = UART_receiveByte(); /*check if the passwords are matched or not*/
				/*if the passwords are matched Door will be unlocked and then locked again*/
				/*if the passwords are not matched he will try again for 3 times*/
				if(passwordsMatchingStatus == MC2_PASSWORDS_MATCH)
				{
					while(UART_receiveByte() != MC2_DOOR_UNLOCKING);
					Timer1_init(&Timer1_Configurations);
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,1,"Door Unlocking");
					while(timerTicks != 18);
					Timer1_deInit();
					timerTicks = 0;

					while(UART_receiveByte() != MC2_DOOR_LOCKING);
					Timer1_init(&Timer1_Configurations);
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,2,"Door Locking");
					while(timerTicks != 15);
					Timer1_deInit();
					timerTicks = 0;

					/*for showing the menu option again on resetting the program after locking back the door*/
					passwordIsSet = TRUE;
					while(UART_receiveByte() != MC2_RESET_PROGRAM);
					reset = TRUE;
				}
				break;
			}
			case '-':
			{
				UART_sendByte(MC1_CHANGE_PASSWORD); /*notifying the control ECU which choice the user wants*/
				passwordEntering(firstPasswordEntry,MC1_FIRST_PASSWORD); /*taking the password from the user*/
				UART_sendByte(MC1_FIRST_PASSWORD);
				sendPassword(firstPasswordEntry); /*sending the password to be checked*/
				passwordsMatchingStatus = UART_receiveByte(); /*check if the passwords are matched or not*/
				/*if the passwords are matched user can change password*/
				/*if the passwords are not matched he will try again for 3 times*/
				if(passwordsMatchingStatus == MC2_PASSWORDS_MATCH)
				{
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,0,"Passwords match");
					LCD_displayStringRowColumn(1,2,"Change Pass");
					passwordIsSet = FALSE; /*in order to create new password and leave the menu options*/
				}
				break;
			}
			default:
				break;
		}

		/*On exceeding 3 times of entering wrong password after choosing any of + or -*/
		if(passswordEntryNumbers == 3)
		{
			UART_sendByte(MC1_SET_ALARM); /*Notify the Control ECU to set the alarm mode on*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 6,"ERORR");
			Timer1_init(&Timer1_Configurations);
			while(timerTicks != 60);
			Timer1_deInit();
			timerTicks = 0;
			while(UART_receiveByte() != MC2_RESET_PROGRAM); /*wait for the control unit to reset the program*/
			reset = TRUE; /*to start again from the menu option*/
			break;
		}
		_delay_ms(300); /*delay before resetting the program*/
	}
}

/*Function to take the user input and store it in password array*/
void passwordEntering(uint8* password, uint8 firstOrSecond)
{
	uint8 i = 0;
	LCD_clearScreen();

	if(firstOrSecond == MC1_FIRST_PASSWORD)
	{
		LCD_displayString("Plz Enter Pass:");
		for(i = 0; i < 5; i++)
		{
			password[i] = KEYPAD_getPressedKey(); /*store the first password in the first array*/
			if(password[i] >= 0 && password[i] <= 9) /*password should be numbers only*/
			{
				LCD_moveCursor(1,i);
				LCD_integerToString(password[i]); /*display the number user entered for 1/2 a sec to be seen then replace it with * as required*/
				_delay_ms(500);
				LCD_moveCursor(1,i);
				LCD_displayCharacter('*');
			}
			else if(password[i] == 'c')
			{
				i = -1;
				LCD_displayStringRowColumn(1,0,"     ");  /*if clear button is pressed clear the screen and reset the counter*/
			}
			else
			{
				i--; /*if any character is entered do not consider it from the 5 numbers password*/
			}
			_delay_ms(50); /*small delay before taking the next number */
		}
	}
	else if(firstOrSecond == MC1_SECOND_PASSWORD)
	{
		LCD_displayString("Plz re-enter the ");
		LCD_displayStringRowColumn(1,0,"same pass:"); /*different strings to be displayed on second password entry*/

		for(i = 0; i < 5; i++)
		{
			password[i] = KEYPAD_getPressedKey(); /*store the second password in the second array*/
			if(password[i] >= 0 && password[i] <= 9)/*password should be numbers only*/
			{
				LCD_moveCursor(1,i+11);
				LCD_integerToString(password[i]); /*display the number user entered for 1/2 a sec to be seen then replace it with * as required*/
				_delay_ms(500);
				LCD_moveCursor(1,i+11);
				LCD_displayCharacter('*');
			}
			else if(password[i] == 'c')
			{
				i = -1;
				LCD_displayStringRowColumn(1,11,"     "); /*if clear button is pressed clear the screen and reset the counter*/
			}
			else
			{
				i--;  /*if any character is entered do not consider it from the 5 numbers password*/
			}
			_delay_ms(50);  /*small delay before taking the next number */
		}
	}

	/*After the 5 digits of password are saved wait for pressing the = button to leave the function*/
	while(KEYPAD_getPressedKey() != '=');
}


/*function to send the password to the control ECU through the UART*/
void sendPassword(uint8* password)
{
	uint8 i = 0;

	for(i = 0; i < PASSWORD_LENGTH; i++)
	{
		UART_sendByte(password[i]); /*send the password byte by byte*/
		_delay_ms(15); /*small delay to avoid overwrite*/
	}
}

/*It is the call back function of the timer*/
void countTimerTicks(void)
{
	timerTicks++; /*increment the timer ticks by 1 upon each interrupt*/
}
