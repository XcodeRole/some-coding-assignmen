#ifndef _RE_Expert_h
#define _RE_Expert_h

#include "defs.h"
#include "minimaxLog.h"
#include "strategyPair.h"
#include "Rmax.h"

class REexpert {
public:
    REexpert();
    REexpert(int _me, double ***_M, int _A[2], int _a1, int _a2);
    REexpert(int _me, double ***_M, int _A[2], int _s1, int _s2, minimaxLog *_attack0, minimaxLog *_attack1);
    ~REexpert();
    
    double maxVal(int a);
    void reset();
    void reset(int acciones[2]);
    int act(int index);
    int generateAction(int index, double *pt);
    void update(int index, int acciones[2]);
    
    int state(int hist[MAXLENGTH][NUM_PLAYERS]);
    int encodeJointAction(int _actions[NUM_PLAYERS]);
    void updateHist(int hist[MAXLENGTH][NUM_PLAYERS], int acciones[2]);
        
    void makeAssessment(int index);
    
    void printExpert(int index, double _vu);
    void printTeacherFollower();
    void printFollowerTeacher();
    
    int whichAction(double *pi, int ind);
    
    double checkBR();
    
    double ***M;
    int A[2];
    int me, numStates, omega;
    
    int acts[2][2];
    int s1, s2;
    double barR[2];
    double expectedValue;
    minimaxLog *attack[2];
    double penalty;
    
    int lastAction;
    double guilt;
    bool guilty;
    
    int **assessment;
    
    strategyPair *asTheTeacher;
    strategyPair *asTheFollower;
    
    bool enforceable;
    
    bool sencillo;
};

#endif
