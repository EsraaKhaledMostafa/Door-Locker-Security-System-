/*
 *  File Name: hmi.h
 *  Description: Application header file for uC1
 *  Author: Esraa Khaled
 */

#ifndef APP_HMI_H_
#define APP_HMI_H_

#include "../LIB/std_types.h"

/*Agreed Definitions for both uCs to operate together*/
#define MC1_READY                  0x00
#define MC2_READY                  0x01
#define MC1_FIRST_PASSWORD         0x02
#define MC1_SECOND_PASSWORD        0x03
#define MC2_PASSWORD_SAVED         0x02
#define MC2_PASSWORDS_MATCH        0x03
#define MC2_PASSWORDS_DONOT_MATCH  0x04
#define MC1_OPEN_DOOR              0x05
#define MC1_CHANGE_PASSWORD        0x06
#define MC1_SET_ALARM              0x07
#define MC2_DOOR_UNLOCKING         0x08
#define MC2_DOOR_LOCKING           0x09
#define MC2_RESET_PROGRAM          0x10

/*password length if needed to be changed later*/
#define PASSWORD_LENGTH  5

/*Function to create password by taking 2 values from the user and sending them to the control ECU to check their equality*/
void creatingPassword(void);

/*Function to take the user input and store it in password array*/
void passwordEntering(uint8* password, uint8 firstOrSecond);

/*function to send the password to the control ECU through the UART*/
void sendPassword(uint8* password);

/*Function to diplay the menu options and choose from it what to do*/
void MenuOptions(void);

/*It is the call back function of the timer*/
void countTimerTicks(void);



#endif /* APP_HMI_H_ */
