#include <stdlib.h>
#include <stdio.h>
#include "KalmanFilter.h"

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
	return 0;
}
