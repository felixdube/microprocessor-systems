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

#endif
