/*
 *  Module: Timer1
 *  File Name: timer1.c
 *  Description: Source file for the AVR Timer1 driver
 *  Author: Esraa Khaled
 */

#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*Global Variables*/
void (*g_callback_ptr)(void) = NULL_PTR;



/*Interrupt Service Routines*/
ISR (TIMER1_OVF_vect)
{
	/*Call the Callback function*/
	if(g_callback_ptr != NULL_PTR)
	{
		(*g_callback_ptr)();
	}
}

ISR (TIMER1_COMPA_vect)
{
	/*Call the Callback function*/
	if(g_callback_ptr != NULL_PTR)
		{
			(*g_callback_ptr)();
		}
}


/*Functions Definitions*/

/*Function to initialize the Timer driver*/
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	/*Set Timer1 Initial value to the required one from the configuration structure*/
	TCNT1 = Config_Ptr->initial_value;

	/*Choose the mode of timer either Compare Mode or Normal Mode*/
	switch(Config_Ptr->mode)
	{
		case(NORMAL_MODE):
		{
			/*Enabling the Overflow(Normal) Mode Interrupt*/
			TIMSK |= (1<<TOIE1);

			/*Normal Mode -> WGM010 & WGM11 in TCCR1A = 0, and WGM13 & WGM12 in TCCR1B = 0*/
			TCCR1A = (1<<FOC1A); /*For Choosing non PWM Mode*/

			/*Choosing the prescaler value for activating the CLK*/
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
			break;
		}
		case (COMPARE_A_MODE):
		{
			/*Put the compare value in the Compare register*/
			OCR1A = Config_Ptr->compare_value;

			/*Enabling the Compare Mode Interrupt*/
			TIMSK |= (1<<OCIE1A);

			/*Compare Mode -> WGM10 & WGM11 in TCCR1A = 0, and WGM13 = 0 & WGM12 = 1 in TCCR1B*/
			TCCR1A = (1<<FOC1A); /*For Choosing non PWM Mode*/


			/*Choosing the prescaler value for activating the CLK*/
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
			TCCR1B |= (1<<WGM12);
			break;
		}
	}
}

/*Function to disable the Timer1*/
void Timer1_deInit(void)
{
	TCNT1 = 0; /*Reset the initial value of timer*/
	TCCR1B = 0; /*Deactivate the clock*/
}

/*Function to set the Call Back function address*/
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callback_ptr = a_ptr; /*Store the Callback Function Address in a global variable so as not to be poped from stack on leaving this function*/
}


