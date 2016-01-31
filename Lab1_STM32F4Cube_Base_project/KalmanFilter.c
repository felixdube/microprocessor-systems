#include <stdlib.h>

struct kalman_state {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
};

int Kalmanfilter_C(float* InputArray, float* OutputArray, struct kalman_state* kstate,int Length) {
	int i;
	for (i = 0; i < Length; i++) {
		kstate->p += kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x += kstate->k * (InputArray[i] - kstate->x);
		kstate->p = (1 - kstate->k) * kstate->p;
		OutputArray[i] = kstate->x;
	}
	return 0;
}