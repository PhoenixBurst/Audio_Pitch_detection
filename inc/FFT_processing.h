/*******  user\apps\Audio_Pitch_Detection\inc\FFT_processing.h
*
* Summary:
*		FFT function 
*
* Dependences:
*		dsp.h
*		
*
* Inputs:
*		Audio input
*		framesize
*		FFT result array (Complex)
*		Pitch detection result(unsigned long)
*
* Outputs:
*		Detected Pitch of a signal
*		FFT Results
*		Converted AudioIN for FFT
*
* Functions:
*		void convertInputForFFT(int[] audioIN, fractional convAudioIN)
*		void FFT(int framsize, fractional *audioIN, fractcomplex *compX)		
*		void 
*
* Notes:
*		none
*
* Originator: 
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		17/03/2016
*
*/
#ifndef FFT_INIT
#include <dsp.h>
#define FFT_INIT
#define FFT_FRAME_SIZE 256
#define SAMPLING_RATE 8000
#define COEFFS_IN_DATA 0xFF00

void convertInputForFFT(int *audioIN, fractional *convAudioIN);
void FFT(int framesize, fractional *audioIN, fractcomplex *compX);
int pitchDetection(fractcomplex *compXFftResults);

#endif
