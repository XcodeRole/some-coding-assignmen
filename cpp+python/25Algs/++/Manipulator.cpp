#include "Manipulator.h"


Manipulator::Manipulator() {
    printf("unfinished Manipulator() constructor\n");
    exit(1);
}

Manipulator::Manipulator(int _me, int _A[2], double ***_M, int _t1, int _t2, char _game[1024], bool _manipulator) {
    me = _me;
    A[0] = _A[0];
    A[1] = _A[1];
    M = _M;
    t1 = _t1;
    t2 = _t2;
    strcpy(game, _game);
    which = _manipulator;
    
    br = new Rmax(me, A, M, 1, 0, 0.95);
    if (which) {
        bully = new Bully(me, A, M, game);
        current = bully;
    }
    else {
        godfather = new Godfather(me, A, M, game);
        current = godfather;
    }
    
    t = 0;
    runningAve = 0.0;
}

Manipulator::~Manipulator() {
    delete br;
    if (which)
        delete bully;
    else
        delete godfather;
}

int Manipulator::move() {
    if (current == NULL) {
        if (which)
            return bully->generateAction(bully->mnmx[me]->ms);
        else
            return godfather->generateAction(godfather->mnmx[me]->ms);
    }
    
    return current->move();
}

void Manipulator::update(int *acts) {
    runningAve += M[0][acts[0]][acts[1]];
    t++;
    
    if (which)
        bully->update(acts);
    else
        godfather->update(acts);
    br->update(acts);

    double val = runningAve / t;
    double epsilon = 0.04;
    
    if (t == t1 / 2)
        t12val = runningAve;
    
    if (which) {
        double valLate = (runningAve - t12val) / (t1 / 2);
        if ((current == bully) && (t >= t1) && (valLate < (bully->expectedPayout - epsilon)))
            current = br;
        if (t == t2)
            beststrategy = current;
        if (t >= t2) {
            if (val < (bully->mnmx[me]->mv - epsilon))
                current = NULL;
            else
                current = beststrategy;
        }
    }
    else {
        double valLate = (runningAve - t12val) / (t1 / 2);        
        if ((current == godfather) && (t >= t1) && (valLate < (godfather->expectedPayout - epsilon)))
            current = br;
        if (t == t2)
            beststrategy = current;
        if (t >= t2) {
            if (val < (godfather->mnmx[me]->mv - epsilon))
                current = NULL;
            else
                current = beststrategy;
        }
    }
    
}

