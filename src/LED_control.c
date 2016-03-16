/*******  user\apps\Audio_Pitch_Detection\inc\LED_control.c
*
* Summary:
*		LED Control function for different programm states
*
* Inputs:
*		State of the programm, LEDs
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
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		15/03/2016
*
*/
#include <board\h\sask.h>
#include <peripherals\timers\inc\ex_timer.h>

#include "..\inc\LED_control.h"

int currentState;

float instruction_clock_frequency, instruction_cycle_time;
float wait_time;


int displayState(int state)
{
	switch(state)
	{
		case 0:
				currentState=0;
				readyState();
				break;
		case 1:
				currentState=1;
				break;
		default:
				currentState=2;
				errorState();
	}

	return currentState;
}

void displayLED(int led_red, int led_yellow, int led_green)
{
	if(led_red==1)
	{
		RED_LED=SASK_LED_ON;
	}
	else if(led_red==0)
	{
		RED_LED=SASK_LED_OFF;
	}

	if(led_yellow==1)
	{
		YELLOW_LED=SASK_LED_ON;
	}
	else if(led_yellow==0)
	{
		YELLOW_LED=SASK_LED_OFF;
	}

	if(led_green==1)
	{
		GREEN_LED=SASK_LED_ON;
	}
	else if(led_green==0)
	{
		GREEN_LED=SASK_LED_OFF;
	}
}

void readyState()
{
	instruction_clock_frequency = clock_frequency;
	instruction_cycle_time = 1.0 / instruction_clock_frequency;

	ex_timer_init( instruction_cycle_time );

	int led_selector=0;
	while(currentState==0)
	{
		if((SWITCH_S1==0) && (SWITCH_S2==1)) //bypassing checkswitch function
		{
			currentState=1;
		}
		else
		{
			switch(led_selector)
			{
				case 0:
					RED_LED=SASK_LED_ON;
					YELLOW_LED=SASK_LED_OFF;
					GREEN_LED=SASK_LED_OFF;
					led_selector=led_selector+1;
					break;
				case 1:
					RED_LED=SASK_LED_OFF;
					YELLOW_LED=SASK_LED_ON;
					GREEN_LED=SASK_LED_OFF;
					led_selector=led_selector+1;
					break;
				case 2:
					RED_LED=SASK_LED_OFF;
					YELLOW_LED=SASK_LED_OFF;
					GREEN_LED=SASK_LED_ON;
					led_selector=0;
					break;
				default:
					errorState();
			}
		
			ex_timer_wait(timeout);
		}
	}
}

void errorState()
{
	instruction_clock_frequency = clock_frequency;
	instruction_cycle_time = 1.0 / instruction_clock_frequency;

	ex_timer_init( instruction_cycle_time );

	int current_led_state=FALSE;

	while(1)
	{
		if((SWITCH_S1==0) && (SWITCH_S2==0))
		{
			current_led_state=FALSE;
			currentState=0;
			break;
		}
		else if(current_led_state==FALSE)
		{
			RED_LED=SASK_LED_ON;
			YELLOW_LED=SASK_LED_ON;
			GREEN_LED=SASK_LED_ON;

			current_led_state=TRUE;
		}
		else if(current_led_state==TRUE)
		{

			RED_LED=SASK_LED_OFF;
			YELLOW_LED=SASK_LED_OFF;
			GREEN_LED=SASK_LED_OFF;

			current_led_state=FALSE;
		}

		ex_timer_wait(timeout);
	}
}
