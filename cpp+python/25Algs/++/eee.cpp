#include "eee.h"

eee::eee() {
}

eee::eee(int _me, double _lambda, int _numExperts) {
  	me = _me;
	numExperts = _numExperts;//_nActions;
    lambda = _lambda;

    M_e = new double[numExperts];
    N_e = new int[numExperts];
    S_e = new int[numExperts];
    int i;
    for (i = 0; i < numExperts; i++) {
        N_e[i] = 0;
        M_e[i] = 0.0;
        S_e[i] = 0.0;
    }

	t = 0;
    experto = -1;
    
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        x_phi[i] = 0;
        cuenta[i] = 0;
    }
}

eee::~eee() {
    delete M_e;
    delete N_e;
    delete S_e;
}

int eee::select(bool *choices) {
	if (t == 0) {
		experto = randomlySelect(choices);
    }
	else {
        //double prob = 0.1;
        double prob = 1.0 / (10 + (t/10.0));
        double num = ((double)rand()) / RAND_MAX;
        if (num < prob) {
            experto = randomlySelect(choices);
        }
        else {
            double best = -99999;
            experto = -1;
            int bestCount = 0;
            int i;
            //printf("M_e: ");
            for (i = 0; i < numExperts; i++) {
                //printf("%.2lf", M_e[i]);
                if (choices[i]) {
                    //printf("x ");
                    if (M_e[i] > best) {
                        best = M_e[i];
                        experto = i;
                        bestCount = 1;
                    }
                    else if (M_e[i] == best) {
                        bestCount ++;
                    }
                }
                //else
                //    printf(" ");
            }
            
            if (bestCount > 1) {
                int num2 = rand() % bestCount;
                int c = 0;
                for (i = 0; i < numExperts; i++) {
                    if ((M_e[i] == best) && choices[i])
                        c ++;
                    
                    if (c > num2) {
                        experto = i;
                        break;
                    }
                }
            }
            //printf("\n");
        }
    }
    //printf("%i selected %i\n", me, experto);
    
    return experto;
}

int eee::randomlySelect(bool *choices) {
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

void eee::update(double R, int tau) {
    int i;    
    for (i = 0; i < tau; i++) {
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    }
    
    N_e[experto] ++;
    S_e[experto] += tau;
    M_e[experto] += ((double)tau / S_e[experto]) * (R - M_e[experto]);
    
    //printf("updating %i\n", experto);
    x_phi[experto] = M_e[experto];
    cuenta[experto] = N_e[experto];
    
    t++;
}



