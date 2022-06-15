#ifndef MEMORY2_H
#define MEMORY2_H

#include "defs.h"
#include "Mala.h"

class Memory2 : public Mala {
public:
	Memory2();
	Memory2(int *_A, int _me, char _game[1024]);
	~Memory2();
	
	int move();
	void update(int *acts);
    
    double generateAction(double val);
	
	int *A;
	int me;
    
    double genoType[34];
    
    int previous[2];
    int previous2[2];
};

#endif