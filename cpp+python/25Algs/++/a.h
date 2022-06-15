#ifndef a_h
#define a_h

#include "defs.h"
#include "expertLearner.h"
#include "omegaStrategy.h"

class a : public expertLearner {
public:
    a();
    a(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda, int _numExperts);
    ~a();
    
    int select(bool *choices);
    void update(double R, int tau);
    
    int randomlySelect(bool *choices);
    
    int me;
    double ***M;
    int A[NUM_PLAYERS];
    
    int lastExpert;
    double rho;
    
    int *S_e;
};



#endif
