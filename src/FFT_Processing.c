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
#include "..\inc\FFT_processing.h"
#include <dsp.h>

fractcomplex FFTcompTwidFactors[FFT_FRAME_SIZE]__attribute__ ((space(xmemory),far));
fractcomplex FFTcompWorkSpace[FFT_FRAME_SIZE]__attribute__ ((space(ymemory),far));

int	peakFrequencyBin = 0;

void convertInputForFFT(int *audioIN, fractional *convAudioIN)
{
	convAudioIN=audioIN;
}

void FFT(int framesize, fractional *audioIN, fractcomplex *FFTcompResult)
{
	int i;
	
	//copy fractional audio signal into real part of complex fractional data type
	for(i=0;i<framesize;i++)
	{
		FFTcompWorkSpace[i].real = audioIN[i];
		FFTcompWorkSpace[i].imag = 0;
	}	
	
	//generate the first half of the set of twiddle factors required by the DFT
	TwidFactorInit (8,FFTcompTwidFactors,0);

	//generate the DFT of the audio signal
	FFTComplex(8,FFTcompResult,FFTcompWorkSpace,FFTcompTwidFactors,0xFF00);

}

int pitchDetection(fractcomplex *compXFftResults)
{
	int detectedPitch=0;
	int detectedPitchLvl=0;
	fractional FftResults[FFT_FRAME_SIZE];

	/* Compute the square magnitude of the complex FFT output array so we have a Real output vetor */
	SquareMagnitudeCplx(FFT_FRAME_SIZE/2, &compXFftResults[0], &FftResults[0]);

	/* Find the frequency Bin ( = index into the SigCmpx[] array) that has the largest energy*/
	/* i.e., the largest spectral component */
	VectorMax(FFT_FRAME_SIZE/2, &FftResults[0], &peakFrequencyBin);

	/* Compute the frequency (in Hz) of the largest spectral component */
	detectedPitch = peakFrequencyBin*(SAMPLING_RATE/FFT_FRAME_SIZE);
	
	if(detectedPitch<=800)
	{
		detectedPitchLvl=0; //Low pitch level
	}
	else if(detectedPitch<=1600)
	{
		detectedPitchLvl=1; //Low\Medium pitch level
	}
	else if(detectedPitch<=2400)
	{
		detectedPitchLvl=2; //Medium pitch level
	}
	else if(detectedPitch<=3200)
	{
		detectedPitchLvl=3; //Medium\High pitch level
	}
	else if(detectedPitch<=4000)
	{
		detectedPitchLvl=4; //High pitch level
	}
	else
	{
		detectedPitchLvl=5; //Error occured
	}

	return detectedPitchLvl;
}
