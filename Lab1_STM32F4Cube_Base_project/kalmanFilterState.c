#include <stdlib.h>
#include "KalmanFilter.h"

struct kalman_state *InitKalmanState(void) {
	struct kalman_state *s = (struct kalman_state*) malloc(sizeof(struct kalman_state));
	s->q = 0.1;
	s->r = 0.1;
	s->x = 0.0;
	s->p = 0.1;
	s->k = 0.0;
	return s;
}
