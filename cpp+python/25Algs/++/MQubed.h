#ifndef MQubed_H
#define MQubed_H

#include "defs.h"
#include "Mala.h"
#include "minimaxLog.h"

class MQubed : public Mala {
public:
	MQubed();
	MQubed(int _numAgents, double ***_M, int *_numActions, int _me, double _gamma, double _alpha, int _eta, int _omega, 
		   bool _q0, bool _nomm, bool _nosecurity, bool _eg, bool _alphad, bool _verbose, bool _hsarsa, char _game[1024]);
	~MQubed();

	//int Move();
    int move();
	int generateAction();

	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);
    void loadPayoffMatrix();
    minimaxLog *computeMaximin(int index);
    void update(int *acts);
	//void Update(int *actions, double r_t);
	double minimum(double a, double b);
	double maximum(double a, double b);
	void updatePolicy(int s);
	double nextValue(int s);
	double maxQ(int s);
	int gmaxQ();
	double expectedQ(int s);
	void updateStateHistory(int ns, int *actions);
	bool inHistory(int s);
	int mostActions(int estado);

	int getNewState(int *actions);

	void dumpInfo(int *a);

    double ***M;
    int *A;
    
	int nAgents, *nActions, numStates, numJointActions;
	double gamma, alpha, alpha_0;
	int eta;
	bool verbose;
	int estado, me;
	int *indices;
	int t;
	int omega;
	int prev;
	
	double totalAcceptableLoss, epsilonLoss;
	bool alwaysPessimistic;
	
	bool q0, nomm, nosecurity, eg, alphad, hsarsa;

	double mu_t, Beta_t, theta_t;
	int beta_t;

	int *stateHistory, *jointActionHistory;
	double **Q;
	int **cs;
	double *pi;
	
	double low, high;
	
	minimaxLog *mnmx;

	int mvcount;

	FILE *fp;
    char game[1024];    
};


#endif