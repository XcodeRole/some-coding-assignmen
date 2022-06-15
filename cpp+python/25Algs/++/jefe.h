#ifndef experts_jefe_h
#define experts_jefe_h

#include "defs.h"
#include "Mala.h"
#include "expertLearner.h"
#include "minimaxLog.h"
#include "REexpert.h"
#include "Rmax.h"

#include "a.h"
#include "Exp3.h"
#include "eee.h"
#include "ucb.h"

class SolutionPair {
public:
    int s1, s2;
    
    double min, one, two;
};

class jefe : public Mala {
public:
    jefe();
    jefe(const char *nombre, int _me, int _A[2], double ***_M, double _lambda, char _game[1024]);
    ~jefe();
    
    int determineExpertsSencillo();
    int determineExperts();
    void determineStrategyPairs();
    void createSolutionPairs(SolutionPair *Theta);
    
    int move();
    void update(int *acts);
    
    int expertType(int index);
    
    void resetCycle();
    double pay(int me, int sol);
    minimaxLog *computeMaximin(int index);
    minimaxLog *computeAttack(int index);
    
    int generateAction(int index, double *pt);
    int encodeJointAction(int _actions[NUM_PLAYERS]);
    
	double ***M;
	int A[2], me, numStates;
	int t, estado, experto;
    bool *beenThere;
    double R;
    int tau;
    bool cycled;
    
    //expertLearner *learner;
    
    minimaxLog *mnmx[2];
    minimaxLog *attack0, *attack1;

    Rmax *br;
    REexpert **re;
    bool *satisficingExperts;
    int numExperts, REcount;
    
    int previousActs[2];
    
    int cycleFull;
    
    char game[1024];
    
    FILE *fpLogExperts;
    
    bool sencillo;
};

#endif
