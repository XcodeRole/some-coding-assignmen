#include "AOplus.h"

AOplus::AOplus() {
	printf("entered into incomplete AOplus constructor\n");
	exit(1);
}

AOplus::AOplus(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	lambda = _lambda;
	
	deriveStrategies();
	
	t = 0;	
	
	v = 0.0;
	currentStrategy = NULL;
	
	omega = 0;
	
	evaluateChange = false;
	
	im = new iModel(me, A, 1);
	br = new BR(me, A, M);
	
	setAspiration0();
	estado = -1;
	
	printf("start in state %i\n", estado);
}

void AOplus::deriveStrategies() {
	int i, j, k, val1, val2, num, ind;
	
	numStates = A[0] * A[1];
	getPotentialPolicies();
	numTeachingStrats = numIStrats = pow(numPolicies, numStates);
	numStrats = determineNumStrats(numTeachingStrats);	
	printf("numStrats = %i; numCombos = %i\n", numStrats, numTeachingStrats);
	
	beenThere = new bool[numStates];
	theTeachingStrats = new omegaPair*[numTeachingStrats];
	
	double **strats = new double*[numStates];
	for (i = 0; i < numStates; i++)
		strats[i] = new double[A[me]];
	
	for (i = 0; i < numTeachingStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(numPolicies, j);
			num = i % val1;
			num = i - num;
			val2 = pow(numPolicies, j+1);
			num = num % val2;
			
			ind = num / val1;
			for (k = 0; k < A[me]; k++)
				strats[j][k] = policies[ind][k];
		}
		
		//printf("init teaching strat %i of %i\n", i, numTeachingStrats);
		theTeachingStrats[i] = new omegaPair(me, A, M, 1, FOLLOWER, strats);
		
		//printf("%i: ", i);
		//theTeachingStrats[i]->ares->printStrat();
		//printf(" (%.2lf; %.2lf; %.2lf; %.2lf) vs ", theTeachingStrats[i]->ares->Valor[0], theTeachingStrats[i]->ares->Valor[1], theTeachingStrats[i]->ares->Valor[2], theTeachingStrats[i]->ares->Valor[3]);
		//theTeachingStrats[i]->thars->printStrat();
		//printf("\n");
	}
	
	for (i = 0; i < numStates; i++)
		delete strats[i];
	delete strats;
	
	strats = new double*[numStates];
	for (i = 0; i < numStates; i++)
		strats[i] = new double[A[1-me]];

	theIStrats = new omegaPair*[numIStrats];
	for (i = 0; i < numIStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(A[1-me], j);
			num = i % val1;
			num = i - num;
			val2 = pow(A[1-me], j+1);
			num = num % val2;

			ind = num / val1;
			for (k = 0; k < A[1-me]; k++)
				strats[j][k] = policies[ind][k];
		}

		//printf("init follower strat %i of %i\n", i, numIStrats);
		theIStrats[i] = new omegaPair(me, A, M, 1, TEACHER, strats);
		//printf("%i: ", i);
		//theIStrats[i]->ares->printStrat();
		//printf(" (%.2lf; %.2lf; %.2lf; %.2lf) vs ", theIStrats[i]->ares->Valor[0], theIStrats[i]->ares->Valor[1], theIStrats[i]->ares->Valor[2], theIStrats[i]->ares->Valor[3]);
		//theIStrats[i]->thars->printStrat();
		//printf("\n");
	}
	
	for (i = 0; i < numStates; i++)
		delete strats[i];
	delete strats;

	theStrats = new omegaStrategy*[numStrats];
	for (i = 0; i < numTeachingStrats; i++)
		theStrats[i] = theTeachingStrats[i]->ares;
	
	if (numStrats > numTeachingStrats) {
		addMaximin();
		theStrats[numTeachingStrats] = minimaxStrat;
		printf("maximin is strat %i\n", numTeachingStrats);
	}
	else {
		bool same;
		for (i = 0; i < numStrats; i++) {
			same = true;
			for (j = 0; j < numStates; j++) {
				if (!isSame(mnmx->ms, theStrats[i]->pi[j])) {
					same = false;
					break;
				}
			}
			if (same == true) {
				minimaxStrat = theStrats[i];
				printf("maximin %.3lf: (%.3lf; %.3lf) is strat %i: ", mnmx->mv, mnmx->ms[0], mnmx->ms[1], i);
				theStrats[i]->printStrat();
				printf("\n");
				break;
			}
		}
	}
	
	selectable = new bool[numStrats];
	for (i = 0; i < numStrats; i++)
		selectable[i] = true;
	
}

AOplus::~AOplus() {
	int i;
	
	for (i = 0; i < numPolicies; i++)
		delete policies[i];
	delete policies;
	
	for (i = 0; i < numTeachingStrats; i++) {
		delete theTeachingStrats[i];
		delete theIStrats[i];
	}
	delete theTeachingStrats;
	delete theIStrats;
	delete im;
	delete br;
	delete beenThere;
	delete selectable;
	//delete satStrats;
	
	if (numStrats != numTeachingStrats)
		delete minimaxStrat;
}

int AOplus::move() {
	if (t == 0)
		return rand() % A[me];
	
	if (t == 1) {
		selectStrategy();
		resetUtils();
	}
	else {
		if (!override()) {
			double rho = computeInertia();
			double num = (double)(rand()) / RAND_MAX;
			if (num > rho) 
				selectStrategy();
		}
	}
	
	if (currentStrategy == NULL) {
		printf("we have a problem\n");
		exit(1);
	}
	
	return currentStrategy->selectAction(estado);
}

void AOplus::update(int *acts) {
	mu += M[me][acts[0]][acts[1]];
	
	if (currentStrategy != NULL) {
		double betita = 1.0 / (currentStrategy->usageCount + 1);
		if (betita <= 2.0 * lambda)
			betita = 2.0 * lambda;
		currentStrategy->r = betita * M[me][acts[0]][acts[1]] + (1.0 - betita) * currentStrategy->r;
		currentStrategy->usageCount ++;
	}
	
	br->update(acts);	
	if (estado >= 0)
		im->update(acts, estado, t);
	
	estado = theTeachingStrats[0]->ares->encodeJointAction(acts);
	if (estado >= 0) {
		if (beenThere[estado])
			evaluateChange = true;
		else
			evaluateChange = false;
		beenThere[estado] = true;	
	}
	
	t++;
	
	sCuenta ++;
	v += M[me][acts[0]][acts[1]];
	
	if (evaluateChange) {
		for (int i = 0; i < sCuenta; i++)
			aspiration = lambda * (v / sCuenta) + (1.0 - lambda) * aspiration;
		
		if (((mnmx->mv * t) - mu) > permissibleLoss) {
			//printf("I am begin exploited (loss = %.2lf)!!!!!!!!!  Trigger to my maximin strategy\n", (mnmx->mv * t) - mu);
			alwaysMM = true;
		}
	}
}

void AOplus::resetUtils() {
	sCuenta = 0;
	v = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

double AOplus::computeInertia() {
	if (!evaluateChange)
		return 1.0;
	
	double rho = (v / sCuenta) / aspiration;
	if (rho > 1.0)
		rho = 1.0;
	
	rho = pow(rho, sCuenta);
	//printf("%i: rho = %.3lf; aspiration = %.3lf; %.3lf (%i); br = %.3lf (%.3lf)\n", 
	//	   me, rho, aspiration, v / sCuenta, sCuenta, br->myBR->ares->Valor[estado], br->myBR->ares->r);
	
	resetUtils();
	
	return rho;
}

int AOplus::setAspiration0() {
	int i, j, index = -1;
	double high = -99999;
	int s;

	for (i = 0; i < numTeachingStrats; i++) {
		s = findSteadyState(theTeachingStrats[i]);
		if (high < theTeachingStrats[i]->ares->Valor[s]) {
			high = theTeachingStrats[i]->ares->Valor[s];
			index = i;
		}
	}
	
	if (high < mnmx->mv) {
		printf("this must be a competitive game\n");
		for (i = 0; i < A[0]; i++) {
			for (j = 0; j < A[1]; j++) {
				if (M[me][i][j] > high)
					high = M[me][i][j];
			}
		}		
	}

	aspiration = high;
	printf("%i: initial aspiration level = %.3lf\n", me, aspiration);
	
	s = findSteadyState(theTeachingStrats[index]);
	return s;
}

bool AOplus::override() {
	if (!evaluateChange)
		return false;
	
	if (alwaysMM) {
		//printf("alwaysMM?\n");
		//fflush(stdout);
		
		currentStrategy = minimaxStrat;
		printf("%i: trigger to minimaxStrat\n", me);
		fflush(stdout);
		return true;
	}
	
	//printf("compute whichOneIsIT(...)\n");
	//fflush(stdout);
	
	int index = whichOneIsIt(br->myBR->ares);
	if (br->myBR->ares->Valor[estado] >= aspiration) {
		int i;
		double val, highval = -99999;
		
		//printf("before first for %.3lf ... ", aspiration);
		//fflush(stdout);
		
		for (i = 0; i < numTeachingStrats; i++) {
			//printf("%i(%i; %.3lf) ", i, estado, theTeachingStrats[i]->ares->Valor[estado]);
			//fflush(stdout);
			if (theTeachingStrats[i]->ares->Valor[estado] >= aspiration) {
				//printf("in if\n");
				//fflush(stdout);
				if (theStrats[i]->usageCount == 0)
					val = 1.0;
				else
					val = theStrats[i]->r;
				if (val > highval)
					highval = val;
			}
		}
		
		//printf("after first for ...\n");
		//fflush(stdout);
		
		for (i = 0; i < numIStrats; i++) {
			if ((theIStrats[i]->ares->Valor[estado] >= aspiration)  && (im->match(theIStrats[i]->thars))) {
				int ind = whichOneIsIt(theIStrats[i]->ares);
				if (theStrats[ind]->usageCount == 0)
					val = 1.0;
				else
					val = theStrats[ind]->r;
				if (val > highval)
					highval = val;
			}
		}
		
		//printf("vs highval = %.3lf\n", highval);
		int index = whichOneIsIt(br->myBR->ares);
		if ((theStrats[index]->r >= highval) || (theStrats[index]->usageCount == 0)) {
			currentStrategy = theStrats[index];
			//printf("%i: br forced (%i)\n", me, index);
			//fflush(stdout);
			return true;
		}
	}
	
	//printf("finishing\n");
	//fflush(stdout);
	
	return false;
}


void AOplus::selectStrategy() {
	
	//printf("in selectStrategy()\n");
	//fflush(stdout);
	
	int numviable = 0;
	int i;
	for (i = 0; i < numStrats; i++)
		selectable[i] = false;
	
	//printf("before teaching: %.3lf\n", aspiration);
	//fflush(stdout);
	
	//im->print();	
	for (i = 0; i < numTeachingStrats; i++) {
		//printf("%i(%i; %.3lf) ", i, estado, theTeachingStrats[i]->ares->Valor[estado]);
		//fflush(stdout);
		if (theTeachingStrats[i]->ares->Valor[estado] >= aspiration) {
			selectable[i] = true;
			numviable ++;
			
			//printf("\t%i: teacher strat %i is viable (%.3lf >= %.3lf)\n", me, i, theTeachingStrats[i]->ares->Valor[estado], aspiration);
		}
	}

	//printf("went through the teachingStrats\n");
	//fflush(stdout);
	
	//printf("\t%i: is follower strat 0 viable (%.3lf >= %.3lf): ", me, theIStrats[0]->ares->Valor[estado], aspiration);
	//theIStrats[0]->ares->printStrat();
	//printf("\n");
	for (i = 0; i < numIStrats; i++) {
		if ((theIStrats[i]->ares->Valor[estado] >= aspiration)  && (im->match(theIStrats[i]->thars))) {
			//printf("on strat %i\n", i);
			//fflush(stdout);
			int ind = whichOneIsIt(theIStrats[i]->ares);
			if (!selectable[ind])
				numviable ++;
			selectable[ind] = true;
		
			//printf("\t%i: follower strat %i is viable (%.3lf >= %.3lf): ", me, ind, theIStrats[i]->ares->Valor[estado], aspiration);
			//theIStrats[i]->ares->printStrat();
			//printf("\n");
		}
	}

	//printf("went through the IStrats\n");
	//fflush(stdout);
	
	// add br if it is satisficing
	int index = whichOneIsIt(br->myBR->ares);
	if (br->myBR->ares->Valor[estado] >= aspiration) {
		if (!selectable[index])
			numviable ++;
		selectable[index] = true;
		//printf("\t%i: br strat %i is viable\n", me, index);		
	}
	
	if (numviable == 0) {
		currentStrategy = theStrats[index];
		//printf("%i: picked br%i because none viable\n", me, index);
		//fflush(stdout);
		return;
	}
	
	//printf("numviable = %i\n", numviable);
	//fflush(stdout);	
	int ind = rand() % numviable;
/*	
	printf("chose number %i\n", ind);
	fflush(stdout);
	for (i = 0; i < numTeachingStrats; i++) {
		printf("%i = %i; ", i, selectable[i]);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
*/	
	int c = 0;
	for (i = 0; i < numStrats; i++) {
		if (selectable[i])
			c++;
		if (c > ind) {
			currentStrategy = theStrats[i];
			//printf("%i: picked %i: ", me, i);
			//fflush(stdout);
			//currentStrategy->printStrat();
			//printf("\n");
			return;
		}
	}
}

int AOplus::whichOneIsIt(omegaStrategy *strat) {
	int i;
	
	//strat->printStrat();
	//printf("\n");
	
	for (i = 0; i < numTeachingStrats; i++) {
		if (strat->matches(theStrats[i]))
			return i;
	}
	
	printf("couldn't identify the strat\n");
	exit(1);
	
	return -1;
}


void AOplus::getPotentialPolicies() {
	int i, j;
	
	// i miminize his maximum
	minimaxLog *mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = -M[1-me][i][j];
			count ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
	
	//if (isPure(mnmx->ms)) {
		numPolicies = A[me];
	//}
	//else {
	//	numPolicies = A[me] + 1;
	//}
	
	policies = new double*[numPolicies];
	for (i = 0; i < numPolicies; i++) {
		policies[i] = new double[A[me]];
		if (i == A[me]) {
			for (j = 0; j < A[me]; j++)
				policies[i][j] = mnmx->ms[j];
		}
		else {
			for (j = 0; j < A[me]; j++)
				policies[i][j] = 0.0;
			policies[i][i] = 1.0;
		}
	}
	
/*	printf("\n%i: Possible Policies:\n", me);
	for (i = 0; i < numPolicies; i++) {
		for (j = 0; j < A[me]; j++) {
			printf("%.3lf  ", policies[i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/	
	delete mnmx;
}

bool AOplus::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.0) && (v[i] < 1.0))
			return false;
	}
	return true;
}

bool AOplus::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}

void AOplus::computeMaximin() {
	int i, j;
	
	mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = M[me][i][j];
			count ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
	
	alwaysMM = false;
	permissibleLoss = 40.0;
	mu = 0.0;
}

void AOplus::addMaximin() {
	int i, j;
	
	minimaxStrat = new omegaStrategy(me, A, 1);
	for (j = 0; j < numStates; j++) {
		for (i = 0; i < A[me]; i++)
			minimaxStrat->pi[j][i] = mnmx->ms[i];
	}
}


int AOplus::determineNumStrats(int numCombos) {
	computeMaximin();
	if (numPolicies == A[me]) {  // attack strat is pure
		if (isPure(mnmx->ms))
			return numCombos;
		else
			return numCombos + 1;
	}
	else {						 // attack strat is mixed
		if (isSame(mnmx->ms, policies[A[me]])) {
			return numCombos;
		}
		else {
			return numCombos + 1;
		}
	}
}

int AOplus::findSteadyState(omegaPair *oPair) {
	int acts[2], i, s;
	bool *visited = new bool[numStates];
	for (i = 0; i < numStates; i++)
		visited[i] = false;
	
	s = rand() % numStates;
	while (!visited[s]) {
		visited[s] = true;
		acts[me] = oPair->ares->selectAction(s);
		acts[1-me] = oPair->thars->selectAction(s);
		s = oPair->ares->encodeJointAction(acts);
	}
	delete visited;
	
	return s;
}


