#include "UOplus.h"

UOplus::UOplus() {
	printf("In incomplete UOplus constructor\n");
	exit(1);
}

UOplus::UOplus(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda, bool _satSelector) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	lambda = _lambda;
	satSelector = _satSelector;
	
	deriveStrategies();

	im = new iModel(me, A, 1);
	br = new BR(me, A, M);
	
	x = new double[numStrats];
	n = new int[numStrats];
	
	for (i = 0; i < numStrats; i++) {
		n[i] = 0;
		x[i] = 0.0;
	}
	
	num = 0;
	count = 0;
	r = 0.0;
	a = -1;
	t = 0;
	estado = -1;
}

void UOplus::deriveStrategies() {
	int i, j, k, val1, val2, num;
	
	numStates = A[0] * A[1];
	numTeachingStrats = numIStrats = pow(A[me], numStates);
	numStrats = numTeachingStrats+1;
	
	beenThere = new bool[numStates];
	theTeachingStrats = new omegaPair*[numTeachingStrats];
	
	int *strats = new int[numStates];
	for (i = 0; i < numTeachingStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(A[me], j);
			num = i % val1;
			num = i - num;
			val2 = pow(A[me], j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		//printf("init teaching strat %i of %i\n", i, numTeachingStrats);
		theTeachingStrats[i] = new omegaPair(me, A, M, 1, FOLLOWER, strats);
	}
	
	theIStrats = new omegaPair*[numIStrats];
	for (i = 0; i < numIStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(A[1-me], j);
			num = i % val1;
			num = i - num;
			val2 = pow(A[1-me], j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		theIStrats[i] = new omegaPair(me, A, M, 1, TEACHER, strats);
	}
	
	delete strats;
	
	theStrats = new omegaStrategy*[numStrats];
	for (i = 0; i < numTeachingStrats; i++)
		theStrats[i] = theTeachingStrats[i]->ares;

	minimaxStuff();
	
	if (mmIndex == numTeachingStrats);
		theStrats[numTeachingStrats] = minimaxStrat;

	selectable = new bool[numStrats];
	for (i = 0; i < numStrats; i++)
		selectable[i] = true;
	
	left = new bool[numStrats];
}

void UOplus::minimaxStuff() {
	int i, j;
	
	mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int c = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[c] = M[me][i][j];
			c ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
	printf("%i: mv = %.3lf; ms = [%.2lf %.2lf]\n", me, mnmx->mv, mnmx->ms[0], mnmx->ms[1]);
	
	if (!isPure(mnmx->ms)) {
		minimaxStrat = new omegaStrategy(me, A, 1);
		for (j = 0; j < numStates; j++) {
			for (i = 0; i < A[me]; i++)
				minimaxStrat->pi[j][i] = mnmx->ms[i];
		}
		mmIndex = numTeachingStrats;
	}
	else {
		numStrats --;
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
				mmIndex = i;
				printf("maximin (%.3lf; %.3lf) is strat %i: ", mnmx->ms[0], mnmx->ms[1], i);
				theStrats[i]->printStrat();
				printf("\n");
				break;
			}
		}
	}
	
	alwaysMM = false;	
	permissibleLoss = 40.0;
	mu = 0.0;
}

UOplus::~UOplus() {
	delete x;
	delete n;
	delete beenThere;

	int i;
	delete theStrats;
	
	for (i = 0; i < numTeachingStrats; i++) {
		delete theTeachingStrats[i];
		delete theIStrats[i];
	}
	delete theTeachingStrats;
	delete theIStrats;
	
	delete im;
	delete br;
	delete mnmx;
	delete selectable;
	delete left;
	
	if (mmIndex == numTeachingStrats)
		delete minimaxStrat;
}

int UOplus::move() {
	if (t == 0)
		return rand() % A[me];
	
	if (t == 1)
		setAspiration0();
	
	//printf("%i: aspiration = %.3lf\n", me, aspiration);
	
	if (count == 0) {
		findSatisficing();
		
		int i, numLeft = 0;
		for (i = 0; i < numStrats; i++) {
			if ((n[i] == 0) && selectable[i]) {
				left[i] = true;
				numLeft ++;
			}
			else
				left[i] = false;
		}
		
		if (numLeft == 0) {
			a = -1;
			double best = -99999;
			double val;
			for (int i = 0; i < numStrats; i++) {
				if (selectable[i]) {
					val = (x[i] / n[i]) + sqrt(2.0 * log((double)num) / n[i]);
					if ((val > best) || ((val == best) && (rand() % 2))) {
						a = i;
						best = val;
					}
				}
			}
		}
		else {
			a = tryUntried(numLeft);
		}
	}
	
	if ((a < 0) || (a > (numStrats-1))) {
		printf("tenemous un a problem\n");
		exit(1);
	}
	//else {
	//	printf("%i: count = %i\n", me, count);
	//}

	return theStrats[a]->selectAction(estado);
}

int UOplus::tryUntried(int numLeft) {
	int i;
	int index = rand() % numLeft, c = 0, act = -1;
	for (i = 0; i < numStrats; i++) {
		if (left[i])
			c++;
		if (index < c) {
			act = i;
			break;
		}
	}
	
	return act;
	
}

void UOplus::update(int *acts) {
	mu += M[me][acts[0]][acts[1]];
	
	br->update(acts);	
	if (estado >= 0)
		im->update(acts, estado, t);
	
	estado = theStrats[0]->encodeJointAction(acts);
	
	if (t > 0) {
		//printf("a = %i\n", a);
		double betita = 1.0 / (theStrats[a]->usageCount + 1);
		if (betita <= 2.0 * lambda)
			betita = 2.0 * lambda;
		theStrats[a]->r = betita * M[me][acts[0]][acts[1]] + (1.0 - betita) * theStrats[a]->r;
		theStrats[a]->usageCount ++;

		r += M[me][acts[0]][acts[1]];
		count ++;
		
		if (beenThere[estado]) {
			//printf("outcome = %.3lf\n", r / count);
			x[a] += r;
			n[a] += count;
			num += count;

			for (int i = 0; i < count; i++)
				aspiration = lambda * (r / count) + (1.0 - lambda) * aspiration;
			
			//printf("updated %i: %.3lf, %i, %i\n", a, x[a], n[a], num);
			
			count = 0;
			r = 0.0;
			
			for (int i = 0; i < numStates; i++)
				beenThere[i] = false;
		}
		if (((mnmx->mv * t) - mu) > permissibleLoss) {
			printf("I am begin exploited (loss = %.2lf)!!!!!!!!!  Trigger to my maximin strategy\n", (mnmx->mv * t) - mu);
			alwaysMM = true;
		}
	}
	
	beenThere[estado] = true;
	
	t++;
}


void UOplus::setAspiration0() {
	int i, j;
	double high = -99999;
	
	for (i = 0; i < numTeachingStrats; i++) {
		if (high < theTeachingStrats[i]->ares->Valor[estado])
			high = theTeachingStrats[i]->ares->Valor[estado];
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
}


void UOplus::findSatisficing() {
	int i, j;
	for (i = 0; i < numStrats; i++)
		selectable[i] = false;

	if (alwaysMM) {
		selectable[mmIndex] = true;
		return;
	}
	
	int numviable = 0;
	double val, highval = -99999;
	for (i = 0; i < numTeachingStrats; i++) {
		if (theTeachingStrats[i]->ares->Valor[estado] >= aspiration) {
			if (theStrats[i]->usageCount == 0)
				val = 1.0;
			else
				val = theStrats[i]->r;
			if (val > highval)
				highval = val;
			selectable[i] = true;
			numviable ++;
		}
		else if (satSelector)
			selectable[i] = false;
		else
			selectable[i] = true;
	}
	
	for (i = 0; i < numIStrats; i++) {
		if ((theIStrats[i]->ares->Valor[estado] >= aspiration)  && (im->match(theIStrats[i]->thars))) {
			int ind = whichOneIsIt(theIStrats[i]->ares);
			if (theStrats[ind]->usageCount == 0)
				val = 1.0;
			else
				val = theStrats[ind]->r;
			if (val > highval)
				highval = val;

			if (!selectable[ind]) {
				numviable ++;
			}
			
			selectable[ind] = true;
		}
	}
		
	if (br->myBR->ares->Valor[estado] >= aspiration) {
		int index = whichOneIsIt(br->myBR->ares);
		if ((theStrats[index]->r > highval) || (theStrats[index]->usageCount == 0)) {
			for (i = 0; i < numTeachingStrats; i++)
				selectable[i] = false;
			
			numviable = 0;
		}
		
		selectable[index] = true;
		numviable ++;
	}
	
	if (numviable == 0) {
		int index = whichOneIsIt(br->myBR->ares);
		selectable[index] = true;
		numviable = 1;
	}
	
/*	printf("Selectable Strategies for Player %i (numviable = %i)\n", me, numviable);
	fflush(stdout);
	for (i = 0; i < numStrats; i++) {
		if (selectable[i]) {
			theStrats[i]->printStrat();
			printf("\n");
			fflush(stdout);
		}
	}
	printf("\n");
	fflush(stdout);*/
}

bool UOplus::alreadyAdded(omegaPair *istrat, omegaPair **satStrats, int numviable) {
	int i;
	
	for (i = 0; i < numviable; i++) {
		if (istrat->ares->matches(satStrats[i]->ares))
			return true;
	}
	
	return false;
}

int UOplus::whichOneIsIt(omegaStrategy *strat) {
	int i;
	
	for (i = 0; i < numTeachingStrats; i++) {
		if (strat->matches(theStrats[i]))
			return i;
	}
	
	printf("couldn't find it\n");
	exit(1);
	
	return -1;
}


bool UOplus::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.0) && (v[i] < 1.0))
			return false;
	}
	
	return true;
}

bool UOplus::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}
