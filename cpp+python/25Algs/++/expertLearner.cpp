#include "expertLearner.h"

expertLearner::expertLearner() {
}

expertLearner::~expertLearner() {
}

int expertLearner::select(bool *choices) {
	return 0;
}

void expertLearner::update(double R, int tau) {
}

double expertLearner::computeERegret(double ave) {
    // see how much e-Regret the agent has
    int i;
    
    double val = 0;
    for (i = 0; i < numExperts; i++) {
        //printf("%i: %lf (%i)\n", i, x_phi[i], cuenta[i]);
        if (val < x_phi[i])
            val = x_phi[i];
    }
    //printf("\n");
    
    return val - ave;
}


