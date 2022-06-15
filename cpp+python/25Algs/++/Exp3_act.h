#ifndef Exp3_act_H
#define Exp3_act_H

#include "defs.h"
#include "Mala.h"

class Exp3_act : public Mala {
public:
	Exp3_act();
	Exp3_act(int _nActions, int _me, double _g, double ***_M);
	~Exp3_act();

	int move();
    void update(int *acts);
	void Update(int *actions, double r_t);

	int generateAction();

    double ***M;
	int K;
	double g, eta, lambda;
	int me;

	double *G, *pt;
	int t;
};


#endif