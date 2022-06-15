#include "algX.h"

algX::algX() {
	printf("entered into incomplete algX constructor\n");
}

algX::algX(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
	int i;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	lambda = _lambda;
	
	verbose = false;
	explore = new double[A[me]];
	
	deriveStrategyPairs();
	
	estado = -1;
	t = 0;	
	
	v = 0.0;
	currentStrategy = NULL;
}

// this only works for 2x2 action games
void algX::deriveStrategyPairs() {
	int i, j, val;
	
	if (verbose)
		printf("BR(long-term model)\n---------------\n");

	numIModels = A[0] * A[1] * A[0] * A[1] + 1;//pow(myBRs[0]->thars->numStates, A[1-me]);
	
	myBRs = new omegaPair*[numIModels+1];
	myBRs[0] = new omegaPair(me, A, M, 1, UNKNOWN);
	
	maximin = new omegaPair(me, A, M, 1, ATTACKER);
	
	kappa = new int*[myBRs[0]->thars->numStates];
	for (i = 0; i < myBRs[0]->thars->numStates; i++) {
		kappa[i] = new int[A[1-me]];
		for (j = 0; j < A[1-me]; j++)
			kappa[i][j] = 1;
	}
	
	if (verbose)
		printf("\nBR(i-models)\n---------------\n");

	int *strats = new int[myBRs[0]->thars->numStates];
	
	for (i = 1; i < numIModels; i++) {
		if (verbose)
			printf("  %i strategy: ", 1-me);
		for (j = 0; j < myBRs[0]->thars->numStates; j++) {
			val = pow(2, j);
			if ((i-1) & val)
				strats[j] = 1.0;
			else
				strats[j] = 0.0;
			
			if (verbose)
				printf("%i    ", strats[j]);
		}
		if (verbose)
			printf("\n");
		
		myBRs[i] = new omegaPair(me, A, M, 1, TEACHER, strats);
	}
	
	if (verbose)
		printf("\nTeachers\n---------------\n");
	numTeachers = pow(myBRs[0]->ares->numStates, A[1-me]);
	myTeachers = new omegaPair*[numTeachers];

	for (i = 0; i < numTeachers; i++) {
		if (verbose)
			printf("  %i strategy: ", me);		
		for (j = 0; j < myBRs[0]->thars->numStates; j++) {
			val = pow(2, j);
			if (i & val)
				strats[j] = 1.0;
			else
				strats[j] = 0.0;
			
			if (verbose)
				printf("%i    ", strats[j]);
		}
		if (verbose)
			printf("\n");
		
		myTeachers[i] = new omegaPair(me, A, M, 1, FOLLOWER, strats);
	}
	
	im = new iModel(me, A, 1);
	
	switch2BRcount = 0;
	
	delete strats;
}

algX::~algX() {
	int i;
	
	for (i = 0; i < numTeachers; i++)
		delete myTeachers[i];
	for (i = 0; i < myBRs[0]->thars->numStates; i++)
		 delete kappa[i];
	for (i = 0; i < numIModels; i++)
		delete myBRs[i];
	
	delete kappa;
	delete myTeachers;
	delete myBRs;
	delete maximin;
}

int algX::move() {
	if (t == 0) {
		sCuenta = 0;
		return rand() % A[me];
	}
	else if (t == 1) {
		setAspiration0(estado);
		
		findViableStrategies();
		int ind = selectViableTeacher();

		//printf("%i picked (%i)\n", me, ind);
		currentStrategy = myTeachers[viableTeacherStrats[ind]];
		
		resetUtils(viableTeacherStrats[ind]);
	}
	else {
		double rho = computeInertia();
		double num = (double)(rand()) / RAND_MAX;
		//printf("%lf > %lf?\n", num, rho);
		if (num > rho) {
			findViableStrategies();
			
			if ((numViableTeachers > 0) || (numViableFollowers > 0)) {
				int i;
				int ind = selectViableStrategy();

				//if (ind < numViableTeachers)
				//	printf("%i picked teaching strategy %i (%i): ", me, ind, viableTeacherStrats[ind]);
				//else
				//	printf("%i picked follower strategy %i (%i): ", me, ind, viableFollowerStrats[ind - numViableTeachers]);
				if (ind < numViableTeachers)
					currentStrategy = myTeachers[viableTeacherStrats[ind]];
				else {
					currentStrategy = myBRs[viableFollowerStrats[ind-numViableTeachers]];
					
					if (viableFollowerStrats[ind-numViableTeachers] == 0) 
						currentValue = myBRs[0]->ares->Valor[estado];
				}
				//for (i = 0; i < currentStrategy->ares->numStates; i++)
				//	printf("%i", 1 - (int)(currentStrategy->ares->pi[i][0]));
				
				//printf("\n");
				
				if (ind < numViableTeachers)
					resetUtils(viableTeacherStrats[ind]);
				else
					resetUtils(viableFollowerStrats[ind-numViableTeachers] + numTeachers);
			}
			
			double epsilon = t / 1000.0;
			num = (double)(rand()) / RAND_MAX;
			if ((num < epsilon) && (myBRs[0]->ares->Valor[estado] > aspiration)) {
				currentStrategy = myBRs[0];
				printf("switched to best response\n");
				resetUtils(numTeachers);
				
				currentValue = myBRs[0]->ares->Valor[estado];
				switch2BRcount ++;
			}
		}
		
		/*double epsilon = 0.1;//t / 200.0;
		num = (double)rand() / RAND_MAX;
		if ((myBRs[0]->computeHypotheticals(5, kappa, estado, aspiration, explore)) && (num < epsilon)) {
			num = (double)(rand()) / (double)(RAND_MAX);
			int i;
			double sum = 0.0;
			for (i = 0; i < A[me]; i++) {
				sum += explore[i];
				if (num < sum)
					break;
			}
			if (i == A[me])
				i--;
			
			printf("exploring action %i\n", i);
			return i;
		}*/		
	}
	
	return currentStrategy->ares->selectAction(estado);
}

void algX::update(int *acts) {
	int sprime = myBRs[0]->ares->encodeJointAction(acts);
	if (estado >= 0) {
		kappa[estado][acts[1-me]] ++;
		myBRs[0]->updateUnknown(kappa, estado);
		im->update(acts, estado, t);
		
		currentStrategy->r += M[me][acts[0]][acts[1]];
		currentStrategy->usageCount ++;
	}	
	
	estado = sprime;
	t++;

	sCuenta ++;
	v += M[me][acts[0]][acts[1]];
						
	aspiration = lambda * M[me][acts[0]][acts[1]] + (1.0 - lambda) * aspiration;
	//printf("%i: aspiration = %lf\n", me, aspiration);	
}

void algX::resetUtils(int ind) {
	sCuenta = 0;
	v = 0.0;
	desiredTarget = ind;
	startState = estado;
}

double algX::computeInertia() {
	double vprime;
	
	if (currentStrategy == myBRs[0])
		vprime = currentValue;  // this needs to change
		//vprime = myBRs[0]->ares->Valor[startState];
	else
		vprime = currentStrategy->shouldHave(startState, sCuenta);
	
	double rho = 1.0;
	
	if (vprime > 0) {
		if (vprime > maximin->ares->Valor[0])
			rho = ((v / sCuenta) - maximin->ares->Valor[0]) / (vprime - maximin->ares->Valor[0]);
		else
			rho = (v / sCuenta) / vprime;
		if (rho > 1.0)
			rho = 1.0;
		if (rho < 0.0)
			rho = 0.0;
	}	
	
	//printf("%i: v = %.2lf; vprime = %.2lf (%i); rho = %.2lf ::: ", sCuenta, v / sCuenta, vprime, startState, rho);
	
	return rho;
}


void algX::findViableStrategies() {
	int i, j, k;
	
	// go through teacher strategies to find which ones are satisficing
	//printf("   satisficing teaching strategies for %i\n", me);
	numViableTeachers = 0;
	for (i = 0; i < numTeachers; i++) {
		if (myTeachers[i]->ares->Valor[estado] >= aspiration) {
			viableTeacherStrats[numViableTeachers] = i;
			numViableTeachers ++;
			
			//printf("      %i: ", numViableTeachers-1);
			//for (j = 0; j < myTeachers[i]->ares->numStates; j++) {
			//	printf("%i", 1 - (int)(myTeachers[i]->ares->pi[j][0]));
			//}
			//printf("\t%.2lf\tmu=%.2lf; kappa=%i; sw=%.2lf\n", myTeachers[i]->ares->Valor[estado], 
			//	   myTeachers[i]->average(), myTeachers[i]->usageCount, myTeachers[i]->socialWelfare(estado));
		}
	}
	
	// go through follower strategies to find which ones are satisficing given im (the current iModel)
	//printf("   satisficing follower strategies against ");
	//im->print();
	numViableFollowers = 0;
	
	//if (myBRs[0]->ares->Valor[estado] >= aspiration) {
	//	viableFollowerStrats[numViableFollowers] = 0;
	//	numViableFollowers ++;
	//}	
	for (i = 1; i < numIModels; i++) {
		if ((myBRs[i]->ares->Valor[estado] >= aspiration) && im->match(myBRs[i]->thars)) {
			viableFollowerStrats[numViableFollowers] = i;
			numViableFollowers ++;
		}
	}
	for (i = 0; i < numViableFollowers; i++) {
		for (j = 0; j < i; j++) {
			if (myBRs[viableFollowerStrats[i]]->ares->matches(myBRs[viableFollowerStrats[j]]->ares)) {
				int prune = i;
				if (myBRs[viableFollowerStrats[i]]->ares->Valor[estado] > myBRs[viableFollowerStrats[j]]->ares->Valor[estado])
					prune = j;
				
				for (k = prune; k < numViableFollowers - 1; k++)
					viableFollowerStrats[k] = viableFollowerStrats[k+1];
				numViableFollowers --;
				i --;
				break;
			}
		}
	}
/*	for (i = 0; i < numViableFollowers; i++) {
		printf("      %i: ", i + numViableTeachers);
		for (j = 0; j < myBRs[viableFollowerStrats[i]]->ares->numStates; j++) {
			printf("%i", 1-(int)(myBRs[viableFollowerStrats[i]]->ares->pi[j][0]));
		}
		printf("\t(");
		if (viableFollowerStrats[i] == 0) {
			printf("xxxx");
		}
		else {
			for (j = 0; j < myBRs[viableFollowerStrats[i]]->thars->numStates; j++)
				printf("%i", 1-(int)(myBRs[viableFollowerStrats[i]]->thars->pi[j][0]));
		}
		

		//printf("%.2lf, %.2lf", myBRs[0]->ares->Valor[estado], myBRs[1]->thars->Valor[estado]);
		
		printf(")\t%.2lf\tmu=%.2lf; kappa=%i; sw=%.2lf\n", myBRs[viableFollowerStrats[i]]->ares->Valor[estado],
			   myBRs[viableFollowerStrats[i]]->average(), myBRs[viableFollowerStrats[i]]->usageCount, myBRs[viableFollowerStrats[i]]->socialWelfare(estado));
	}	*/
}

int algX::selectViableTeacher() {
	return rand() % numViableTeachers; // for now
}

int algX::selectViableStrategy() {
	double *UTeach = new double[numViableTeachers];
	double *UFollow = new double[numViableFollowers];
	int i;
	double w;
	double T = 1.0 * pow(0.998, t);
	if (T < 0.01)
		T = 0.01;
	
	double sum = 0.0;
	for (i = 0; i < numViableTeachers; i++) {
		w = 10.0 / (10.0 + myTeachers[viableTeacherStrats[i]]->usageCount);
		UTeach[i] = w * myTeachers[viableTeacherStrats[i]]->socialWelfare(estado) + 
					(1-w) * myTeachers[viableTeacherStrats[i]]->average();
		sum += exp(UTeach[i] / T);
	}
	
	for (i = 0; i < numViableFollowers; i++) {
		w = 10.0 / (10.0 + myBRs[viableFollowerStrats[i]]->usageCount);
		UFollow[i] = w * myBRs[viableFollowerStrats[i]]->socialWelfare(estado) + 
					(1-w) * myBRs[viableFollowerStrats[i]]->average();
		sum += exp(UFollow[i] / T);
	}
	
	for (i = 0; i < numViableTeachers; i++) {
		//UTeach[i] /= sum;
		UTeach[i] = exp(UTeach[i] / T) / sum;
		//printf("%.2lf ", UTeach[i]);
	}
	//printf("; ");
	for (i = 0; i < numViableFollowers; i++) {
		//UFollow[i] /= sum;
		UFollow[i] = exp(UFollow[i] / T) / sum;
		//printf("%.2lf ", UFollow[i]);
	}
	//printf("\n");
	
	double num = (double)(rand()) / RAND_MAX;
	sum = 0.0;
	int ind = -1;
	for (i = 0; i < numViableTeachers; i++) {
		sum += UTeach[i];
		if (num <= sum) {
			ind = i;
			break;
		}
	}
	if (ind < 0) {
		for (i = 0; i < numViableFollowers; i++) {
			sum += UFollow[i];
			if (num <= sum) {
				ind = i + numViableTeachers;
				break;
			}
		}
	}
	
	if (ind < 0) {
		printf("didn't select an action: %lf; %lf\n", num, sum);
		exit(1);
	}
	
	delete UTeach;
	delete UFollow;
	
	return ind;
	//return rand() % (numViableTeachers + numViableFollowers);
}

void algX::setAspiration0(int _estado) {
	int i, ind = 0;
	for (i = 1; i < numTeachers; i++) {
		if (myTeachers[i]->ares->Valor[_estado] > myTeachers[ind]->ares->Valor[_estado])
			ind = i;
		else if ((myTeachers[i]->ares->Valor[_estado] == myTeachers[ind]->ares->Valor[_estado]) && (rand() % 2)) {
			ind = i;
		}
	}
	
	aspiration = myTeachers[ind]->ares->Valor[_estado] - 0.000001;
	printf("%i: initial aspiration level = %.2lf\n", me, aspiration);
}




