/*************************************************************************************/
// File: kalmanfilter.c
// Description: Implement a 1D kalman filter
// Date: February, 2016
// Author: August Lalande, Juan Morency Trudel, Felix Dube
/*************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fenv.h>

#include "kalmanfilter.h"

void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k) {
	kstate->q = q;
	kstate->r = r;
	kstate->x = x;
	kstate->p = p;
	kstate->k = k;
}

int kalmanUpdate(kalmanState* kstate, float input) {
	kstate->p += kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x += kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;
	
	if (fetestexcept(FE_INVALID) || fetestexcept(FE_OVERFLOW)) return 15;
	else return 0;
}
