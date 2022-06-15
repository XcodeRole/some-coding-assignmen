#ifndef PAVLOV_H
#define PAVLOV_H

#include "defs.h"
#include "Mala.h"

class Pavlov : public Mala {
public:
	Pavlov();
	Pavlov(int *_A, int _me, double ***_M);
	~Pavlov();
	
	int move();
	void update(int *acts);
	
    double ***M;
	int *A;
	int me;
    
    int curAction;
    double threshold;
};

#endif