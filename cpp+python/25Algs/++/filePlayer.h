#ifndef filePlayer_h
#define filePlayer_h

#include "defs.h"
#include "Mala.h"

class filePlayer : public Mala {
public:
    filePlayer();
    filePlayer(int _me, int _A[NUM_PLAYERS], double ***_M, const char *filename);
    ~filePlayer();
    
	int move();
	void update(int *acts);
    
    FILE *fp;
    double ***M;
    int A[NUM_PLAYERS], me;
};

#endif
