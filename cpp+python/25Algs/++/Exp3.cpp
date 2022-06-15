#include "Exp3.h"

Exp3::Exp3() {
}

Exp3::Exp3(int _me, double _T, double _lambda, int _numExperts) {
	K = numExperts = _numExperts;//_nActions;
	me = _me;
    lambda = _lambda;

	G = new double[K];
	pt = new double[K];

	lambdita = sqrt((K * log((double)K))/((2.7183 - 1)*_T));
    if (lambdita > 1.0)
        lambdita = 1.0;
	printf("lambdita = %lf\n", lambdita);
	eta = lambdita / K;
    //eta = log(1.0 + sqrt(2.0 * log(K) / _T));
    printf("eta = %.4lf\n", eta);

    int i;
	for (i = 0; i < K; i++)
		G[i] = 0.0;

	t = 0;
    
    printf("numExperts = %i\n", K);
    
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    S_e = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        S_e[i] = 0;
        x_phi[i] = 0;
        cuenta[i] = 0;
    }

}

Exp3::~Exp3() {
	delete G;
	delete pt;
}

int Exp3::select(bool *choices) {
	// fill pt
	int i;
	double sum = 0.0;

    int Kcount = 0;
	for (i = 0; i < K; i++) {
        if (choices[i]) {
            sum += exp(eta * G[i]);
            Kcount ++;
        }
    }

    //printf("pt: ");
	for (i = 0; i < K; i++) {
        if (choices[i])
            pt[i] = (1.0 - lambdita) * (exp(eta * G[i]) / sum) + (lambdita / Kcount);
        else
            pt[i] = 0.0;
        
        //printf("%.2lf ", pt[i]);
    }
/*    printf("\n");
    printf("pt: ");
	for (i = 0; i < K; i++) {
        printf("%.3lf ", pt[i]);
    }
    printf("\n");
*/
	t ++;

	experto = pickExpert();
    
    //printf("; selected expert %i\n", experto);
    //printf("selected expert %i\n", experto);
    
    return experto;
}

int Exp3::pickExpert() {
	int num = rand() % RAND_MAX;
	int i;

	double sum = 0.0;
	for (i = 0; i < K; i++) {
		sum += pt[i] * RAND_MAX;
		if (num < sum) {
			return i;
		}
	}
	printf("i never selected an action: num = %i\n", num);
	char ch;
	//scanf("press a key...", &ch);
	//printf("\n");
	//getch();
	exit(1);

	return -1;
}

void Exp3::update(double R, int tau) {
    int i;
    
    for (i = 0; i < tau; i++) {
        G[experto] += R / pt[experto];
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    }
    //printf("%i: aspiration = %.2lf\n", me, aspiration);
    
    //printf("updating %i\n", experto);
    S_e[experto] += tau;
    x_phi[experto] += ((double)tau / S_e[experto]) * (R - x_phi[experto]);
    cuenta[experto] ++;
    
}



