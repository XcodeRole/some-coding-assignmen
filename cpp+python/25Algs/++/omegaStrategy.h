#ifndef OMEGASTRATEGY
#define OMEGASTRATEGY

#include "defs.h"

class omegaStrategy {
public:
	omegaStrategy();
	omegaStrategy(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega);
	void init(double ***_M);
	
	~omegaStrategy();
	
	int selectAction(int s);
	void setPi(double **_pi);
	int state(int hist[MAXLENGTH][NUM_PLAYERS]);
    int getAction(double *pi_er, int numActs);
	int encodeJointAction(int _actions[NUM_PLAYERS]);
	
	void bestResponse(double ***M, omegaStrategy *opi, int option);
    double estimatePureValue(double ***M, omegaStrategy *opi, int _s);
	void valuesGivenStrategyPair(double ***M, omegaStrategy *opi);
	void initV(double ***_M);
    int smartValueIteration(double ***M, omegaStrategy *opi);
	double valueIteration(double ***M, omegaStrategy *opi);
    int smartGreedyValueIteration(double ***M, omegaStrategy *opi);
	double greedyValueIteration(double ***M, omegaStrategy *opi);    
	void updateHist(int hist[MAXLENGTH][NUM_PLAYERS], int a_me, int a_you);
	int argmax(int s);
	double maxV(int s);
	double expectedV(int s);
	void printV();
	bool matches(omegaStrategy *os);
	void printStrat();
    void printStrat3();
    
    void buildString(char nombre[1024], omegaStrategy *opi);
	
	int me, A[NUM_PLAYERS], omega;
	int numStates, numJointActions;
	double **pi;
	
	double **V, gamma;
	double *Valor;
	
	bool verbose;
	
	double r;
	int usageCount;
    
    //bool tieFlag;
};

#endif