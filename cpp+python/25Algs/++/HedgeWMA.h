#ifndef HEDGEWMA_H
#define HEDGEWMA_H

#include "defs.h"
#include "Mala.h"

class HedgeWMA : public Mala {
public:
	HedgeWMA();
	HedgeWMA(int _nActions, int _me, double _beta, bool _verbose, double ***_M);
	~HedgeWMA();
	
	int move();
    void update(int *acts);
	void Update(int *actions, double r_t);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);
	void dump(int a);
	
	int generateAction();
	
    double ***M;
	double *pt, *w;
	//double **Scores, highval, lowval; // assumption of two players
	double beta;
	int K;
	int me;
	int t;
	
	bool verbose;
	FILE *fp;
};


#endif