#include <stdlib.h>
#include "KalmanFilter.h"

extern void kalmanFilter_asm(float* InputArray, float* OutputArray, int Length, struct kalman_state* kstate);

int main(void) {
	int const length = 6;
	float input[] = {-10, 10, -10, 10, -10, 10};
	float output[length];
	
	struct kalman_state s = {0.1, 0.1, 0.0, 0.1, 0.0};
	
	Kalmanfilter_C(input, output, &s, length);
//kalmanFilter_asm(input, output, length, &s);
	
	return 0;
}
