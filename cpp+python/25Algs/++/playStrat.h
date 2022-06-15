#ifndef playStrat_h
#define playStrat_h

#include "defs.h"
#include "Mala.h"
#include "REexpert.h"
#include "minimaxLog.h"

class playStrat : public Mala {
public:
    playStrat();
    playStrat(int _me, int _A[2], double ***_M, char _game[1024], int _number);
    ~playStrat();
    
    void createExpert();
    double pay(int me, int sol);
    minimaxLog *computeMaximin(int index);
    minimaxLog *computeAttack(int index);
    
    int move();
    void update(int *acts);
    
    int generateAction(double *pt);
    
	double ***M;
	int A[2], me;
    
    int expertNumber;
    int estado, numStates;
    
    minimaxLog *mnmx[2], *attack0, *attack1;
    Rmax *br;
    REexpert *re;
    
    char game[1024];
    
    bool expertExists;
};



#endif
