#ifndef GENE2_H

#include "defs.h"

class Gene2 {
public:
    Gene2();
    Gene2(int *_A, double ***_M, double _genome[34]);
    ~Gene2();

    void reset(double _genome[34], double mutationRate);
    void restart(int neighbor, int index);

    int act(bool isRow, int index);
    double generateAction(double val);
    void update(bool isRow, int *acts, int index);

    int *A;
    double ***M;

    double genome[34], oldGenome[34];
    int round, oldRound;
    double score, oldScore;
    
    int rowPrevious[8][2];
    int colPrevious[8][2];
    int rowPrevious2[8][2];
    int colPrevious2[8][2];
    bool nuevo[8][2];
};

#endif