#ifndef DEEPQ_H
#define DEEPQ_H

#include "defs.h"
#include "Mala.h"
#include "MySocket.h"

class DeepQ : public Mala {
public:
	DeepQ();
	DeepQ(int _me, int _A[2], double ***_M, int _iterations, int _history, int _repeat);
	~DeepQ();
	
	int move();
	void update(int *acts);
    
    void sendGame(int index, double ***M, int A[2]);

    int me;
    double ***M;
    int A[2];
    int iterations, history, repeat;
    
    MySocket *coms;
};

#endif