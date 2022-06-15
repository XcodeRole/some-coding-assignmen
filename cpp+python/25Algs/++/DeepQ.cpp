#include "DeepQ.h"

DeepQ::DeepQ() {
}

DeepQ::DeepQ(int _me, int _A[2], double ***_M, int _iterations, int _history, int _repeat) {
    me = _me;
    A[0] = _A[0];
    A[1] = _A[1];
    M = _M;
    iterations = _iterations;
    history = _history;
    repeat = _repeat;

    char tmp[1024];
    coms = new MySocket(3000+me);

    // start up the agent
    char cmndr[1024];
    if (me == 0)
        sprintf(cmndr, "java -classpath sherief.jar misc.AgentJacobDeepEncog row iterations:%i history:%i repeat:%i &", iterations, history, repeat);
    else
        sprintf(cmndr, "java -classpath sherief.jar misc.AgentJacobDeepEncog col iterations:%i history:%i repeat:%i &", iterations, history, repeat);
    system(cmndr);

    coms->AcceptEm();
    coms->ReadMessage(tmp);
    
    sendGame(me, M, A);
}

DeepQ::~DeepQ() {
    char buf[1024];
    
    strcpy(buf, "Done\r\n");
    coms->SendMessage(buf, strlen(buf));

    delete coms;
}
	
int DeepQ::move() {
    char buf[1024];
    int a;
    
    coms->ReadMessage(buf);
    a = atoi(buf);
    
    return a;
}

void DeepQ::update(int *acts) {
    char buf[1024];
    
    sprintf(buf, "%i\r\n%i\r\n", acts[me], acts[1-me]);
    coms->SendMessage(buf, strlen(buf));
}

void DeepQ::sendGame(int index, double ***M, int A[2]) {
	char gameInfo[1024];

    int numActions = A[index];
    sprintf(gameInfo, "%i\n", numActions);

	int row, col;
	if (index == 0) {
		for (row = 0; row < numActions; row ++) {
			for (col = 0; col < numActions; col ++) {
				// everyone thinks they are the row player
				sprintf(gameInfo, "%s%lf\n", gameInfo, M[0][row][col]);
			}
		}
	}
	else {
		for (col = 0; col < numActions; col ++) {
			for (row = 0; row < numActions; row ++) {
				// everyone thinks they are the row player
				sprintf(gameInfo, "%s%lf\n", gameInfo, M[1][row][col]);
			}
		}
	}

    //printf("%s\n", gameInfo);

	coms->SendMessage(gameInfo, (int)strlen(gameInfo));

}

