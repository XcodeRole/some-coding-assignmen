#include "ucb.h"

ucb::ucb() {
}

ucb::ucb(int _me, double _lambda, int _numExperts) {
  	me = _me;
	numExperts = _numExperts;//_nActions;
    lambda = _lambda;

	x = new double[numExperts];
	n = new int[numExperts];

	int i;
	for (i = 0; i < numExperts; i++) {
		n[i] = 0;
		x[i] = 0.0;
	}
    num = 0;

	t = 0;
    experto = -1;
    
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        x_phi[i] = 0;
        cuenta[i] = 0;
    }
}

ucb::~ucb() {
    delete n;
    delete x;
}

int ucb::select(bool *choices) {
	if (t == 0)
		return experto = randomlySelect(choices);
	
	bool *left = new bool[numExperts];
	int i, numLeft = 0;
	for (i = 0; i < numExperts; i++) {
		if ((n[i] == 0) && choices[i]) {
			left[i] = true;
			numLeft ++;
		}
		else
			left[i] = false;
	}
	
	if (numLeft != 0) {
		experto = rand() % numExperts;
		while (!(left[experto]))
			experto = rand() % numExperts;
		delete[] left;
        
		//if (me == 0)
		//	printf("%i: strat %i; %.3lf; %i\n", me, a, x[a], n[a]);
		
		return experto;
	}
    
	delete[] left;
	
	experto = -1;
	double best = -99999;//(x[experto] / n[experto]) + sqrt(2.0 * log((double)num) / n[experto]);
	//printf("0: %.3lf (%i; %.3lf)\n", best, n[0], x[0] / n[0]);
	double val;
	for (int i = 0; i < numExperts; i++) {
        if (choices[i]) {
            val = (x[i] / n[i]) + sqrt(2.0 * log((double)num) / n[i]);
            //printf("%i: %.3lf (%i; %.3lf)\n", i, val, n[i], x[i] / n[i]);
            if ((val > best) || ((val == best) && (rand() % 2))) {
                experto = i;
                best = val;
            }
        }
	}
    
	//if (me == 0)
	//	printf("%i: strat %i; %.3lf; %i\n", me, a, x[a], n[a]);	
	//printf("experto = %i\n", experto);
    //fflush(stdout);
    
    return experto;
}

int ucb::randomlySelect(bool *choices) {
    int i, cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i])
            cnt ++;
    }
    
    int pick = rand() % cnt;
    cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i]) {
            if (cnt == pick)
                return i;
            cnt ++;
        }
    }
    
    printf("we have a problem\n");
    exit(1);
    
    return -1;
}

void ucb::update(double R, int tau) {
    //printf("update experto %i\n", experto);
    //fflush(stdout);
    
    int i;    
    for (i = 0; i < tau; i++)
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    
    x[experto] += R * tau;
    n[experto] += tau;
    num += tau;
    
    t++;
    
    x_phi[experto] += ((double)tau / n[experto]) * (R - x_phi[experto]);
    cuenta[experto] ++;    
}



