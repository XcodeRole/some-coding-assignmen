#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

typedef struct Cell {
	double r1, r2;
} Cell;

#define NumDecisionPointsMax    5

#define NUM_PLAYERS		2
#define MAXLENGTH		2

#define MAX_EXPERTS     500

#endif