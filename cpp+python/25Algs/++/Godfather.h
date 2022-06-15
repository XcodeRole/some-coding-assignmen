#ifndef GODFATHER_H
#define GODFATHER_H

#include "defs.h"
#include "Mala.h"
#include "REexpert.h"
#include "minimaxLog.h"

class Godfather : public Mala {
public:
    Godfather();
    Godfather(int _me, int _A[2], double ***_M, char _game[1024]);
    ~Godfather();

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
    
    char game[1024];
    
    double expectedPayout;
};

#endif
