/**
  ******************************************************************************
  * File Name          : kalmanfilter.c
  * Description        : Implement a 1D kalman filter
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fenv.h>

#include "kalmanfilter.h"


/**
	* @brief Initialize the kalman filter
	* @param q: Process noise covariance
	* @param r: Measurement noise covariance
	* @param x: Estimated value
	* @param p: Estimation error cavariance
	* @param k: Adaptive kalman filter gain
	* @retval None
	*/
void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k) {
	kstate->q = q;
	kstate->r = r;
	kstate->x = x;
	kstate->p = p;
	kstate->k = k;
}

/**
	* @brief Update the kalman state and filter measured value
	* @param kstate: kalmanState struct instance needing update
	* @param input: input value of the filter
	* @retval None
	*/
void kalmanUpdate(kalmanState* kstate, float input) {
	kstate->p += kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x += kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;
	return;
}
