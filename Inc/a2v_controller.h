/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __A2V_CONTROLLER_H
#define __A2V_CONTROLLER_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "main.h"

typedef struct {
	int loudnessOut;	
	int loudnessOut_Base;
	
	int in_vdClamp;
	int in_autoBrake;
	int in_preFFTGain;
	int in_lrGapCutting;
	int in_loudnessThres; 
	int in_cntFreqLow;
	int in_cntFreqMid;
	int in_cntFreqHigh;
	int in_AdaptiveFreq[6];
	
	short wn_FFT[NFFT3_4];  /* for twiddle factor */
	short br_FFT[NFFT];     /* for bit reversal   */
	short duR_1st[NFFT];
	short duI[NFFT];
	int duPow[NFFT2];

	int out_loudnessLow;
	int out_loudnessMid;
	int out_loudnessHigh;	
}parameterA2V;

int DW_A2V_Init(parameterA2V *dw);
int DW_A2V_Ctrl(parameterA2V *dw, short* audioInLib,unsigned char *tuningA2V);

#ifdef __cplusplus
	}
#endif
#endif /* __A2V_CONTROLLER_H */

