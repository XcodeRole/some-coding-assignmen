#ifndef experts_jefe_plus_h
#define experts_jefe_plus_h

#include "defs.h"
#include "Mala.h"
#include "expertLearner.h"
#include "minimaxLog.h"
#include "REexpert.h"
#include "Rmax.h"
#include "iModel.h"
#include "jefe.h"

#include "a.h"
#include "Exp3.h"
#include "eee.h"
#include "ucb.h"

class jefe_plus : public Mala {
public:
    jefe_plus();
    jefe_plus(const char *nombre, int _me, int _A[2], double ***_M, double _lambda, char _game[1024]);
    ~jefe_plus();
    
    int determineExpertsSencillo();
    int determineExperts();
    void determineStrategyPairs();
    void createSolutionPairs(SolutionPair *Theta);
    
    int move();
    void update(int *acts);
    
    int expertType(int index);
    
    void determineSatisficingExperts();
    void determineSatisficingExpertsSencillo();
    bool override();
    void resetCycle();
    double pay(int me, int sol);
    minimaxLog *computeMaximin(int index);
    minimaxLog *computeAttack(int index);
    
    void setAspirationHigh();
    void setAspirationFolkEgal();
    void setAspirationHighestEnforceable();
    double setAspirationFolkEgal_sencillo();
    double setAspirationHighestEnforceable_sencillo();
    
    int generateAction(int index, double *pt);
    int encodeJointAction(int _actions[NUM_PLAYERS]);
    
    void printStrat(int strat);
    
	double ***M;
	int A[2], me, numStates;
	int t, estado, experto;
    bool *beenThere;
    double R, mu;
    int tau;
    bool cycled;
    
    //expertLearner *learner;
    
    minimaxLog *mnmx[2], *attack0, *attack1;
    Rmax *br;
    REexpert **re;
    bool *satisficingExperts;
    int numExperts, REcount, numSatExperts;
    
    bool cycleFull;
    
    iModel *im;
    double *vu;
    int *usage;
    double lambda;
    
    int previousActs[2];
    
    bool alwaysMM;
    double permissibleLoss;
    
    char game[1024];
    
    double lowAspiration;
    
    FILE *fpLogExperts;
    
    bool sencillo;
};

#endif
