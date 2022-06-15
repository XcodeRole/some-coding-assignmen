#ifndef littleTest_strategyPair_h
#define littleTest_strategyPair_h

#include "defs.h"

class strategyPair {
public:
    strategyPair();
    
    // the variable [me] names the teacher in the strategy pair
    strategyPair(int _me, double ***_M, int _A[2], int _acts[2][2], int s1, int s2, int **_assessment);
    
    ~strategyPair();
    
    bool isTeacher(int _me);
    
    int me, numStates;
    
    double **teacher, **follower;
};

#endif
