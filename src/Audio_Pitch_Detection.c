/*******        user\apps\audio\Audio_Pipe\Audio_Pipe.c
  * Usage:
  *       Execute using MPLAB IDE
  * Summary:
  *         Simple audio pipeline based on DMA in and out of the audio codec
  *
  * Inputs:
  *       Audio codec input
  * Outputs:
  *       Audio codec output

  * Dependencies:
  *      Sask_Init()
  *		__delay32()
  * Notes:
  *		none

  * ToDo:
  *     none

  * Originator:
  *     Chris Bore, BORES Signal processing, chris@bores.com, www.bores.com

  * History:
  *      Version 1.00     13/03/2013
  *****/

#include <p33FJ256GP506.h>
#include <dsp\h\dsp.h>
#include <board\h\sask.h>
#include <peripherals\adc\h\ADCChannelDrv.h>
#include <peripherals\pwm\h\OCPWMDrv.h>

#include <board\inc\ex_sask_generic.h>
#include <board\inc\ex_sask_led.h>
#include <peripherals\timers\inc\ex_timer.h>

#include "..\inc\ex_audio_process.h"
#include "..\inc\LED_control.h"
#include "..\inc\FFT_processing.h"

#define FRAME_SIZE 				256

int		adcBuffer		[ADC_CHANNEL_DMA_BUFSIZE] 	__attribute__((space(dma)));
int		ocPWMBuffer		[OCPWM_DMA_BUFSIZE]		__attribute__((space(dma)));

fractional 		AudioIn[FRAME_SIZE], AudioWorkSpace[ FRAME_SIZE ], AudioOut [FRAME_SIZE];

int			i;
int 		state;
int			pitchResult;

fractional FftFormatedAudio[FRAME_SIZE]__attribute__ ((space(xmemory),far));
fractcomplex FFTcompResults[FRAME_SIZE]__attribute__ ((space(ymemory),far));

ADCChannelHandle adcChannelHandle;
OCPWMHandle 	ocPWMHandle;

ADCChannelHandle *pADCChannelHandle 	= &adcChannelHandle;
OCPWMHandle 	*pOCPWMHandle 		= &ocPWMHandle;

int main(void)
{
	ex_sask_init( );

	ADCChannelInit	(pADCChannelHandle,adcBuffer);
	OCPWMInit		(pOCPWMHandle,ocPWMBuffer);

	ADCChannelStart	(pADCChannelHandle);
	OCPWMStart		(pOCPWMHandle);	

	while(1)
	{
		if(state==0)
		{
			state=0;
			state=displayState(STATE_READY);
			displayLED(LED_OFF, LED_OFF, LED_OFF);
		}
		else if(state==1)
		{
			while(ADCChannelIsBusy(pADCChannelHandle));
			ADCChannelRead	(pADCChannelHandle,AudioIn,FRAME_SIZE);
	
			//ex_audio_process( FRAME_SIZE, AudioIn, AudioWorkSpace, AudioOut );
		
			convertInputForFFT(&AudioIn, &FftFormatedAudio);
			FFT(FRAME_SIZE, &AudioIn, &FFTcompResults);
			pitchResult=pitchDetection(&FFTcompResults);


			switch(pitchResult)
			{
				case 0:
					displayLED(LED_OFF, LED_OFF, LED_ON);
					break;
				case 1:
					displayLED(LED_OFF, LED_ON, LED_ON);
					break;
				case 2:
					displayLED(LED_OFF, LED_ON, LED_OFF);
					break;				
				case 3:
					displayLED(LED_ON, LED_ON, LED_OFF);
					break;
				case 4:
					displayLED(LED_ON, LED_OFF, LED_OFF);
					break;
				default:
					state=displayState(STATE_ERROR);
					break;
			}
			while(OCPWMIsBusy(pOCPWMHandle));	
			OCPWMWrite (pOCPWMHandle,AudioOut,FRAME_SIZE);

			if((SWITCH_S2==0) && (SWITCH_S1==1))
			{
				pitchResult=pitchResult+1;
			}
		}
		else if(state==2)
		{
			state=displayState(STATE_ERROR);
		}
	}
}
