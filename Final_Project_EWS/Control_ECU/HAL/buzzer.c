/*
 *  Module: Buzzer
 *  File Name: buzzer.c
 *  Description: Source file for the Buzzer driver
 *  Author: Esraa Khaled
 */

#include "buzzer.h"
#include "../MCAL/gpio.h"

/*Functions Definitions*/
/*Initialization Function of the Buzzer*/
void Buzzer_init()
{
	/*Setup Buzzer Pin as o/p pin*/
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	/*turn off the buzzer at the beginning*/
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

/*Function to enable the Buzzer through the GPIO*/
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}

/*Function to disable the Buzzer through the GPIO*/
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

