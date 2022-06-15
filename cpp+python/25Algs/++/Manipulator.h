#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "defs.h"
#include "Mala.h"
#include "minimaxLog.h"
#include "Bully.h"
#include "Godfather.h"
#include "Rmax.h"

class Manipulator : public Mala {
public:
    Manipulator();
    Manipulator(int _me, int _A[2], double ***_M, int _t1, int _t2, char _game[1024], bool _manipulator);
    ~Manipulator();
    
    int move();
    void update(int *acts);
    
    int me;
    int A[2];
    double ***M;
    int t, t1, t2;
    char game[1024];
    double runningAve, t12val;
    bool which;
    
    Bully *bully;
    Godfather *godfather;
    Rmax *br;
    
    Mala *current, *beststrategy;
};


#endif
