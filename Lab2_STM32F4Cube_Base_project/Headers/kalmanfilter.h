/**
  ******************************************************************************
  * File Name          : kalmanfilter.h
  * Description        : kalmanfilter.c
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	

#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#define INIT_q 0.001 // this parameter controls the speed of convergence
#define INIT_r 2 // this parameter controls the speed of convergence
#define INIT_x 1075 //this is approximetely 35 C as a starting value
#define INIT_p 0.044224 //this was set to the value it converges to when the filter runs for a while
#define INIT_k 0.022112 //this was set to the value it converges to when the filter runs for a while

typedef struct kalmanState {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
} kalmanState;

void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k);
void kalmanUpdate(kalmanState* kstate, float input);

#endif
