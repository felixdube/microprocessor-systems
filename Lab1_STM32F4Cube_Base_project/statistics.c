/*************************************************************************************/
// File: statistics.c
// Description:  	Implement substraction, stadard deviation, average, correlation
//								convolution of 32bits float array
// Date: February, 2016
// Author: August Lalande, Juan Morency Trudel, Felix Dube
/*************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "statistics.h"
#include <fenv.h>

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
	
	for (i = 0; i < (2 * length - 1); i++) {
		for (j = 0; j < length; j++){
			output[i] = inputA[j]*inputB[j-i];
		}
	}
}

void home_conv_f32 (float *inputA, float *inputB, int length, float *output) {
	int i;
	int j;
	
	for (i = 0; i < (2 * length - 1); i++) {
		output[i] = 0;
		for (j = 0; j < length; j++){
			if (i - j >= 0 && i - j < length) {
				output[i] += inputA[j] * inputB[i-j];
			}
		}
	}
}
