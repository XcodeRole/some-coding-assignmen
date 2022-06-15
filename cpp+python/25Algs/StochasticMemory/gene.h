#ifndef GENE_H

#include "defs.h"

class Gene {
public:
    Gene();
    Gene(int *_A, double ***_M, double _genome[10]);
    ~Gene();

    void reset(double _genome[10], double mutationRate);

    int act(bool isRow, int index);
    double generateAction(double val);
    void update(bool isRow, int *acts, int index);

    int *A;
    double ***M;

    double genome[10], oldGenome[10];
    int round, oldRound;
    double score, oldScore;
    
    int rowPrevious[8][2];
    int colPrevious[8][2];
    bool nuevo[8][2];
};

#endif