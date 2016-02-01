#include <stdlib.h>

struct state {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
};

int kalmanFilterState(void){
 struct state instance1;
	instance1.q = 1.0;
	instance1.r = 2.0;
	instance1.x = 3.0;
	instance1.p = 5.0;
	instance1.k = 4.0;
	return (0);
		}
