#ifndef HBA_H
#define HBA_H

#include "defs.h"
#include "Mala.h"

class HBA {
public:

    
    int move();
    void update(int *acts);
    
    int me;
    int A[2];
    double ***M;
    char game[1024];
    int t;

}




#endif
