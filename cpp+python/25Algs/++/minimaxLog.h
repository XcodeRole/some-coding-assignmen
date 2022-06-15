#ifndef MINIMAXLOG_H
#define MINIMAXLOG_H

#include "defs.h"

#define GRANULARITY		0.000001
#define SENTINAL		-999999

class minimaxLog {
public:
	minimaxLog();
	minimaxLog(int _nActions);//, int _numStates);
	~minimaxLog();

	void getMinimax(int *nActions, int me, double *payoff);
	double getDimmensionHigh(double *mpi, int *nActions, int me, double *payoff, int index, 
									double remaining, double l, double m, double r);
	void getOrder(double lVal, double mVal, double rVal, int *order);
	double policyIterations(double *mpi, int *nActions, int me, double *payoff, double remaining, 
								  int index, double l, double m, double r);
	double evaluatePolicy(int *nActions, double *mpi, double *payoff, int me);

	bool pureStrategy(double *mpi, int *nActions, int me, double *payoff);
	bool highest(int index, int *nActions, double *payoff, int me, bool *nott);

	//int numStates;
	double mv, lowmv;
	double *ms, *mpi;
	//double *c_payoffs;
	bool *nott;

	double epi;
};

#endif
