#include "defs.h"
#include "gene.h"
#include "gene2.h"

#define GRID_WIDTH      32
#define GRID_HEIGHT     32
#define NUM_PLAYERS     2

void initMemory1(int *A, double ***M);
void initMemory2(int *A, double ***M);
void evolveMemory1(int *A, double ***M);
void evolveMemory2(int *A, double ***M);
void game(Gene *row, Gene *col, int rowIndex, int colIndex);
void game2(Gene2 *row, Gene2 *col, int rowIndex, int colIndex);
int getInd(int num);
void replace(int i, int j, int ind2[2]);
void replace2(int i, int j, int ind2[2]);
//void replace(Gene ***players, int ind2[2], double mag);
double ***readPayoffMatrixFromFile(int _A[2], const char *game);
void cleanPayoffMatrix(double ***_M, int _A[2]);
void printGame(int _A[2], double ***_M);
void cleanGridMemory1();
void cleanGridMemory2();
void printTopHalf();
void printTopHalf2();

//Gene ***rowPlayers;
//Gene ***colPlayers;
Gene ***Players;
Gene2 ***Players2;

double replacementRate;
double mutationRate;

char theGame[1024];

//double ***MM, total;

int main(int argc, char *argv[]) {
    srand(time(NULL));

	double ***M;
	int A[NUM_PLAYERS];
    
    // first evolve the memory 1 strategy
    M = readPayoffMatrixFromFile(A, argv[1]);
    strcpy(theGame, argv[1]);

    initMemory1(A, M);
    if (argc > 3) {
        replacementRate = atof(argv[2]);
        mutationRate = atof(argv[3]);
    }
    else {
        replacementRate = 0.01;
        mutationRate = 0.01;
    }
    
    evolveMemory1(A, M);
    
    cleanPayoffMatrix(M, A);
    cleanGridMemory1();
    //cleanGridMemory2();




    // next evolve the memory 2 strategy
    M = readPayoffMatrixFromFile(A, argv[1]);
    strcpy(theGame, argv[1]);

    initMemory2(A, M);
    
    if (argc > 3) {
        replacementRate = atof(argv[2]);
        mutationRate = atof(argv[3]);
    }
    else {
        replacementRate = 0.01;
        mutationRate = 0.01;
    }
    evolveMemory2(A, M);
    
    cleanPayoffMatrix(M, A);
    cleanGridMemory2();


    return 0;
}

void evolveMemory1(int *A, double ***M) {
    int gen, w, h, j, i;
    int numGenerations = 1;
    double num, sum;
    int ind2[2];
    double aveGenome[10];
    //double aveGenomeCol[5];
    
    printf("evolveMemory1; this is going to take a minute or two.  Please be patient.\n"); fflush(stdout);
    
    for (gen = 0; gen < 500000; gen++) {
        //printf("gen=%i\n", gen);
        //printf("gen = %i\n", gen); fflush(stdout);
        // everyone plays a round of the game with each of their neighbors (as both the row and col player)
        for (w = 0; w < GRID_WIDTH; w++) {
            for (h = 0; h < GRID_HEIGHT; h++) {
                game(Players[w][h], Players[getInd(w-1)][h], 0, 4);
                game(Players[w][h], Players[getInd(w-1)][getInd(h-1)], 1, 5);
                game(Players[w][h], Players[w][getInd(h-1)], 2, 6);
                game(Players[w][h], Players[getInd(w+1)][getInd(h-1)], 3, 7);
                
                game(Players[w][h], Players[getInd(w+1)][h], 4, 0);
                game(Players[w][h], Players[getInd(w+1)][getInd(h+1)], 5, 1);
                game(Players[w][h], Players[w][getInd(h+1)], 6, 2);
                game(Players[w][h], Players[getInd(w-1)][getInd(h+1)], 7, 3);
            }
        }
        
        //printf("aca\n"); fflush(stdout);

        // decide who to replace
        for (w = 0; w < GRID_WIDTH; w++) {
            for (h = 0; h < GRID_HEIGHT; h++) {
                num = rand() / (double)RAND_MAX;
                if (num <= replacementRate) { // replace this player with a neighbor
                    replace(w, h, ind2);
                    //printf("Replayed %i, %i with %i, %i ... ", w, h, ind2[0], ind2[1]); fflush(stdout);
                    Players[w][h]->reset(Players[ind2[0]][ind2[1]]->oldGenome, mutationRate);
                    //printf("reset\n"); fflush(stdout);
                    for (j = 0; j < 1; j++) {
                        Players[getInd(w-1)][getInd(h-1)]->nuevo[5][j] = true;
                        Players[getInd(w-1)][getInd(h)]->nuevo[4][j] = true;
                        Players[getInd(w-1)][getInd(h+1)]->nuevo[3][j] = true;
                        Players[getInd(w)][getInd(h-1)]->nuevo[6][j] = true;
                        Players[getInd(w)][getInd(h+1)]->nuevo[2][j] = true;
                        Players[getInd(w+1)][getInd(h-1)]->nuevo[7][j] = true;
                        Players[getInd(w+1)][getInd(h)]->nuevo[0][j] = true;
                        Players[getInd(w+1)][getInd(h+1)]->nuevo[1][j] = true;
                    }
                }
            }
        }

        //printf("aqui\n"); fflush(stdout);


        if ((gen % 1000) == 0) {
            //printf("gen=%i; ", gen); fflush(stdout);

            for (i = 0; i < 10; i++) {
                aveGenome[i] = 0.0;
            }

            sum = 0.0;
            for (w = 0; w < GRID_WIDTH; w++) {
                for (h = 0; h < GRID_HEIGHT; h++) {
                    //printf("%i, %i\n", rowPlayers[w][h]->oldRound, colPlayers[w][h]->oldRound); fflush(stdout);
                    sum += pow(Players[w][h]->oldScore / Players[w][h]->oldRound, 1.0);
                    for (i = 0; i < 10; i++) {
                        aveGenome[i] += Players[w][h]->genome[i];
                    }
                }
            }
 /*
            printf("%i: Average: ", gen);
            for (i = 0; i < 10; i++) {
                printf("%.3lf ", aveGenome[i] / (GRID_WIDTH * GRID_HEIGHT));
                if (i == 4)
                    printf("   ");
            }
            printf("; %.2lf\n", sum / (GRID_WIDTH * GRID_HEIGHT));
*/
        }
    }

    printTopHalf();
    
}

void evolveMemory2(int *A, double ***M) {
    int gen, w, h, j, i;
    int numGenerations = 1;
    double num, sum;
    int ind2[2];
    double aveGenome[34];
    //double aveGenomeCol[5];
    
    printf("evolveMemory2; this is going to take even longer.  Please be patient.\n"); fflush(stdout);    
    
    for (gen = 0; gen < 1000000; gen++) {
        //printf("gen = %i\n", gen); fflush(stdout);
        // everyone plays a round of the game with each of their neighbors (as both the row and col player)
        //total = 0.0;
        for (w = 0; w < GRID_WIDTH; w++) {
            for (h = 0; h < GRID_HEIGHT; h++) {
                game2(Players2[w][h], Players2[getInd(w-1)][h], 0, 4);
                game2(Players2[w][h], Players2[getInd(w-1)][getInd(h-1)], 1, 5);
                game2(Players2[w][h], Players2[w][getInd(h-1)], 2, 6);
                game2(Players2[w][h], Players2[getInd(w+1)][getInd(h-1)], 3, 7);
                
                game2(Players2[w][h], Players2[getInd(w+1)][h], 4, 0);
                game2(Players2[w][h], Players2[getInd(w+1)][getInd(h+1)], 5, 1);
                game2(Players2[w][h], Players2[w][getInd(h+1)], 6, 2);
                game2(Players2[w][h], Players2[getInd(w-1)][getInd(h+1)], 7, 3);
            }
        }
        //printf("total = %lf\n", total);
        
        //printf("aca\n"); fflush(stdout);

        // decide who to replace
        for (w = 0; w < GRID_WIDTH; w++) {
            for (h = 0; h < GRID_HEIGHT; h++) {
                //printf("%lf (%i)  ", Players2[w][h]->score / Players2[w][h]->round, Players2[w][h]->round);
            
                num = rand() / (double)RAND_MAX;
                if (num <= replacementRate) { // replace this player with a neighbor
                    replace2(w, h, ind2);
                    //printf("Replayed %i, %i with %i, %i ... ", w, h, ind2[0], ind2[1]); fflush(stdout);
                    Players2[w][h]->reset(Players2[ind2[0]][ind2[1]]->oldGenome, mutationRate);
                    //printf("reset\n"); fflush(stdout);
                    for (j = 0; j < 1; j++) {
                        Players2[getInd(w-1)][getInd(h-1)]->restart(5, j);
                        Players2[getInd(w-1)][getInd(h)]->restart(4, j);
                        Players2[getInd(w-1)][getInd(h+1)]->restart(3, j);
                        Players2[getInd(w)][getInd(h-1)]->restart(6, j);
                        Players2[getInd(w)][getInd(h+1)]->restart(2, j);
                        Players2[getInd(w+1)][getInd(h-1)]->restart(7, j);
                        Players2[getInd(w+1)][getInd(h)]->restart(0, j);
                        Players2[getInd(w+1)][getInd(h+1)]->restart(1, j);
                    }
                }
            }
            //printf("\n");
        }

        //printf("aqui\n"); fflush(stdout);


        if ((gen % 1000) == 0) {
            for (i = 0; i < 34; i++) {
                aveGenome[i] = 0.0;
            }

            sum = 0.0;
            for (w = 0; w < GRID_WIDTH; w++) {
                for (h = 0; h < GRID_HEIGHT; h++) {
                    //printf("%i, %i\n", rowPlayers[w][h]->oldRound, colPlayers[w][h]->oldRound); fflush(stdout);
                    sum += pow(Players2[w][h]->oldScore / Players2[w][h]->oldRound, 1.0);
                    for (i = 0; i < 34; i++) {
                        aveGenome[i] += Players2[w][h]->genome[i];
                    }
                }
            }
/*
            printf("%i: Average: ", gen);
            for (i = 0; i < 34; i++) {
                printf("%.3lf ", aveGenome[i] / (GRID_WIDTH * GRID_HEIGHT));
                if (i == 16)
                    printf("   ");
            }
            printf("; %.4lf\n", sum / (GRID_WIDTH * GRID_HEIGHT));*/
        }
    }
    
    //printf("alli\n"); fflush(stdout);

    printTopHalf2();
    
}

void printTopHalf() {
    int ind, i, w, h;
    double scores[GRID_HEIGHT * GRID_WIDTH+1];
    int count = 0;
    double val;
    
    for (w = 0; w < GRID_WIDTH; w++) {
        for (h = 0; h < GRID_HEIGHT; h++) {
            val = Players[w][h]->oldScore / Players[w][h]->oldRound;
            
            //printf("val = %lf\n", val);
            
            ind = 0;
            while ((scores[ind] >= val) && (ind < count)) {
                ind++;
            }
            for (i = count; i > ind; i--) {
                scores[i] = scores[i-1];
            }
            scores[ind] = val;
            
            count ++;
        }
    }

    double threshold = scores[GRID_WIDTH * GRID_HEIGHT / 2];
    double aveGenome[10];
    for (i = 0; i < 10; i++) {
        aveGenome[i] = 0.0;
    }
    
    count = 0;
    for (w = 0; w < GRID_WIDTH; w++) {
        for (h = 0; h < GRID_HEIGHT; h++) {
            val = Players[w][h]->oldScore / Players[w][h]->oldRound;
            
            if (val >= threshold) {
                for (i = 0; i < 10; i++) {
                    aveGenome[i] += Players[w][h]->genome[i];
                }
                count ++;
            }
        }
    }
    
    printf("Final genotype: ");
    for (i = 0; i < 10; i++) {
        printf("%.3lf ", aveGenome[i] / count);
    }
    printf("\n");
    
    char fname[1024];
    sprintf(fname, "../Memory_one/%s.txt", theGame);
    FILE *fp = fopen(fname, "w");
    for (i = 0; i < 10; i++)
        fprintf(fp, "%lf\n", aveGenome[i] / count);
    fclose(fp);
}

void printTopHalf2() {
    int ind, i, w, h;
    double scores[GRID_HEIGHT * GRID_WIDTH+1];
    int count = 0;
    double val;
    
    for (w = 0; w < GRID_WIDTH; w++) {
        for (h = 0; h < GRID_HEIGHT; h++) {
            val = Players2[w][h]->oldScore / Players2[w][h]->oldRound;
            
            //printf("val = %lf\n", val);
            
            ind = 0;
            while ((scores[ind] >= val) && (ind < count)) {
                ind++;
            }
            for (i = count; i > ind; i--) {
                scores[i] = scores[i-1];
            }
            scores[ind] = val;
            
            count ++;
        }
    }

    double threshold = scores[GRID_WIDTH * GRID_HEIGHT / 2];
    double aveGenome[34];
    for (i = 0; i < 34; i++) {
        aveGenome[i] = 0.0;
    }
    
    count = 0;
    for (w = 0; w < GRID_WIDTH; w++) {
        for (h = 0; h < GRID_HEIGHT; h++) {
            val = Players2[w][h]->oldScore / Players2[w][h]->oldRound;
            
            if (val >= threshold) {
                for (i = 0; i < 34; i++) {
                    aveGenome[i] += Players2[w][h]->genome[i];
                }
                count ++;
            }
        }
    }
    
    printf("Final genotype: ");
    for (i = 0; i < 34; i++) {
        printf("%.3lf ", aveGenome[i] / count);
    }
    printf("\n");
    
    char fname[1024];
    sprintf(fname, "../Memory_two/%s.txt", theGame);
    FILE *fp = fopen(fname, "w");
    for (i = 0; i < 34; i++)
        fprintf(fp, "%lf\n", aveGenome[i] / count);
    fclose(fp);
}


void replace(int i, int j, int ind2[2]) {
    double mag = 0.0;
    int w, h;
    
    //printf("%i, %i: ", i, j);
    for (w = i-1; w < i+2; w++) {
        for (h = j-1; h < j+2; h++) {
            mag += pow(Players[getInd(w)][getInd(h)]->oldScore / Players[getInd(w)][getInd(h)]->oldRound, 3.0);
            //printf("<%i, %i>; ", w, h);
        }
    }
    //printf("\n");
    
    if (mag == 0) {
        printf("mag problem\n");
        exit(1);
    }
    
    double num = rand() / (double)RAND_MAX;
    //printf("num = %lf; mag = %lf\n", num, mag);
    double sum = 0.0;
    for (w = i-1; w < i+2; w++) {
        for (h = j-1; h < j+2; h++) {
            sum += pow(Players[getInd(w)][getInd(h)]->oldScore / Players[getInd(w)][getInd(h)]->oldRound, 3.0);
            if (num < (sum / mag)) {
                ind2[0] = getInd(w);
                ind2[1] = getInd(h);
                //printf("picked: %i, %i\n", w, h);
                //exit(1);
                return;
            }
        }
    }
    
    printf("major replace problems\n");
    exit(1);
}

void replace2(int i, int j, int ind2[2]) {
    double mag = 0.0;
    int w, h;
    
    //printf("%i, %i: ", i, j);
    for (w = i-1; w < i+2; w++) {
        for (h = j-1; h < j+2; h++) {
            mag += pow(Players2[getInd(w)][getInd(h)]->oldScore / Players2[getInd(w)][getInd(h)]->oldRound, 3.0);
            //printf("<%i, %i>; ", w, h);
        }
    }
    //printf("\n");
    
    if (mag == 0) {
        printf("mag problem\n");
        exit(1);
    }
    
    double num = rand() / (double)RAND_MAX;
    //printf("num = %lf; mag = %lf\n", num, mag);
    double sum = 0.0;
    for (w = i-1; w < i+2; w++) {
        for (h = j-1; h < j+2; h++) {
            sum += pow(Players2[getInd(w)][getInd(h)]->oldScore / Players2[getInd(w)][getInd(h)]->oldRound, 3.0);
            if (num < (sum / mag)) {
                ind2[0] = getInd(w);
                ind2[1] = getInd(h);
                //printf("picked: %i, %i\n", w, h);
                //exit(1);
                return;
            }
        }
    }
    
    printf("major replace problems\n");
    exit(1);
}

void game(Gene *row, Gene *col, int rowIndex, int colIndex) {
    int acts[2];
    
    acts[0] = row->act(true, rowIndex);
    acts[1] = col->act(false, colIndex);

    row->update(true, acts, rowIndex);
    col->update(false, acts, colIndex);
}

void game2(Gene2 *row, Gene2 *col, int rowIndex, int colIndex) {
    int acts[2];
    
    acts[0] = row->act(true, rowIndex);
    acts[1] = col->act(false, colIndex);
    
    //total += MM[0][acts[0]][acts[1]];
    //total += MM[1][acts[0]][acts[1]];

    row->update(true, acts, rowIndex);
    col->update(false, acts, colIndex);
}

int getInd(int num) {
    if (num < 0)
        num = GRID_WIDTH-1;
    if (num >= GRID_WIDTH)
        num = 0;

    return num;
}

void initMemory1(int *A, double ***M) {
    int i, j;
    double randomGene[10] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    //double randomGene[10] = {0.0, 0.3, 0.0, 0.9, 0.0, 0.0, 0.7, 1.0, 1.0, 1.0};
    //double randomGene[10] = {0.0, 0.9, 0.0, 1.0, 0.0, 0.0, 0.1, 1.0, 1.0, 1.0};

    //rowPlayers = new Gene**[GRID_WIDTH];
    //colPlayers = new Gene**[GRID_WIDTH];
    Players = new Gene**[GRID_WIDTH];

    for (i = 0; i < GRID_WIDTH; i++) {
        //rowPlayers[i] = new Gene*[GRID_HEIGHT];
        //colPlayers[i] = new Gene*[GRID_HEIGHT];
        Players[i] = new Gene*[GRID_HEIGHT];
        for (j = 0; j < GRID_HEIGHT; j++) {
            //rowPlayers[i][j] = new Gene(0, A, M, randomGene);
            //colPlayers[i][j] = new Gene(1, A, M, randomGene);
            Players[i][j] = new Gene(A, M, randomGene);
        }
    }
}

void initMemory2(int *A, double ***M) {
    int i, j;
    double randomGene[34];
    
    for (i = 0; i < 34; i++)
        randomGene[i] = 0.5;

    //rowPlayers = new Gene**[GRID_WIDTH];
    //colPlayers = new Gene**[GRID_WIDTH];
    Players2 = new Gene2**[GRID_WIDTH];

    for (i = 0; i < GRID_WIDTH; i++) {
        //rowPlayers[i] = new Gene*[GRID_HEIGHT];
        //colPlayers[i] = new Gene*[GRID_HEIGHT];
        Players2[i] = new Gene2*[GRID_HEIGHT];
        for (j = 0; j < GRID_HEIGHT; j++) {
            //rowPlayers[i][j] = new Gene(0, A, M, randomGene);
            //colPlayers[i][j] = new Gene(1, A, M, randomGene);
            Players2[i][j] = new Gene2(A, M, randomGene);
        }
    }
}


double ***readPayoffMatrixFromFile(int _A[2], const char *game) {
	double ***_M;
    
	char filename[1024];
	sprintf(filename, "..//..//games//%s.txt", game);
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		// check in an alternate directory before giving up
		sprintf(filename, "..//games//%s.txt", game);
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("file %s not found\n", filename);
			exit(1);
		}
	}

	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[1]));
	
	int i, j;
	_M = new double**[2];
	for (i = 0; i < 2; i++) {
		_M[i] = new double*[_A[0]];
		for (j = 0; j < _A[0]; j++)
			_M[i][j] = new double[_A[1]];
	}

	for (i = 0; i < _A[1]; i++) {
		for (j = 0; j < _A[0]; j++) {
			fscanf(fp, "%lf %lf", &(_M[0][j][i]), &(_M[1][j][i]));
		}
	}
    
    printf("game: %s\n", game);
    //printGame(_A, _M);
    
	return _M;
}

void printGame(int _A[2], double ***_M) {
    int i, j;
    
    printf("\n   |      ");
    
    for (i = 0; i < _A[1]; i++)
        printf("%i      |      ", i);
    printf("\n");
    for (i = 0; i < _A[0]; i++) {
        printf("--------------------------------------\n %i | ", i);
        for (j = 0; j < _A[1]; j++) {
            printf("%.2lf , %.2lf | ", _M[0][i][j], _M[1][i][j]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n\n");
}

void cleanPayoffMatrix(double ***_M, int _A[2]) {
	int i, j;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < _A[0]; j++)
			delete _M[i][j];
		delete _M[i];
	}
	delete _M;
}

void cleanGridMemory1() {
    int i, j;
    
    for (i = 0; i < GRID_WIDTH; i++) {
        for (j = 0; j < GRID_HEIGHT; j++) {
            //delete rowPlayers[i][j];
            //delete colPlayers[i][j];
            delete Players[i][j];
        }
        //delete rowPlayers[i];
        //delete colPlayers[i];
        delete Players[i];
    }
    
    //delete rowPlayers;
    //delete colPlayers;
    delete Players;
}

void cleanGridMemory2() {
    int i, j;
    
    for (i = 0; i < GRID_WIDTH; i++) {
        for (j = 0; j < GRID_HEIGHT; j++) {
            //delete rowPlayers[i][j];
            //delete colPlayers[i][j];
            delete Players2[i][j];
        }
        //delete rowPlayers[i];
        //delete colPlayers[i];
        delete Players2[i];
    }
    
    //delete rowPlayers;
    //delete colPlayers;
    delete Players2;
}

