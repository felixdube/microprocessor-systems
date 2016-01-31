#include <stdlib.h>

struct state {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
};

int main(){
 struct state instance1;
	instance1.q = 1;
	instance1.r = 2;
	instance1.x = 3;
	instance1.p = 5;
	instance1.k = 4;
		
}