#ifndef KALMANFILTER_H
#define KALMANFILTER_H

typedef struct kalman_state {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
} kalman_state;

int Kalmanfilter_C(float* InputArray, float* OutputArray, struct kalman_state* kstate, int Length);

void home_sub_f32 (float *inputA, float *inputB, float *output, int length);

void home_std_f32 (float *input, int length, float *output);

void home_mean_f32 (float *input, int length, float *output);

void home_correlate_f32 (float *inputA, float *inputB, int length, float *output);

void home_conv_f32 (float *inputA, float *inputB, int length, float *output);

#endif
