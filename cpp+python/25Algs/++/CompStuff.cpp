#include "defs.h"
#include "minimaxLog.h"


void writeMaximinLog(int A[2], double ***M);
minimaxLog *computeMaximin(int index, int A[2], double ***M);
minimaxLog *computeAttack(int index, int A[2], double ***M);
double *generateNormalCDF(double _mu, double _sigma, int num);
double NB(int index);

extern void Littman(Cell pago[NumDecisionPointsMax][NumDecisionPointsMax], int c[2], int a1[2], int a2[2], int _numDPA);
extern char game[1024], theGame[1024];


void writeMaximinLog(int A[2], double ***M) {
    int i;
    minimaxLog *mnmx0 = computeMaximin(0, A, M);
    minimaxLog *mnmx1 = computeMaximin(1, A, M);
    minimaxLog *attack0 = computeAttack(0, A, M);
    minimaxLog *attack1 = computeAttack(1, A, M);

    char fname[1024];
    sprintf(fname, "../results/maximinVals/%s", game);
    FILE *fp = fopen(fname, "w");
    
    fprintf(fp, "%lf\n", mnmx0->mv);
     for (i = 0; i < A[0]; i++)
        fprintf(fp, "%lf ", mnmx0->ms[i]);
    fprintf(fp, "\n");
    fprintf(fp, "%lf\n", mnmx1->mv);
    for (i = 0; i < A[1]; i++)
        fprintf(fp, "%lf ", mnmx1->ms[i]);
    fprintf(fp, "\n");
    fprintf(fp, "%lf\n", -attack0->mv);
    for (i = 0; i < A[0]; i++)
        fprintf(fp, "%lf ", attack0->ms[i]);
    fprintf(fp, "\n");
    fprintf(fp, "%lf\n", -attack1->mv);
    for (i = 0; i < A[1]; i++)
        fprintf(fp, "%lf ", attack1->ms[i]);
    fprintf(fp, "\n");
    
    fclose(fp);
    
    delete mnmx0;
    delete mnmx1;
    delete attack0;
    delete attack1;
}

minimaxLog *computeMaximin(int index, int A[2], double ***M) {
	int i, j;
	
    int numStates = A[0] * A[1];
    
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = M[index][i][j];
			count ++;
		}
	}
	minimaxLog *mm = new minimaxLog(A[index]);
	mm->getMinimax(A, index, payoff);
    
	delete[] payoff;
    
    return mm;
}

minimaxLog *computeAttack(int index, int A[2], double ***M) {
	int i, j;
	
    int numStates = A[0] * A[1];
    
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = -M[1-index][i][j];
			count ++;
		}
	}
	minimaxLog *mm = new minimaxLog(A[index]);
	mm->getMinimax(A, index, payoff);
    
	delete[] payoff;
    
    return mm;
}

double sampleNormal(double *CDF, int numCDFelements, double mu, double sigma){
	int i;
	double num = rand() / (double)RAND_MAX;
	//printf("num = %lf\n", num);
	double inc = (6.0 * sigma) / (numCDFelements - 1);
	for (i = 0; i < numCDFelements; i++) {
		if (num <= CDF[i]) {
			//printf("i = %i\n", i);
			return (mu - (3.0 * sigma) + (i * inc));
		}
	}
	
	printf("normal sample not found\n");
    exit(1);
	
	return -1.0;
}

double *generateNormalCDF(double _mu, double _sigma, int num) {
	int numCDFelements = num;
	double *normalCDF = new double[numCDFelements];
	
	int i;
	double x;
	double inc = (6.0 * _sigma) / (numCDFelements - 1);
	double sum = 0.0;
	for (x = _mu - 3.0*_sigma, i = 0; i < numCDFelements; x += inc, i++) {
		normalCDF[i] = (1.0 / (_sigma * sqrt(2.0 * 3.1415926))) * exp(-((x - _mu) * (x - _mu))/(2.0 * _sigma * _sigma));
		sum += normalCDF[i];
	}
	
	for (i = 0; i < numCDFelements; i++) {
		normalCDF[i] /= sum;
		if (i > 0)
			normalCDF[i] += normalCDF[i-1];
	}
    
    return normalCDF;
}

double NB(int index) {
	Cell Payoffs[NumDecisionPointsMax][NumDecisionPointsMax];
	int c[2], a1[2], a2[2];
    double bargain1, bargain2;
	
	int i, j;
	double p1, p2;
	
    char filename[1024];
	if (!strncmp(theGame, "rc", 2) || !strncmp(theGame, "rg", 2))
		sprintf(filename, "../../games/%s_%i.txt", theGame, index);
	else
		sprintf(filename, "../../games/%s.txt", theGame);		
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		// check in an alternate directory before giving up
		if (!strncmp(theGame, "rc", 2) || !strncmp(theGame, "rg", 2) || !strncmp(theGame, "rz", 2))
			sprintf(filename, "../games/%s_%i.txt", theGame, index);
		else
			sprintf(filename, "../games/%s.txt", theGame);		
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("file %s not found\n", filename);
			exit(1);
		}
	}
    
	int npls, nacts1, nacts2;
	fscanf(fp, "%i", &npls);
	fscanf(fp, "%i", &nacts1);
	fscanf(fp, "%i", &nacts2);
    
	int count = 0;
	for (i = 0; i < nacts1; i++) {
		for (j = 0; j < nacts2; j++) {
			fscanf(fp, "%lf %lf", &(Payoffs[j][i].r1), &(Payoffs[j][i].r2));
			//Payoffs[j][i].r1 = payoffs[count][0];
			//Payoffs[j][i].r2 = payoffs[count][1];
			count ++;
		}
	}
	fclose(fp);
    
	Littman(Payoffs, c, a1, a2, nacts1);
    
	p1 = (Payoffs[a1[0]][a2[0]].r1 * c[0]) + (Payoffs[a1[1]][a2[1]].r1 * c[1]);
	p1 /= c[0] + c[1];
	p2 = (Payoffs[a1[0]][a2[0]].r2 * c[0]) + (Payoffs[a1[1]][a2[1]].r2 * c[1]);
	p2 /= c[0] + c[1];
	bargain1 = p1;
	bargain2 = p2;
	
	printf("\n\nNB = (%lf, %lf)\n", bargain1, bargain2); fflush(stdout);
    
    char fnm[1024];
    sprintf(fnm, "../results/NBS/%s", theGame);
    FILE *fpw = fopen(fnm, "w");
    fprintf(fpw, "%lf\t%lf\n", bargain1, bargain2);
    fclose(fpw);
    
    return (bargain1 + bargain2) / 2.0;
}

