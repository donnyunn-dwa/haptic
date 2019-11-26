#include "a2v_controller.h"
#include <math.h>

float hanning_window[NFFT] = {
	0.000000000,0.000037649,0.000150591,0.000338808,0.000602272,0.000940944,0.001354772,0.001843694,
	0.002407637,0.003046515,0.003760233,0.004548682,0.005411745,0.006349291,0.007361179,0.008447256,
	0.009607360,0.010841315,0.012148935,0.013530024,0.014984373,0.016511764,0.018111967,0.019784740,
	0.021529832,0.023346980,0.025235910,0.027196337,0.029227967,0.031330494,0.033503601,0.035746960,
	0.038060234,0.040443074,0.042895122,0.045416008,0.048005353,0.050662767,0.053387849,0.056180190,
	0.059039368,0.061964953,0.064956504,0.068013572,0.071135695,0.074322403,0.077573217,0.080887647,
	0.084265194,0.087705349,0.091207593,0.094771401,0.098396234,0.102081548,0.105826786,0.109631386,
	0.113494773,0.117416367,0.121395577,0.125431803,0.129524437,0.133672864,0.137876459,0.142134587,
	0.146446609,0.150811875,0.155229728,0.159699501,0.164220523,0.168792111,0.173413579,0.178084229,
	0.182803358,0.187570256,0.192384205,0.197244479,0.202150348,0.207101071,0.212095904,0.217134095,
	0.222214883,0.227337506,0.232501190,0.237705159,0.242948628,0.248230808,0.253550904,0.258908114,
	0.264301632,0.269730645,0.275194335,0.280691881,0.286222453,0.291785220,0.297379343,0.303003980,
	0.308658284,0.314341403,0.320052482,0.325790660,0.331555073,0.337344854,0.343159130,0.348997025,
	0.354857661,0.360740155,0.366643621,0.372567170,0.378509910,0.384470946,0.390449380,0.396444312,
	0.402454839,0.408480056,0.414519056,0.420570928,0.426634763,0.432709646,0.438794662,0.444888896,
	0.450991430,0.457101344,0.463217718,0.469339632,0.475466163,0.481596389,0.487729386,0.493864231,
	0.500000000,0.506135769,0.512270614,0.518403611,0.524533837,0.530660368,0.536782282,0.542898656,
	0.549008570,0.555111104,0.561205338,0.567290354,0.573365237,0.579429072,0.585480944,0.591519944,
	0.597545161,0.603555688,0.609550620,0.615529054,0.621490090,0.627432830,0.633356379,0.639259845,
	0.645142339,0.651002975,0.656840870,0.662655146,0.668444927,0.674209340,0.679947518,0.685658597,
	0.691341716,0.696996020,0.702620657,0.708214780,0.713777547,0.719308119,0.724805665,0.730269355,
	0.735698368,0.741091886,0.746449096,0.751769192,0.757051372,0.762294841,0.767498810,0.772662494,
	0.777785117,0.782865905,0.787904096,0.792898929,0.797849652,0.802755521,0.807615795,0.812429744,
	0.817196642,0.821915771,0.826586421,0.831207889,0.835779477,0.840300499,0.844770272,0.849188125,
	0.853553391,0.857865413,0.862123541,0.866327136,0.870475563,0.874568197,0.878604423,0.882583633,
	0.886505227,0.890368614,0.894173214,0.897918452,0.901603766,0.905228599,0.908792407,0.912294651,
	0.915734806,0.919112353,0.922426783,0.925677597,0.928864305,0.931986428,0.935043496,0.938035047,
	0.940960632,0.943819810,0.946612151,0.949337233,0.951994647,0.954583992,0.957104878,0.959556926,
	0.961939766,0.964253040,0.966496399,0.968669506,0.970772033,0.972803663,0.974764090,0.976653020,
	0.978470168,0.980215260,0.981888033,0.983488236,0.985015627,0.986469976,0.987851065,0.989158685,
	0.990392640,0.991552744,0.992638821,0.993650709,0.994588255,0.995451318,0.996239767,0.996953485,
	0.997592363,0.998156306,0.998645228,0.999059056,0.999397728,0.999661192,0.999849409,0.999962351,
	1.000000000,0.999962351,0.999849409,0.999661192,0.999397728,0.999059056,0.998645228,0.998156306,
	0.997592363,0.996953485,0.996239767,0.995451318,0.994588255,0.993650709,0.992638821,0.991552744,
	0.990392640,0.989158685,0.987851065,0.986469976,0.985015627,0.983488236,0.981888033,0.980215260,
	0.978470168,0.976653020,0.974764090,0.972803663,0.970772033,0.968669506,0.966496399,0.964253040,
	0.961939766,0.959556926,0.957104878,0.954583992,0.951994647,0.949337233,0.946612151,0.943819810,
	0.940960632,0.938035047,0.935043496,0.931986428,0.928864305,0.925677597,0.922426783,0.919112353,
	0.915734806,0.912294651,0.908792407,0.905228599,0.901603766,0.897918452,0.894173214,0.890368614,
	0.886505227,0.882583633,0.878604423,0.874568197,0.870475563,0.866327136,0.862123541,0.857865413,
	0.853553391,0.849188125,0.844770272,0.840300499,0.835779477,0.831207889,0.826586421,0.821915771,
	0.817196642,0.812429744,0.807615795,0.802755521,0.797849652,0.792898929,0.787904096,0.782865905,
	0.777785117,0.772662494,0.767498810,0.762294841,0.757051372,0.751769192,0.746449096,0.741091886,
	0.735698368,0.730269355,0.724805665,0.719308119,0.713777547,0.708214780,0.702620657,0.696996020,
	0.691341716,0.685658597,0.679947518,0.674209340,0.668444927,0.662655146,0.656840870,0.651002975,
	0.645142339,0.639259845,0.633356379,0.627432830,0.621490090,0.615529054,0.609550620,0.603555688,
	0.597545161,0.591519944,0.585480944,0.579429072,0.573365237,0.567290354,0.561205338,0.555111104,
	0.549008570,0.542898656,0.536782282,0.530660368,0.524533837,0.518403611,0.512270614,0.506135769,
	0.500000000,0.493864231,0.487729386,0.481596389,0.475466163,0.469339632,0.463217718,0.457101344,
	0.450991430,0.444888896,0.438794662,0.432709646,0.426634763,0.420570928,0.414519056,0.408480056,
	0.402454839,0.396444312,0.390449380,0.384470946,0.378509910,0.372567170,0.366643621,0.360740155,
	0.354857661,0.348997025,0.343159130,0.337344854,0.331555073,0.325790660,0.320052482,0.314341403,
	0.308658284,0.303003980,0.297379343,0.291785220,0.286222453,0.280691881,0.275194335,0.269730645,
	0.264301632,0.258908114,0.253550904,0.248230808,0.242948628,0.237705159,0.232501190,0.227337506,
	0.222214883,0.217134095,0.212095904,0.207101071,0.202150348,0.197244479,0.192384205,0.187570256,
	0.182803358,0.178084229,0.173413579,0.168792111,0.164220523,0.159699501,0.155229728,0.150811875,
	0.146446609,0.142134587,0.137876459,0.133672864,0.129524437,0.125431803,0.121395577,0.117416367,
	0.113494773,0.109631386,0.105826786,0.102081548,0.098396234,0.094771401,0.091207593,0.087705349,
	0.084265194,0.080887647,0.077573217,0.074322403,0.071135695,0.068013572,0.064956504,0.061964953,
	0.059039368,0.056180190,0.053387849,0.050662767,0.048005353,0.045416008,0.042895122,0.040443074,
	0.038060234,0.035746960,0.033503601,0.031330494,0.029227967,0.027196337,0.025235910,0.023346980,
	0.021529832,0.019784740,0.018111967,0.016511764,0.014984373,0.013530024,0.012148935,0.010841315,
	0.009607360,0.008447256,0.007361179,0.006349291,0.005411745,0.004548682,0.003760233,0.003046515,
	0.002407637,0.001843694,0.001354772,0.000940944,0.000602272,0.000338808,0.000150591,0.000037649
};


unsigned char	awf_unit_l[19] =
{
	1,2,2,3,3,4,5,6,8,10,13,15,20,25,31,38,49,61,127
};

int awfa_l[19] = {
	52, 58, 60, 61, 65, 66, 67, 68, 66, 65,
	68, 72, 72, 71, 67, 52, 57, 56, 60
};

int sqrtI2I( int v )
{
    int t, q, b, r;
    r = v;           				// r = v - x�
    b = 0x40000000;  				// a�
    q = 0;           				// 2ax
    while( b > 0 )				
    {				
        t = q + b;   				// t = 2ax + a�
        q >>= 1;     				// if a' = a/2, then q' = q/2
        if( r >= t ) 				// if (v - x�) >= 2ax + a�
        {				
            r -= t;  				// r' = (v - x�) - (2ax + a�)
            q += b;  				// if x' = (x + a) then ax' = ax + a�, thus q' = q' + b
        }				
        b >>= 2;     				// if a' = a/2, then b' = b / 4
    }
    return q;
}

void swap(short *a, short *b)
{
    short tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

short Rnd(int x, short s) 
{ 
	return (x + (1<<(s-1)))>>s; 
}

short shortSin(short x)
{
    const short int a1 =  18704;      /* Q15, a1 - 1 */
    const short int a3 = -21165;      /* Q15         */
    const short int a5 =   2603;      /* Q15         */
    const short int a7 =   -142;      /* Q15         */
    short x2;
		int	abs_x;
    int acc;

		abs_x =  x < 0 ? -x : x;

    if ( abs_x > 0x4000) x = 0x8000 - x;   /* -2 - x */
    x2  = Rnd(x*x, 14);
    acc = (a5<<14) + a7*x2;
    acc = (a3<<14) + Rnd(acc, 14)*x2;
    acc = (a1<<14) + Rnd(acc, 14)*x2;
    acc = (x<<15)  + Rnd(acc, 14)*x;

    return Rnd(acc, 15);
}

void fftFixTable(short *wn_FFT, short *br_FFT, int N_FFT)
{
		int i, n_half, ne, jp;
    short arg;
	// Calculation of twiddle factor
    arg = 4*(16384/N_FFT);    /* Q14 */
    for (i=0; i<((N_FFT*3)>>2); i++) wn_FFT[i] = shortSin(arg*i+16384);
	// Calculation of bit reversal table
    n_half = N_FFT>>1;
    br_FFT[0] = 0;
    for (ne=1; ne<N_FFT; ne=ne<<1)
    {
        for (jp=0; jp<ne; jp++) br_FFT[jp+ne] = br_FFT[jp] + n_half;
        n_half = n_half>>1;
    }
}

void fftFix(short *xR, short *xI, short *wn_FFT, short *br_FFT, int N_FFT)		//	BufferFly cal
{
    int   xtmpR, xtmpI;  /* must be not short but int */
    short xtmpRs, xtmpIs;
    int   j, jnh, k, jxC, jxS, ne, n_half, n_half2;

		n_half = N_FFT>>1;
		for (ne=1; ne<N_FFT; ne=ne<<1)
		{
			n_half2 = n_half<<1;
			for (k=0; k<N_FFT; k=k+n_half2)
			{
				jxC = 0;
				jxS = N_FFT>>2;
				for (j=k; j<(k+n_half); j++)
				{
					jnh = j + n_half;
	// beginning of butterfly operations 
					xtmpR = xR[j];
					xtmpI = xI[j];
					xR[j] = (xtmpR + xR[jnh])>>1;
					xI[j] = (xtmpI + xI[jnh])>>1;
					xtmpRs = (xtmpR - xR[jnh])>>1;
					xtmpIs = (xtmpI - xI[jnh])>>1;
					xR[jnh] = Rnd(xtmpRs*wn_FFT[jxC] - xtmpIs*wn_FFT[jxS], 14);
					xI[jnh] = Rnd(xtmpRs*wn_FFT[jxS] + xtmpIs*wn_FFT[jxC], 14);
	//end of butterfly operations
					jxC = jxC + ne;
					jxS = jxS + ne;
				}
			}
			n_half = n_half>>1;
		}
	
	// Bit reverse */
	for (j=0; j<N_FFT; j++)
	{
		if (j<br_FFT[j])
		{
			swap(&xR[j], &xR[br_FFT[j]]);
			swap(&xI[j], &xI[br_FFT[j]]);
		}
	}
}

void calcPowerSpectrum(int *xp, short *xr, short *xi)
{
	int i;

	for (i = 0; i < NFFT2; i++)
	{
		xp[i] = sqrtI2I((int)xr[i] * (int)xr[i] + (int)xi[i] * (int)xi[i]);	
	}
}

void fftFunc(parameterA2V *dw, short *adcFFTin_func)
{	
	for(int w = 0; w < NFFT; w++)
		dw->duR_1st[w] = adcFFTin_func[w];													// FFT FIFO
	
	fftFix(dw->duR_1st, dw->duI, dw->wn_FFT, dw->br_FFT, NFFT);			// FixedPoint FFT
	calcPowerSpectrum(dw->duPow, dw->duR_1st, dw->duI);							// Power spectrum Calibration Output : dw.duPow
}

#define CNT_EQ1 3
#define CNT_EQ2 5
#define CNT_EQ3 9
#define CNT_EQ4 18
#define CNT_EQ5 35
#define CNT_EQ6 69
void adaptEq(int *fft_duPow,int *SoundFreq_t)
{
	int i = 0;
	
	fft_duPow[i++] = 0;
	
	for(;i < CNT_EQ1; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[0] / 128) - 2));

	for(;i < CNT_EQ2; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[1] / 128) - 2));

	for(;i < CNT_EQ3; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[2] / 128) - 2));

	for(;i < CNT_EQ4; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[3] / 128) - 2));

	for(;i < CNT_EQ5; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[4] / 128) - 2));

	for(;i < CNT_EQ6; i++)
		fft_duPow[i] = fft_duPow[i] + (int)(fft_duPow[i] * (exp(SoundFreq_t[5] / 128) - 2));

	for(;i < NFFT2; i++)
		fft_duPow[i] = 0;
}

int loudnCalc(int *fftPow,parameterA2V *dw)
{
	int loudness;
	int loudness_temp;
	int i, cnt;

	i = 1;
	loudness = 0;

	loudness_temp = 0;
	cnt = 0;
	for(; i < dw->in_cntFreqLow; i++, cnt++) {
		loudness_temp += fftPow[i];
	}
	dw->out_loudnessLow = (int)(loudness_temp / cnt);
	loudness += dw->out_loudnessLow;

	loudness_temp = 0;
	cnt = 0;
	for(; i < dw->in_cntFreqMid; i++, cnt++) {
		loudness_temp += fftPow[i];
	}
	dw->out_loudnessMid = (int)(loudness_temp / cnt);
	loudness += dw->out_loudnessMid;

	loudness_temp = 0;
	cnt = 0;
	for(; i < dw->in_cntFreqHigh; i++, cnt++) {
		loudness_temp += fftPow[i];
	}
	dw->out_loudnessHigh = (int)(loudness_temp / cnt);
	loudness += dw->out_loudnessHigh;
/*
	loudness_temp = 0;
	cnt = 0;
	for(; i < CNT_EQ6; i++, cnt++) {
		loudness_temp += fftPow[i];
	}
	loudness += (int)(loudness_temp / cnt);
*/
	return loudness;
}

#define HYST 10
int vibFreqGenSet(parameterA2V *dw)
{
	int loudness_cmp;
	int runFlag;

	loudness_cmp = dw->loudnessOut - dw->loudnessOut_Base;

	if(loudness_cmp < (dw->in_loudnessThres - HYST)) {
		runFlag = 0;
	} else {
		runFlag = 1;
	}

	/*
	if(running) {
		if(loudness_cmp < (dw->in_loudnessThres - HYST)) {
			runFlag = 0;
		}		
	} else {
		if(loudness_cmp > (dw->in_loudnessThres + HYST)) {
			runFlag = 1;
		} else {
			if(loudness_cmp > 0) {
				dw->loudnessOut_Base = (15*dw->loudnessOut_Base + dw->loudnessOut) / 16;
			} else {
				dw->loudnessOut_Base = dw->loudnessOut;
			}
		}
	}*/

	return runFlag;
}

//----------------- DW_A2V_Ctrl()------------------------------------------
//	Parameter : 
//							1. audioInLib  	: audio Input signal
//							2. tuningA2V 		: tuning data for A2V
//  Output 		: dw.a2vVibOut		: vibration data for A2V (-128 ~ 127)
// ------------------------------------------------------------------------
int DW_A2V_Ctrl(parameterA2V *dw, short* audioInLib,unsigned char *tuningA2V)
{
	int cnt = 0;
	
	dw->in_vdClamp = tuningA2V[cnt++];
	dw->in_autoBrake = tuningA2V[cnt++];
	dw->in_preFFTGain = tuningA2V[cnt++]; 
	dw->in_lrGapCutting = tuningA2V[cnt++];
	dw->in_loudnessThres = tuningA2V[cnt++];
	dw->in_cntFreqLow = tuningA2V[cnt++];
	dw->in_cntFreqMid = tuningA2V[cnt++];
	dw->in_cntFreqHigh = tuningA2V[cnt++];
	
	for(int i = 0; i < 6; i++) {		
			dw->in_AdaptiveFreq[i] = (int)tuningA2V[cnt++];
	}

	// Hanning Windowing
	for(int i = 0; i < NFFT; i++) {
		audioInLib[i] = (short)(audioInLib[i]*hanning_window[i]);
	}

	// pre-gain
	for(int i = 0; i < NFFT; i++) {
		audioInLib[i] = audioInLib[i] * dw->in_preFFTGain;
	}

	// control
	fftFunc(dw, audioInLib);			// start FFT 
	adaptEq(dw->duPow,dw->in_AdaptiveFreq);			// equalizer
	//dw->loudnessOut_R = ((31*dw->loudnessOut_R) + (loudnCalc(dw->duPow,dw)))/32;										// loudness Calibartion
	dw->loudnessOut = loudnCalc(dw->duPow,dw);
	return vibFreqGenSet(dw);													// set vibration amp & freq 

}

int DW_A2V_Init(parameterA2V *dw)
{
	int ret;

	fftFixTable(dw->wn_FFT,dw->br_FFT,NFFT);
	ret = 0;

	return ret;
}

