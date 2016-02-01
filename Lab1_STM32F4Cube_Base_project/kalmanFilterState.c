#include <stdlib.h>

struct stateVariables {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
};

int kalmanFilterState(void){
 struct stateVariables s;
	s.q = 0.1;
	s.r = 0.1;
	s.x = 0.0;
	s.p = 0.1;
	s.k = 0.0;
	return (0);
		}
