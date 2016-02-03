#include <stdlib.h>
#include <stdio.h>
#include "KalmanFilter.h"
#include "arm_math.h"

extern void kalmanFilter_asm(float* InputArray, float* OutputArray, int Length, struct kalman_state* kstate);

int main(void) {
	int i;
	int const length = 6;
	float input[] = {-10, 10, -10, 10, -10, 10};
	float output[length];
	
	// output for substraction
	float arm_sub[length];
	float home_sub[length];
	
	// outputs for standard deviation
	float arm_std;
	float home_std;
	
	// outputs for average
	float arm_mean;
	float home_mean;
	
	// outputs for correlation
	float arm_cor[2*length -1];
	float home_cor[2*length -1];
	
	// outputs for convolution
	float arm_conv[2*length -1];
	float home_conv[2*length -1];
	
	struct kalman_state s = {0.1, 0.1, 0.0, 0.1, 0.0};
	
	Kalmanfilter_C(input, output, &s, length);
//kalmanFilter_asm(input, output, length, &s);
	
	/*** PART A ***/
	// substraction
	printf("---- Substration ----\n");
	arm_sub_f32(input, output, arm_sub, length);
	for (i = 0; i < length; i++) {
		printf("%f, ", arm_sub[i]);
	}
	printf("\n");
	
	home_sub_f32(input, output, home_sub, length);
	for (i = 0; i < length; i++) {
		printf("%f, ", home_sub[i]);
	}
	printf("\n");
	
	/*** PART B ***/
	// standard deviation
	printf("---- Sandard Deviation ----\n");
	arm_std_f32(arm_sub, length, &arm_std);
	printf("%f \n", arm_std);
	
	home_std_f32(home_sub, length, &home_std);
	printf("%f \n", home_std);
	
	// average
	printf("---- Average ----\n");
	arm_mean_f32(arm_sub, length, &arm_mean);
	printf("%f \n", arm_mean);
	
	home_mean_f32(home_sub, length, &home_mean);
	printf("%f \n", home_mean);
	
		/*** PART C ***/
	// correlation
	printf("---- Correlation ----\n");
	arm_correlate_f32(input, length, output, length, arm_cor);
	for (i = 0; i < (2*length-1) ; i++) {
		printf("%f, ", arm_cor[i]);
	}
	printf("\n");
	
	home_correlate_f32(input, output, length, home_cor);
	for (i = 0; i < (2*length-1) ; i++) {
		printf("%f, ", home_cor[i]);
	}
	printf("\n");
	
	/*** PART D ***/
	// convolution
	printf("---- Convolution ----\n");
	arm_conv_f32(input, length, output, length, arm_conv);
	for (i = 0; i < (2*length-1) ; i++) {
		printf("%f, ", arm_conv[i]);
	}
	printf("\n");
	
	home_conv_f32(input, output, length, home_conv);
	for (i = 0; i < (2*length-1) ; i++) {
		printf("%f, ", home_conv[i]);
	}
	printf("\n");
	
	return 0;
}
