#include "Rndm.h"

Rndm::Rndm() {
	printf("incomplete Rndm constructor\n");
	exit(1);
}

Rndm::Rndm(int _numActs, double *_p) {
	numActs = _numActs;
	
	p = new double[numActs];
	for (int i = 0; i < numActs; i++)
		p[i] = _p[i];
	
}

Rndm::~Rndm() {
	delete p;
}

int Rndm::move() {
	double sum = 0.0, num = (double)(rand()) / RAND_MAX;
	int i;
	
	for (i = 0; i < numActs - 1; i++) {
		sum += p[i];
		if (num < sum)
			return i;
	}
	return numActs - 1;
}

void Rndm::update(int *acts) {	
}
