/*
 *  Module: Timer1
 *  File Name: timer1.h
 *  Description: Header file for the AVR Timer1 driver
 *  Author: Esraa Khaled
 */

#ifndef MCAL_TIMER1_H_
#define MCAL_TIMER1_H_

#include "../LIB/std_types.h"

/*Type Definitions*/

typedef enum
{
	NO_CLK,CLK_1,CLK_8,CLK_64,CLK_256,CLK_1024,EXT_CLK_FALLING_EDGE,EXT_CLK_RISING_EDGE

} Timer1_Prescaler;

typedef enum
{
	NORMAL_MODE, COMPARE_A_MODE = 4
} Timer1_Mode;


typedef struct
{
	uint16 initial_value;
	uint16 compare_value; /*it will be used in compare mode only.*/
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;


/*Functions Prototypes*/
/*Function to initialize the Timer driver*/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*Function to disable the Timer1*/
void Timer1_deInit(void);

/*Function to set the Call Back function address*/
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* MCAL_TIMER1_H_ */
