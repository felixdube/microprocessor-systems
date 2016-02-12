/**
  ******************************************************************************
  * File Name          : kalmanfilter.h
  * Description        : Implement a 1D kalman filter
	* Author						 : Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version            : 1.0.0
	* Date							 : February, 2016
  ******************************************************************************
  */
	

#ifndef KALMANFILTER_H
#define KALMANFILTER_H

typedef struct kalmanState {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
} kalmanState;

void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k);
int kalmanUpdate(kalmanState* kstate, float input);

#endif
