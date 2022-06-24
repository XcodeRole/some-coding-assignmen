# distutils: language = c++
# cython: language_level = 3

from libcpp cimport *
cdef extern from "Mala.h":
    cdef cppclass Mala:
        Mala()
        virtual ~Mala()
        
        virtual int move()
        virtual void update(int *acts)
        
        expertLearner *learner

cdef extern from "def.h":
    enum:NumDecisionPointsMax
    enum:NUM_PLAYERS
    enum:MAXLENGTH
    enum:MAX_EXPERTS
cdef extern from "omegaStrategy.cpp":
    pass
cdef extern from "omegaStrategy.h":
    cdef cppclass omegaStrategy:
        omegaStrategy()
        omegaStrategy(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega)
        void init(double ***_M)
        
        int selectAction(int s)
        void setPi(double **_pi)
        int state(int hist[MAXLENGTH][NUM_PLAYERS])
        int getAction(double *pi_er, int numActs)
        int encodeJointAction(int _actions[NUM_PLAYERS])
        
        void bestResponse(double ***M, omegaStrategy *opi, int option)
        double estimatePureValue(double ***M, omegaStrategy *opi, int _s)
        void valuesGivenStrategyPair(double ***M, omegaStrategy *opi)
        void initV(double ***_M)
        int smartValueIteration(double ***M, omegaStrategy *opi)
        double valueIteration(double ***M, omegaStrategy *opi)
        int smartGreedyValueIteration(double ***M, omegaStrategy *opi)
        double greedyValueIteration(double ***M, omegaStrategy *opi)
        void updateHist(int hist[MAXLENGTH][NUM_PLAYERS], int a_me, int a_you)
        int argmax(int s)
        double maxV(int s)
        double expectedV(int s)
        void printV()
        bool matches(omegaStrategy *os)
        void printStrat()
        void printStrat3()
        
        void buildString(char nombre[1024], omegaStrategy *opi);
        
        int me
        int A[NUM_PLAYERS]
        int omega
        int numStates
        int numJointActions
        double **pi
        
        double **V
        double gamma
        double *Valor
        
        bool verbose
        
        double r
        int usageCount
    
cdef extern from "omegaPair.cpp":
    pass
cdef extern from "omegaPair.h":
    cdef cppclass omegaPair:
        omegaPair()
        omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType)
        omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, int *_pi)
        omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, double **_pi)
        omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int *hisStrat, int **myStrats, int _s)
        
        void updateUnknown(int **_kappa, int s)
        double shouldHave(int startState, int sCuenta)
        double average()
        double socialWelfare(int s)
        
        bool computeHypotheticals(int samps, int **_kappa, int estado, double aspiration, double *explore)
        void base10tobaseK(int num, int K, int *digit)
        bool unique(int *digits, int *lacking)
        
        int me, omega
        double ***M
        int A[NUM_PLAYERS]
        
        omegaStrategy *ares
        omegaStrategy *thars
        omegaStrategy *hypoares
        omegaStrategy *hypothars


        
        double r;
        int usageCount;



cdef extern from "BR.cpp":
    pass
cdef extern from "BR.h":
    cdef cppclass BR:
        BR()
        BR(int _me, int _A[NUM_PLAYERS], double ***_M)
        
        int move()
        void update(int *acts)
        
        void print()
        
        double ***M
        int A[NUM_PLAYERS]
        int me
        
        omegaPair *myBR
        int **kappa
        
        int t, estado
        
        bool verbose

cdef extern from "iModel.cpp":
    pass
cdef extern from "iModel.h":
    cdef cppclass iModel:
        iModel()
        iModel(int _me, int _A[NUM_PLAYERS], int _omega)
        
        void init()
        void update(int *acts, int estado, int _t)
        void print()
        bool match(omegaStrategy *thars)
        bool match(double **pi)
        int me
        int A[NUM_PLAYERS]
        int omega
        int *lastSeen
        int *lastTime
        int numStates


cdef extern from "algOmega.cpp":
    pass

cdef extern from "algOmega.h":
    cdef cppclass algOmega:
        algOmega()
        algOmega(int _me, int _A[NUM_PLAYERS], double ***_M, double _decay)
        int move()
        void update(int *acts)
	                                                                                                                          
        void updateImBR()
        
        double ***M
        int A[NUM_PLAYERS]
        int me

        iModel *im
        BR *br
        
        omegaPair *imBR
        int **currentImKappa
        
        double decay
        double theta