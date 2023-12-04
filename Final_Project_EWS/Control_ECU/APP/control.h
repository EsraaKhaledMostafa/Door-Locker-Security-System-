/*
 *  File Name: control.h
 *  Description: Application header file for uC2
 *  Author: Esraa Khaled
 */


#ifndef APP_CONTROL_H_
#define APP_CONTROL_H_

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


/*Function to receive more than one byte password length and store it in the array*/
void receivePassword(uint8* password);

/*Function to check 2 given passwords if they match or not*/
uint8 checkPassword(uint8* firstPasswordEntry, uint8* secondPasswordEntry);

/*Function to check the password if it matches the one stored in the memory*/
uint8 checkPasswordInMemory(void);

/*Function to save the password in the EEPROM through the I2C*/
void passwordSaving(uint8* password);

/*It is the call back function of the timer*/
void countTimerTicks(void);







#endif /* APP_CONTROL_H_ */
