#ifndef CJAL_H
#define CJAL_H

#include "defs.h"
#include "Mala.h"
//#include "minimaxLog.h"

class CJAL : public Mala {
public:
	CJAL();
	CJAL(int _numAgents, int *_numActions, double ***_M, int _me, int _N, double _explor, bool _verbose);
	~CJAL();
	
    int move();
    void update(int *acts);

	int generateAction();
	void Update(int *actions, double r_t);
	void dump();
	
    double ***M;
	int **counts;	
	int nAgents, *nActions, me;
	int N;	
	double *pi, explor;
	int t;
	//minimaxLog *mnmx;
	
	bool verbose;
	FILE *fp;
};

#endif