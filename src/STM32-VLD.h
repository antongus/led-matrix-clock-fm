/**
*  @file STM32-H103.h
*
*  Hardware definitions for STM32-H103 board
*
*  Created on: 14.01.2011
*  Copyright (c) 2009 by Anton Gusev aka AHTOXA
**/

#ifndef STM32_VLD_H_INCLUDED
#define STM32_VLD_H_INCLUDED

typedef Pin<'C', 9, 'H'> green_led_t;
typedef Pin<'C', 8, 'H'> blue_led_t;
typedef Pin<'A', 0, 'H'> key_button_t;

extern green_led_t green_led;
extern blue_led_t blue_led;
extern key_button_t key_button;

#endif // STM32_VLD_H_INCLUDED
