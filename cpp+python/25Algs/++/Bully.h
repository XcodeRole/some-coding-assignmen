#ifndef BULLY_H
#define BULLY_H

#include "defs.h"
#include "Mala.h"
#include "REexpert.h"
#include "minimaxLog.h"

class Bully : public Mala {
public:
    Bully();
    Bully(int _me, int _A[NUM_PLAYERS], double ***_M, char _game[1024]);
    ~Bully();

    void createExpert();
    double pay(int me, int sol);
    minimaxLog *computeMaximin(int index);
    minimaxLog *computeAttack(int index);
    
    int move();
    void update(int *acts);
    
    int generateAction(double *pt);
    
	double ***M;
	int A[2], me, t, numStates;

    minimaxLog *mnmx[2], *attack0, *attack1;
    REexpert *re;
    
    double expectedPayout;
    
    char game[1024];
};



#endif
