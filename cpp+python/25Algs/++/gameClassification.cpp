#include "defs.h"

int ms[144][2][2][2];
char games[5][1024];

int firstrow, firstcol;

int theNBSTable[144][2][2];

void nameGames() {
	strcpy(games[0], "cg_standard");
	strcpy(games[1], "cg_sqrt");
	strcpy(games[2], "cg_sep");
	strcpy(games[3], "cg_pow2");
	strcpy(games[4], "cg_pow3");
}

int RobinsonGoforthClass(int g1, int g2) {
    if ((g1 >= 72) && (g1 < 108))
        return 1;
    
    if (((g1 >= 60) && (g1 < 72)) || ((g1 >= 12) && (g1 < 24)) ||
        ((g2 >= 60) && (g2 < 72)) || ((g2 >= 12) && (g2 < 24))) {
        return 2;
    }
    
    if ((g1 == 52) || (g1 == 53) || (g1 == 58) || (g1 == 59) ||
        (g1 == 28) || (g1 == 29) || (g1 == 34) || (g1 == 35) ||
        (g1 == 134) || (g1 == 135) || (g1 == 140) || (g1 == 141)) {
        return 3;
    }
    
    if (((g1 >= 36) && (g1 < 42)) ||
        (g1 == 7) || (g1 == 25) || (g1 == 31) || (g1 == 1) ||
        ((g2 >= 36) && (g2 < 42)) ||
        (g2 == 7) || (g2 == 25) || (g2 == 31) || (g2 == 1)) {
        return 4;
    }
    
    if ((g1 == 0) || (g1 == 4) || (g1 == 5) || (g1 == 42) || (g1 == 48) || (g1 == 54) || (g1 == 46) || (g1 == 47) ||
        (g2 == 0) || (g2 == 4) || (g2 == 5) || (g2 == 42) || (g2 == 48) || (g2 == 54) || (g2 == 46) || (g2 == 47)) {
        return 5;
    }
    
    if (((g1 >= 55) && (g1 < 58)) || ((g1 >= 49) && (g1 < 52)) || ((g1 >= 43) && (g1 < 46)) ||
        ((g2 >= 55) && (g2 < 58)) || ((g2 >= 49) && (g2 < 52)) || ((g2 >= 43) && (g2 < 46))) {
        return 6;
    }

    printf("Error: Didn't find RobinsonGoforth Classification: %i, %i\n", g1, g2);
    exit(1);
    return -1;
}

int RapoportClass(int g1, int g2) {
    if ((g1 >= 72) && (g1 < 108))
        return 1; // no conflict

    if (((g1 >= 55) && (g1 < 58)) || ((g1 >= 49) && (g1 < 52)) || ((g1 >= 43) && (g1 < 46)) ||
        ((g2 >= 55) && (g2 < 58)) || ((g2 >= 49) && (g2 < 52)) || ((g2 >= 43) && (g2 < 46))) {
        return 2; // no equilibrium
    }
    
    if ((g1 == 14) || (g1 == 15) || (g1 == 20) || (g1 == 21) || (g1 == 7))
        return 3; // 2 equilibrium, non-eq solutions
    
    if ((g1 == 8) || (g1 == 9) || (g1 == 13) || (g1 == 19))
        return 4; // 2 equilibrium, eq solutions
    
    if (g1 == 0)
        return 5; // strongly stable deficient
    
    if ((g1 == 4) || (g1 == 5) || (g1 == 30) || (g1 == 34) || (g1 == 35) || (g1 == 24) || (g1 == 29) || (g1 == 28))
        return 6; // strongly stable
    
    if ((g1 == 40) || (g1 == 41) || (g1 == 70) || (g1 == 71) || (g1 == 64) || (g1 == 65) ||
        (g1 == 31) || (g1 == 32) || (g1 == 33) || (g1 == 25) || (g1 == 26) || (g1 == 27) ||
        (g2 == 40) || (g2 == 41) || (g2 == 70) || (g2 == 71) || (g2 == 64) || (g2 == 65) ||
        (g2 == 31) || (g2 == 32) || (g2 == 33) || (g2 == 25) || (g2 == 26) || (g2 == 27)) {
        return 7; // stable
    }
    
    if ((g1 == 63) || (g1 == 69) || (g1 == 39) ||
        (g1 == 42) || (g1 == 47) || (g1 == 46) || (g1 == 48) || (g1 == 53) || (g1 == 52) ||
        (g2 == 63) || (g2 == 69) || (g2 == 39) ||
        (g2 == 42) || (g2 == 47) || (g2 == 46) || (g2 == 48) || (g2 == 53) || (g2 == 52)) {
        return 8; // force-vulnerable
    }
    
    if ((g1 == 36) || (g1 == 66) || (g1 == 60) ||
        (g1 == 1) || (g1 == 2) || (g1 == 3) ||
        (g2 == 36) || (g2 == 66) || (g2 == 60) ||
        (g2 == 1) || (g2 == 2) || (g2 == 3)) {
        return 9; // threat-vulnerable
    }

    if ((g1 == 54) || (g1 == 58) || (g1 == 59) ||
        (g1 == 37) || (g1 == 38) || (g1 == 61) || (g1 == 62) || (g1 == 67) || (g1 == 68) ||
        (g2 == 54) || (g2 == 58) || (g2 == 59) ||
        (g2 == 37) || (g2 == 38) || (g2 == 61) || (g2 == 62) || (g2 == 67) || (g2 == 68)) {
        return 10; // Unstable
    }

    printf("Error: Didn't find Rapoport Classification\n");
    exit(1);
    return -1;

}

int BramsClass(int g1, int g2) {
    if ((g1 >= 72) && (g1 < 108))
        return 1; // no conflict

    if (g1 < 36)
        return 2; // Not cyclic
    
    if ((g1 == 36) || (g1 == 41) || (g1 == 40) || (g1 == 66) || (g1 == 71) || (g1 == 70) ||
        (g1 == 60) || (g1 == 64) || (g1 == 65) ||
        (g2 == 36) || (g2 == 41) || (g2 == 40) || (g1 == 66) || (g1 == 71) || (g1 == 70) ||
        (g2 == 60) || (g2 == 64) || (g2 == 65)) {
        return 3; // Weakly cyclic
    }

    if (((g1 >= 55) && (g1 < 58)) || ((g1 >= 49) && (g1 < 52)) || ((g1 >= 43) && (g1 < 46)) ||
        ((g2 >= 55) && (g2 < 58)) || ((g2 >= 49) && (g2 < 52)) || ((g2 >= 43) && (g2 < 46))) {
        return 5; // Strongly cyclic
    }

    else {
    
        return 4; // Moderately cyclic
    }
}

void printGameDouble(double _M[2][2][2]) {
    int i, j;
    
    int _A[2] = {2, 2};
    
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

void printGame(int m[2][2][2]) {
    int i, j;
    
    printf("   |   0   |   1   |\n");
    printf("--------------------\n");
    for (i = 0; i < 2; i++) {
        printf(" 0 |");
        for (j = 0; j < 2; j++) {
            printf(" %i , %i |", m[0][i][j], m[1][i][j]);
        }
        printf("\n--------------------\n");
    }
}

void fprintGame(FILE *fp, int m[2][2][2]) {
    int i, j;
    
    //printf("aqui\n");
    
    fprintf(fp, "\n");
    fprintf(fp, "   |   0   |   1   |\n");
    fprintf(fp, "--------------------\n");
    for (i = 0; i < 2; i++) {
        fprintf(fp, " 0 |");
        for (j = 0; j < 2; j++) {
            fprintf(fp, " %i , %i |", m[0][i][j], m[1][i][j]);
        }
        fprintf(fp, "\n--------------------\n");
    }
}

void swap(int m[2][2][2], int player, int val1, int val2) {
    int fi, fj;
    int si, sj;
    int i, j;
    
    //printf("swap %i and %i for player %i\n", val1, val2, player);
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (m[player][i][j] == val1) {
                fi = i;
                fj = j;
            }
            if (m[player][i][j] == val2) {
                si = i;
                sj = j;
            }
        }
    }

    m[player][fi][fj] = val2;
    m[player][si][sj] = val1;
}

void generateGame(int index, int layer, int row, int col) {
    int i, j, k;
    
    // load the base matrix
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                ms[index][i][j][k] = ms[0][i][j][k];
            }
        }
    }

    // transform to the right layer
    if (layer == 2) {
        swap(ms[index], 0, 1, 2);
        swap(ms[index], 0, 3, 4);
    }
    else if (layer == 3) {
        swap(ms[index], 0, 1, 2);
        swap(ms[index], 0, 3, 4);
        swap(ms[index], 1, 1, 2);
        swap(ms[index], 1, 3, 4);
    }
    else if (layer == 4) {
        swap(ms[index], 1, 1, 2);
        swap(ms[index], 1, 3, 4);
    }


    // transform to the right row (row-player swaps)
    if (row == 2)
        swap(ms[index], 0, 1, 2);
    else if (row == 3) {
        swap(ms[index], 0, 2, 3);
        swap(ms[index], 0, 1, 3);
    }
    else if (row == 4) {
        swap(ms[index], 0, 1, 3);
    }
    else if (row == 5) {
        swap(ms[index], 0, 2, 3);
        swap(ms[index], 0, 1, 2);
    }
    else if (row == 6) {
        swap(ms[index], 0, 2, 3);
    }
    
    
    // transform to the right col (col-player swaps)
    if (col == 2)
        swap(ms[index], 1, 1, 2);
    else if (col == 3) {
        swap(ms[index], 1, 2, 3);
        swap(ms[index], 1, 1, 3);
    }
    else if (col == 4) {
        swap(ms[index], 1, 1, 3);
    }
    else if (col == 5) {
        swap(ms[index], 1, 2, 3);
        swap(ms[index], 1, 1, 2);
    }
    else if (col == 6) {
        swap(ms[index], 1, 2, 3);
    }
}

void generateGames() {
    int m[2][2][2];
    
    // start with the pd
    ms[0][0][0][0] = 1; ms[0][1][0][0] = 4; ms[0][0][0][1] = 3; ms[0][1][0][1] = 3;
    ms[0][0][1][0] = 2; ms[0][1][1][0] = 2; ms[0][0][1][1] = 4; ms[0][1][1][1] = 1;
    
    int row, col, layer;
    int count = 0;
    for (layer = 1; layer < 5; layer++) {
        for (row = 1; row < 7; row++) {
            for (col = 1; col < 7; col++) {
                generateGame(count, layer, row, col);
                
                count ++;
            }
        }
    }
/*
    printf("%i games\n", count);
    layer = 4;
    col = 1;
    for (row = 1; row < 7; row++) {
        count = 36 * (layer - 1) + 6 * (row - 1) + (col - 1);
        printf("\nGame %i%i%i (%i)\n", layer, row, col, count);
        printGame(ms[count]);
    }
*/
}

void readGame(double M[2][2][2], int g, int index) {
    FILE *fp;
    char fnombre[1024];
    
  	strcpy(games[0], "cg_standard");
	strcpy(games[1], "cg_sqrt");
	strcpy(games[2], "cg_sep");
	strcpy(games[3], "cg_pow2");
	strcpy(games[4], "cg_pow3");

    
    sprintf(fnombre, "../games/%s_%i.txt", games[g], index);
    fp = fopen(fnombre, "r");
    
    int d1, d2, d3;
    fscanf(fp, "%i %i %i", &d1, &d2, &d3);

    int i, j;
    for (j = 0; j < d3; j++) {
        for (i = 0; i < d2; i++) {
            fscanf(fp, "%lf %lf", &(M[0][i][j]), &(M[1][i][j]));
        }
    }
    
    fclose(fp);
}

void makeOrdinal(int m[2][2][2], double M[2][2][2]) {
    int i, j, k;
    
    double vals[2], tmp;
    int count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if ((M[0][i][j] > 0.01) && (M[0][i][j] < 0.99)) {
                if (count == 0) {
                    vals[0] = M[0][i][j];
                    count ++;
                }
                else {
                    if (vals[0] < M[0][i][j])
                        vals[1] = M[0][i][j];
                    else {
                        tmp = vals[0];
                        vals[0] = M[0][i][j];
                        vals[1] = tmp;
                    }
                    
                    count++;
                    break;
                }
            }
        }
        if (count > 1)
            break;
    }
    
    //printf("vals = %lf, %lf\n", vals[0], vals[1]);
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                if (M[i][j][k] < 0.05)
                    m[i][j][k] = 1;
                else if (M[i][j][k] == vals[0])
                    m[i][j][k] = 2;
                else if (M[i][j][k] == vals[1])
                    m[i][j][k] = 3;
                else
                    m[i][j][k] = 4;
            }
        }
    }
}

void orientRU(int m[2][2][2], int mp[2][2][2]) {
    firstrow = 0;
    firstcol = 0;
    int i, j;
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (m[0][i][j] == 4)
                firstcol = j;
            if (m[1][i][j] == 4)
                firstrow = i;
        }
    }

    mp[0][0][0] = m[0][firstrow][1-firstcol];   mp[0][0][1] = m[0][firstrow][firstcol];
    mp[1][0][0] = m[1][firstrow][1-firstcol];   mp[1][0][1] = m[1][firstrow][firstcol];

    mp[0][1][0] = m[0][1-firstrow][1-firstcol];   mp[0][1][1] = m[0][1-firstrow][firstcol];
    mp[1][1][0] = m[1][1-firstrow][1-firstcol];   mp[1][1][1] = m[1][1-firstrow][firstcol];

}

void getGameCode(char gameCode[5], int index) {
    int layer, row, col;
    
    layer = index / 36;
    index -= layer * 36;
    row = index / 6;
    col = (index % 6);
    //index -= col;
    //row = index / 6;
    //layer = 1;
    
    sprintf(gameCode, "%i%i%i", layer+1, row+1, col+1);

}

int findGame(int m[2][2][2]) {
    int i, j, k, g;
    char gameCode[5];
    
    for (g = 0; g < 144; g++) {
        bool wrong = false;
        
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                for (k = 0; k < 2; k++) {
                    if (m[i][j][k] != ms[g][i][j][k]) {
                        wrong = true;
                        break;
                    }
                }
                if (wrong)
                    break;
            }
            if (wrong)
                break;
        }
        
        if (!wrong) {
            //getGameCode(gameCode, g);
            //printf("this is game %i (%s)\n", g, gameCode);
            break;
        }
    }
    
    return g;
}

void switchRowColPlayers(int mp[2][2][2], int mpp[2][2][2]) {
    int i, j, k;
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                mpp[i][j][k] = mp[1-i][k][j];
            }
        }
    }
}

void getPoint(double p1[2], double p2[2], double pt[2], double t) {
    pt[0] = (1-t) * p1[0] + t * p2[0];
    pt[1] = (1-t) * p1[1] + t * p2[1];
}

bool computeTheNBS(int g, int index, double Payoffs[2][2][2], int nbsActions[2][2], double mv[2], double nbsVals[2]) {
    //double nbs1, nbs2;
    double pi[2];
    char nombre[1024];

    sprintf(nombre, "../results/maximinVals/%s_%i", games[g], index);
    FILE *fp = fopen(nombre, "r");
    fscanf(fp, "%lf %lf %lf", &(mv[0]), &(pi[0]), &(pi[1]));
    fscanf(fp, "%lf %lf %lf", &(mv[1]), &(pi[0]), &(pi[1]));
    fclose(fp);
    printf("maxmin: %.3lf, %.3lf\n", mv[0], mv[1]);
    sprintf(nombre, "../results/NBS/%s_%i", games[g], index);
    printf("%s\n", nombre);
    fp = fopen(nombre, "r");
    fscanf(fp, "%lf %lf", &(nbsVals[0]), &(nbsVals[1]));
    fclose(fp);
    printf("NBS: %.3lf, %.3lf\n", nbsVals[0], nbsVals[1]);

//    sprintf(nombre, "../games/%s_%i.txt", games[g], index);
//    fp = fopen(nombre, "r");
    
    double p1[2], p2[2], pt[2];
    double thePt[2], maxprod = -1.0, prod, x, y;
    bool alt = false;
    int i, j, m, n;
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
                            printf("0 maxprod = %lf\n", maxprod);
                            thePt[0] = pt[0];
                            thePt[1] = pt[1];
                            alt = false;
                            
                            nbsActions[0][0] = nbsActions[1][0] = i;
                            nbsActions[0][1] = nbsActions[1][1] = j;
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
                                printf("1 maxprod = %lf\n", maxprod);
                                thePt[0] = pt[0];
                                thePt[1] = pt[1];
                                alt = true;
                                nbsActions[0][0] = i;
                                nbsActions[0][1] = j;
                                nbsActions[1][0] = m;
                                nbsActions[1][1] = n;
                            }
                        }
                        
                        getPoint(p1, p2, pt, 1.0);
                        x = pt[0] - mv[0];
                        y = pt[1] - mv[1];
                        if ((x >= 0) && (y >= 0)) {
                            prod = x * y;
                            if (prod > maxprod) {
                                maxprod = prod;
                                printf("2 maxprod = %lf; %i, %i: %lf, %lf\n", maxprod, m, n, p2[0], p2[1]);
                                thePt[0] = pt[0];
                                thePt[1] = pt[1];
                                alt = false;

                                nbsActions[0][0] = nbsActions[1][0] = m;
                                nbsActions[0][1] = nbsActions[1][1] = n;
                            }
                        }
                    }
                }
            }
        }
    }

    //if (!alt)
    //    printf("pure NBS: %.2lf, %.2lf (%i, %i)\n", thePt[0], thePt[1], nbsActions[0][0], nbsActions[0][1]);
    //else
    //    printf("Altr NBS: %.2lf, %.2lf (%i, %i -- %i, %i)\n", thePt[0], thePt[1], nbsActions[0][0], nbsActions[0][1], nbsActions[1][0], nbsActions[1][1]);
    
    if (maxprod > 0.0)
        return true;
    else
        return false;
}

int listNE(double Payoffs[2][2][2], int nelist[4][2]) {
    int i, j;
   
    int num = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if ((Payoffs[0][i][j] >= Payoffs[0][1-i][j]) && (Payoffs[1][i][j] >= Payoffs[1][i][1-j])) {
                //printf("NE: (%i, %i)\n", i, j);
                nelist[num][0] = i;
                nelist[num][1] = j;
                num++;
            }
        }
    }
    
    return num;
}

bool mismo(double v1, double v2) {
    if (fabs(v1 - v2) < 0.0001)
        return true;
    
    return false;
}

bool isSame(int num, int nelist[4][2], int nbsActions[2][2], double mv[2], double nbs[2], double mne[2]) {
    if (mismo(mv[0],nbs[0]) && mismo(mv[0],mne[0]) && mismo(mv[1],nbs[1]) && mismo(mv[1],mne[1]))
        return true;

    if ((nbsActions[0][0] != nbsActions[1][0]) || (nbsActions[0][1] != nbsActions[1][1]))
        return false;

    int i;
    for (i = 0; i < num; i++) {
        if ((nelist[i][0] == nbsActions[0][0]) && (nelist[i][1] == nbsActions[0][1]))
            return true;
    }
    
    return false;
}

void indifference(double ne[2], double Payoffs[2][2][2]) {
    // first for the row
    double a = Payoffs[0][0][0];
    double b = Payoffs[0][0][1];
    double c = Payoffs[0][1][0];
    double d = Payoffs[0][1][1];
    
    double q = (d-b) / (a-b-c+d);
    
    double e = Payoffs[1][0][0];
    double f = Payoffs[1][0][1];
    double g = Payoffs[1][1][0];
    double h = Payoffs[1][1][1];
    
    double p = (h-g) / (e-g-f+h);
    
    ne[0] = (p * q * a) + (p * (1-q) * b) + ((1-p) * q * c) + ((1-p) * (1 - q) * d);
    ne[1] = (p * q * e) + (p * (1-q) * f) + ((1-p) * q * g) + ((1-p) * (1 - q) * h);
}

bool hasDeficientNE(int num, int nelist[4][2], int nbsActions[2][2], double Payoffs[2][2][2]) {
    double nbs1 = (Payoffs[0][nbsActions[0][0]][nbsActions[0][1]] + Payoffs[0][nbsActions[1][0]][nbsActions[1][1]]) / 2.0;
    double nbs2 = (Payoffs[1][nbsActions[0][0]][nbsActions[0][1]] + Payoffs[1][nbsActions[1][0]][nbsActions[1][1]]) / 2.0;

    if (num == 0) {
        double ne[2];
        indifference(ne, Payoffs);
        //printf("ne: %lf, %lf vs nbs: %lf, %lf\n", ne[0], ne[1], nbs1, nbs2);
        // determine if the mixed NE is dominated
        if ((nbs1 >= ne[0]) && (nbs2 >= ne[1]))
            return true;
    }

    //printf("%lf, %lf\n", nbs1, nbs2);
    
    int i;
    for (i = 0; i < num; i++) {
        if ((Payoffs[0][nelist[i][0]][nelist[i][1]] < nbs1) && (Payoffs[1][nelist[i][0]][nelist[i][1]] < nbs2)) {
            //printf("%i: %lf, %lf\n", i, Payoffs[0][nelist[i][0]][nelist[i][1]], Payoffs[1][nelist[i][0]][nelist[i][1]]);
            return true;
        }
    }

    return false;
}

void computeMixedNE(double Payoffs[2][2][2], double mne[2]) {

    // see if there is an indifference point
    double a0 = Payoffs[0][0][0];
    double b0 = Payoffs[0][0][1];
    double c0 = Payoffs[0][1][0];
    double d0 = Payoffs[0][1][1];

    double a1 = Payoffs[1][0][0];
    double b1 = Payoffs[1][0][1];
    double c1 = Payoffs[1][1][0];
    double d1 = Payoffs[1][1][1];
    
    double q = (d0 - b0) / (a0 - b0 - c0 + d0);
    double p = (d1 - c1) / (a1 - c1 - b1 + d1);

    mne[0] = a0*p*q + b0*p*(1-q) + c0*(1-p)*q + d0*(1-p)*(1-q);
    mne[1] = a1*p*q + b1*p*(1-q) + c1*(1-p)*q + d1*(1-p)*(1-q);
    
    printf("p = %lf; q = %lf\n", p, q);
    printf("mne = %lf, %lf\n", mne[0], mne[1]);
    //exit(1);
}

bool canBully(int num, int nelist[4][2], int nbsActions[2][2], double Payoffs[2][2][2], double nbsVals[2], double mne[2]) {
    int i;
    
    double nbs1 = nbsVals[0];//(Payoffs[0][nbsActions[0][0]][nbsActions[0][1]] + Payoffs[0][nbsActions[1][0]][nbsActions[1][1]]) / 2.0;
    double nbs2 = nbsVals[1];//(Payoffs[1][nbsActions[0][0]][nbsActions[0][1]] + Payoffs[1][nbsActions[1][0]][nbsActions[1][1]]) / 2.0;
    
    mne[0] = mne[1] = -1;
    
    for (i = 0; i < num; i++) {
        if ((Payoffs[0][nelist[i][0]][nelist[i][1]] > (nbs1+0.01)) || (Payoffs[1][nelist[i][0]][nelist[i][1]] > (nbs2+0.01)))
            return true;
    }
    
    if (num == 0) {
        //double mne[2];
        computeMixedNE(Payoffs, mne);
        
        if ((mne[0] > (nbs1+0.01)) || (mne[1] > (nbs2+0.01)))
            return true;
    }

    return false;
}

void enterValue(int g, int code, int theTable[4][6][6]) {
    int layer = g / 36;
    g -= layer * 36;
    int row = g / 6;
    int col = g % 6;

    theTable[layer][row][col] = code;
}

void printTable(int theTable[4][6][6]) {
    int l, r, c;
    
    int rowOrder[6] = {1, 2, 3, 4, 5, 0};
    int colOrder[12] = {0, 5, 4, 3, 2, 1, 0, 5, 4, 3, 2, 1};
    int layer1[12] = {3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0};
    int layer2[12] = {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1};
    
    for (r = 0; r < 6; r++) {
        for (c = 0; c < 12; c++) {
            printf("%i ", theTable[layer1[c]][rowOrder[r]][colOrder[c]]);
            if (c == 5)
                printf(" | ");
        }
        printf("\n");
    }
    
    printf("__________________________\n\n");
    
    for (r = 0; r < 6; r++) {
        for (c = 0; c < 12; c++) {
            printf("%i ", theTable[layer2[c]][rowOrder[r]][colOrder[c]]);
            if (c == 5)
                printf(" | ");
        }
        printf("\n");
    }
    
}

void identifyGames() {
    int i;
    double Payoffs[2][2][2];
    int m[2][2][2], mp[2][2][2], mpp[2][2][2], mppp[2][2][2];
    int g1, g2;
    char gameCode1[5], gameCode2[5];
    FILE *fp;
    char fnombre[1024];
    int num, nelist[4][2], nbsActions[2][2];
    double mv[2], nbsVals[2], mne[2];
    int gameVal;
    int f1, c1;
    
    int theTable[4][6][6];
    
    int typeCount[5];
    for (i = 0; i < 5; i++)
        typeCount[i] = 0;
    
    for (i = 0; i < 144; i++) {
        theNBSTable[i][0][0] = theNBSTable[i][0][1] = theNBSTable[i][1][0] = theNBSTable[i][1][1] = -1;
    }
    
    int nbsAlt = 0;
    int theG = 4;
    for (i = 0; i < 78; i++) {
    //for (i = 10; i < 11; i++) {
        printf("Game: %i\n", i);
    
        readGame(Payoffs, theG, i+1);
        makeOrdinal(m, Payoffs);
        
        printGameDouble(Payoffs);
        printf("Ordinal Payoffs:\n");
        printGame(m);

        printf("\nOriented-RU\n");
        orientRU(m, mp);
        printGame(mp);
        
        f1 = firstrow;
        c1 = firstcol;
        //printf("firstrow = %i\n", firstrow);
        //printf("firstcol = %i\n", firstcol);
        
        switchRowColPlayers(mp, mpp);
        orientRU(mpp, mppp);
        //printf("\nSymmetric Game\n");
        //printGame(mppp);
        //printf("firstrow = %i\n", firstrow);
        //printf("firstcol = %i\n", firstcol);
        
        g1 = findGame(mp);
        g2 = findGame(mppp);
        printf("Equivalent: %i, %i\n", g1, g2);
        
        //printf("\nData for game %i:\n", i);
        sprintf(fnombre, "../gameClass/%s_%i.txt", games[theG], i+1);
        fp = fopen(fnombre, "w");
        
        if (g1 == g2) {
            getGameCode(gameCode1, g1);
            fprintf(fp, "Game %i %s\n", g1, gameCode1);
        }
        else {
            getGameCode(gameCode1, g1);
            getGameCode(gameCode2, g2);
            fprintf(fp, "Games %i %s %i %s\n", g1, gameCode1, g2, gameCode2);
        }
        fprintf(fp, "RobinsonGoforth: %i\n", RobinsonGoforthClass(g1, g2));
        fprintf(fp, "Rapoport: %i\n", RapoportClass(g1, g2));
        fprintf(fp, "Brams: %i\n", BramsClass(g1, g2));

        //printf("one-shot (pure) NE for Game %i\n", i+1);
        num = listNE(Payoffs, nelist);
        
        //printf("\nThe NBS:\n");
        computeTheNBS(theG, i+1, Payoffs, nbsActions, mv, nbsVals);
        
        if (f1 == 1) {
            theNBSTable[g1][0][0] = 1-nbsActions[0][0];
            theNBSTable[g1][1][0] = 1-nbsActions[1][0];
        }
        else {
            theNBSTable[g1][0][0] = nbsActions[0][0];
            theNBSTable[g1][1][0] = nbsActions[1][0];
        }
        if (c1 == 0) {
            theNBSTable[g1][0][1] = 1-nbsActions[0][1];
            theNBSTable[g1][1][1] = 1-nbsActions[1][1];
        }
        else {
            theNBSTable[g1][0][1] = nbsActions[0][1];
            theNBSTable[g1][1][1] = nbsActions[1][1];
        }
/*
        if ((g1 == 40) || (g1 == 132)) {
            //printf("row = %i; col = %i\n", f1, c1);
            //printf("%i, %i <-> %i, %i\n", nbsActions[0][0], nbsActions[0][1], nbsActions[1][0], nbsActions[1][1]);
            //printf("%i, %i <-> %i, %i\n", theNBSTable[g1][0][0], theNBSTable[g1][0][1], theNBSTable[g1][1][0], theNBSTable[g1][1][1]);
            canBully(num, nelist, nbsActions, Payoffs, nbsVals, mne);
            printf("maximin = %lf, %lf\n", mv[0], mv[1]);
            printf("NBS = %lf, %lf\n", nbsVals[0], nbsVals[1]);
            printf("mne = %lf, %lf\n", mne[0], mne[1]);
            printf("isSame = %i\n", (int)(isSame(num, nelist, nbsActions, mv, nbsVals, mne)));
            exit(1);
        }
*/
        if ((nbsActions[0][0] != nbsActions[1][0]) || (nbsActions[0][1] != nbsActions[1][1])) {
            //printf("Game = %i, %i\n", g1, g2);
            if (g1 == g2)
                nbsAlt++;
            else
                nbsAlt+=2;
        }

        if (!canBully(num, nelist, nbsActions, Payoffs, nbsVals, mne) && isSame(num, nelist, nbsActions, mv, nbsVals, mne))
            gameVal = 0;
        else if (canBully(num, nelist, nbsActions, Payoffs, nbsVals, mne)) {
            //if ((nbsActions[0][0] != nbsActions[1][0]) || (nbsActions[0][1] != nbsActions[1][1]))
            //    gameVal = 3;
            //else
                gameVal = 1;
        }
        else {
            //if ((nbsActions[0][0] != nbsActions[1][0]) || (nbsActions[0][1] != nbsActions[1][1]))
            //    gameVal = 4;
            //else
                gameVal = 2;
        }
        

        fprintf(fp, "Crandall: %i\n", gameVal);

        //gameVal = (int)(isSame(num, nelist, nbsActions)) * 4 +
        //          (int)(canBully(num, nelist, nbsActions, Payoffs)) * 2 +
        //          (int)(hasDeficientNE(num, nelist, nbsActions, Payoffs));
        
        if (g1 == g2)
            typeCount[gameVal] ++;
        else
            typeCount[gameVal] += 2;
/*
        if (gameVal == 6) {
            printf("Game %i (%i)\n", i, g1);
            printGame(mp);
            printf("\n");
        }
*/
        enterValue(g1, gameVal, theTable);
        if (g1 != g2)
            enterValue(g2, gameVal, theTable);

        fprintGame(fp, mp);
        fprintf(fp, "firstrow = %i\n", f1);//irstrow);
        fprintf(fp, "firstcol = %i\n", c1);//firstcol);

        fclose(fp);
    }
    
    printf("numAlt = %i\n", nbsAlt);
    
    printTable(theTable);
    
    for (i = 0; i < 5; i++) {
        printf("%i: %i\n", i, typeCount[i]);
    }
    
    FILE *nbsFP = fopen("NBSlist.txt", "w");
    
    for (i = 0; i < 144; i++) {
        fprintf(nbsFP, "%i: ", i);
        
        if (theNBSTable[i][0][0] != -1) {
            if ((theNBSTable[i][0][0] != theNBSTable[i][1][0]) || (theNBSTable[i][0][1] != theNBSTable[i][1][1])) {
                fprintf(nbsFP, "(%i, %i) <-> (%i, %i)\n", theNBSTable[i][0][0], theNBSTable[i][0][1], theNBSTable[i][1][0], theNBSTable[i][1][1]);
            }
            else {
                fprintf(nbsFP, "(%i, %i)\n", theNBSTable[i][0][0], theNBSTable[i][0][1]);
            }
        }
        else {
            fprintf(nbsFP, "\n");
        }
    
    }
    
    fclose(fp);
}

int main(int argc, char *argv[]) {
    nameGames();
    generateGames();

    // identify the games
    identifyGames();

    return 0;
}