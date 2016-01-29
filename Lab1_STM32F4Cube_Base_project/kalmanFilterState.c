struct state {
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //estimated value
	float p; //estimation error covariance
	float k; //adapive Kalman filter gain
};