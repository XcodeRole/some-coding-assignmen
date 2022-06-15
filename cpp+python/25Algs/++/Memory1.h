#ifndef MEMORY1_H
#define MEMORY1_H

#include "defs.h"
#include "Mala.h"

class Memory1 : public Mala {
public:
	Memory1();
	Memory1(int *_A, int _me, char _game[1024]);
	~Memory1();
	
	int move();
	void update(int *acts);
    
    double generateAction(double val);
	
	int *A;
	int me;
    
    double genoType[10];
    
    int previous[2];
    
    bool nuevo;
};

#endif