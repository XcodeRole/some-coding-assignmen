#include "a.h"

a::a() {
    printf("incomplete constructor\n");
    exit(1);
}

a::a(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda, int _numExperts) {
    //int i, j;
    
    me = _me;
    numExperts = _numExperts;
    
	lambda = _lambda;
    
/*	aspiration = -99999;
	for (i = 0; i < _A[0]; i++) {
		for (j = 0; j < _A[1]; j++) {
			if (_M[me][i][j] > aspiration)
				aspiration = _M[me][i][j];
		}
	}
*/
    
    //printf("aspiration = %.3lf\n", aspiration);
    lastExpert = -1;
    
    int i;
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    S_e = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        x_phi[i] = 0;
        cuenta[i] = 0;
        S_e[i] = 0.0;
    }
}

a::~a() {}

int a::select(bool *choices) {
	if (lastExpert == -1)
        lastExpert = randomlySelect(choices);
    else {
        if (!choices[lastExpert])
            rho = -1.0; // takes care of override
        
        double num = ((double)(rand())) / RAND_MAX;
        if (num > rho) 
            lastExpert = randomlySelect(choices);
    }
	
	return lastExpert;
}

int a::randomlySelect(bool *choices) {
    int i, cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i])
            cnt ++;
    }
    
    if (cnt == 0) {
        printf("no choices\naspiration = %lf\n", aspiration);
        exit(1);
    }
    
    //printf("%i: cnt = %i; aspiration = %lf\n", me, cnt, aspiration);
    
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

void a::update(double R, int tau) {
    int i;
    for (i = 0; i < tau; i++)
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    
	rho = R / aspiration;
	if (rho > 1.0)
		rho = 1.0;
	
	rho = pow(rho, tau);
    
    //printf("%i: aspiration = %.3lf; R = %.3lf; tau = %i; rho = %.3lf\n", me, aspiration, R, tau, rho);
    
    S_e[lastExpert] += tau;
    x_phi[lastExpert] += ((double)tau / S_e[lastExpert]) * (R - x_phi[lastExpert]);
    cuenta[lastExpert] ++;    
}

