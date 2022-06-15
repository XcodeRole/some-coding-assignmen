#ifndef OMEGAPAIR
#define OMEGAPAIR

#include "defs.h"
#include "omegaStrategy.h"
#include "minimaxLog.h"

#define TEACHER			0
#define FOLLOWER		1
#define ATTACKER		2
#define UNKNOWN			3

class omegaPair {
public:
	omegaPair();
	omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType);  // ATTACKER and UNKNOWN
	omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, int *_pi);  // TEACHER and FOLLOWER
	omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, double **_pi);  // TEACHER and FOLLOWER
    omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int *hisStrat, int **myStrats, int _s);
	~omegaPair();
	
	void updateUnknown(int **_kappa, int s);
	double shouldHave(int startState, int sCuenta);
	double average();
	double socialWelfare(int s);
	
	bool computeHypotheticals(int samps, int **_kappa, int estado, double aspiration, double *explore);
	void base10tobaseK(int num, int K, int *digit);
	bool unique(int *digits, int *lacking);
	
	int me, omega;
	double ***M;
	int A[NUM_PLAYERS];
	
	omegaStrategy *ares, *thars;
	omegaStrategy *hypoares, *hypothars;

	//int opponentType;
	
	double r;
	int usageCount;
};

#endif
