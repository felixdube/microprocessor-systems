#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "KalmanFilter.h"
#include <fenv.h>

int Kalmanfilter_C(float* InputArray, float* OutputArray, struct kalman_state* kstate, int Length) {
	int i;
	for (i = 0; i < Length; i++) {
//		printf("q: %f, r: %f, x: %f, p: %f, k: %f\n", kstate->q, kstate->r, kstate->x, kstate->p, kstate->k);
		kstate->p += kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x += kstate->k * (InputArray[i] - kstate->x);
		kstate->p = (1 - kstate->k) * kstate->p;
		OutputArray[i] = kstate->x;
//		printf("%f\n", kstate->x);
	}
	if (fetestexcept(FE_INVALID) || fetestexcept(FE_OVERFLOW)) return 15;
	else return 10;
}

void home_sub_f32 (float *inputA, float *inputB, float *output, int length) {
	int i;
	for(i = 0; i < length; i++) {
		output[i] = inputA[i] - inputB[i];
	}
}

void home_std_f32 (float *input, int length, float *output){
	int i;
	float sumOfSquare = 0;
	float sum = 0;
	
	for(i = 0; i < length; i++){
		sumOfSquare = sumOfSquare + input[i]*input[i];
	}
	
	for (i = 0; i < length; i++) {
		sum = sum + input[i];
	}
	
	*output = sqrt((sumOfSquare - sum*sum/length) / (length -1));
}

void home_mean_f32 (float *input, int length, float *output) {
	int i;
	float sum = 0;
	
	for (i = 0; i < length; i++) {
		sum = sum + input[i];
	}
	
	*output = sum/length;
}

void home_correlate_f32 (float *inputA, float *inputB, int length, float *output) {
	int i;
	int j;
	
	for (i = 0; i < (2*length-1); i++) {
		for (j = 0; j < length; j++){
			output[i] = inputA[j]*inputB[j-i];
		}
	}
}

void home_conv_f32 (float *inputA, float *inputB, int length, float *output) {
	int i;
	int j;
	
	for (i = 0; i < (2*length-1); i++) {
		for (j = 0; j < length; j++){
			output[i] = inputA[j]*inputB[i-j];
		}
	}
}
