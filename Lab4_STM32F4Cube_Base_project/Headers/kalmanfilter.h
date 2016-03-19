/**
  ******************************************************************************
  * @file    kalmanfilter.h
  * @author  Auguste Lalande, Felix Dube, Juan Morency Trudel
  * @version 1.0.0
  * @date    February-2016
  * @brief   Implement a 1-D kalman filter
  ******************************************************************************
  */


#ifndef KALMANFILTER_H
#define KALMANFILTER_H

typedef struct kalmanState {
  float q;                /* process noise covariance */
  float r;                /* measurement noise covariance */
  float x;                /* estimated value */
  float p;                /* estimation error covariance */
  float k;                /* adapive Kalman filter gain */
} kalmanState;

void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k);
void kalmanUpdate(kalmanState* kstate, float input);

#endif
