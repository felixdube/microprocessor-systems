/*************************************************************************************/
// File: KalmanFilter.c
// Description: Implement a 1D kalman filter
// Date: February, 2016
// Author: August Lalande, Juan Morency Trudel, Felix Dube
/*************************************************************************************/

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

