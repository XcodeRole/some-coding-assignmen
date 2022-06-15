#include "defs.h"
#include "omegaStrategy.h"
#include "Mala.h"
#include "Rmax.h"
#include "Rndm.h"
#include "Bully.h"
#include "Godfather.h"
#include "jefe.h"
#include "jefe++.h"
#include "WoLF.h"
#include "QLearner.h"
#include "minimaxLog.h"
#include "playStrat.h"
#include "filePlayer.h"
#include "MQubed.h"
#include "Manipulator.h"
#include "CJAL.h"
#include "Exp3_act.h"
#include "fp.h"
#include "gigawolf.h"
#include "HedgeWMA.h"
#include "Salg.h"
#include "sfp.h"
#include "DeepQ.h"
#include "Pavlov.h"
#include "Memory1.h"
#include "Memory2.h"

void runGame(double ***M, int A[NUM_PLAYERS], int len, int indy);
double ***readPayoffMatrixFromFile(int _A[2], const char *game);
double ***readPayoffMatrix(int _A[NUM_PLAYERS], const char *game);
void cleanPayoffMatrix(double ***_M, int _A[NUM_PLAYERS]);
void recordSummaryResults(double resultados[50000][4], int len, int indy);

Mala *createMala(int index, char name[1024], double ***_M, int A[NUM_PLAYERS]);
void printGame(int _A[2], double ***_M);

double ***createNoiseMatrix(int A[2], double ***_M, int index, double stdDev);
double *generateNormalCDF(double _mu, double _sigma, int num);
double sampleNormal(double *CDF, int numCDFelements, double mu, double sigma);

void writeMaximinLog(int A[2], double ***M);
minimaxLog *computeMaximin(int index, int A[2], double ***M);
minimaxLog *computeAttack(int index, int A[2], double ***M);

char agent0[1024], agent1[1024], game[1024];

int numSamps = 1;

int iteraciones;

bool randomGame = false;

bool createNoise = false;//true;
double ***nM0, ***nM1;

extern void Littman(Cell pago[NumDecisionPointsMax][NumDecisionPointsMax], int c[2], int a1[2], int a2[2], int _numDPA);
double NB(int index);
char theGame[1024], gamer[1024];


/* ****************************************************
//
//	./game <game> <agent0> <agent1> <iters> <numSamps>
//  ./game prisoners S++ br1 1000 1
// **************************************************** */


int main(int argc, char** argv) {
/* ****************************************************
	//  argc[0] = ./game
	//  argc[1] = prisoners
	//  argc[2] = S++
	//  argc[3] = br1
	//  argc[4] = 1000 (round)
	//  argc[5] = 1 (numSamps)
//***************************************************** */
	srand(time(NULL)); // 以当前时间作为随机种子
	int i, j;

    if (argc >= 5) {
        iteraciones = atoi(argv[4]); // round
    }
    
	double ***M;
	int A[NUM_PLAYERS];
	if (argc < 2)
		M = readPayoffMatrixFromFile(A, "prisoners");
	else {
        M = readPayoffMatrixFromFile(A, argv[1]);   // 读取博弈矩阵 
    }
    
	strcpy(game, argv[1]);   //将game赋值为prisoners
	strcpy(agent0, argv[2]); //将agent0赋值为S++
	strcpy(agent1, argv[3]);//将agent1赋值为br1
    
    if (argc > 5)
        numSamps = atoi(argv[5]);  //numSamps 采样次数
    
	for (i = 0; i < numSamps; i++) {
        if (randomGame) {
            if (numSamps == 1)
                sprintf(game, "%s_%i", argv[1], i+rand()%50);
            else    
                sprintf(game, "%s_%i", argv[1], i);
            M = readPayoffMatrixFromFile(A, game);
        }
        
		if (argc < 5)
			runGame(M, A, 100, i); // 没有round参数就跑100轮
        else
            runGame(M, A, atoi(argv[4]), i);  // 有round参数就跑round轮
	}
    
	cleanPayoffMatrix(M, A);

	return 0;
}

void runGame(double ***M, int A[2], int len, int indy) { // indy 表示现在是第几个采样轮只是用来记录
	int i, j;
	int *act = new int[2];
	char buf[1024];
	double total[2] = {0.0, 0.0};
	double n;
    
    double resultados[50000][4]; // resultados:[0]记录palyer0当轮的行动 [1]记录palyer1当轮的行动 [2]记录palyer0当轮的收益 [3]记录palyer1当轮的收益
    
    sprintf(gamer, "%s_%s_%s_%i", game, agent0, agent1, indy);
    
    printf("before creating players\n");
    
	Mala *player0, *player1;
    if (createNoise) {
        player0 = createMala(0, agent0, nM0, A);
        player1 = createMala(1, agent1, nM1, A);        
    }
    else {
        player0 = createMala(0, agent0, M, A); //创建Male，0表示player0，将agent0(算法名称)传入
        player1 = createMala(1, agent1, M, A); //创建Male，1表示player1，将agent1(算法名称)传入
    }
    
    printf("a\n"); fflush(stdout);
    
    if (player0 == NULL) {     // 检验是否两个player都创建成功
        if (player1 != NULL)
            delete player1;
        return;
    }
    else if (player1 == NULL) {
        if (player1 != NULL)
            delete player0;
        return;
    }
    
    sprintf(buf, "../log/%s_%s_%s_%i.txt", game, agent0, agent1, indy);
    //printf("%s\n", buf);
    FILE *logfp = fopen(buf, "w");
    
	for (i = 0; i < len; i++) {   // 博弈本体
		act[0] = player0->move();
		act[1] = player1->move();
		
        // if ((i % 500) == 0)  // 这部分只是每500轮输出一下当轮的结果
		// 	printf("%i : (%i, %i) -> %.2lf, %.2lf\n", i, act[0], act[1], M[0][act[0]][act[1]], M[1][act[0]][act[1]]);
        
        // fprintf(logfp, "%i\t%i\t%lf\t%lf\n", act[0], act[1], M[0][act[0]][act[1]], M[1][act[0]][act[1]]);
		
		for (j = 0; j < NUM_PLAYERS; j++) {
			total[j] += M[j][act[0]][act[1]]; // 累计双方总收益
        }

        resultados[i][0] = act[0];
        resultados[i][1] = act[1];
        resultados[i][2] = M[0][act[0]][act[1]];
        resultados[i][3] = M[1][act[0]][act[1]];
		// resultados:[0]记录palyer0当轮的行动 [1]记录palyer1当轮的行动 [2]记录palyer0当轮的收益 [3]记录palyer1当轮的收益
        
		//fprintf(fp, "%lf %lf\n", M[0][act[0]][act[1]], M[1][act[0]][act[1]]);
        
        //printf("1  *%i, %i*  ", act[0], act[1]); fflush(stdout);
		player0->update(act);
        //printf("2"); fflush(stdout);
		player1->update(act);
        //printf("3"); fflush(stdout);
	}
	
	printf("\nAverages: (%.3lf, %.3lf)\n\n", total[0] / len, total[1] / len);
	printf("\n");

	delete player0;
	delete player1;
    
    //printf("Deleted the player\n"); fflush(stdout);
	
    recordSummaryResults(resultados, len, indy);
    
    //printf("Recorded the summary\n"); fflush(stdout);
    
	//fflush(fp);
	fclose(logfp);
}

void recordSummaryResults(double resultados[50000][4], int len, int indy) {
    char fnombre[1024];
    sprintf(fnombre, "../results/%s_%s_%s_%i.txt", game, agent0, agent1, indy);
    FILE *fpres = fopen(fnombre, "w");
    
    int i;
    double sum[2] = {0.0, 0.0};
    for (i = 0; i < len; i++) {
        sum[0] += resultados[i][2];
        sum[1] += resultados[i][3];
    }
    fprintf(fpres, "allRounds:\t%lf\t%lf\n", sum[0] / len, sum[1] / len);
    
    sum[0] = sum[1] = 0.0;
    for (i = 0; i < 100; i++) {
        sum[0] += resultados[i][2];
        sum[1] += resultados[i][3];
    }
    fprintf(fpres, "first100:\t%lf\t%lf\n", sum[0] / 100, sum[1] / 100);
    
    sum[0] = sum[1] = 0.0;
    for (i = 0; i < 1000; i++) {
        sum[0] += resultados[i][2];
        sum[1] += resultados[i][3];
    }
    fprintf(fpres, "first1000:\t%lf\t%lf\n", sum[0] / 1000, sum[1] / 1000);

    if (len > 1000) {
        sum[0] = sum[1] = 0.0;
        for (i = len - 1000; i < len; i++) {
            sum[0] += resultados[i][2];
            sum[1] += resultados[i][3];
        }
        fprintf(fpres, "last1000:\t%lf\t%lf\n", sum[0] / 1000, sum[1] / 1000);
    }
    
    fclose(fpres);
}

Mala *createMala(int index, char name[1024], double ***M, int A[NUM_PLAYERS]) {
    Mala *pl = NULL;
    
    if (!strcmp(name, "S")) {
		printf("%s\n", name);
		pl = new jefe(name, index, A, M, 0.99, gamer);
	}

    else if (!strcmp(name, "S_simp")) {
		printf("%s\n", name);
		pl = new jefe(name, index, A, M, 0.99, gamer);
	}

	else if (!strcmp(name, "S++")) {
		printf("%s\n", name);
       	pl = new jefe_plus(name, index, A, M, 0.99, gamer);
	}
    
	else if (!strcmp(name, "S++_simp")) {
		printf("%s\n", name);
       	pl = new jefe_plus(name, index, A, M, 0.99, gamer);
	}

	else if (!strcmp(name, "exp3w")) {
		printf("%s\n", name);
		pl = new jefe(name, index, A, M, iteraciones, game);//iteraciones);
	}

	else if (!strcmp(name, "exp3w_simp")) {
		printf("%s\n", name);
		pl = new jefe(name, index, A, M, iteraciones, game);//iteraciones);
	}

	else if (!strcmp(name, "exp3w++")) {
		printf("%s\n", name);
		pl = new jefe_plus(name, index, A, M, iteraciones, game);//iteraciones);
	}

	else if (!strcmp(name, "eeew")) {
		printf("%s\n", name);		
		pl = new jefe(name, index, A, M, 0.99, game);//iteraciones);
	}		

	else if (!strcmp(name, "eeew_simp")) {
		printf("%s\n", name);		
		pl = new jefe(name, index, A, M, 0.99, game);//iteraciones);
	}		

	else if (!strcmp(name, "eeew++")) {
		printf("%s\n", name);		
		pl = new jefe_plus(name, index, A, M, 0.99, game);//iteraciones);
	}		

	else if (!strcmp(name, "ucbw")) {
		printf("%s\n", name);		
		pl = new jefe(name, index, A, M, 0.99, game);//iteraciones);
	}
    
	else if (!strcmp(name, "ucbw++")) {
		printf("%s\n", name);		
		pl = new jefe_plus(name, index, A, M, 0.99, game);//iteraciones);
	}

	else if (!strcmp(name, "br1")) {
		printf("%s => rmax\n", name);		
		pl = new Rmax(index, A, M, 1, 1, 0.95);
	}
	
	else if (!strcmp(name, "br2")) {
		printf("%s => rmax\n", name);		
		pl = new Rmax(index, A, M, 2, 1, 0.95);
	}

	else if (!strcmp(name, "wolf")) {
		printf("%s => wolf\n", name);		
		pl = new WoLF(index, M, A, 0, false);
	}
    
	else if (!strcmp(name, "qlearn")) {
		printf("%s => qlearn\n", name);		
		pl = new QLearner(index, M, A, 0.95, true);
	}
    
	else if (!strcmp(name, "mqubed")) {
		printf("%s => mqubed\n", name);		
		pl = new MQubed(2, M, A, index, 0.95, 0.1, 4, 1, false, false, false, false, false, false, false, game);
	}

	else if (!strcmp(name, "manipulator-bully")) {
		printf("%s => manipulator-bully\n", name);		
		pl = new Manipulator(index, A, M, 150, 300, game, true);
	}

	else if (!strcmp(name, "manipulator-gf")) {
		printf("%s => manipulator-godfather\n", name);		
		pl = new Manipulator(index, A, M, 150, 300, game, false);
	}
    
	else if (!strcmp(name, "bully")) {
		printf("%s => Bully\n", name);
        pl = new Bully(index, A, M, game);
	}
	
	else if (!strcmp(name, "godfather")) {
		printf("%s => godfather\n", name);		
		pl = new Godfather(index, A, M, game);
	}
	
	else if (!strcmp(name, "random")) {
		printf("%s => random\n", name);
		double *v = new double[A[index]];
		for (int i = 0; i < A[index]; i++)
			v[i] = 1.0 / A[index];
		pl = new Rndm(A[index], v);
		delete[] v;
	}
	
	else if (!strcmp(name, "a0")) {
		printf("%s => coop\n", name);
		double *v = new double[A[index]];
		v[0] = 1.0;
		v[1] = 0.0;
		pl = new Rndm(A[index], v);
		delete[] v;
	}
	
	else if (!strcmp(name, "a1")) {
		printf("%s => coop\n", name);
		double *v = new double[A[index]];
		v[0] = 0.0;
		v[1] = 1.0;
		pl = new Rndm(A[index], v);
		delete[] v;
	}

    else if (!strncmp(name, "expert", 6)) {
        printf("%s => expert\n", name);
        pl = new playStrat(index, A, M, game, atoi(name+6));
        
        if (!(((playStrat *)pl)->expertExists)) {
            printf("stopping things\n"); fflush(stdout);
            delete pl;
            pl = NULL;
        }
    }
    
    else if (!strcmp(name, "cjal")) {
        printf("%s => CJAL\n", name);
        
        pl = new CJAL(2, A, M, index, 100, 0.0, false);
    }

    else if (!strcmp(name, "fp")) {
        printf("%s => FP\n", name);
        
        pl = new FP(A[0], 1, index, 0, M);
    }
    
    else if (!strcmp(name, "exp3_a")) {
        printf("%s => Exp3_act\n", name);
        
        pl = new Exp3_act(A[0], index, 50000, M);
    }
    
    else if (!strcmp(name, "gigawolf")) {
        printf("%s => gigawolf\n", name);
        
        pl = new GIGA_WoLF(2, A, index, false, M);
    }
    
    else if (!strcmp(name, "wma")) {
        printf("%s => wma\n", name);
        
        pl = new HedgeWMA(A[0], index, 0.99, false, M);
    }
    
    else if (!strcmp(name, "salg")) {
        printf("%s => salg\n", name);
        
        pl = new SAlg(A[0], 0.99, index, M);
    }
    
    else if (!strcmp(name, "sfp")) {
        printf("%s => sfp\n", name);
        
        pl = new sFP(A[0], 1, index, 0, false, M);
    }
    
    else if (!strcmp(name, "pavlov")) {
        printf("%s => pavlov\n", name);
        
        pl = new Pavlov(A, index, M);
    }

    else if (!strcmp(name, "memory1")) {
        printf("%s => memory1\n", name);
        
        pl = new Memory1(A, index, game);
    }
    
    else if (!strcmp(name, "memory2")) {
        printf("%s => memory2\n", name);
        
        pl = new Memory2(A, index, game);
    }

    else if (!strcmp(name, "deepq")) {
        printf("%s => deepq\n", name);
        
        pl = new DeepQ(index, A, M, 9, 9, 9);
    }

    else if (!strncmp(name, "fPl_", 4)) {
        printf("%s => filePlayer\n", name);
        pl = new filePlayer(index, A, M, name+4);
    }
    
    else {
        printf("player %s not found\n", name);
        exit(1);
    }
	
	return pl;
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
    printGame(_A, _M);
	
    if (createNoise) {
        nM0 = createNoiseMatrix(_A, _M, 0, 0.30);
        nM1 = createNoiseMatrix(_A, _M, 1, 0.30);
    }
    
	return _M;
}


double ***readPayoffMatrix(int _A[2], const char *game) {
	double ***_M;
	
	_A[0] = 2;
	_A[1] = 2;
	int i, j;
	_M = new double**[2];
	for (i = 0; i < 2; i++) {
		_M[i] = new double*[_A[0]];
		for (j = 0; j < _A[0]; j++)
			_M[i][j] = new double[_A[1]];
	}
	
	if (!strcmp(game, "prisoners")) {
		printf("prisoner's dilemma:\n");
		_M[0][0][0] = 3.0;
		_M[0][0][1] = 0.0;
		_M[0][1][0] = 5.0;
		_M[0][1][1] = 1.0;
		//mm_val[0] = 1.0;
		
		_M[1][0][0] = 3.0;
		_M[1][0][1] = 5.0;
		_M[1][1][0] = 0.0;
		_M[1][1][1] = 1.0;
		//mm_val[1] = 1.0;
	}
	else if (!strcmp(game, "bofs")) {
		printf("battle of the sexes:\n");
		_M[0][0][0] = 2.0;
		_M[0][0][1] = 4.0;
		_M[0][1][0] = 3.0;
		_M[0][1][1] = 1.0;
		//mm_val[0] = 2.5;
		
		_M[1][0][0] = 2.0;
		_M[1][0][1] = 3.0;
		_M[1][1][0] = 4.0;
		_M[1][1][1] = 1.0;
		//mm_val[1] = 2.5;
	}
	else if (!strcmp(game, "tricky")) {
		printf("tricky game:\n");
		_M[0][0][0] = 0.0;
		_M[0][0][1] = 3.0;
		_M[0][1][0] = 1.0;
		_M[0][1][1] = 2.0;
		//mm_val[0] = 1.5;
		
		_M[1][0][0] = 3.0;
		_M[1][0][1] = 2.0;
		_M[1][1][0] = 0.0;
		_M[1][1][1] = 1.0;
		//mm_val[0] = 1.5;		
	}
	else if (!strcmp(game, "chicken")) {
		printf("chicken:\n");
		_M[0][0][0] = 3.0;
		_M[0][0][1] = 1.0;
		_M[0][1][0] = 4.0;
		_M[0][1][1] = 0.0;
		//mm_val[0] = 1.0;
		
		_M[1][0][0] = 3.0;
		_M[1][0][1] = 4.0;
		_M[1][1][0] = 1.0;
		_M[1][1][1] = 0.0;
		//mm_val[1] = 1.0;
	}
	else if (!strcmp(game, "staghunt")) {
		printf("chicken:\n");
		_M[0][0][0] = 4.0;
		_M[0][0][1] = -5.0;
		_M[0][1][0] = 3.0;
		_M[0][1][1] = 2.0;
		//mm_val[0] = 1.0;
		
		_M[1][0][0] = 4.0;
		_M[1][0][1] = 3.0;
		_M[1][1][0] = -5.0;
		_M[1][1][1] = 2.0;
		//mm_val[1] = 1.0;
	}
	else {
		printf("game %s not found\n", game);
		exit(1);
	}
	
	printGame(_A, _M);
	
	return _M;
}

void cleanPayoffMatrix(double ***_M, int _A[2]) {
	int i, j;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < _A[0]; j++)
			delete _M[i][j];
		delete _M[i];
	}
	delete _M;
    
    if (createNoise) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < _A[0]; j++) {
                delete nM0[i][j];
                delete nM1[i][j];
            }
            delete nM0[i];
            delete nM1[i];
        }
        delete nM0;
        delete nM1;
    }
}

double ***createNoiseMatrix(int A[2], double ***_M, int index, double stdDev) {
    int i, j;
    double ***M = new double**[2];
    for (i = 0; i < 2; i++) {
        M[i] = new double*[A[0]];
        for (j = 0; j < A[0]; j++)
            M[i][j] = new double[A[1]];
    }
    
    double mu = 0.0, sigma = stdDev;
    double *CDF = generateNormalCDF(mu, sigma, 100);
    
    //for (i = 0; i < 50; i++) {
    //    printf("%lf\n", sampleNormal(CDF, 100, mu, sigma));
    //}
    
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            M[index][i][j] = _M[index][i][j];
            M[1-index][i][j] = _M[1-index][i][j] + sampleNormal(CDF, 100, mu, sigma);
            
            //if (M[1-index][i][j] > 1.0)
            //    M[1-index][i][j] = 1.0;
            //if (M[1-index][i][j] < 0.0)
            //    M[1-index][i][j] = 0.0;
        }
    }
    
    printf("distorted game for %i\n", index);
    printGame(A, M);
    
    return M;
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

