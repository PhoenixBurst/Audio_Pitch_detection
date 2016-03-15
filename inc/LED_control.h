/*******  user\apps\Audio_Pitch_Detection\inc\LED_control.h
*
* Summary:
*		LED Control function for different programm states
*
* Dependences:
*		ex_timer.h
*
* Inputs:
*		State of the programm, LEDs		int, defined States
*
* Outputs:
*		LEDs output
*
* Functions:
*		void displayState(int state)
*		void displayLED(int led_red, int led_yellow, int led_green)
*
* Notes:
*		none
*
* Originator: 
*		S�ren Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		15/03/2016
*
*/

#ifndef __LED_CONTROL__
#define __LED_CONTROL__

#define STATE_READY 0
#define STATE_RUN 1
#define STATE_ERROR 2

#define LED_ON 1
#define LED_OFF 0

#define clock_frequency 40e6
#define timeout 0.5

int displayState(int state); //switch between different running states

void readyState(); //display ready state on leds

void errorState(); //display error state on leds

void displayLED(int led_red, int led_yellow, int led_green); //display user selected leds

#endif