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

#define INIT_q 0.5 				/* this parameter controls the speed of convergence the higher, the faster it converges */
#define INIT_r 2 					/* this parameter controls the speed of convergence */
#define INIT_x 0 			  	/* starting on a flat surface in mg */
#define INIT_y 0 			  	/* starting on a flat surface in mg */
#define INIT_z 1000 			/* starting on a flat surface in mg */
#define INIT_p 0.780776 	/* this was set to the value it converges to when the filter runs for a while */
#define INIT_k 0.390388 	/* this was set to the value it converges to when the filter runs for a while */


typedef struct kalmanState {
	float q; 								/* process noise covariance */
	float r; 								/* measurement noise covariance */
	float x; 								/* estimated value */
	float p; 								/* estimation error covariance */
	float k;								/* adapive Kalman filter gain */
} kalmanState;

void kalmanInit(kalmanState* kstate, float q, float r, float x, float p, float k);
void kalmanUpdate(kalmanState* kstate, float input);

#endif
