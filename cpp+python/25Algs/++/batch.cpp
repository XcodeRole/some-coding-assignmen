#include "defs.h"

#define cSTATIC     0
#define cRL         1
#define cEXPERT     2
#define cOTHER      3

typedef struct {
    double v100[19500*25];
    double v1000[19500*25];
    double v50000[19500*25];
    double mean100, mean1000, mean50000;
    double stdev100, stdev1000, stdev50000;
    int count;
} partnerEntries;

typedef struct {
    double vals[19500*25];
    double mean;
    double stdev;
    int count;
} lengthEntries;

typedef struct {
    double vals[19500*25];
    double mean;
    double stdev;
    int count;
} gameEntries;


char agents[30][1024];
int numAgents = 25;
char games[5][1024];
int numGames = 5;

double meanGameScore[3][5][78][2];
double meanGameScore2[3][5][144];
int countGameScore2[3][5][144];

partnerEntries pEntries[4];
lengthEntries lEntries[3];
gameEntries gEntries[3];

bool zerosum = true;
bool common = true;

void nameAgents() {
    strcpy(agents[0], "bully");
	strcpy(agents[1], "S++");
    strcpy(agents[2], "manipulator-bully");
	strcpy(agents[3], "mqubed");
	strcpy(agents[4], "br2");
    strcpy(agents[5], "qlearn");
    strcpy(agents[6], "eeew");
	strcpy(agents[7], "S");
	strcpy(agents[8], "br1");
    strcpy(agents[9], "wolf");
    strcpy(agents[10], "cjal");
    strcpy(agents[11], "fp");
    strcpy(agents[12], "wma");
    strcpy(agents[13], "manipulator-gf");
    strcpy(agents[14], "godfather");
    strcpy(agents[15], "exp3w_simp");
    strcpy(agents[16], "gigawolf");
    strcpy(agents[17], "random");
    strcpy(agents[18], "eeew_simp");
    strcpy(agents[19], "sfp");
    strcpy(agents[20], "pavlov");
    strcpy(agents[21], "memory1");
    strcpy(agents[22], "memory2");
    strcpy(agents[23], "exp3w");
    strcpy(agents[24], "S++_simp");
    //strcpy(agents[25], "S_simp");
    strcpy(agents[25], "eeew++");
}

void nameGames() {
	strcpy(games[0], "cg_standard");
	strcpy(games[1], "cg_sqrt");
	strcpy(games[2], "cg_sep");
	strcpy(games[3], "cg_pow2");
	strcpy(games[4], "cg_pow3");
}


void runGames_rgsg() {
    int i, j, count;
    char thecmd[1024];
    
    for (count = 0; count < 50; count++) {
        for (i = 0; i < numAgents; i++) {
            for (j = 0; j < numAgents; j++) {
                sprintf(thecmd, "./game rgsg_2x3_%i %s %s 50000", count, agents[i], agents[j]);
                system(thecmd);
            }
        }
    }
}

void runGame(int index) {
    int i, j, count;
    char thecmd[1024];
    
    for (count = 1; count < 79; count++) {
        for (i = 0; i < numAgents; i++) {
            for (j = 0; j < numAgents; j++) {
                if ((i == 16) || (j == 16) || (i == 19) || (j == 19) || (i == 25) || (j == 25)) {
                    sprintf(thecmd, "./game %s_%i %s %s 50000", games[index], count, agents[i], agents[j]);
                    system(thecmd);
                }
            }
        }
    }
}

void runGames_self(int index) {
    int i, j, count;
    char thecmd[1024];
    
    for (count = 1; count < 79; count++) {
        for (i = 0; i < numAgents; i++) {
            sprintf(thecmd, "./game %s_%i %s %s 50000", games[index], count, agents[i], agents[i]);
            system(thecmd);
        }
    }
}

void runGame_Ssimp(int index) {
    int i, j, count;
    char thecmd[1024];
    
    for (count = 1; count < 79; count++) {
        for (j = 0; j < numAgents; j++) {
            sprintf(thecmd, "./game %s_%i S_simp %s 50000", games[index], count, agents[j]);
            system(thecmd);
            sprintf(thecmd, "./game %s_%i %s S_simp 50000", games[index], count, agents[j]);
            system(thecmd);
        }
    }
}

void runGames() {
    int i;
    
    for (i = 0; i < 5; i++) {
        //runGame(i);
        runGame_Ssimp(i);
    }
}


bool paretoOptimal(int a1, int a2, double Payoffs[2][2][2]) {
    int i, j, k, l;
    double v1, v2;
    //printf("<%lf, %lf>", Payoffs[0][a1][a2], Payoffs[1][a1][a2]);
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if ((Payoffs[0][a1][a2] < Payoffs[0][i][j]) && (Payoffs[1][a1][a2] < Payoffs[1][i][j]))
                return false;
        }
    }
/*
    // now look at combinations of point instead of just the points
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 2; l++) {
                    v1 = (Payoffs[0][i][j] + Payoffs[0][k][l]) / 2.0;
                    v2 = (Payoffs[1][i][j] + Payoffs[1][k][l]) / 2.0;
                    if ((Payoffs[0][a1][a2] < v1) && (Payoffs[1][a1][a2] < v2)) {
                        printf("got here\n");
                        return false;
                    }
                }
            }
        }
    }
*/
    return true;
}

bool NBS(double point[2], int g, int index) {
    char nombre[1024];
   
    sprintf(nombre, "../results21/NBS/%s_%i", games[g], index);
    //printf("%s\n", nombre);
    FILE *fp = fopen(nombre, "r");
    
    double v1, v2;
    
    fscanf(fp, "%lf %lf", &v1, &v2);
    
    fclose(fp);
    
    //printf("$%lf, %lf$\n", v1, v2);
    if ((point[0] == v1) && (point[1] == v2))
        return true;
    else
        return false;
}

bool pureNBS(int g, int index) {
    char nombre[1024];
   
    sprintf(nombre, "../results21/NBS/%s_%i", games[g], index);
    //printf("%s\n", nombre);
    FILE *fp = fopen(nombre, "r");
    
    double NBSv1, NBSv2;
    
    fscanf(fp, "%lf %lf", &NBSv1, &NBSv2);
    
    fclose(fp);


    double Payoffs[2][2][2];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    fp = fopen(nombre, "r");

    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    int i, j;
    bool puro = false;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
            
            if ((Payoffs[0][i][j] == NBSv1) && (Payoffs[1][i][j] == NBSv2))
                puro = true;
        }
    }

    fclose(fp);
    
    return puro;
}

bool commonInterestGame(int g, int index) {
    FILE *fp;
    char nombre[1024];
    int d1, d2, d3;
    double v1, v2;
    int i;
    
    zerosum = true;
    common = true;
    bool easy = false;
    double constant = -99999;
    
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    
    //printf("nombre: %s\n", nombre); fflush(stdout);
    
    fp = fopen(nombre, "r");

    fscanf(fp, "%i %i %i", &d1, &d2, &d3);
    for (i = 0; i < 4; i++) {
        fscanf(fp, "%lf %lf", &v1, &v2);
        
        if (v1 != v2) {
            common = false;
        }
        
        if ((v1 >= 1.000) && (v2 >= 1.000))
            easy = true;
        
        if (constant < 0)
            constant = v1 + v2;
        else {
            if ((v1 + v2) != constant)
                zerosum = false;
        }
    }

    fclose(fp);
    
    if (zerosum || common || easy)
        return true;
    else
        return false;
}

bool uninteresting(int g, int index) {
    bool isinteresting = false;

    double Payoffs[2][2][2];
    commonInterestGame(g, index);

    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    
    //printf("nombre: %s\n", nombre); fflush(stdout);
    
    FILE *fp = fopen(nombre, "r");
    
    //printf("a"); fflush(stdout);

    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    //printf("b"); fflush(stdout);

    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);

    bool hasNE = false;
    double point[2];
    for (i = 0; i < d2; i++) {
        for (j = 0; j < d3; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                hasNE = true;
                if (paretoOptimal(i, j, Payoffs)) {
                    point[0] = Payoffs[0][i][j];
                    point[1] = Payoffs[1][i][j];
                    if (!NBS(point, g, index))
                        isinteresting = true;
                }
                else
                    isinteresting = true;
            }
        }
    }

    if (!hasNE)
        isinteresting = true;
    
    return isinteresting;
}

int NEConfusion(int g, int index) {
    double Payoffs[2][2][2];
    commonInterestGame(g, index);

    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    FILE *fp = fopen(nombre, "r");
    
    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);

    int NEcount = 0, countPONE = 0;
    for (i = 0; i < d2; i++) {
        for (j = 0; j < d3; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                NEcount ++;
                if (paretoOptimal(i, j, Payoffs))
                    countPONE ++;
            }
        }
    }

    if ((NEcount > 1) && (countPONE > 0)) //|| ((countPONE == 0) && (NEcount == 1)))
        return 1;
    else if (countPONE == 0)
        return 2;
    
    return 0;
}


void listNE(int g, int index) {
    double Payoffs[2][2][2];
    commonInterestGame(g, index);

    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    FILE *fp = fopen(nombre, "r");

    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);
    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);
    
    bool NE[2][2];
    printf("%i ", index);
    if (common)
        printf("CI ");
    if (zerosum)
        printf("ZS ");
    printf(": ");
    double point[2];
    for (i = 0; i < d2; i++) {
        for (j = 0; j < d3; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                NE[i][j] = true;
                if (paretoOptimal(i, j, Payoffs)) {
                    point[0] = Payoffs[0][i][j];
                    point[1] = Payoffs[1][i][j];
                    if (NBS(point, g, index))
                        printf("pNBS(%i, %i);  ", i, j);
                    else
                        printf("p(%i, %i);  ", i, j);
                }
                else
                    printf("d(%i, %i);  ", i, j);
            }
            else
                NE[i][j] = false;
        }
    }
    //printf("\n");
}

bool amongSelected(int index) {
//    if ((index != 0) && (index != 2) && (index != 5) && (index != 18) && (index != 7))
//        return false;
    if ((index != 0) && (index != 2) && (index != 5) && (index != 18) && (index != 7) &&
        (index != 6) && (index != 15) && (index != 3) && (index != 13) && (index != 17))
        return false;
    
    return true;
}

void printGame(int g, int index) {
    int i, j;
    
    int _A[2] = {2, 2};
    double _M[2][2][2];
    
    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    FILE *fp = fopen(nombre, "r");
    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(_M[0][i][j]), &(_M[1][i][j]));
        }
    }

    fclose(fp);
    
    
    printf("   |      ");
    
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

void computeTheNEs(int g, int index) {
    double Payoffs[2][2][2];

    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    FILE *fp = fopen(nombre, "r");
    
    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);

    for (i = 0; i < d2; i++) {
        for (j = 0; j < d3; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                if (paretoOptimal(i, j, Payoffs)) {
                    printf("Pareto optimal NE: %i and %i\n", i, j);
                }
                else {
                    printf("Pareto dominated NE: %i and %i\n", i, j);
                }
            }
        }
    }
}

void getPoint(double p1[2], double p2[2], double pt[2], double t) {
    pt[0] = (1-t) * p1[0] + t * p2[0];
    pt[1] = (1-t) * p1[1] + t * p2[1];
}

void computeTheNBS(int g, int index) {
    //double nbs1, nbs2;
    double mv[2], pi[2];
    char nombre[1024];
    double Payoffs[2][2][2];
/*
    sprintf(nombre, "../results/NBS/%s_%i", games[g], index);
    FILE *fp = fopen(nombre, "r");
    fscanf(fp, "%lf %lf", &nbs1, &nbs2);
    fclose(fp);

    printf("NBS: %.2lf, %.2lf\n", nbs1, nbs2);
    //bool NBS(double point[2], int g, int index) {
*/

    sprintf(nombre, "../results/maximinVals/%s_%i", games[g], index);
    FILE *fp = fopen(nombre, "r");
    fscanf(fp, "%lf %lf %lf", &(mv[0]), &(pi[0]), &(pi[1]));
    fscanf(fp, "%lf %lf %lf", &(mv[1]), &(pi[0]), &(pi[1]));
    fclose(fp);
    printf("maxmin: %.3lf, %.3lf\n", mv[0], mv[1]);

    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    fp = fopen(nombre, "r");
    
    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    int i, j, m, n;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);
    
    double p1[2], p2[2], pt[2];
    double thePt[2], maxprod = -1.0, prod, x, y;
    bool alt = false;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            p1[0] = Payoffs[0][i][j];
            p1[1] = Payoffs[1][i][j];
            for (m = 0; m < 2; m++) {
                for (n = 0; n < 2; n++) {
                    p2[0] = Payoffs[0][m][n];
                    p2[1] = Payoffs[1][m][n];

                    getPoint(p1, p2, pt, 0.0);
                    x = pt[0] - mv[0];
                    y = pt[1] - mv[1];
                    if ((x >= 0) && (y >= 0)) {
                        prod = x * y;
                        if (prod > maxprod) {
                            maxprod = prod;
                            thePt[0] = pt[0];
                            thePt[1] = pt[1];
                            alt = false;
                        }
                    }
                    
                    if ((i != m) || (j != n)) {
                        getPoint(p1, p2, pt, 0.5);
                        x = pt[0] - mv[0];
                        y = pt[1] - mv[1];
                        if ((x >= 0) && (y >= 0)) {
                            prod = x * y;
                            if (prod > maxprod) {
                                maxprod = prod;
                                thePt[0] = pt[0];
                                thePt[1] = pt[1];
                                alt = true;
                            }
                        }
                        
                        getPoint(p1, p2, pt, 1.0);
                        x = pt[0] - mv[0];
                        y = pt[1] - mv[1];
                        if ((x >= 0) && (y >= 0)) {
                            prod = x * y;
                            if (prod > maxprod) {
                                maxprod = prod;
                                thePt[0] = pt[0];
                                thePt[1] = pt[1];
                                alt = false;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!alt)
        printf("pure NBS: %.2lf, %.2lf\n", thePt[0], thePt[1]);
    else
        printf("Altr NBS: %.2lf, %.2lf\n", thePt[0], thePt[1]);
}

void countGames2() {
    int index, g;
    
    for (g = 0; g < 1; g++) {
        for (index = 0; index < 78; index++) {
            printf("\n\nGame %i\n", index+1);
            printGame(g, index+1);
            computeTheNEs(g, index+1);
            computeTheNBS(g, index+1);
        }
    }

}

void countGames() {
    int index, g;
    //int countNEConfusion = 0;
    //int countCommon = 0;
    //int countConstant = 0;
    //int puroNBS = 0;
    int countSimple = 0;
    int countMultiple = 0;
    int countPONE = 0;
    
    for (g = 0; g < 1; g++) {
        for (index = 0; index < 78; index++) {
            printGame(g, index+1);
            switch (NEConfusion(g, index+1)) {
                case 0: countSimple++; printf("Simple\n"); break;
                case 1: countMultiple++; printf("Multiple\n"); break;
                case 2: countPONE++; printf("No PONE\n"); break;
            
            }
            printf("\n");
/*            printGame(g, index+1);
            if (NEConfusion(g, index+1)) {
                countNEConfusion ++;
                printf("NEConfusion\n");
            }
            if (common) {
                countCommon ++;
                printf("Common Interest\n");
            }
            if (zerosum) {
                countConstant ++;
                printf("Constant Sum\n");
            }
            
            if (pureNBS(g, index+1)) {
                puroNBS ++;
                printf("Simple NBS\n");
            }*/
        }
    }

    printf("\n\nTotal Counts:\n");
    printf("\tSimple = %i\n", countSimple);
    printf("\tMultiple NE = %i\n", countMultiple);
    printf("\tNo PONE = %i\n", countPONE);
    //printf("countInteresting = %i\n", countNEConfusion);
    //printf("countCommon = %i\n", countCommon);
    //printf("countConstant = %i\n", countConstant);
    //printf("pureNBS = %i\n", puroNBS);
}

void createFilteredMasterCSV() {
    int g, index, a1, a2;
    double all1, all2, f100_1, f100_2, f1000_1, f1000_2, flast_1, flast_2;
    char gameName[1024], nombre[1024], dummy[1024];
    FILE *fp;
    FILE *fpw = fopen("Filtered_Master_Sim2_All.csv", "w");
    
    fprintf(fpw, "Game,Partner,Agent,R_all,R_100,R_1000,Rlast\n");
    
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            //if (!uninteresting(g, index)) {
            //    printf("%i_%i\n", g, index); fflush(stdout);
            //    cuenta ++;
            //    continue;
            //}

            sprintf(gameName, "%s_%i", games[g], index);
            for (a1 = 0; a1 < numAgents; a1++) {
                if (!strcmp(agents[a1], "eeew++"))
                    continue;
                
                for (a2 = 0; a2 < numAgents; a2++) {
                    if (!strcmp(agents[a2], "eeew++"))
                        continue;
                    
                    if (amongSelected(a1) || amongSelected(a2)) {
                        sprintf(nombre, "../results21/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                        fp = fopen(nombre, "r");
                        
                        
                        fscanf(fp, "%s %lf %lf", dummy, &all1, &all2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &f100_1, &f100_2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &f1000_1, &f1000_2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &flast_1, &flast_2);
                        
                        fclose(fp);
                        
                        
                        if (amongSelected(a1)) {
                            if ((a2 == 0) || (a2 == 2))
                                fprintf(fpw, "%s,%s,%s,%lf,%lf,%lf,%lf\n", gameName, agents[a1], agents[a2], all2, f100_2, f1000_2, flast_2);
                            else
                                fprintf(fpw, "%s,%s,Other,%lf,%lf,%lf,%lf\n", gameName, agents[a1], all2, f100_2, f1000_2, flast_2);
                        }
                        
                        if (amongSelected(a2)) {
                            if ((a1 == 0) || (a1 == 2))
                                fprintf(fpw, "%s,%s,%s,%lf,%lf,%lf,%lf\n", gameName, agents[a2], agents[a1], all1, f100_1, f1000_1, flast_1);
                            else
                                fprintf(fpw, "%s,%s,Other,%lf,%lf,%lf,%lf\n", gameName, agents[a2], all1, f100_1, f1000_1, flast_1);
                        }
                    }
                    
                    if (a1 != a2) {
                        if (amongSelected(a1) || amongSelected(a2)) {
                            sprintf(nombre, "../results21/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                            fp = fopen(nombre, "r");
                            
                            fscanf(fp, "%s %lf %lf", dummy, &all1, &all2);
                            
                            fscanf(fp, "%s %lf %lf", dummy, &f100_1, &f100_2);
                            
                            fscanf(fp, "%s %lf %lf", dummy, &f1000_1, &f1000_2);
                            
                            fscanf(fp, "%s %lf %lf", dummy, &flast_1, &flast_2);
                            
                            fclose(fp);
                            
                            if (amongSelected(a2)) {
                                if ((a1 == 0) || (a1 == 2))
                                    fprintf(fpw, "%s,%s,%s,%lf,%lf,%lf,%lf\n", gameName, agents[a2], agents[a1], all2, f100_2, f1000_2, flast_2);
                                else
                                    fprintf(fpw, "%s,%s,Other,%lf,%lf,%lf,%lf\n", gameName, agents[a2], all2, f100_2, f1000_2, flast_2);
                            }
                            
                            if (amongSelected(a1)) {
                                if ((a2 == 0) || (a2 == 2))
                                    fprintf(fpw, "%s,%s,%s,%lf,%lf,%lf,%lf\n", gameName, agents[a1], agents[a2], all1, f100_1, f1000_1, flast_1);
                                else
                                    fprintf(fpw, "%s,%s,Other,%lf,%lf,%lf,%lf\n", gameName, agents[a1], all1, f100_1, f1000_1, flast_1);
                            }
                        }
                    }
                }
            }
        }
    }
    
    fclose(fpw);
}

void createMasterCSV() {
    int g, index, a1, a2;
    double v1, v2;
    char gameName[1024], nombre[1024], dummy[1024];
    FILE *fp;
    FILE *fpw = fopen("Master.csv", "w");
    
    fprintf(fpw, "Game,Agent1,Agent2,All_1,All_2,First100_1,First100_2,First1000_1,First1000_2,last1000_1,last1000_2\n");
    
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            sprintf(gameName, "%s_%i", games[g], index);
            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = 0; a2 < numAgents; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    //printf("%s\n", nombre); fflush(stdout);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    fprintf(fpw, "%s,%s,%s,%lf,%lf", gameName, agents[a1], agents[a2], v1, v2);
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    fprintf(fpw, ",%lf,%lf", v1, v2);

                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    fprintf(fpw, ",%lf,%lf", v1, v2);

                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    fprintf(fpw, ",%lf,%lf\n", v1, v2);

                    fclose(fp);
                    
                    if (a1 != a2) {
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        fprintf(fpw, "%s,%s,%s,%lf,%lf", gameName, agents[a2], agents[a1], v1, v2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        fprintf(fpw, ",%lf,%lf", v1, v2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        fprintf(fpw, ",%lf,%lf", v1, v2);
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        fprintf(fpw, ",%lf,%lf\n", v1, v2);
                        
                        fclose(fp);
                    }
                }
            }
        }
    }
    
    fclose(fpw);
}

void analyzeSsimp() {
    int counts[4][26];
    double sums[4][26], v1, v2;
    char nombre[1024], dummy[1024];
    int a1, g, i;
    
    for (a1 = 0; a1 < numAgents; a1++) {
        for (i = 0; i < 4; i++) {
            counts[i][a1] = 0;
            sums[i][a1] = 0.0;
        }
    }
    
    FILE *fp;
    
    for (g = 0; g < 5; g++) {
        printf("count: %i, %i, %i\n", counts[0][0], counts[1][0], counts[2][0]);
    
        for (i = 1; i < 79; i++) {
            for (a1 = 0; a1 < numAgents; a1++) {
                sprintf(nombre, "../results/%s_%i_S_simp_%s.txt", games[g], i, agents[a1]);
                //printf("%s\n", nombre);
                fp = fopen(nombre, "r");
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[0][a1] += v1;
                counts[0][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[1][a1] += v1;
                counts[1][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[2][a1] += v1;
                counts[2][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[3][a1] += v1;
                counts[3][a1] ++;
                
                fclose(fp);
                
                sprintf(nombre, "../results/%s_%i_%s_S_simp.txt", games[g], i, agents[a1]);
                fp = fopen(nombre, "r");
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[0][a1] += v2;
                counts[0][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[1][a1] += v2;
                counts[1][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[2][a1] += v2;
                counts[2][a1] ++;
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                sums[3][a1] += v2;
                counts[3][a1] ++;
                fclose(fp);
            }
        }
    }
    
    printf("count: %i, %i, %i\n", counts[0][0], counts[1][0], counts[2][0]);
    
    FILE *fper = fopen("S_simp.csv", "w");
    //fper = fopen("summary_100_ci.csv", "w");
    
    fprintf(fp, "Agent,Opponent,GameLength,Value\n");
    
    for (i = 0; i < numAgents-1; i++)
        fprintf(fper, "S_simp,%s,100,%lf\n", agents[i], sums[1][i] / counts[1][i]);
    for (i = 0; i < numAgents-1; i++)
        fprintf(fper, "S_simp,%s,1000,%lf\n", agents[i], sums[2][i] / counts[2][i]);
    for (i = 0; i < numAgents-1; i++)
        fprintf(fper, "S_simp,%s,50000,%lf\n", agents[i], sums[0][i] / counts[0][i]);

    fclose(fper);
    


}

void analyzeGames() {
    int g, index, a1, a2, i, j;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double v1, v2;
    
    double sums[4][30][30];
    int counts[4][30][30];
    
    for (a1 = 0; a1 < numAgents; a1++) {
        for (a2 = 0; a2 < numAgents; a2++) {
            counts[0][a1][a2] = counts[1][a1][a2] = counts[2][a1][a2] = counts[3][a1][a2] = 0;
            sums[0][a1][a2] = sums[1][a1][a2] = sums[2][a1][a2] = sums[3][a1][a2] = 0.0;
        }
    }
    
    int cuenta = 0;
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            //printf("index = %i\n", index); fflush(stdout);
            //if (!uninteresting(g, index)) {
            if (NEConfusion(g, index) != 2) {  // 0: Simple Games; 1: MultipleNE; 2: No pure PO NE
            //    printf("%i_%i\n", g, index); fflush(stdout);
                cuenta ++;
                continue;
            }
        
            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = 0; a2 < numAgents; a2++) {
                    //printf("(%s_%i: %s, %s) ... ", games[g], index, agents[a1], agents[a2]); fflush(stdout);
                
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    //printf("%s\n", nombre); fflush(stdout);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    
                    //printf("%lf, %lf\n", v1, v2); fflush(stdout);
                    
                    sums[0][a1][a2] += v1;
                    counts[0][a1][a2] ++;
                    sums[0][a2][a1] += v2;
                    counts[0][a2][a1] ++;
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[1][a1][a2] += v1;
                    counts[1][a1][a2] ++;
                    sums[1][a2][a1] += v2;
                    counts[1][a2][a1] ++;

                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[2][a1][a2] += v1;
                    counts[2][a1][a2] ++;
                    sums[2][a2][a1] += v2;
                    counts[2][a2][a1] ++;

                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[3][a1][a2] += v1;
                    counts[3][a1][a2] ++;
                    sums[3][a2][a1] += v2;
                    counts[3][a2][a1] ++;

                    fclose(fp);
/*
                    if (a1 != a2) {
                        printf("(%s, %s) ... ", agents[a2], agents[a1]); fflush(stdout);
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[0][a2][a1] += v1;
                        counts[0][a2][a1] ++;
                        sums[0][a1][a2] += v2;
                        counts[0][a1][a2] ++;
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[1][a2][a1] += v1;
                        counts[1][a2][a1] ++;
                        sums[1][a1][a2] += v2;
                        counts[1][a1][a2] ++;
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[2][a2][a1] += v1;
                        counts[2][a2][a1] ++;
                        sums[2][a1][a2] += v2;
                        counts[2][a1][a2] ++;
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[3][a2][a1] += v1;
                        counts[3][a2][a1] ++;
                        sums[3][a1][a2] += v2;
                        counts[3][a1][a2] ++;
                        
                        fclose(fp);
                    }
*/
                    //printf("\n"); fflush(stdout);
                }
            }
        }
    }
    
    printf("cuenta = %i\n", 390 - cuenta);
    
    FILE *fper;
/*
    fper = fopen("summary_all.csv", "w");
    //FILE *fper = fopen("summary_all_ci.csv", "w");

    for (i = 0; i < numAgents; i++)
        fprintf(fper, ",%s", agents[i]);
    fprintf(fper, "\n");
    
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s,", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf,", sums[0][i][j] / counts[0][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);
    
    fper = fopen("summary_100.csv", "w");
    //fper = fopen("summary_100_ci.csv", "w");
    
    for (i = 0; i < numAgents; i++)
        fprintf(fper, ",%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s,", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf,", sums[1][i][j] / counts[1][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_1000.csv", "w");
    //fper = fopen("summary_1000_ci.csv", "w");

    for (i = 0; i < numAgents; i++)
        fprintf(fper, ",%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s,", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf,", sums[2][i][j] / counts[2][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_last1000.csv", "w");
    //fper = fopen("summary_last1000_ci.csv", "w");

    //fprintf(fper, "\n\nLast 1000: \n");
    for (i = 0; i < numAgents; i++)
        fprintf(fper, ",%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s,", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf,", sums[3][i][j] / counts[3][i][j]);
        }
        fprintf(fper, "\n");
    }
    
    fclose(fper);
*/
    fper = fopen("summary_50000.txt", "w");
    //fper = fopen("summary_50000_ci.txt", "w");
    
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[0][i][j] / counts[0][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);
    
    fper = fopen("summary_100.txt", "w");
    //fper = fopen("summary_100_ci.txt", "w");
    
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[1][i][j] / counts[1][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_1000.txt", "w");
    //fper = fopen("summary_1000_ci.txt", "w");

    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[2][i][j] / counts[2][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_last1000.txt", "w");
    //fper = fopen("summary_last1000_ci.txt", "w");

    //fprintf(fper, "\n\nLast 1000: \n");
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[3][i][j] / counts[3][i][j]);
        }
        fprintf(fper, "\n");
    }
    
    fclose(fper);
    
}

void analyzeGamesNew() {
    int g, index, a1, a2, i, j;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double v1, v2;
    
    double sums[4][30][30];
    int counts[4][30][30];
    
    for (a1 = 0; a1 < numAgents; a1++) {
        for (a2 = 0; a2 < numAgents; a2++) {
            counts[0][a1][a2] = counts[1][a1][a2] = counts[2][a1][a2] = counts[3][a1][a2] = 0;
            sums[0][a1][a2] = sums[1][a1][a2] = sums[2][a1][a2] = sums[3][a1][a2] = 0.0;
        }
    }
    
    int cuenta = 0;
    bool symmetric = false;
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            sprintf(nombre, "../gameClass/%i.txt", index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%s", dummy);
            if (!strcmp(dummy, "Game")) {
                symmetric = true;
                cuenta ++;
            }
            else {
                symmetric = false;
                cuenta += 2;
            }
            
            fclose(fp);

            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = a1; a2 < numAgents; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    fp = fopen(nombre, "r");
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    
                    sums[0][a1][a2] += v1;
                    counts[0][a1][a2] ++;
                    if (a1 != a2) {
                        sums[0][a2][a1] += v2;
                        counts[0][a2][a1] ++;
                    }
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[1][a1][a2] += v1;
                    counts[1][a1][a2] ++;
                    if (a1 != a2) {
                        sums[1][a2][a1] += v2;
                        counts[1][a2][a1] ++;
                    }
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[2][a1][a2] += v1;
                    counts[2][a1][a2] ++;
                    if (a1 != a2) {
                        sums[2][a2][a1] += v2;
                        counts[2][a2][a1] ++;
                    }
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[3][a1][a2] += v1;
                    counts[3][a1][a2] ++;
                    if (a1 != a2) {
                        sums[3][a2][a1] += v2;
                        counts[3][a2][a1] ++;
                    }
                    fclose(fp);

                    if (!symmetric) {
                        //printf("(%s, %s) ... ", agents[a2], agents[a1]); fflush(stdout);
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[0][a2][a1] += v1;
                        counts[0][a2][a1] ++;
                        if (a1 != a2) {
                            sums[0][a1][a2] += v2;
                            counts[0][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[1][a2][a1] += v1;
                        counts[1][a2][a1] ++;
                        if (a1 != a2) {
                            sums[1][a1][a2] += v2;
                            counts[1][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[2][a2][a1] += v1;
                        counts[2][a2][a1] ++;
                        if (a1 != a2) {
                            sums[2][a1][a2] += v2;
                            counts[2][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[3][a2][a1] += v1;
                        counts[3][a2][a1] ++;
                        if (a1 != a2) {
                            sums[3][a1][a2] += v2;
                            counts[3][a1][a2] ++;
                        }
                        fclose(fp);
                    }
                    
                }
            }
        }
    }

    //printf("counts = %i, %i, %i\n", counts[0][0][0], counts[0][0][1], counts[0][1][0]);
    //printf("cuenta = %i\n", cuenta);

    //printf("cuenta = %i\n", 390 - cuenta);
    
    FILE *fper;

    fper = fopen("summary_50000.txt", "w");
    //fper = fopen("summary_50000_ci.txt", "w");
    
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[0][i][j] / counts[0][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);
    
    fper = fopen("summary_100.txt", "w");
    //fper = fopen("summary_100_ci.txt", "w");
    
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[1][i][j] / counts[1][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_1000.txt", "w");
    //fper = fopen("summary_1000_ci.txt", "w");

    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[2][i][j] / counts[2][i][j]);
        }
        fprintf(fper, "\n");
    }

    fclose(fper);

    fper = fopen("summary_last1000.txt", "w");
    //fper = fopen("summary_last1000_ci.txt", "w");

    //fprintf(fper, "\n\nLast 1000: \n");
    for (i = 0; i < numAgents; i++)
        fprintf(fper, "\t%s", agents[i]);
    fprintf(fper, "\n");
    for (i = 0; i < numAgents; i++) {
        fprintf(fper, "%s\t", agents[i]);
        for (j = 0; j < numAgents; j++) {
            fprintf(fper, "%lf\t", sums[3][i][j] / counts[3][i][j]);
        }
        fprintf(fper, "\n");
    }
    
    fclose(fper);

}


void getLowHighNE(int g, int index, double lowhigh[2][2]) {
    double Payoffs[2][2][2];
    commonInterestGame(g, index);

    char nombre[1024];
    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
    FILE *fp = fopen(nombre, "r");

    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);
    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(Payoffs[0][i][j]), &(Payoffs[1][i][j]));
        }
    }

    fclose(fp);
    
    lowhigh[0][0] = lowhigh[1][0] = 99999;
    lowhigh[0][1] = lowhigh[1][1] = -99999;
    
    
    bool NE[2][2];
    double point[2];
    for (i = 0; i < d2; i++) {
        for (j = 0; j < d3; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                if (Payoffs[0][i][j] > lowhigh[0][1])
                    lowhigh[0][1] = Payoffs[0][i][j];
                if (Payoffs[1][i][j] > lowhigh[1][1])
                    lowhigh[1][1] = Payoffs[1][i][j];

                if (Payoffs[0][i][j] < lowhigh[0][0])
                    lowhigh[0][0] = Payoffs[0][i][j];
                if (Payoffs[1][i][j] < lowhigh[1][0])
                    lowhigh[1][0] = Payoffs[1][i][j];
            }
        }
    }
    //printf("\n");

}

void getGameCharacteristics() {
    int g, index;
    char nombre[1024];
    FILE *fp;
    
    double mv1, mv2, d1, d2;
    double summm = 0.0;
    int countmm = 0;
    double sumnbs = 0.0;
    double countnbs = 0;
    double sumbully = 0.0, sumbullied = 0.0;
    double countbully = 0;
    double nbs1, nbs2, bully1, bully2, bullied1, bullied2;
    double lowhigh[2][2] = {{0,0},{0,0}};
    
    double sumlowne = 0.0, sumhighne = 0.0;
    int countne = 0;
    
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            if (!uninteresting(g, index))
                continue;

            sprintf(nombre, "../results/maximinVals/%s_%i", games[g], index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%lf %lf %lf", &mv1, &d1, &d2);
            fscanf(fp, "%lf", &mv2);
            
            summm += mv1 + mv2;
            countmm += 2;
            
            fclose(fp);

            sprintf(nombre, "../results/NBS/%s_%i", games[g], index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%lf %lf", &nbs1, &nbs2);
            
            sumnbs += nbs1 + nbs2;
            countnbs += 2;
            
            fclose(fp);

            sprintf(nombre, "../results/NBS/bully_%s_%i", games[g], index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%lf %lf", &bully1, &bully2);
            fscanf(fp, "%lf %lf", &bullied1, &bullied2);
            
            sumbully += bully1 + bully2;
            sumbullied += bullied1 + bullied2;
            countbully += 2;
            
            fclose(fp);
            
            //printf("%i, %i: %lf vs %lf\n", g, index, (nbs1 + nbs2)/2.0, (bully1 + bully2)/2.0);
            
            getLowHighNE(g, index, lowhigh);

            // set equal to maxmin if there isn't a pure NE
            if (lowhigh[0][1] < 0) {
                lowhigh[0][0] = lowhigh[0][1] = mv1;
                lowhigh[1][0] = lowhigh[1][1] = mv2;
            }
            
            //printf("low %i: %lf, %lf\n", index, lowhigh[0][0], lowhigh[1][0]);
            //printf("high %i: %lf, %lf\n", index, lowhigh[0][1], lowhigh[1][1]);
            
            sumlowne += lowhigh[0][0] + lowhigh[1][0];
            sumhighne += lowhigh[0][1] + lowhigh[1][1];
            countne += 2;
        }
    }

    printf("average maxmin: %lf\n", summm / countmm);
    printf("average NBS: %lf\n", sumnbs / countnbs);
    printf("average Bully value: %lf\n", sumbully / countbully);
    printf("average Bullied value: %lf\n", sumbullied / countbully);
    printf("average low NE value: %lf\n", sumlowne / countne);
    printf("average high NE value: %lf\n", sumhighne / countne);
}

void testGodfather() {
    int i;
    int g = 0;
    double nbs1, nbs2;
    
    char nombre[1024];
    FILE *fp;
    
    char dummy[1024];
    double score1, score2;
    double gotS = 0.0, shouldaS = 0.0;
    double gotG = 0.0, shouldaG = 0.0;
    
    for (g = 0; g < 5; g++) {
        for (i = 1; i < 79; i++) {
            sprintf(nombre, "../results/NBS/%s_%i", games[g], i);
            fp = fopen(nombre, "r");
            fscanf(fp, "%lf %lf", &nbs1, &nbs2);
            fclose(fp);
            
            sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], i, agents[10], agents[10]);
            fp = fopen(nombre, "r");
            fscanf(fp, "%s %lf %lf", dummy, &score1, &score2);
            fclose(fp);
            
            if ((fabs(score1 - nbs1) > 0.02) || (fabs(score2 - nbs2) > 0.02))
                printf("check game %i: (%lf, %lf) vs (%lf, %lf)\n", i, score1, score2, nbs1, nbs2);
            
            gotS += score1;
            shouldaS += nbs1;
            gotG += score2;
            shouldaG += nbs2;

            sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], i, agents[10], agents[10]);
            fp = fopen(nombre, "r");
            fscanf(fp, "%s %lf %lf", dummy, &score1, &score2);
            fclose(fp);
            
            if ((fabs(score1 - nbs1) > 0.02) || (fabs(score2 - nbs2) > 0.02))
                printf("check game %i: (%lf, %lf) vs (%lf, %lf)\n", i, score1, score2, nbs1, nbs2);
            
            gotS += score2;
            shouldaS += nbs2;
            gotG += score1;
            shouldaG += nbs1;
        }
    }
    
    printf("S++: %lf instead of %lf\n", gotS / (10*78), shouldaS / (10*78));
    printf("Godfather: %lf instead of %lf\n", gotG / (10*78), shouldaG / (10*78));
}


void evolve(double outcomes[25][25], int iters, double proportion[10001][25]) {

    int i, j, k;
    double fitness[25], avefit;
    for (i = 0; i < iters; i++) {
        avefit = 0.0;
        for (j = 0; j < 25; j++) {
            fitness[j] = 0.0;
            for (k = 0; k < 25; k++) {
                fitness[j] += proportion[i][k] * outcomes[j][k];
            }
            avefit += fitness[j] * proportion[i][j];
        }
        
        double sum = 0.0;
        for (j = 0; j < 25; j++) {
            proportion[i+1][j] = proportion[i][j] + (proportion[i][j] * (fitness[j] - avefit));
            sum += proportion[i+1][j];
        }
    }
}

void evolutionaryRank(int whichGames, int length) {
    double outcomes[25][25], proportion[10001][25];

    // read outcomes
    char fnombre[1024], dummy[1024];
    double ddummy;
    switch (whichGames) {
        case 0: sprintf(fnombre, "saved/Simple%i.txt", length); break;
        case 1: sprintf(fnombre, "saved/Multiple%i.txt", length); break;
        case 2: sprintf(fnombre, "saved/NoPONE%i.txt", length); break;
        default: sprintf(fnombre, "saved144/finalRaw%i.txt", length); break;
    }
    
    //if (allgames)
    //    sprintf(fnombre, "saved/finalRaw%i.txt", length);
    //else
    //    sprintf(fnombre, "summary_%i_ci.txt", length);
    FILE *fp = fopen(fnombre, "r");
    
    int i, j;
    for (i = 0; i < 26; i++)
        fscanf(fp, "%s", dummy);

    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        for (j = 0; j < 25; j++) {
            fscanf(fp, "%lf", &(outcomes[i][j]));
        }
        fscanf(fp, "%lf", &ddummy);
    }
    
    fclose(fp);
    
    int haveWon[25];
    int count, ind, pass;
    double val;
    double usage[i];
    for (i = 0; i < 25; i++)
        haveWon[i] = -1;
    
    int generaciones = 10000;
    
    for (pass = 0; pass < 24; pass++) {
        
        count = 0;
        for (i = 0; i < 25; i++) {
            if (haveWon[i] < 0) {
                proportion[0][i] = 1.0;
                count ++;
            }
            else
                proportion[0][i] = 0.0;
        }
        for (i = 0; i < 25; i++)
            proportion[0][i] /= count;
        
        evolve(outcomes, generaciones, proportion);
        
        ind = 0;
        for (i = 0; i < 25; i++) {
            val = 0.0;
            for (j = 0; j < generaciones+1; j++) {
                val += proportion[j][i];
            }
            usage[i] = val / (generaciones+1);
            printf("%.5s:\t%lf\n", agents[i], usage[i]);
            
            if (usage[i] > usage[ind])
                ind = i;
        }
        
        printf("%s won pass %i\n\n", agents[ind], pass);
        haveWon[ind] = pass;
    }
    
    printf("\nRanks %i:\n", length);
    for (i = 0; i < 25; i++) {
        //printf("%s: %i\n", agents[i], haveWon[i]+1);
        printf("%i\n", haveWon[i]+1);
        //printf("%s\n", agents[i]);
    }

}

void evolutionaryTournament(int whichGames, int length) {
    double outcomes[25][25], proportion[1001][25];

    printf("whichGames = %i\n", whichGames);

    // read outcomes
    char fnombre[1024], dummy[1024];
    //double ddummy;
    switch (whichGames) {
        case 0: sprintf(fnombre, "saved/Simple%i.txt", length); break;
        case 1: sprintf(fnombre, "saved/Multiple%i.txt", length); break;
        case 2: sprintf(fnombre, "saved/NoPONE%i.txt", length); break;
        default: sprintf(fnombre, "saved/finalRaw%i.txt", length); break;
    }
    
    printf("%s\n", fnombre);
    //if (allgames)
    //    sprintf(fnombre, "saved/finalRaw%i.txt", length);
    //else
    //    sprintf(fnombre, "summary_%i_ci.txt", length);
    FILE *fp = fopen(fnombre, "r");
    
    int i, j;
    for (i = 0; i < 25; i++)
        fscanf(fp, "%s", dummy);

    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        //printf("%s: ", dummy);
        for (j = 0; j < 25; j++) {
            fscanf(fp, "%lf", &(outcomes[i][j]));
            //printf("%.2lf ", outcomes[i][j]);
        }
        //printf("\n");
        //fscanf(fp, "%lf", &ddummy);
    }
    
    fclose(fp);


    // get initial population; assign evenly for now
    for (i = 0; i < 25; i++)
        proportion[0][i] = 1.0 / 25.0;
    //proportion[0][1] = 0.0000;

    // evolve
    evolve(outcomes, 1000, proportion);
    
    double val;
    for (i = 0; i < 25; i++) {
        val = 0.0;
        for (j = 0; j < 1001; j++) {
            val += proportion[j][i];
        }
        printf("%.5s:\t%lf\n", agents[i], val / 1001);
    }

    switch (whichGames) {
        case 0: sprintf(fnombre, "saved/evolve_Simple%i.txt", length); break;
        case 1: sprintf(fnombre, "saved/evolve_Multiple%i.txt", length); break;
        case 2: sprintf(fnombre, "saved/evolve_NoPONE%i.txt", length); break;
        default: sprintf(fnombre, "saved/evolve_%i.txt", length); break;
    }
    //if (allgames)
    //    sprintf(fnombre, "saved/evolve_%i.csv", length);
    //else
    //    sprintf(fnombre, "saved/evolve_%i_ci.csv", length);
    
    fp = fopen(fnombre, "w");
    
    fprintf(fp, "Agent,Generation,Proportion\n");
    for (j = 0; j < 1001; j++) {
        for (i = 0; i < 25; i++) {
            fprintf(fp, "%s,%i,%lf\n", agents[i], j, proportion[j][i]);
        }
    }
    
    fclose(fp);
}

void createDeepQProportions() {
    FILE *fpw = fopen("DeepQ_Proportions_SD.csv", "w");
    fprintf(fpw, "Game,Round,Entry,Value\n");

    int i, k, step=50, rnd, a0, a1;
    double p0, p1, sum0, sum1;
    int coop, exploit1, exploit2;
    FILE *fpr;
    char nme[1024];

    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/prisoners_S++_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.59) && (p0 < 0.61))
                    coop ++;
                else if (p0 > 0.61)
                    exploit1 ++;
                else if (p1 > 0.61)
                    exploit2 ++;
            }
            fprintf(fpw, "prisoners,%i,Mutual,%lf\n", k+step/2, (double)coop / step);
            fprintf(fpw, "prisoners,%i,ExploitationSpp,%lf\n", k+step/2, (double)exploit1 / step);
            fprintf(fpw, "prisoners,%i,ExploitationDeepQ,%lf\n", k+step/2, (double)exploit2 / step);
        }
        
        fclose(fpr);
    }

    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/chicken2_S++_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.80) && (p0 < 0.85))
                    coop ++;
                else if (p0 > 0.85)
                    exploit1 ++;
                else if (p1 > 0.85)
                    exploit2 ++;
            }
            fprintf(fpw, "chicken,%i,Mutual,%lf\n", k+step/2, (double)coop / step);
            fprintf(fpw, "chicken,%i,ExploitationSpp,%lf\n", k+step/2, (double)exploit1 / step);
            fprintf(fpw, "chicken,%i,ExploitationDeepQ,%lf\n", k+step/2, (double)exploit2 / step);
        }
        
        fclose(fpr);
    }


    fclose(fpw);
}

void createDeepQMaster() {
    FILE *fpw = fopen("DeepQ_Master.csv", "w");
    
    fprintf(fpw, "Game,Combo,Name1,Name2,Round,Payoff1,Payoff2,Ave,MCoop,Exploit1,Exploit2\n");
    
    int i, k, step=50, rnd, a0, a1;
    double p0, p1, sum0, sum1;
    int coop, exploit1, exploit2;
    FILE *fpr;
    char nme[1024];
    
    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/prisoners_DeepQ_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.59) && (p0 < 0.61))
                    coop ++;
                else if (p0 > 0.61)
                    exploit1 ++;
                else if (p1 > 0.61)
                    exploit2 ++;
            }
            fprintf(fpw, "prisoners,DD,DeepQ,DeepQ,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum0 / step, sum1 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit1 / step, (double)exploit2 / step);
        }
        
        fclose(fpr);
    }

    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/prisoners_S++_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.59) && (p0 < 0.61))
                    coop ++;
                else if (p0 > 0.61)
                    exploit1 ++;
                else if (p1 > 0.61)
                    exploit2 ++;
            }
            fprintf(fpw, "prisoners,SD,S++,DeepQ,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum0 / step, sum1 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit1 / step, (double)exploit2 / step);
            fprintf(fpw, "prisoners,DS,DeepQ,S++,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum1 / step, sum0 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit2 / step, (double)exploit1 / step);
        }
        
        fclose(fpr);
    }

    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/chicken2_DeepQ_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.80) && (p0 < 0.85))
                    coop ++;
                else if (p0 > 0.85)
                    exploit1 ++;
                else if (p1 > 0.85)
                    exploit2 ++;
            }
            fprintf(fpw, "chicken,DD,DeepQ,DeepQ,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum0 / step, sum1 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit1 / step, (double)exploit2 / step);
        }
        
        fclose(fpr);
    }

    for (i = 0; i < 50; i++) {
        sprintf(nme, "../log/chicken2_S++_DeepQ_%i.txt", i);
        fpr = fopen(nme, "r");
        
        for (k = 0; k < 1000; k+=step) {
            sum0 = sum1 = 0.0;
            coop = exploit1 = exploit2 = 0;
            for (rnd = k; rnd < k+step; rnd++) {
                fscanf(fpr, "%i %i %lf %lf", &a0, &a1, &p0, &p1);
                sum0 += p0;
                sum1 += p1;
                if ((p0 > 0.80) && (p0 < 0.85))
                    coop ++;
                else if (p0 > 0.85)
                    exploit1 ++;
                else if (p1 > 0.85)
                    exploit2 ++;
            }
            fprintf(fpw, "chicken,SD,S++,DeepQ,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum0 / step, sum1 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit1 / step, (double)exploit2 / step);
            fprintf(fpw, "chicken,DS,DeepQ,S++,%i,%lf,%lf,%lf,%lf,%lf,%lf\n", k+step/2, sum1 / step, sum0 / step,
                        (sum0 + sum1) / (2*step), (double)coop / step, (double)exploit2 / step, (double)exploit1 / step);
        }
        
        fclose(fpr);
    }
    
    fclose(fpw);
}

char *round2Str(int rnd) {
    char *strng = new char[1024];
    
    switch (rnd) {
        case 0: strcpy(strng, "1-5"); break;
        case 1: strcpy(strng, "6-15"); break;
        case 2: strcpy(strng, "16-30"); break;
        case 3: strcpy(strng, "31-60"); break;
        case 4: strcpy(strng, "61-125"); break;
        case 5: strcpy(strng, "126-250"); break;
        case 6: strcpy(strng, "251-500"); break;
        case 7: strcpy(strng, "501-999"); break;
    }

    return strng;
}

char * exprt2Str(int exprt) {
    char *strng = new char[1024];
    
    switch (exprt) {
        case 0: strcpy(strng, "Maxmin"); break;
        case 1: strcpy(strng, "MBRL-1"); break;
        case 2: strcpy(strng, "Bully-L"); break;
        case 3: strcpy(strng, "Fair-L"); break;
        case 4: strcpy(strng, "Bullied-L"); break;
        case 5: strcpy(strng, "Other-L"); break;
        case 6: strcpy(strng, "Bully-F"); break;
        case 7: strcpy(strng, "Fair-F"); break;
        case 8: strcpy(strng, "Bullied-F"); break;
        case 9: strcpy(strng, "Other-F"); break;
    }

    return strng;
}

int getRoundBin(int rnd) {
    // 1-5
    // 6-15
    // 16-30
    // 31-60
    // 61-125
    // 126-250
    // 251-500
    // 501-1000

    if (rnd <= 5)
        return 0;
    else if (rnd <= 15)
        return 1;
    else if (rnd <= 30)
        return 2;
    else if (rnd <= 60)
        return 3;
    else if (rnd <= 125)
        return 4;
    else if (rnd <= 250)
        return 5;
    else if (rnd <= 500)
        return 6;
    else
        return 7;
}

void createExpertsLog() {
    FILE *wfp = fopen("Experts_Chicken.csv", "w");
    FILE *rfp;
    char fname[1024], agent1[1024], agent2[1024];
    int i, j;
    int rnd, exprt;
    
    fprintf(wfp, "Pairing,Round,Expert\n");
    
    int counter[8][10];
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            counter[i][j] = 0;
        }
    }
    
    for (i = 0; i < 50; i++) {
        strcpy(agent1, "S++");
        strcpy(agent2, "S++");
        
        sprintf(fname, "../experts/log_chicken2_%s_%s_%i.txt", agent1, agent2, i);
        rfp = fopen(fname, "r");
    
        fscanf(rfp, "%i %i", &rnd, &exprt);
        for (j = 1; j < 1000; j++) {
            fscanf(rfp, "%i %i", &rnd, &exprt);
            fprintf(wfp, "%s-%s,%i,%i\n", agent1, agent2, rnd, exprt);
            
            counter[getRoundBin(rnd)][exprt] ++;
        }
    
        fclose(rfp);
    }
    
    FILE *wfp_spp = fopen("Usage_Chicken_Spp.csv", "w");
    //fprintf(wfp_spp, "Round,Maxmin,MBRL-1,Bully Leader,Fair Leader,Bullied Leader,Other Leader,Bully Follower,Fair Follower,Bullied Follower,Other Follower\n");
    fprintf(wfp_spp, "Rounds,Experts,Value\n");
    int sum;
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j < 10; j++) {
            sum += counter[i][j];
        }
        //fprintf(wfp_spp, "%i", i);
        for (j = 0; j < 10; j++) {
            //fprintf(wfp_spp, ",%.3lf", counter[i][j] / (double)sum);
            fprintf(wfp_spp, "%s,%s,%lf\n", round2Str(i), exprt2Str(j), counter[i][j] / (double)sum);
        }
        //fprintf(wfp_spp, "\n");
    }
    fclose(wfp_spp);

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            counter[i][j] = 0;
        }
    }

    for (i = 0; i < 50; i++) {
        strcpy(agent1, "S++");
        strcpy(agent2, "bully");
        
        sprintf(fname, "../experts/log_chicken2_%s_%s_%i.txt", agent1, agent2, i);
        rfp = fopen(fname, "r");
    
        fscanf(rfp, "%i %i", &rnd, &exprt);
        for (j = 1; j < 1000; j++) {
            fscanf(rfp, "%i %i", &rnd, &exprt);
            fprintf(wfp, "%s-%s,%i,%i\n", agent1, agent2, rnd, exprt);
            
            counter[getRoundBin(rnd)][exprt] ++;
        }
    
        fclose(rfp);
    }

    FILE *wfp_bully = fopen("Usage_Chicken_Bully.csv", "w");
    //fprintf(wfp_bully, "Round,Maxmin,MBRL-1,Bully Leader,Fair Leader,Bullied Leader,Other Leader,Bully Follower,Fair Follower,Bullied Follower,Other Follower\n");
    fprintf(wfp_bully, "Rounds,Experts,Value\n");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j < 10; j++) {
            sum += counter[i][j];
        }
        //fprintf(wfp_bully, "%i", i);
        for (j = 0; j < 10; j++) {
            //fprintf(wfp_bully, ",%.3lf", counter[i][j] / (double)sum);
            fprintf(wfp_bully, "%s,%s,%lf\n", round2Str(i), exprt2Str(j), counter[i][j] / (double)sum);
        }
        //fprintf(wfp_bully, "\n");
    }
    fclose(wfp_bully);

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            counter[i][j] = 0;
        }
    }
    
    for (i = 0; i < 50; i++) {
        strcpy(agent1, "S++");
        strcpy(agent2, "br2");
        
        sprintf(fname, "../experts/log_chicken2_%s_%s_%i.txt", agent1, agent2, i);
        rfp = fopen(fname, "r");
    
        fscanf(rfp, "%i %i", &rnd, &exprt);
        for (j = 1; j < 1000; j++) {
            fscanf(rfp, "%i %i", &rnd, &exprt);
            fprintf(wfp, "%s-%s,%i,%i\n", agent1, agent2, rnd, exprt);
            
            counter[getRoundBin(rnd)][exprt] ++;
        }
    
        fclose(rfp);
    }
    
    FILE *wfp_br2 = fopen("Usage_Chicken_mbrl2.csv", "w");
    //fprintf(wfp_br2, "Round,Maxmin,MBRL-1,Bully Leader,Fair Leader,Bullied Leader,Other Leader,Bully Follower,Fair Follower,Bullied Follower,Other Follower\n");
    fprintf(wfp_br2, "Rounds,Experts,Value\n");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j < 10; j++) {
            sum += counter[i][j];
        }
        //fprintf(wfp_br2, "%i", i);
        for (j = 0; j < 10; j++) {
            //fprintf(wfp_br2, ",%.3lf", counter[i][j] / (double)sum);
            fprintf(wfp_br2, "%s,%s,%lf\n", round2Str(i), exprt2Str(j), counter[i][j] / (double)sum);
        }
        //fprintf(wfp_br2, "\n");
    }
    fclose(wfp_br2);
    
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            counter[i][j] = 0;
        }
    }

    for (i = 0; i < 50; i++) {
        strcpy(agent1, "S++");
        strcpy(agent2, "eeew");
        
        sprintf(fname, "../experts/log_chicken2_%s_%s_%i.txt", agent1, agent2, i);
        rfp = fopen(fname, "r");
    
        fscanf(rfp, "%i %i", &rnd, &exprt);
        for (j = 1; j < 1000; j++) {
            fscanf(rfp, "%i %i", &rnd, &exprt);
            fprintf(wfp, "%s-%s,%i,%i\n", agent1, agent2, rnd, exprt);
            
            counter[getRoundBin(rnd)][exprt] ++;
        }
    
        fclose(rfp);
    }
    
    FILE *wfp_eee = fopen("Usage_Chicken_EEE.csv", "w");
    //fprintf(wfp_eee, "Round,Maxmin,MBRL-1,Bully Leader,Fair Leader,Bullied Leader,Other Leader,Bully Follower,Fair Follower,Bullied Follower,Other Follower\n");
    fprintf(wfp_eee, "Rounds,Experts,Value\n");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j < 10; j++) {
            sum += counter[i][j];
        }
        //fprintf(wfp_eee, "%i", i);
        for (j = 0; j < 10; j++) {
            //fprintf(wfp_eee, ",%.3lf", counter[i][j] / (double)sum);
            fprintf(wfp_eee, "%s,%s,%lf\n", round2Str(i), exprt2Str(j), counter[i][j] / (double)sum);
        }
        //fprintf(wfp_eee, "\n");
    }
    fclose(wfp_eee);


    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            counter[i][j] = 0;
        }
    }
    
    for (i = 0; i < 50; i++) {
        strcpy(agent1, "S");
        strcpy(agent2, "br2");
        
        sprintf(fname, "../experts/log_chicken2_%s_%s_%i.txt", agent1, agent2, i);
        rfp = fopen(fname, "r");
    
        fscanf(rfp, "%i %i", &rnd, &exprt);
        for (j = 1; j < 1000; j++) {
            fscanf(rfp, "%i %i", &rnd, &exprt);
            fprintf(wfp, "%s-%s,%i,%i\n", agent1, agent2, rnd, exprt);
            
            counter[getRoundBin(rnd)][exprt] ++;
        }
    
        fclose(rfp);
    }
    
    FILE *wfp_Sbr2 = fopen("Usage_Chicken_Smbrl2.csv", "w");
    //fprintf(wfp_br2, "Round,Maxmin,MBRL-1,Bully Leader,Fair Leader,Bullied Leader,Other Leader,Bully Follower,Fair Follower,Bullied Follower,Other Follower\n");
    fprintf(wfp_Sbr2, "Rounds,Experts,Value\n");
    for (i = 0; i < 8; i++) {
        sum = 0;
        for (j = 0; j < 10; j++) {
            sum += counter[i][j];
        }
        //fprintf(wfp_br2, "%i", i);
        for (j = 0; j < 10; j++) {
            //fprintf(wfp_br2, ",%.3lf", counter[i][j] / (double)sum);
            fprintf(wfp_Sbr2, "%s,%s,%lf\n", round2Str(i), exprt2Str(j), counter[i][j] / (double)sum);
        }
        //fprintf(wfp_br2, "\n");
    }
    fclose(wfp_Sbr2);



    fclose(wfp);
}

void deepQ_PD_results() {
    double vals[500000];
    char fname[1024];
    
    int a1, a2;
    double v1, v2;
    
    int i, j;
    for (i = 0; i < 50000; i++) {
        vals[i] = 0.0;
    }
    
    for (j = 0; j < 50; j++) {
        sprintf(fname, "../../forAbdallah/newResults/prisoners_DeepQ_DeepQ_activity_%i.txt", j);
        FILE *fp = fopen(fname, "r");

        for (i = 0; i < 50000; i++) {
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &v1, &v2);
        
            vals[i] += v1 + v2;
            
            //printf("%lf\n", vals[i]);
        }
        fclose(fp);
    }

    int count = 2;
    for (i = 0; i < 50000; i++) {
        vals[i] /= 100.0;
        
        if (((int)(pow(1.4, count))) == (i+1)) {
            printf("%i,DeepQ,%lf\n", i+1, vals[i]);
            count ++;
        }
    }
    
    printf("50000,DeepQ,%lf\n", vals[49999]);
    
    
}

void bubbleSort(int ranks[25], double values[25]) {
    int i, j;
    //for (i = 0; i < 25; i++) {
    //    printf("%lf; %i\n", values[i], ranks[i]);
    //}
    int tmpi;
    double tmpd;
    
    for (i = 0; i < 24; i++) {
        for (j = 0; j < 24; j++) {
            if (values[j] < values[j+1]) {
                tmpd = values[j];
                values[j] = values[j+1];
                values[j+1] = tmpd;
                tmpd = ranks[j];
                ranks[j] = ranks[j+1];
                ranks[j+1] = tmpd;
            }
        }
    }
    //printf("\n");
}

bool wrong(int theOrder[25], int times[25][25], int ind1, int ind2) {
    int i;
    
    for (i = 0; i < 25; i++) {
        if (times[theOrder[ind1]][i] > times[theOrder[ind2]][i])
            return false;
        else if (times[theOrder[ind1]][i] < times[theOrder[ind2]][i])
            return true;
    }
    
    return false;
}

void orderSort(int theOrder[25], int times[25][25]) {
    int i, j;
    
    int tmp;
    
    for (i = 0; i < 24; i++) {
        for (j = 0; j < 24; j++) {
            if (wrong(theOrder, times, j, j+1)) {
                tmp = theOrder[j];
                theOrder[j] = theOrder[j+1];
                theOrder[j+1] = tmp;
            }
        }
    }

    //for (i = 0; i < 25; i++)
    //    printf("%i ", theOrder[i]);
}



void getOutcomes(double outcomes[25][25], bool allgames, int length) {
    // read outcomes
    char fnombre[1024], dummy[1024];
    double ddummy;
    if (allgames)
        //sprintf(fnombre, "saved/finalRaw%i.txt", length);
        sprintf(fnombre, "saved144/finalRaw%i.txt", length);
    //else
    //    sprintf(fnombre, "summary_%i_ci.txt", length);
    FILE *fp = fopen(fnombre, "r");
    
    int i, j;
    for (i = 0; i < 26; i++)
        fscanf(fp, "%s", dummy);

    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        for (j = 0; j < 25; j++) {
            fscanf(fp, "%lf", &(outcomes[j][i]));
        }
        fscanf(fp, "%lf", &ddummy);
        
        //printf("%.5s\t%lf\n", dummy, ddummy);
    }
    //printf("\n");
    
    fclose(fp);
}

int myRank(int index, int theOrder[25]) {
    int i;
    
    for (i = 0; i < 25; i++) {
        if (theOrder[i] == index)
            return i;
    }
    
    return -1;
}

void getWins(bool allgames, int length) {
    double outcomes[25][25], proportion[1001][25];

    getOutcomes(outcomes, allgames, length);
    
    //printf("%lf vs %lf\n", outcomes[0][0], outcomes[0][1]);
    
    int i, j;
    int best[25];
    for (i = 0; i < 25; i++)
        best[i] = 0;
    
    int ind;
    for (i = 0; i < 25; i++) {
        ind = 0;
        for (j = 0; j < 25; j++) {
            //printf("%lf\n", outcomes[i][j]);
            if (outcomes[i][j] > outcomes[i][ind])
                ind = j;
        }
        best[ind] ++;
    }

    for (i = 0; i < 25; i++) {
        printf("%.5s\t%lf\n", agents[i], best[i] / 25.0);
    }
    
    //int rankTotals[25];
    //int best[25];
/* // this code is for the best metric
    int ranks[25][25];
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            ranks[j][i] = i;
        }
    }
    for (i = 0; i < 25; i++) {
        //rankTotals[i] = 0;
        worst[i] = 99999.0;
        //best[i] = 0;
        bubbleSort(ranks[i], outcomes[i]);
    }
    
    int times[25][25];


    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            times[0][0] = 0;
        }
    }

    //for (i = 0; i < 25; i++) {
    //    printf("%lf (%i)\n", outcomes[0][i], ranks[0][i]);
    //}


    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            //rankTotals[ranks[j][i]] += i+1;
            times[ranks[j][i]][i] ++;
        }
    }
    
    int theOrder[25];
    for (i = 0; i < 25; i++)
        theOrder[i] = i;
    orderSort(theOrder, times);
    
    for (i = 0; i < 25; i++) {
        printf("%i: %s: ", i+1, agents[theOrder[i]]);
        for (j = 0; j < 25; j++) {
            printf("%i ", times[theOrder[i]][j]);
        }
        printf("\n");
    }
    
    for (i = 0; i < 25; i++) {
        printf("%.5s\t%lf\t%i\n", agents[i], best[i] / 25.0, myRank(i, theOrder)+1);
    }
    

    //for (i = 0; i < 25; i++) {
    //    printf("%.5s\t%.2lf -- %i\n", agents[i], rankTotals[i] / 25.0, worst[i]);
    //}
    
    printf("%lf vs %lf\n", outcomes[0][0], outcomes[0][1]);
*/
    double worst[25];
    for (i = 0; i < 25; i++)
        worst[i] = 0.99999;

    double ave;
    for (i = 0; i < 25; i++) {
        ave = 0.0;
        for (j = 0; j < 25; j++) {
            ave += outcomes[i][j];
        }
        ave /= 25.0;

        for (j = 0; j < 25; j++) {
            if ((outcomes[i][j] - ave) < worst[j])
                worst[j] = outcomes[i][j] - ave;
        }
    }

    for (i = 0; i < 25; i++) {
        printf("%.5s\t%lf\n", agents[i], worst[i]);
    }
}

void getGroup(int group[6], bool chosen[25]) {
    int i;
    
    for (i = 0; i < 6; i++) {
        group[i] = rand() % 25;
        while (chosen[group[i]])
            group[i] = rand() % 25;
        chosen[group[i]] = true;
    }
    
    //printf("group: ");
    //for (i = 0; i < 6; i++) {
    //    printf("%s, ", agents[group[i]]);
    //}
    //printf("\n");


}

void subRR(int *group, double outcomes[25][25], double *results, int num, int top[2]) {
    int i, j;
    
    for (i = 0; i < num; i++)
        results[i] = 0.0;
    
    for (i = 0; i < num; i++) {
        for (j = 0; j < num; j++) {
            results[i] += outcomes[group[j]][group[i]];
        }
    }

    top[0] = 0;
    for (i = 0; i < num; i++) {
        results[i] /= num;
        //printf("%s\t\t%lf\n", agents[group[i]], results[i]);
        if (results[top[0]] < results[i])
            top[0] = i;
    }
    
    if (top[0] == 0)
        top[1] = 1;
    else
        top[1] = 0;
    for (i = 0; i < num; i++) {
        if ((results[top[1]] < results[i]) && (i != top[0]))
            top[1] = i;
    }
    
    //printf("top: %s, %s\n", agents[group[top[0]]], agents[group[top[1]]]);
    //printf("\n");

}

int runOff1(bool allgames, int length, int winners[4]) {

    double outcomes[25][25];
    
    getOutcomes(outcomes, allgames, length);

    // divide into four groups
    bool chosen[25];
    int i;
    for (i = 0; i < 25; i++)
        chosen[i] = false;
    
    int g1[6], g2[6], g3[6], g4[7];
    getGroup(g1, chosen);
    getGroup(g2, chosen);
    getGroup(g3, chosen);
    int c = 0;
    for (i = 0; i < 25; i++) {
        if (!chosen[i]) {
            g4[c] = i;
            c++;
        }
    }
    
    double r1[6], r2[6], r3[6], r4[7], rw[4];
    int top1[2], top2[2], top3[2], top4[2], topw[2];
    subRR(g1, outcomes, r1, 6, top1);
    subRR(g2, outcomes, r2, 6, top2);
    subRR(g3, outcomes, r3, 6, top3);
    subRR(g4, outcomes, r4, 7, top4);

    winners[0] = g1[top1[0]];
    winners[1] = g2[top2[0]];
    winners[2] = g3[top3[0]];
    winners[3] = g4[top4[0]];
    
    subRR(winners, outcomes, rw, 4, topw);

    return winners[topw[0]];
}

int runOff2(bool allgames, int length, int winners[8], int finalists[2]) {
    double outcomes[25][25];
    
    getOutcomes(outcomes, allgames, length);

    // divide into four groups
    bool chosen[25];
    int i;
    for (i = 0; i < 25; i++)
        chosen[i] = false;
    
    int g1[6], g2[6], g3[6], g4[7];
    getGroup(g1, chosen);
    getGroup(g2, chosen);
    getGroup(g3, chosen);
    int c = 0;
    for (i = 0; i < 25; i++) {
        if (!chosen[i]) {
            g4[c] = i;
            c++;
        }
    }
    
    double r1[6], r2[6], r3[6], r4[7], rw[8], rf[2];
    int top1[2], top2[2], top3[2], top4[2], topw[2], topf[2];
    subRR(g1, outcomes, r1, 6, top1);
    subRR(g2, outcomes, r2, 6, top2);
    subRR(g3, outcomes, r3, 6, top3);
    subRR(g4, outcomes, r4, 7, top4);

    winners[0] = g1[top1[0]];
    winners[1] = g2[top2[0]];
    winners[2] = g3[top3[0]];
    winners[3] = g4[top4[0]];
    winners[4] = g1[top1[1]];
    winners[5] = g2[top2[1]];
    winners[6] = g3[top3[1]];
    winners[7] = g4[top4[1]];
    
    subRR(winners, outcomes, rw, 8, topw);
    finalists[0] = winners[topw[0]];
    finalists[1] = winners[topw[1]];

    subRR(finalists, outcomes, rf, 2, topf);

    return finalists[topf[0]];
}

void runOffs1(bool allgames, int length) {
    int i, j;
    int w;
    int numWins[25];
    int winners[4];
    int advance[25];

    
    for (i = 0; i < 25; i++)
        numWins[i] = advance[i] = 0;
    
    int num = 10000;
    for (i = 0; i < num; i++) {
        w = runOff1(allgames, length, winners);
        //printf("%s\n", agents[w]);
        numWins[w] ++;
        for (j = 0; j < 4; j++)
            advance[winners[j]] ++;
    }

    for (i = 0; i < 25; i++) {
        printf("%s:\t\t%.4lf\t%.4lf\n", agents[i], advance[i] / (double)num, numWins[i] / (double)num);
    }
}

void runOffs2(bool allgames, int length) {
    int w;
    int winners[8], finalists[2];
    int i, j;
    int numWins[25];
    int advance1[25], advance2[25];

    for (i = 0; i < 25; i++)
        numWins[i] = advance1[i] = advance2[i] = 0;
    
    int num = 10000;
    for (i = 0; i < num; i++) {
        w = runOff2(allgames, length, winners, finalists);
        //printf("winner: %s\n", agents[w]);
        numWins[w] ++;
        for (j = 0; j < 8; j++)
            advance1[winners[j]] ++;
        for (j = 0; j < 2; j++)
            advance2[finalists[j]] ++;
    }

    for (i = 0; i < 25; i++) {
        printf("%s:\t\t%.4lf\t%.4lf\t%.4lf\n", agents[i], advance1[i] / (double)num, advance2[i] / (double)num, numWins[i] / (double)num);
    }
}

void reformat() {
    FILE *fp = fopen("saved/allmachines.csv", "w");
    fprintf(fp, "Agent,Opponent,GameLength,Value\n");

    double outcomes[25][25];
    int i, j;
    
    getOutcomes(outcomes, true, 100);
    for (j = 0; j < 25; j++) {
        for (i = 0; i < 25; i++) {
            fprintf(fp, "%s,%s,100,%lf\n", agents[i], agents[j], outcomes[j][i]);
        }
    }
    fprintf(fp, "\n");
    
    getOutcomes(outcomes, true, 1000);
    for (j = 0; j < 25; j++) {
        for (i = 0; i < 25; i++) {
            fprintf(fp, "%s,%s,1000,%lf\n", agents[i], agents[j], outcomes[j][i]);
        }
    }
    fprintf(fp, "\n");
    
    getOutcomes(outcomes, true, 50000);
    for (j = 0; j < 25; j++) {
        for (i = 0; i < 25; i++) {
            fprintf(fp, "%s,%s,50000,%lf\n", agents[i], agents[j], outcomes[j][i]);
        }
    }
    fprintf(fp, "\n");


    fclose(fp);
}

const char *gameClass(int g, int index) {
    switch (NEConfusion(g, index)) {
        case 0: return "  Simple";
        case 1: return " Multiple Pure NE";
        case 2: return "Dominated Pure NE";
    }
    
    return "Unknown";
}

int agentClass(int id) {

    switch (id) {
        case 0: return cSTATIC;
        case 1: return cEXPERT;
        case 2: return cOTHER;
        case 3: return cRL;
        case 4: return cRL;
        case 5: return cRL;
        case 6: return cEXPERT;
        case 7: return cEXPERT;
        case 8: return cRL;
        case 9: return cRL;
        case 10: return cRL;
        case 11: return cOTHER;
        case 12: return cEXPERT;
        case 13: return cOTHER;
        case 14: return cSTATIC;
        case 15: return cEXPERT;
        case 16: return cEXPERT;
        case 17: return cSTATIC;
        case 18: return cEXPERT;
        case 19: return cOTHER;
        case 20: return cSTATIC;
        case 21: return cSTATIC;
        case 22: return cSTATIC;
        case 23: return cEXPERT;
        case 24: return cEXPERT;
        case 25: return cEXPERT;
    }
    
    printf("type not found\n");

    return -1;
}

const char *agentsStr(int index) {
    switch (index) {
        case 0: return "Bully";
        case 1: return "S++";
        case 2: return "manipulator-bully";
        case 3: return "M-Qubed";
        case 4: return "MBRL-2";
        case 5: return "qlearn";
        case 6: return "EEE";
        case 7: return "S";
        case 8: return "br1";
        case 9: return "wolf";
        case 10: return "CJAL";
        case 11: return "Fictitious Play";
        case 12: return "WMA";
        case 13: return "manipulator-gf";
        case 14: return "Godfather";
        case 15: return "Exp3_simp";
        case 16: return "gigawolf";
        case 17: return "random";
        case 18: return "EEE_simp";
        case 19: return "sfp";
        case 20: return "pavlov";
        case 21: return "memory1";
        case 22: return "memory2";
        case 23: return "Exp3";
        case 24: return "S++_simp";
        case 25: return "S_simp";
    }
    
    return "Garbage";
}

const char *toStrng(int index) {
    switch (index) {
        case 0: return "Static";
        case 1: return "RL";
        case 2: return "Expert";
        case 3: return "Other";
    }
    
    return "Nothing";
}

void fillPartnerEntries() {
    int g, index, a1, a2, i, j;
    
    printf("Standardizing payoffs\n");
    
    FILE *fp;
    char nombre[1024], dummy[1024];
    double val100_0, val100_1, val1000_0, val1000_1, val50000_0, val50000_1;
    
    for (i = 0; i < 4; i++) {
        pEntries[i].count = 0;
    }
    
    for (i = 0; i < 3; i++) {
        lEntries[i].count = 0;
    }
    
    for (i = 0; i < 3; i++) {
        gEntries[i].count = 0;
    }
    
    int ind = 0;
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            ind = NEConfusion(g, index);
            for (a1 = 0; a1 < numAgents-1; a1++) {
                for (a2 = 0; a2 < numAgents-1; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);
                    
                    val100_0 = val100_0 - meanGameScore[0][g][index-1][0];
                    val100_1 = val100_1 - meanGameScore[0][g][index-1][1];
                    val1000_0 = val1000_0 - meanGameScore[1][g][index-1][0];
                    val1000_1 = val1000_1 - meanGameScore[1][g][index-1][1];
                    val50000_0 = val50000_0 - meanGameScore[2][g][index-1][0];
                    val50000_1 = val50000_1 - meanGameScore[2][g][index-1][1];
                    
                    if (a1 != 17) { // remove random -- it's an outlier
                        pEntries[agentClass(a2)].v100[pEntries[agentClass(a2)].count] = val100_0;
                        pEntries[agentClass(a2)].v1000[pEntries[agentClass(a2)].count] = val1000_0;
                        pEntries[agentClass(a2)].v50000[pEntries[agentClass(a2)].count] = val50000_0;
                        pEntries[agentClass(a2)].count ++;
                        
                        lEntries[0].vals[lEntries[0].count] = val100_0;
                        lEntries[0].count ++;
                        lEntries[1].vals[lEntries[1].count] = val1000_0;
                        lEntries[1].count ++;
                        lEntries[2].vals[lEntries[2].count] = val50000_0;
                        lEntries[2].count ++;
                        
                        gEntries[ind].vals[gEntries[ind].count] = val50000_0;
                        gEntries[ind].count ++;
                    }
                    
                    if (a2 != 17) {
                        pEntries[agentClass(a1)].v100[pEntries[agentClass(a1)].count] = val100_1;
                        pEntries[agentClass(a1)].v1000[pEntries[agentClass(a1)].count] = val1000_1;
                        pEntries[agentClass(a1)].v50000[pEntries[agentClass(a1)].count] = val50000_1;
                        pEntries[agentClass(a1)].count ++;

                        lEntries[0].vals[lEntries[0].count] = val100_1;
                        lEntries[0].count ++;
                        lEntries[1].vals[lEntries[1].count] = val1000_1;
                        lEntries[1].count ++;
                        lEntries[2].vals[lEntries[2].count] = val50000_1;
                        lEntries[2].count ++;

                        gEntries[ind].vals[gEntries[ind].count] = val50000_1;
                        gEntries[ind].count ++;
                    }
                    
                    fclose(fp);
                }
            }
        }
    }
    
    for (i = 0; i < 4; i++) {
        pEntries[i].mean100 = pEntries[i].mean1000 = pEntries[i].mean50000 = 0.0;
        pEntries[i].stdev100 = pEntries[i].stdev1000 = pEntries[i].stdev50000 = 0.0;
        for (j = 0; j < pEntries[i].count; j++) {
            pEntries[i].mean100 += pEntries[i].v100[j];
            pEntries[i].mean1000 += pEntries[i].v1000[j];
            pEntries[i].mean50000 += pEntries[i].v50000[j];
        }
        pEntries[i].mean100 /= pEntries[i].count;
        pEntries[i].mean1000 /= pEntries[i].count;
        pEntries[i].mean50000 /= pEntries[i].count;

        for (j = 0; j < pEntries[i].count; j++) {
            pEntries[i].stdev100 += (pEntries[i].v100[j] - pEntries[i].mean100) * (pEntries[i].v100[j] - pEntries[i].mean100);
            pEntries[i].stdev1000 += (pEntries[i].v1000[j] - pEntries[i].mean1000) * (pEntries[i].v1000[j] - pEntries[i].mean1000);
            pEntries[i].stdev50000 += (pEntries[i].v50000[j] - pEntries[i].mean50000) * (pEntries[i].v50000[j] - pEntries[i].mean50000);
        }

        //printf("sum = %lf\n", pEntries[i].stdev100);
        pEntries[i].stdev100 = sqrt(pEntries[i].stdev100/pEntries[i].count);
        pEntries[i].stdev1000 = sqrt(pEntries[i].stdev1000/pEntries[i].count);
        pEntries[i].stdev50000 = sqrt(pEntries[i].stdev50000/pEntries[i].count);
        pEntries[i].stdev100 = 1;
        pEntries[i].stdev1000 = 1;
        pEntries[i].stdev50000 = 1;

        printf("%i (%i): mean = (%lf, %lf, %lf); stdev = (%.3lf, %.3lf, %.3lf)\n", i, pEntries[i].count,
                    pEntries[i].mean100, pEntries[i].mean1000, pEntries[i].mean50000,
                    pEntries[i].stdev100, pEntries[i].stdev1000, pEntries[i].stdev50000);
    
    }
    
    printf("\nLength:\n");
    
    for (i = 0; i < 3; i++) {
        lEntries[i].mean = lEntries[i].stdev = 0;
        for (j = 0; j < lEntries[i].count; j++) {
            lEntries[i].mean += lEntries[i].vals[j];
        }
        lEntries[i].mean /= lEntries[i].count;
        
        for (j = 0; j < lEntries[i].count; j++) {
            lEntries[i].stdev += (lEntries[i].vals[j] - lEntries[i].mean) * (lEntries[i].vals[j] - lEntries[i].mean);
        }
        lEntries[i].stdev = sqrt(lEntries[i].stdev / lEntries[i].count);
        lEntries[i].stdev = 1;
        
        printf("%i (%i): mean = %.2lf; stdev = %.3lf\n", i, lEntries[i].count, lEntries[i].mean, lEntries[i].stdev);
    }
    
    printf("\nGameType:\n");
    for (i = 0; i < 3; i++) {
        gEntries[i].mean = gEntries[i].stdev = 0;
        for (j = 0; j < gEntries[i].count; j++) {
            gEntries[i].mean += gEntries[i].vals[j];
        }
        gEntries[i].mean /= gEntries[i].count;
        
        for (j = 0; j < gEntries[i].count; j++) {
            gEntries[i].stdev += (gEntries[i].vals[j] - gEntries[i].mean) * (gEntries[i].vals[j] - gEntries[i].mean);
        }
        gEntries[i].stdev = sqrt(gEntries[i].stdev / gEntries[i].count);
        gEntries[i].stdev = 1;
        
        printf("%i (%i): mean = %.2lf; stdev = %.3lf\n", i, gEntries[i].count, gEntries[i].mean, gEntries[i].stdev);
    }
}

void gameReview() {
    int g = 0;
    int index, gt;
    
    int NEcount = 0;
    int NotSimple = 0;
    
    for (g = 0; g < 5; g++) {
    for (index = 1; index < 79; index++) {
        printGame(g, index);
        gt = NEConfusion(g, index);
        if (gt == 1) {
            printf("NEConfusion\n");
            NEcount ++;
        }
        else if (gt == 2) {
            printf("NotSimple\n");
            NotSimple ++;
        }
        
        printf("\n\n");
    }
    }
    
    printf("NEcount = %i\n", NEcount);
    printf("NotSimple = %i\n", NotSimple);
}

void computeMeanGameScore(int g, int index) {
    int a1, a2, i;
    char dummy[1024], nombre[1024];
    double val50000_0, val50000_1, val100_0, val100_1, val1000_0, val1000_1;
    FILE *fp;
    
    for (i = 0; i < 3; i++)
        meanGameScore[i][g][index][0] = meanGameScore[i][g][index][1] = 0.0;
    
    for (a1 = 0; a1 < numAgents-1; a1++) {
        for (a2 = 0; a2 < numAgents-1; a2++) {
            sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index+1, agents[a1], agents[a2]);
            
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
            fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
            fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);
            
            if (a1 != 17) {
                meanGameScore[0][g][index][0] += val100_0;
                meanGameScore[1][g][index][0] += val1000_0;
                meanGameScore[2][g][index][0] += val50000_0;
            }

            if (a2 != 17) {
                meanGameScore[0][g][index][1] += val100_1;
                meanGameScore[1][g][index][1] += val1000_1;
                meanGameScore[2][g][index][1] += val50000_1;
            }
            
            fclose(fp);
        }
    }
    
    for (i = 0; i < 3; i++) {
        meanGameScore[i][g][index][0] /= (numAgents-2) * (numAgents-1);
        meanGameScore[i][g][index][1] /= (numAgents-2) * (numAgents-1);
    }
    

}

void computeMeanGameScores() {
    int g, index;
    int i, j;
    
    printf("Computing the Mean Game Scores\n");
    for (g = 0; g < 5; g++) {
        for (index = 0; index < 78; index++) {
            computeMeanGameScore(g, index);
        }
    }
}


int determineGameType(const char *specifier, FILE *fp) {
    char nombre[1024];
    int theG;
    
    while (true) {
        fscanf(fp, "%s %i", nombre, &theG);
        if (!strcmp(nombre, specifier))
            return theG;
    }

    return -1;
}

int getGameInfo(int g, int index, int theGs[2]) {
    FILE *fp;
    char nombre[1024], dummy[1024];
    int d1;
    
    sprintf(nombre, "../gameClass/%s_%i.txt", games[g], index);
    fp = fopen(nombre, "r");
    
    fscanf(fp, "%s", dummy);
    if (!strcmp(dummy, "Game")) {
        fscanf(fp, "%i %i", &(theGs[0]), &d1);
        theGs[1] = theGs[2];
    }
    else {
        fscanf(fp, "%i %i", &(theGs[0]), &d1);
        fscanf(fp, "%i %i", &(theGs[1]), &d1);
    }
    
    int typeG = determineGameType("RobinsonGoforth:", fp);
    
    fclose(fp);
    
    return typeG-1;
}


void computeMeanGameScore2(int g, int index) {
    int theGs[2];
    int a1, a2;
    char nombre[1024], dummy[1024];
    double v1, v2;
    FILE *fp;
    
    // find the game
    getGameInfo(g, index, theGs);
    //printf("games %i and %i\n", theGs[0], theGs[1]);
    bool symmetric = false;
    if (theGs[0] == theGs[1])
        symmetric = true;
    
    for (a1 = 0; a1 < 25; a1++) {
        for (a2 = a1; a2 < 25; a2++) {
            sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
            meanGameScore2[2][g][theGs[0]] += v1;
            countGameScore2[2][g][theGs[0]] ++;
            //printf("Recorded %s vs %s in %i\n", agents[a1], agents[a2], theGs[0]);
            
            //if (a1 != a2) {
            meanGameScore2[2][g][theGs[1]] += v2;
            countGameScore2[2][g][theGs[1]] ++;
            
            //printf("Recorded %s vs %s in %i\n", agents[a2], agents[a1], theGs[1]);
            //}
            
            fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
            meanGameScore2[0][g][theGs[0]] += v1;
            countGameScore2[0][g][theGs[0]] ++;
            //if (a1 != a2) {
            meanGameScore2[0][g][theGs[1]] += v2;
            countGameScore2[0][g][theGs[1]] ++;
            //}
            
            fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
            meanGameScore2[1][g][theGs[0]] += v1;
            countGameScore2[1][g][theGs[0]] ++;
            //if (a1 != a2) {
            meanGameScore2[1][g][theGs[1]] += v2;
            countGameScore2[1][g][theGs[1]] ++;
            //}

            fclose(fp);

            if (!symmetric && (a1 != a2)) {
                //printf("(%s, %s) ... ", agents[a2], agents[a1]); fflush(stdout);
                sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                fp = fopen(nombre, "r");
                
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                meanGameScore2[2][g][theGs[0]] += v1;
                countGameScore2[2][g][theGs[0]] ++;
                //printf("Recorded %s vs %s in %i\n", agents[a2], agents[a1], theGs[0]);
                meanGameScore2[2][g][theGs[1]] += v2;
                countGameScore2[2][g][theGs[1]] ++;
                //printf("Recorded %s vs %s in %i\n", agents[a1], agents[a2], theGs[1]);
                
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                meanGameScore2[0][g][theGs[0]] += v1;
                countGameScore2[0][g][theGs[0]] ++;
                meanGameScore2[0][g][theGs[1]] += v2;
                countGameScore2[0][g][theGs[1]] ++;
                
                fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                meanGameScore2[1][g][theGs[0]] += v1;
                countGameScore2[1][g][theGs[0]] ++;
                meanGameScore2[1][g][theGs[1]] += v2;
                countGameScore2[1][g][theGs[1]] ++;
                
                fclose(fp);
            }
        }
    }

}

void computeMeanGameScores2() {
    int g, index;
    int i, j;
    
    printf("Computing the Mean Game Scores\n");
    for (g = 0; g < 5; g++) {
        printf("computing %s\n", games[g]);
        for (index = 0; index < 78; index++) {
            computeMeanGameScore2(g, index+1);
        }
    }

    for (g = 0; g < 5; g++) {
        for (index = 0; index < 144; index++) {
            meanGameScore2[0][g][index] /= (25*25);
            meanGameScore2[1][g][index] /= (25*25);
            meanGameScore2[2][g][index] /= (25*25);
        }
    }

    printf("%lf (%i)\n", meanGameScore2[0][0][90] / (25 * 25), countGameScore2[0][0][90]);
    printf("%lf (%i)\n", meanGameScore2[0][0][75] / (25 * 25), countGameScore2[0][0][75]);
}

void computeAverages() {
    int g, index, a1, a2;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double val100_0, val100_1, val1000_0, val1000_1, val50000_0, val50000_1;
    double tval100_0, tval100_1, tval1000_0, tval1000_1, tval50000_0, tval50000_1;
    
    double losPuntos_Partner[3][25][4];
    double losPuntos_Game[3][25][3];
    
    int i, j;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 4; j++) {
            losPuntos_Partner[0][i][j] = losPuntos_Partner[1][i][j] = losPuntos_Partner[2][i][j] = 0.0;
        }
        for (j = 0; j < 3; j++) {
            losPuntos_Game[0][i][j] = losPuntos_Game[1][i][j] = losPuntos_Game[2][i][j] = 0.0;
        }
    }
    
    int ind;
    int count[3] = {0, 0, 0};
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            ind = NEConfusion(g, index);
            for (a1 = 0; a1 < numAgents-1; a1++) {
                for (a2 = 0; a2 < numAgents-1; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);

                    losPuntos_Partner[0][a1][agentClass(a2)] += val100_0;
                    losPuntos_Partner[0][a2][agentClass(a1)] += val100_1;
                    losPuntos_Partner[1][a1][agentClass(a2)] += val1000_0;
                    losPuntos_Partner[1][a2][agentClass(a1)] += val1000_1;
                    losPuntos_Partner[2][a1][agentClass(a2)] += val50000_0;
                    losPuntos_Partner[2][a2][agentClass(a1)] += val50000_1;
                    
                    losPuntos_Game[0][a1][NEConfusion(g, index)] += val100_0;
                    losPuntos_Game[0][a2][NEConfusion(g, index)] += val100_1;
                    losPuntos_Game[1][a1][NEConfusion(g, index)] += val1000_0;
                    losPuntos_Game[1][a2][NEConfusion(g, index)] += val1000_1;
                    losPuntos_Game[2][a1][NEConfusion(g, index)] += val50000_0;
                    losPuntos_Game[2][a2][NEConfusion(g, index)] += val50000_1;
                    
                    count[NEConfusion(g, index)] ++;
                    count[NEConfusion(g, index)] ++;
                    
                    fclose(fp);
                }
            }
        }
    }
    
    for (i = 0; i < 3; i++)
        printf("count%i = %i\n", i, count[i] / 25);
    
    for (i = 0; i < 25; i++) {
        printf("%s\t%lf\t%lf\t%lf\n", agents[i],
                                    losPuntos_Game[0][i][0] / (25*265*2),
                                    losPuntos_Game[0][i][1] / (25*60*2),
                                    losPuntos_Game[0][i][2] / (25*65*2));
    
    }
    
    printf("\n\n");
    for (i = 0; i < 25; i++) {
        printf("%s\t%lf\t%lf\t%lf\t%lf\n", agents[i],
                                    losPuntos_Partner[2][i][0] / (390*25*2),
                                    losPuntos_Partner[2][i][1] / (390*25*2),
                                    losPuntos_Partner[2][i][2] / (390*25*2),
                                    losPuntos_Partner[2][i][3] / (390*25*2));
    
    }
}

void computeAveragesNew() {
    int g, index, a1, a2, i, j, G;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double v1, v2;
    int d1, d2;
    
    double sums[10][4][30][30];
    int counts[10][4][30][30];
    
    for (G = 0; G < 10; G++) {
        for (a1 = 0; a1 < numAgents; a1++) {
            for (a2 = 0; a2 < numAgents; a2++) {
                counts[G][0][a1][a2] = counts[G][1][a1][a2] = counts[G][2][a1][a2] = counts[G][3][a1][a2] = 0;
                sums[G][0][a1][a2] = sums[G][1][a1][a2] = sums[G][2][a1][a2] = sums[G][3][a1][a2] = 0.0;
            }
        }
    }
    
    int cuenta = 0;
    bool symmetric = false;
    int typeG;
    for (g = 0; g < 5; g++) {
        printf("Processing %s games ... \n", games[g]);
        for (index = 1; index < 79; index++) {
            sprintf(nombre, "../gameClass/%s_%i.txt", games[g], index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%s", dummy);
            if (!strcmp(dummy, "Game")) {
                symmetric = true;
                cuenta ++;
                fscanf(fp, "%i %i", &d1, &d2);
            }
            else {
                symmetric = false;
                cuenta += 2;
                fscanf(fp, "%i %i", &d1, &d2);
                fscanf(fp, "%i %i", &d1, &d2);
            }
            
            typeG = determineGameType("Rapoport:", fp);
            typeG --;
            
            //if (typeG != 0)
            //    continue;
            //printf("%i\n", typeG);
            
            fclose(fp);

            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = a1; a2 < numAgents; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    fp = fopen(nombre, "r");
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    
                    sums[typeG][0][a1][a2] += v1;
                    counts[typeG][0][a1][a2] ++;
                    if (a1 != a2) {
                        sums[typeG][0][a2][a1] += v2;
                        counts[typeG][0][a2][a1] ++;
                    }
                    
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[typeG][1][a1][a2] += v1;
                    counts[typeG][1][a1][a2] ++;
                    if (a1 != a2) {
                        sums[typeG][1][a2][a1] += v2;
                        counts[typeG][1][a2][a1] ++;
                    }
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[typeG][2][a1][a2] += v1;
                    counts[typeG][2][a1][a2] ++;
                    if (a1 != a2) {
                        sums[typeG][2][a2][a1] += v2;
                        counts[typeG][2][a2][a1] ++;
                    }
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[typeG][3][a1][a2] += v1;
                    counts[typeG][3][a1][a2] ++;
                    if (a1 != a2) {
                        sums[typeG][3][a2][a1] += v2;
                        counts[typeG][3][a2][a1] ++;
                    }
                    fclose(fp);

                    if (!symmetric) {
                        //printf("(%s, %s) ... ", agents[a2], agents[a1]); fflush(stdout);
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[typeG][0][a2][a1] += v1;
                        counts[typeG][0][a2][a1] ++;
                        if (a1 != a2) {
                            sums[typeG][0][a1][a2] += v2;
                            counts[typeG][0][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[typeG][1][a2][a1] += v1;
                        counts[typeG][1][a2][a1] ++;
                        if (a1 != a2) {
                            sums[typeG][1][a1][a2] += v2;
                            counts[typeG][1][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[typeG][2][a2][a1] += v1;
                        counts[typeG][2][a2][a1] ++;
                        if (a1 != a2) {
                            sums[typeG][2][a1][a2] += v2;
                            counts[typeG][2][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[typeG][3][a2][a1] += v1;
                        counts[typeG][3][a2][a1] ++;
                        if (a1 != a2) {
                            sums[typeG][3][a1][a2] += v2;
                            counts[typeG][3][a1][a2] ++;
                        }
                        fclose(fp);
                    }
                }
            }
        }
    }
    
    printf("termino\n"); fflush(stdout);
    
    double sum[10];
    int count[10];
    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (G = 0; G < 10; G++) {
            sum[G] = 0.0;
            count[G] = 0;
            for (j = 0; j < 25; j++) {
                sum[G] += sums[G][1][i][j];
                count[G] += counts[G][1][i][j];
            }
            printf("%lf\t", sum[G] / count[G]);
        }
        printf("\n");
    }
    
    printf("\n\n");
    
    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (G = 0; G < 10; G++) {
            sum[G] = 0.0;
            count[G] = 0;
            for (j = 0; j < 25; j++) {
                sum[G] += sums[G][2][i][j];
                count[G] += counts[G][2][i][j];
            }
            printf("%lf\t", sum[G] / count[G]);
        }
        printf("\n");
    }
    
    printf("\n\n");
    
    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (G = 0; G < 10; G++) {
            sum[G] = 0.0;
            count[G] = 0;
            for (j = 0; j < 25; j++) {
                sum[G] += sums[G][0][i][j];
                count[G] += counts[G][0][i][j];
            }
            printf("%lf\t", sum[G] / count[G]);
        }
        printf("\n");
    }
    
}

void computeAveragesNew2() {
    int g, index, a1, a2, i, j, G;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double v1, v2;
    int d1, d2;
    
    double sums[10][4][30][30];
    int counts[10][4][30][30];
    
    for (G = 0; G < 10; G++) {
        for (a1 = 0; a1 < numAgents; a1++) {
            for (a2 = 0; a2 < numAgents; a2++) {
                counts[G][0][a1][a2] = counts[G][1][a1][a2] = counts[G][2][a1][a2] = counts[G][3][a1][a2] = 0;
                sums[G][0][a1][a2] = sums[G][1][a1][a2] = sums[G][2][a1][a2] = sums[G][3][a1][a2] = 0.0;
            }
        }
    }
    
    int cuenta = 0;
    bool symmetric = false;
    //int typeG;
    for (g = 0; g < 5; g++) {
        printf("Processing %s games ... \n", games[g]);
        for (index = 1; index < 79; index++) {
            sprintf(nombre, "../gameClass/%s_%i.txt", games[g], index);
            fp = fopen(nombre, "r");
            
            fscanf(fp, "%s", dummy);
            if (!strcmp(dummy, "Game")) {
                symmetric = true;
                cuenta ++;
                fscanf(fp, "%i %i", &d1, &d2);
            }
            else {
                symmetric = false;
                cuenta += 2;
                fscanf(fp, "%i %i", &d1, &d2);
                fscanf(fp, "%i %i", &d1, &d2);
            }
            
            //typeG = determineGameType("Crandall:", fp);
            //printf("%i\n", typeG);
            
            fclose(fp);

            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = a1; a2 < numAgents; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    fp = fopen(nombre, "r");
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    
                    sums[agentClass(a2)][0][a1][a2] += v1;
                    counts[agentClass(a2)][0][a1][a2] ++;
                    if (a1 != a2) {
                        sums[agentClass(a1)][0][a2][a1] += v2;
                        counts[agentClass(a1)][0][a2][a1] ++;
                    }
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[agentClass(a2)][1][a1][a2] += v1;
                    counts[agentClass(a2)][1][a1][a2] ++;
                    if (a1 != a2) {
                        sums[agentClass(a1)][1][a2][a1] += v2;
                        counts[agentClass(a1)][1][a2][a1] ++;
                    }
                    
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[agentClass(a2)][2][a1][a2] += v1;
                    counts[agentClass(a2)][2][a1][a2] ++;
                    if (a1 != a2) {
                        sums[agentClass(a1)][2][a2][a1] += v2;
                        counts[agentClass(a1)][2][a2][a1] ++;
                    }
                    fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                    sums[agentClass(a2)][3][a1][a2] += v1;
                    counts[agentClass(a2)][3][a1][a2] ++;
                    if (a1 != a2) {
                        sums[agentClass(a1)][3][a2][a1] += v2;
                        counts[agentClass(a1)][3][a2][a1] ++;
                    }
                    fclose(fp);

                    if (!symmetric) {
                        //printf("(%s, %s) ... ", agents[a2], agents[a1]); fflush(stdout);
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[agentClass(a1)][0][a2][a1] += v1;
                        counts[agentClass(a1)][0][a2][a1] ++;
                        if (a1 != a2) {
                            sums[agentClass(a2)][0][a1][a2] += v2;
                            counts[agentClass(a2)][0][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[agentClass(a1)][1][a2][a1] += v1;
                        counts[agentClass(a1)][1][a2][a1] ++;
                        if (a1 != a2) {
                            sums[agentClass(a2)][1][a1][a2] += v2;
                            counts[agentClass(a2)][1][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[agentClass(a1)][2][a2][a1] += v1;
                        counts[agentClass(a1)][2][a2][a1] ++;
                        if (a1 != a2) {
                            sums[agentClass(a2)][2][a1][a2] += v2;
                            counts[agentClass(a2)][2][a1][a2] ++;
                        }
                        fscanf(fp, "%s %lf %lf", dummy, &v1, &v2);
                        sums[agentClass(a1)][3][a2][a1] += v1;
                        counts[agentClass(a1)][3][a2][a1] ++;
                        if (a1 != a2) {
                            sums[agentClass(a2)][3][a1][a2] += v2;
                            counts[agentClass(a2)][3][a1][a2] ++;
                        }
                        fclose(fp);
                    }
                    
                }
            }
        }
    }
    
    double sum[5];
    int count[5];
    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (G = 0; G < 4; G++) {
            sum[G] = 0.0;
            count[G] = 0;
            for (j = 0; j < 25; j++) {
                sum[G] += sums[G][0][i][j];
                count[G] += counts[G][0][i][j];
            }
            printf("%lf\t", sum[G] / count[G]);
        }
        printf("\n");
    }
    
}


void masterPerformanceSpreadsheet() {
    int g, index, a1, a2;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double val100_0, val100_1, val1000_0, val1000_1, val50000_0, val50000_1;
    double tval100_0, tval100_1, tval1000_0, tval1000_1, tval50000_0, tval50000_1;
    
    //gameReview();
    //return;
    
    computeMeanGameScores();
    
    fillPartnerEntries();

    printf("Creating the Master File\n");

    FILE *masterfp = fopen("masterPerformanceSpreadsheet.csv", "w");
    
    fprintf(masterfp, "Game,GameClass,Algorithm,Partner,PartnerClass,v100,v1000,v50000,vp100,vp1000,vp50000,vl100,vl1000,vl50000,vg50000\n");
    
    int ind;
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            ind = NEConfusion(g, index);
            for (a1 = 0; a1 < numAgents; a1++) {
                for (a2 = 0; a2 < numAgents; a2++) {
                    if ((a1 == (numAgents-1)) && (a2 == (numAgents-1)))
                        continue;
                
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    //printf("%s\n", nombre); fflush(stdout);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);

                    tval100_0 = val100_0 - meanGameScore[0][g][index-1][0];
                    tval100_1 = val100_1 - meanGameScore[0][g][index-1][1];
                    tval1000_0 = val1000_0 - meanGameScore[1][g][index-1][0];
                    tval1000_1 = val1000_1 - meanGameScore[1][g][index-1][1];
                    tval50000_0 = val50000_0 - meanGameScore[2][g][index-1][0];
                    tval50000_1 = val50000_1 - meanGameScore[2][g][index-1][1];


                    if ((a1 != 17) && (a2 != (numAgents-1))) {
                        fprintf(masterfp, "%s_%i_row,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                games[g], index, gameClass(g, index), agentsStr(a1), agentsStr(a2),
                                toStrng(agentClass(a2)), val100_0, val1000_0, val50000_0,
                                (tval100_0 - pEntries[agentClass(a2)].mean100) / pEntries[agentClass(a2)].stdev100,
                                (tval1000_0 - pEntries[agentClass(a2)].mean1000) / pEntries[agentClass(a2)].stdev1000,
                                (tval50000_0 - pEntries[agentClass(a2)].mean50000) / pEntries[agentClass(a2)].stdev50000,
                                (tval100_0 - lEntries[0].mean) / lEntries[0].stdev,
                                (tval1000_0 - lEntries[1].mean) / lEntries[1].stdev,
                                (tval50000_0 - lEntries[2].mean) / lEntries[2].stdev,
                                (tval50000_0 - gEntries[ind].mean) / gEntries[ind].stdev);
                    }
                    
                    if ((a2 != 17) && (a1 != (numAgents-1))) {
                        fprintf(masterfp, "%s_%i_col,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                games[g], index, gameClass(g, index), agentsStr(a2), agentsStr(a1),
                                toStrng(agentClass(a1)), val100_1, val1000_1, val50000_1,
                                (tval100_1 - pEntries[agentClass(a1)].mean100) / pEntries[agentClass(a1)].stdev100,
                                (tval1000_1 - pEntries[agentClass(a1)].mean1000) / pEntries[agentClass(a1)].stdev1000,
                                (tval50000_1 - pEntries[agentClass(a1)].mean50000) / pEntries[agentClass(a1)].stdev50000,
                                (tval100_1 - lEntries[0].mean) / lEntries[0].stdev,
                                (tval1000_1 - lEntries[1].mean) / lEntries[1].stdev,
                                (tval50000_1 - lEntries[2].mean) / lEntries[2].stdev,
                                (tval50000_1 - gEntries[ind].mean) / gEntries[ind].stdev);
                    }
                    
                    fclose(fp);
                }
            }
        }
    }

    fclose(masterfp);
}

const char *strGame(int tipo) {
    switch (tipo) {
        case 0: return "Win-Win"; break;
        case 1: return "Biased"; break;
        case 2: return "Second Best"; break;
        case 3: return "Unfair"; break;
        case 4: return "Inferior"; break;
        case 5: return "Cyclic"; break;
    }
    
    return "not found";
}

void masterPerformanceSpreadsheet2() {
    int g, index, a1, a2;
    FILE *fp;
    char nombre[1024], dummy[1024];
    double val100_0, val100_1, val1000_0, val1000_1, val50000_0, val50000_1;
    double tval100_0, tval100_1, tval1000_0, tval1000_1, tval50000_0, tval50000_1;
    
    //gameReview();
    //return;
    
    computeMeanGameScores2();
    
    //fillPartnerEntries2();

    printf("Creating the Master File 2\n");

    
    FILE *masterfp = fopen("masterPerformanceSpreadsheet2.csv", "w");
    
    fprintf(masterfp, "Game,GameNumber,GameClass,Algorithm,Partner,PartnerClass,v100,v1000,v50000,d100,d1000,d50000,m100,m1000,m50000\n");
    
    int tipo;
    int theGs[2];
    for (g = 0; g < 5; g++) {
        for (index = 1; index < 79; index++) {
            tipo = getGameInfo(g, index, theGs);
            for (a1 = 0; a1 < 25; a1++) {
                for (a2 = a1; a2 < 25; a2++) {
                    sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a1], agents[a2]);
                    
                    fp = fopen(nombre, "r");
                    
                    fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
                    fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);

                    tval100_0 = val100_0 - meanGameScore2[0][g][theGs[0]];
                    tval100_1 = val100_1 - meanGameScore2[0][g][theGs[1]];
                    tval1000_0 = val1000_0 - meanGameScore2[1][g][theGs[0]];
                    tval1000_1 = val1000_1 - meanGameScore2[1][g][theGs[1]];
                    tval50000_0 = val50000_0 - meanGameScore2[2][g][theGs[0]];
                    tval50000_1 = val50000_1 - meanGameScore2[2][g][theGs[1]];

                    if (a1 != 17) {
                        fprintf(masterfp, "%s_%i_row,%i,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                games[g], index, theGs[0], strGame(tipo), agentsStr(a1), agentsStr(a2), toStrng(agentClass(a2)),
                                val100_0, val1000_0, val50000_0, tval100_0, tval1000_0, tval50000_0,
                                meanGameScore2[0][g][theGs[0]], meanGameScore2[1][g][theGs[0]], meanGameScore2[2][g][theGs[0]]);
                    }
                    
                    if ((a2 != 17)) {
                        fprintf(masterfp, "%s_%i_col,%i,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                games[g], index, theGs[1], strGame(tipo), agentsStr(a2), agentsStr(a1), toStrng(agentClass(a1)),
                                val100_1, val1000_1, val50000_1, tval100_1, tval1000_1, tval50000_1,
                                meanGameScore2[0][g][theGs[1]], meanGameScore2[1][g][theGs[1]], meanGameScore2[2][g][theGs[1]]);
                    }
                    
                    fclose(fp);
                    
                    if ((theGs[0] != theGs[1]) && (a1 != a2)) {
                        sprintf(nombre, "../results/%s_%i_%s_%s.txt", games[g], index, agents[a2], agents[a1]);
                        
                        fp = fopen(nombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &val50000_0, &val50000_1);
                        fscanf(fp, "%s %lf %lf", dummy, &val100_0, &val100_1);
                        fscanf(fp, "%s %lf %lf", dummy, &val1000_0, &val1000_1);
                        
                        tval100_0 = val100_0 - meanGameScore2[0][g][theGs[0]];
                        tval100_1 = val100_1 - meanGameScore2[0][g][theGs[1]];
                        tval1000_0 = val1000_0 - meanGameScore2[1][g][theGs[0]];
                        tval1000_1 = val1000_1 - meanGameScore2[1][g][theGs[1]];
                        tval50000_0 = val50000_0 - meanGameScore2[2][g][theGs[0]];
                        tval50000_1 = val50000_1 - meanGameScore2[2][g][theGs[1]];
                        
                        if (a1 != 17) {
                            fprintf(masterfp, "%s_%i_row,%i,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                    games[g], index, theGs[0], strGame(tipo), agentsStr(a2), agentsStr(a1), toStrng(agentClass(a1)),
                                    val100_0, val1000_0, val50000_0, tval100_0, tval1000_0, tval50000_0,
                                    meanGameScore2[0][g][theGs[0]], meanGameScore2[1][g][theGs[0]], meanGameScore2[2][g][theGs[0]]);
                        }
                        
                        if (a2 != 17) {
                            fprintf(masterfp, "%s_%i_col,%i,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                    games[g], index, theGs[1], strGame(tipo), agentsStr(a1), agentsStr(a2), toStrng(agentClass(a2)),
                                    val100_1, val1000_1, val50000_1, tval100_1, tval1000_1, tval50000_1,
                                    meanGameScore2[0][g][theGs[1]], meanGameScore2[1][g][theGs[1]], meanGameScore2[2][g][theGs[1]]);
                        }
                        
                        fclose(fp);
                    }
                }
            }
        }
    }

    fclose(masterfp);
}


void writeLaTeXTable(int len) {
    char fnombre[1024];
    sprintf(fnombre, "saved144/summary_%i.txt", len);
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    double val;

    int i, j;
    for (i = 0; i < 26; i++)
        fscanf(fp, "%s", dummy);
    
    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "\\multirow{2}{*}{\\sc %s} & ", dummy);
        
        for (j = 0; j < 25; j++) {
            fscanf(fp, "%lf", &val);
            
            fprintf(texfp, "\\multirow{2}{*}{%.3lf} & ", val);
        }
        fscanf(fp, "%lf", &val);
        fprintf(texfp, "\\multirow{2}{*}{\\bf %.3lf}\\\\ \n", val);
        fprintf(texfp, "& & & & & & & & & & & & & & & & & & & & & & & & & & \\\\ \n\n");
    }

    fclose(texfp);
    fclose(fp);
}

void writeLaTeXTable_ranksummary_all() {
    char fnombre[1024];
    sprintf(fnombre, "saved144/RankSummary_all.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    double val;
    int ival1, ival2, ival3;
    
    int i, j;
    for (i = 0; i < 11; i++) {
        fscanf(fp, "%s", dummy);
        printf("<%s>\n", dummy);
    }
    
    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);
        
        for (j = 0; j < 6; j++) {
            fscanf(fp, "%i %i %i", &ival1, &ival2, &ival3);
            if (ival1 < 10)
                fprintf(texfp, "~~%i, ", ival1);
            else
                fprintf(texfp, "%i, ", ival1);
            
            if (ival2 < 10)
                fprintf(texfp, "~~%i, ", ival2);
            else
                fprintf(texfp, "%i, ", ival2);

            if (ival3 < 10)
                fprintf(texfp, "~~%i & ", ival3);
            else
                fprintf(texfp, "%i & ", ival3);
            
            //fprintf(texfp, "%i, %i, %i & ", ival1, ival2, ival3);
        }
        
        fscanf(fp, "%i %lf %i", &ival1, &val, &ival2);
        
        if (ival1 < 10)
            fprintf(texfp, "~~%i -- ", ival1);
        else
            fprintf(texfp, "%i -- ", ival1);

        if (val < 10)
            fprintf(texfp, "~~%.1lf -- ", val);
        else
            fprintf(texfp, "%.1lf -- ", val);

        if (ival2 < 10)
            fprintf(texfp, "~~%i \\\\ \n", ival2);
        else
            fprintf(texfp, "%i \\\\ \n", ival2);
        
        //fprintf(texfp, "%i -- %.1lf -- %i \\\\ \n", ival1, val, ival2);
    
    }

    fclose(texfp);
    fclose(fp);
}

void writeLaTeXTable_ranksummary(int len) {
    char fnombre[1024];
    sprintf(fnombre, "saved144/RankSummary_%i.txt", len);
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    double val, val2, val3;
    int ival;
    
    int i, j;
    for (i = 0; i < 8; i++) {
        fscanf(fp, "%s", dummy);
        printf("<%s>\n", dummy);
    }
    
    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);

        // round-robin ave
        fscanf(fp, "%lf %i", &val, &ival);
        if (ival < 10)
            fprintf(texfp, "%.2lf ~~(%i) & ", val, ival);
        else
            fprintf(texfp, "%.2lf (%i) & ", val, ival);

        // best score
        fscanf(fp, "%lf %i", &val, &ival);
        if (ival < 10) {
            if (val < .1)
                fprintf(texfp, "~~%.0lf\\%% ~~(%i) & ", val * 100, ival);
            else
                fprintf(texfp, "%.0lf\\%% ~~(%i) & ", val * 100, ival);
        }
        else {
            if (val < .1)
                fprintf(texfp, "~~%.0lf\\%% (%i) & ", val * 100, ival);
            else
                fprintf(texfp, "%.lf\\%% (%i) & ", val * 100, ival);
        }

        // worst score
        fscanf(fp, "%lf %i", &val, &ival);
        if (ival < 10) {
            if (val > 0)
                fprintf(texfp, "~%.3lf ~~(%i) & ", val, ival);
            else
                fprintf(texfp, "%.3lf ~~(%i) & ", val, ival);
        }
        else {
            if (val > 0)
                fprintf(texfp, "~%.3lf (%i) & ", val, ival);
            else
                fprintf(texfp, "%.3lf (%i) & ", val, ival);
        }
        
        // replicator dynamic
        fscanf(fp, "%lf %i", &val, &ival);
        if (ival < 10) {
            if (val < .1)
                fprintf(texfp, "~~%.0lf\\%% ~~(%i) & ", val * 100, ival);
            else
                fprintf(texfp, "%.0lf\\%% ~~(%i) & ", val * 100, ival);
        }
        else {
            if (val < .1)
                fprintf(texfp, "~~%.0lf\\%% (%i) & ", val * 100, ival);
            else
                fprintf(texfp, "%.0lf\\%% (%i) & ", val * 100, ival);
        }
        
        // group1 tourney
        fscanf(fp, "%lf %lf %i", &val, &val2, &ival);
        if (ival < 10) {
            if (val < .1) {
                if (val2 < .1)
                    fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, ival);
                else
                    fprintf(texfp, "~~%.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, ival);
            }
            else {
                if (val2 < .1)
                    fprintf(texfp, "%.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, ival);
                else
                    fprintf(texfp, "%.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, ival);
            }
        }
        else {
            if (val < .1) {
                if (val2 < .1)
                    fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, ival);
                else
                    fprintf(texfp, "~~%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, ival);
            }
            else {
                if (val2 < .1)
                    fprintf(texfp, "%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, ival);
                else
                    fprintf(texfp, "%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, ival);
            }
        }
        
        // group2 tourney
        fscanf(fp, "%lf %lf %lf %i", &val, &val2, &val3, &ival);
        if (ival < 10) {
            if (val < .1) {
                if (val2 < .1) {
                    if (val3 < .1)
                        fprintf(texfp, "~~~~%.0lf\\%% / ~~~~%.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~~~%.0lf\\%% / ~~~~%.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
                else {
                    if (val3 < .1)
                        fprintf(texfp, "~~~~%.0lf\\%% / %.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~~~%.0lf\\%% / %.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
            }
            else {
                if (val2 < .1) {
                    if (val3 < .1)
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
                else {
                    if (val3 < .1)
                        fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% / ~~%.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% / %.0lf\\%% ~~(%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
            }
        }
        else {
            if (val < .1) {
                if (val2 < .1) {
                    if (val3 < .1)
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
                else {
                    if (val3 < .1)
                        fprintf(texfp, "~~~~%.0lf\\%% / ~~%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~~~%.0lf\\%% / ~~%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
            }
            else {
                if (val2 < .1) {
                    if (val3 < .1)
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~%.0lf\\%% / ~~~~%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
                else {
                    if (val3 < .1)
                        fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% / ~~%.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                    else
                        fprintf(texfp, "~~%.0lf\\%% / ~~%.0lf\\%% / %.0lf\\%% (%i) & ", val * 100, val2 * 100, val3 * 100, ival);
                }
            }
        }
        
        // RankAve
        fscanf(fp, "%lf", &val);
        fprintf(texfp, "%.2lf \\\\ \n", val);
    }

    fclose(texfp);
    fclose(fp);
}

void writeGroup(int v1, int v2, int v3, FILE *fp) {

    if (v1 < 10)
        fprintf(fp, "~~%i, ", v1);
    else
        fprintf(fp, "%i, ", v1);

    if (v2 < 10)
        fprintf(fp, "~~%i, ", v2);
    else
        fprintf(fp, "%i, ", v2);

    if (v3 < 10)
        fprintf(fp, "~~%i & ", v3);
    else
        fprintf(fp, "%i & ", v3);

}

void writeLaTeXTable_bygame_partner2() {
    char fnombre[1024];
    sprintf(fnombre, "saved144/bygame_partner2.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    int val1, val2, val3;
    double fval;
    
    int i, j;
    for (i = 0; i < 19; i++) {
        fscanf(fp, "%s", dummy);
        printf("<%s>\n", dummy);
    }

    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);

        for (j = 0; j < 10; j++) {
            fscanf(fp, "%i %i %i", &val1, &val2, &val3);
            writeGroup(val1, val2, val3, texfp);
        }

        fscanf(fp, "%i %lf %i", &val1, &fval, &val3);
        
        if (val1 < 10)
            fprintf(texfp, "~~%i -- ", val1);
        else
            fprintf(texfp, "%i -- ", val1);
        
        if (fval < 10)
            fprintf(texfp, "~~%.2lf -- ", fval);
        else
            fprintf(texfp, "%.2lf -- ", fval);
        
        if (val3 < 10)
            fprintf(texfp, "~~%i \\\\ \n", val3);
        else
            fprintf(texfp, "%i \\\\ \n", val3);
    }

    fclose(texfp);
    fclose(fp);
}

void writeLaTeXTable_bygame_partner() {
    char fnombre[1024];
    sprintf(fnombre, "saved144/bygame_partner.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    int val1, val2, val3;
    double fval;
    
    int i, j;
    for (i = 0; i < 12; i++) {
        fscanf(fp, "%s", dummy);
        printf("<%s>\n", dummy);
    }

    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);

        for (j = 0; j < 9; j++) {
            fscanf(fp, "%i %i %i", &val1, &val2, &val3);
            writeGroup(val1, val2, val3, texfp);
        }

        fscanf(fp, "%i %lf %i", &val1, &fval, &val3);
        
        if (val1 < 10)
            fprintf(texfp, "~~%i -- ", val1);
        else
            fprintf(texfp, "%i -- ", val1);
        
        if (fval < 10)
            fprintf(texfp, "~~%.2lf -- ", fval);
        else
            fprintf(texfp, "%.2lf -- ", fval);
        
        if (val3 < 10)
            fprintf(texfp, "~~%i \\\\ \n", val3);
        else
            fprintf(texfp, "%i \\\\ \n", val3);
    }

    fclose(texfp);
    fclose(fp);
}


int main(int argc, char *argv[]) {
    srand(time(NULL));
    
	nameAgents();
    nameGames();

    for (int i = 1; i < 5; i++)
        runGames_self(i);
    
    //masterPerformanceSpreadsheet();
    //masterPerformanceSpreadsheet2();
    //computeAverages();
    //computeAveragesNew();
    //computeAveragesNew2();

    //countGames2();

    //deepQ_PD_results();
    //createDeepQMaster();
    //createDeepQProportions();

    //testGodfather();
    
    //runGames();
    //runGames_rgsg();
    //analyzeGames();
    //analyzeGamesNew();
    //writeLaTeXTable(50000);
    //writeLaTeXTable_ranksummary_all();
    //writeLaTeXTable_ranksummary(50000);
    //writeLaTeXTable_bygame_partner2();
    //analyzeSsimp();
    //getWins(true, 100);
    //runOffs1(true, 50000);
    //runOffs2(true, 50000);
    //reformat();
    
    //evolutionaryTournament(2, 50000);
    //evolutionaryTournament(2, 1000);
    //evolutionaryTournament(2, 100);
    
    //evolutionaryRank(3, 50000);
    
    //createMasterCSV();
    //createFilteredMasterCSV();
    
    //createExpertsLog();
    
    //getGameCharacteristics();
/*
    for (int i = 1; i < 79; i++) {
        listNE(0,i);
        if (uninteresting(0, i))
            printf("*****%i", i);
        printf("\n");
    }
*/
	return 0;
}