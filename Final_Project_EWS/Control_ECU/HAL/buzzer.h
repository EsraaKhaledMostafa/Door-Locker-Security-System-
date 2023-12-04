/*
 *  Module: Buzzer
 *  File Name: buzzer.h
 *  Description: Header file for the Buzzer driver
 *  Author: Esraa Khaled
 */

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_

#include "../LIB/std_types.h"

/*Buzzer HW pin*/
#define BUZZER_PORT_ID    PORTD_ID
#define BUZZER_PIN_ID     PIN7_ID

/*Functions Prototypes*/
/*Initialization Function of the Buzzer*/
void Buzzer_init();

/*Function to enable the Buzzer through the GPIO*/
void Buzzer_on(void);

/*Function to disable the Buzzer through the GPIO*/
void Buzzer_off(void);


#endif /* HAL_BUZZER_H_ */
