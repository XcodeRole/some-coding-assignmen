#ifndef SALG_H
#define SALG_H

#include "defs.h"
#include "Mala.h"

class SAlg : public Mala {
public:
	SAlg();
	SAlg(int _nActions, double _lambda, int _me, double ***_M);
	~SAlg();

	int move();
    void update(int *acts);
	//void Update(int *actions, double r_t);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);
	//void setAspiration(double _val);

    double ***M;
	bool satisficing;
	double aspiration;
	double lambda;
	int nActions;
	int prevAction;
	int episode;
	double r_max;
	int me;
};


#endif