#include "defs.h"

#define cSTATIC     0
#define cRL         1
#define cEXPERT     2
#define cOTHER      3

char agents[30][1024];
int numAgents = 25;
char games[5][1024];
int numGames = 5;

double scores100[5][144][26][25];
double scores1000[5][144][26][25];
double scores50000[5][144][26][25];
double theNBSs[5][144];
int countScores[5][144][25][25];
int gameMap[78][2], reverseGameMap[144][3];
int crandallFamily[5][144];
double averages[3][25][25];
double gameAverages[3][5][144], gameStdev[3][5][144];
double partnerAverages[3][25], partnerStdev[3][25];

extern void Littman(Cell pago[NumDecisionPointsMax][NumDecisionPointsMax], int c[2], int a1[2], int a2[2], int _numDPA);

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
    strcpy(agents[25], "S_simp");
    //strcpy(agents[25], "eeew++");
}

void nameGames() {
	strcpy(games[0], "cg_standard");
	strcpy(games[1], "cg_sqrt");
	strcpy(games[2], "cg_sep");
	strcpy(games[3], "cg_pow2");
	strcpy(games[4], "cg_pow3");
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
    sprintf(fnombre, "saved144new/bygame_partner2.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    int val1, val2, val3;
    double fval;
    
    int i, j;
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
            fprintf(texfp, "~~%.1lf -- ", fval);
        else
            fprintf(texfp, "%.1lf -- ", fval);
        
        if (val3 < 10)
            fprintf(texfp, "~~%i \\\\ \n", val3);
        else
            fprintf(texfp, "%i \\\\ \n", val3);
    }

    fclose(texfp);
    fclose(fp);
}

void writeLaTeXTable_byGameFamily() {
    char fnombre[1024];
    sprintf(fnombre, "saved144new/byGameFamilies.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    int val1, val2, val3;
    double fval;
    
    int i, j;
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
            fprintf(texfp, "~~%.1lf -- ", fval);
        else
            fprintf(texfp, "%.1lf -- ", fval);
        
        if (val3 < 10)
            fprintf(texfp, "~~%i \\\\ \n", val3);
        else
            fprintf(texfp, "%i \\\\ \n", val3);
    }

    fclose(texfp);
    fclose(fp);
}

void writeLaTeXTable_byPartnerFamily() {
    char fnombre[1024];
    sprintf(fnombre, "saved144new/byPartnerFamily.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    int val1, val2, val3;
    double fval;
    
    int i, j;
    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);

        for (j = 0; j < 4; j++) {
            fscanf(fp, "%i %i %i", &val1, &val2, &val3);
            writeGroup(val1, val2, val3, texfp);
        }

        fscanf(fp, "%i %lf %i", &val1, &fval, &val3);
        
        if (val1 < 10)
            fprintf(texfp, "~~%i -- ", val1);
        else
            fprintf(texfp, "%i -- ", val1);
        
        if (fval < 10)
            fprintf(texfp, "~~%.1lf -- ", fval);
        else
            fprintf(texfp, "%.1lf -- ", fval);
        
        if (val3 < 10)
            fprintf(texfp, "~~%i \\\\ \n", val3);
        else
            fprintf(texfp, "%i \\\\ \n", val3);
    }

    fclose(texfp);
    fclose(fp);
}


void writeLaTeXTable_ranksummary_all() {
    char fnombre[1024];
    sprintf(fnombre, "saved144new/RankSummary_all.txt");
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    double val;
    int ival1, ival2, ival3;
    
    int i, j;
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
    sprintf(fnombre, "saved144new/RankSummary_%i.txt", len);
    FILE *fp = fopen(fnombre, "r");
    char dummy[1024];
    double val, val2, val3;
    int ival;
    
    int i, j;
    
    FILE *texfp = fopen("texdump.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fscanf(fp, "%s", dummy);
        fprintf(texfp, "{\\sc %s} & ", dummy);

        // round-robin ave
        fscanf(fp, "%lf %i", &val, &ival);
        if (ival < 10)
            fprintf(texfp, "%.3lf ~~(%i) & ", val, ival);
        else
            fprintf(texfp, "%.3lf (%i) & ", val, ival);

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

void createGameMap() {
    int i, g;
    FILE *fp;
    char fnombre[1024], dummy[1024];
    int v1, v2, v3, v4;
    
    for (g = 0; g < 5; g++) {
        for (i = 0; i < 78; i++) {
            sprintf(fnombre, "../gameClass/%s_%i.txt", games[g], i+1);
            fp = fopen(fnombre, "r");
            
            fscanf(fp, "%s", dummy);
            
            if (!strcmp(dummy, "Games")) {
                fscanf(fp, "%i %i %i %i", &v1, &v2, &v3, &v4);
                gameMap[i][0] = v1;
                gameMap[i][1] = v3;
                
                fscanf(fp, "%s %i", dummy, &v2);
                
                if (g == 0) {
                reverseGameMap[v1][0] = i+1;
                reverseGameMap[v1][1] = 0;
                reverseGameMap[v1][2] = v2-1;
                reverseGameMap[v3][0] = i+1;
                reverseGameMap[v3][1] = 1;
                reverseGameMap[v3][2] = v2-1;
                }

                fscanf(fp, "%s %i", dummy, &v2);
                fscanf(fp, "%s %i", dummy, &v2);
                fscanf(fp, "%s %i", dummy, &v2);

                crandallFamily[g][v1] = v2;
                crandallFamily[g][v3] = v2;
            }
            
            else {
                fscanf(fp, "%i %i", &v1, &v2);
                gameMap[i][0] = gameMap[i][1] = v1;
                
                fscanf(fp, "%s %i", dummy, &v2);
                
                if (g == 0) {
                    reverseGameMap[v1][0] = i+1;
                    reverseGameMap[v1][1] = 0;
                    reverseGameMap[v1][2] = v2-1;
                }

                fscanf(fp, "%s %i", dummy, &v2);
                fscanf(fp, "%s %i", dummy, &v2);
                fscanf(fp, "%s %i", dummy, &v2);

                crandallFamily[g][v1] = v2;
            }
            
            //printf("%i: %i, %i\n", i+1, gameMap[i][0], gameMap[i][1]);
            
            fclose(fp);
        }
    }
    
    //printf("\nReverse Game Map:\n");
    //for (i = 0; i < 144; i++) {
    //    printf("%i: %i %i %i\n", i, reverseGameMap[i][0], reverseGameMap[i][1], reverseGameMap[i][2]);
    //}
}

void readScores() {
    int g, index, i, j;
    char fnombre[1024], dummy[1024];
    FILE *fp;
    double val;
    
    for (g = 0; g < 5; g++) {
        printf("reading %s\n", games[g]);
        for (index = 0; index < 144; index++) {
            for (i = 0; i < 26; i++) {
                for (j = 0; j < 25; j++) {
                    if (reverseGameMap[index][1] == 0) {
                        sprintf(fnombre, "../results/%s_%i_%s_%s.txt", games[g], reverseGameMap[index][0], agents[i], agents[j]);
                        fp = fopen(fnombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &(scores50000[g][index][i][j]), &val);
                        fscanf(fp, "%s %lf %lf", dummy, &(scores100[g][index][i][j]), &val);
                        fscanf(fp, "%s %lf %lf", dummy, &(scores1000[g][index][i][j]), &val);
                        countScores[g][index][i][j] ++;
                        
                        fclose(fp);
                    }
                    else {
                        sprintf(fnombre, "../results/%s_%i_%s_%s.txt", games[g], reverseGameMap[index][0], agents[j], agents[i]);
                        fp = fopen(fnombre, "r");
                        
                        fscanf(fp, "%s %lf %lf", dummy, &val, &(scores50000[g][index][i][j]));
                        fscanf(fp, "%s %lf %lf", dummy, &val, &(scores100[g][index][i][j]));
                        fscanf(fp, "%s %lf %lf", dummy, &val, &(scores1000[g][index][i][j]));
                        countScores[g][index][i][j] ++;
                        
                        fclose(fp);
                    }
                }
            }
        }
    }
}

void computeAveragesAll() {
    int g, index, i, j;

    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            averages[0][i][j] = averages[1][i][j] = averages[2][i][j] = 0.0;
            for (g = 0; g < 5; g++) {
                for (index = 0; index < 144; index++) {
                    averages[0][i][j] += scores100[g][index][i][j];
                    averages[1][i][j] += scores1000[g][index][i][j];
                    averages[2][i][j] += scores50000[g][index][i][j];
                }
            }
            averages[0][i][j] /= 144 * 5;
            averages[1][i][j] /= 144 * 5;
            averages[2][i][j] /= 144 * 5;
        }
    }
}

void computeAveragesPerGame() {
    int i, j, k, m;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 144; j++) {
            gameAverages[0][i][j] = gameAverages[1][i][j] = gameAverages[2][i][j] = 0.0;
            for (k = 0; k < 25; k++) {
                //if (k == 17) // don't count random
                //    continue;
                for (m = 0; m < 25; m++) {
                    gameAverages[0][i][j] += scores100[i][j][k][m];
                    gameAverages[1][i][j] += scores1000[i][j][k][m];
                    gameAverages[2][i][j] += scores50000[i][j][k][m];
                }
            }
            gameAverages[0][i][j] /= 25 * 25;
            gameAverages[1][i][j] /= 25 * 25;
            gameAverages[2][i][j] /= 25 * 25;
        }
    }
    
    //for (i = 0; i < 5; i++)
    //    printf("averages 7: %lf, %lf, %lf\n", gameAverages[0][i][7], gameAverages[1][i][7], gameAverages[2][i][7]);

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 144; j++) {
            gameStdev[0][i][j] = gameStdev[1][i][j] = gameStdev[2][i][j] = 0.0;
            for (k = 0; k < 25; k++) {
                if (k == 17) // don't count random
                    continue;
                for (m = 0; m < 25; m++) {
                    gameStdev[0][i][j] += sqrt((scores100[i][j][k][m]-gameAverages[0][i][j])*(scores100[i][j][k][m]-gameAverages[0][i][j]));
                    gameStdev[1][i][j] += sqrt((scores1000[i][j][k][m]-gameAverages[1][i][j])*(scores1000[i][j][k][m]-gameAverages[1][i][j]));
                    gameStdev[2][i][j] += sqrt((scores50000[i][j][k][m]-gameAverages[2][i][j])*(scores50000[i][j][k][m]-gameAverages[2][i][j]));
                }
            }
            gameStdev[0][i][j] /= 24 * 25;
            gameStdev[1][i][j] /= 24 * 25;
            gameStdev[2][i][j] /= 24 * 25;
        }
    }
    
    //for (i = 0; i < 5; i++)
    //    printf("stdev 7: %lf, %lf, %lf\n", gameStdev[0][i][7], gameStdev[1][i][7], gameStdev[2][i][7]);
}

void computeAveragesPerPartner() {
    int i, j, k, m;

    for (m = 0; m < 25; m++) {
        partnerAverages[0][m] = partnerAverages[1][m] = partnerAverages[2][m] = 0.0;
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 144; j++) {
                for (k = 0; k < 25; k++) {
                    partnerAverages[0][m] += scores100[i][j][k][m];
                    partnerAverages[1][m] += scores1000[i][j][k][m];
                    partnerAverages[2][m] += scores50000[i][j][k][m];
                }
            }
        }
        partnerAverages[0][m] /= 5 * 144 * 25;
        partnerAverages[1][m] /= 5 * 144 * 25;
        partnerAverages[2][m] /= 5 * 144 * 25;
    }
    
    printf("Averages:\n");
    for (i = 0; i < 25; i++)
        printf("%.7s:\t%lf, %lf, %lf\n", agents[i], partnerAverages[0][i], partnerAverages[1][i], partnerAverages[1][i]);

    for (m = 0; m < 25; m++) {
        partnerStdev[0][m] = partnerStdev[1][m] = partnerStdev[2][m] = 0.0;
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 144; j++) {
                for (k = 0; k < 25; k++) {
                    partnerStdev[0][m] += sqrt((scores100[i][j][k][m]-partnerAverages[0][m]) * (scores100[i][j][k][m]-partnerAverages[0][m]));
                    partnerStdev[1][m] += sqrt((scores1000[i][j][k][m]-partnerAverages[1][m]) * (scores1000[i][j][k][m]-partnerAverages[1][m]));
                    partnerStdev[2][m] += sqrt((scores50000[i][j][k][m]-partnerAverages[2][m]) * (scores50000[i][j][k][m]-partnerAverages[2][m]));
                }
            }
        }
        partnerStdev[0][m] /= 5 * 144 * 25;
        partnerStdev[1][m] /= 5 * 144 * 25;
        partnerStdev[2][m] /= 5 * 144 * 25;
    }
    
    printf("Stdev:\n");
    for (i = 0; i < 25; i++)
        printf("%.7s:\t%lf, %lf, %lf\n", agents[i], partnerStdev[0][i], partnerStdev[1][i], partnerStdev[1][i]);

}

void computeAverages(int tipo) {
    int g, index, i, j;
    int count;

    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            averages[0][i][j] = averages[1][i][j] = averages[2][i][j] = 0.0;
            count = 0;
            for (g = 0; g < 5; g++) {
                for (index = 0; index < 144; index++) {
                    if (reverseGameMap[index][2] == tipo) {
                        averages[0][i][j] += scores100[g][index][i][j];
                        averages[1][i][j] += scores1000[g][index][i][j];
                        averages[2][i][j] += scores50000[g][index][i][j];
                        count ++;
                    }
                }
            }
            averages[0][i][j] /= count;
            averages[1][i][j] /= count;
            averages[2][i][j] /= count;
        }
    }
}


void outputAverages() {
    int i, j;
    FILE *fp;
    
    fp = fopen("newOutput100.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fprintf(fp, "\t%s", agents[i]);
    }
    fprintf(fp, "\n");

    for (i = 0; i < 25; i++) {
        fprintf(fp, "%s\t", agents[i]);
        for (j = 0; j < 25; j++) {
            fprintf(fp, "%lf\t", averages[0][i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    
    fp = fopen("newOutput1000.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fprintf(fp, "\t%s", agents[i]);
    }
    fprintf(fp, "\n");

    for (i = 0; i < 25; i++) {
        fprintf(fp, "%s\t", agents[i]);
        for (j = 0; j < 25; j++) {
            fprintf(fp, "%lf\t", averages[1][i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    
    fp = fopen("newOutput50000.txt", "w");
    
    for (i = 0; i < 25; i++) {
        fprintf(fp, "\t%s", agents[i]);
    }
    fprintf(fp, "\n");

    for (i = 0; i < 25; i++) {
        fprintf(fp, "%s\t", agents[i]);
        for (j = 0; j < 25; j++) {
            fprintf(fp, "%lf\t", averages[2][i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void writeLaTeXTable(int len) {
    char fnombre[1024];
    sprintf(fnombre, "saved144new/newOutput%i.txt", len);
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

int getMax(double vals[25], bool done[25]) {
    int index = -1, i;
    double max = -99999;
    
    for (i = 0; i < 25; i++) {
        if ((vals[i] > max) && !(done[i])) {
            index = i;
            max = vals[i];
        }
    }

    return index;
}

int getMax(int counts[25][25], bool done[25]) {
    int index = -1, i, j;
    int max = -1;
    
    for (i = 0; i < 25; i++) {
        if ((counts[i][0] > max) && !(done[i])) {
            index = i;
            max = counts[i][0];
        }
        else if ((counts[i][0] == max) && !(done[i])) {
            for (j = 1; j < 25; j++) {
                if (counts[i][j] > counts[index][j]) {
                    index = i;
                    max = counts[i][0];
                    break;
                }
                else if (counts[i][j] < counts[index][j])
                    break;
            }
        }
    }

    return index;
}

int getMax(double vals1[25], double vals2[25], bool done[25]) {
    int index = -1, i, j;
    double max = -1;
    
    for (i = 0; i < 25; i++) {
        if ((vals2[i] > max) && !(done[i])) {
            index = i;
            max = vals2[i];
            //printf("%lf ", max);
        }
        else if ((vals2[i] == max) && !(done[i])) {
            if (vals1[i] > vals1[index]) {
                index = i;
                max = vals2[i];
            }
        }
    }

    return index;
}

int getMax(double vals1[25], double vals2[25], double vals3[25], bool done[25]) {
    int index = -1, i, j;
    double max = -1;
    
    for (i = 0; i < 25; i++) {
        if ((vals3[i] > max) && !(done[i])) {
            index = i;
            max = vals3[i];
            //printf("%lf ", max);
        }
        else if ((vals3[i] == max) && !(done[i])) {
            if (vals2[i] > vals2[index]) {
                index = i;
                max = vals3[i];
            }
            else if (vals2[i] == vals2[index]) {
                if (vals1[i] > vals1[index]) {
                    index = i;
                    max = vals3[i];
                }
            }
        }
    }

    return index;
}

void doPlacement(int ranks[25], double vals[25]) {
    bool done[25];
    int i;
    for (i = 0; i < 25; i++) {
        done[i] = false;
    }
    int mx;
    for (i = 0; i < 25; i++) {
        mx = getMax(vals, done);
        done[mx] = true;
        ranks[mx] = i+1;
    }
}

void doPlacement(int ranks[25], double vals1[25], double vals2[25]) {
    bool done[25];
    int i;
    for (i = 0; i < 25; i++) {
        done[i] = false;
    }
    int mx;
    for (i = 0; i < 25; i++) {
        mx = getMax(vals1, vals2, done);
        done[mx] = true;
        ranks[mx] = i+1;
    }
}


void doPlacement(int ranks[25], double vals1[25], double vals2[25], double vals3[25]) {
    bool done[25];
    int i;
    for (i = 0; i < 25; i++) {
        done[i] = false;
    }
    int mx;
    for (i = 0; i < 25; i++) {
        mx = getMax(vals1, vals2, vals3, done);
        done[mx] = true;
        ranks[mx] = i+1;
    }
}

void doPlacement(int ranks[25], int counts[25][25]) {
    bool done[25];
    int i;
    for (i = 0; i < 25; i++) {
        done[i] = false;
    }
    int mx;
    for (i = 0; i < 25; i++) {
        mx = getMax(counts, done);
        
        done[mx] = true;
        ranks[mx] = i+1;
    }
}

void totalAverages(int index, double vals[25], int ranks[25]) {
    int i, j;

    for (i = 0; i < 25; i++) {
        vals[i] = 0.0;
        for (j = 0; j < 25; j++) {
            vals[i] += averages[index][i][j];
        }
        vals[i] /= 25.0;
    }
    
    doPlacement(ranks, vals);
}


void totalAverages_selected(int tipo, int index, double vals[25], int ranks[25]) {
    int i, j;
    int count;

    for (i = 0; i < 25; i++) {
        vals[i] = 0.0;
        count = 0;
        for (j = 0; j < 25; j++) {
            if (tipo != agentClass(j))
                continue;
            count ++;
            vals[i] += averages[index][i][j];
        }
        vals[i] /= count;
    }
    
    doPlacement(ranks, vals);
}

void getWins(int index, double vals[25], int ranks[25]) {
    int placement[25][25];
    int i, j;
    
    double tmp[25];
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            tmp[j] = averages[index][j][i];
        }
        
        doPlacement(placement[i], tmp);
    }

    int counts[25][25];
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++)
            counts[i][j] = 0;
        
        for (j = 0; j < 25; j++) {
            counts[i][placement[j][i]-1]++;
        }
        vals[i] = counts[i][0] / 25.0;
    }

    doPlacement(ranks, counts);
}

void getWorst(int index, double vals[25], int ranks[25]) {
    double againstAve[25];
    int i, j;
    
    for (j = 0; j < 25; j++) {
        againstAve[j] = 0.0;
        for (i = 0; i < 25; i++) {
            againstAve[j] += averages[index][i][j];
        }
        againstAve[j] /= 25.0;
    }

    double v;
    for (i = 0; i < 25; i++) {
        vals[i] = 99999;
        for (j = 0; j < 25; j++) {
            v = averages[index][i][j] - againstAve[j];
            if (v < vals[i])
                vals[i] = v;
        }
    }
    
    doPlacement(ranks, vals);
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

void replicator(int index, double vals[25], int ranks[25]) {
    double proportion[1001][25];
    int i, j, k;

    for (i = 0; i < 25; i++)
        proportion[0][i] = 1.0 / 25.0;

    evolve(averages[index], 1000, proportion);
    
    double val;
    for (i = 0; i < 25; i++) {
        vals[i] = 0.0;
        for (j = 0; j < 1001; j++) {
            vals[i] += proportion[j][i];
        }
        vals[i] /= 1001;
    }


    // record them for later
    char fnombre[1024];
    switch (index) {
        case 0: strcpy(fnombre, "saved144new/evolve_100.txt"); break;
        case 1: strcpy(fnombre, "saved144new/evolve_1000.txt"); break;
        case 2: strcpy(fnombre, "saved144new/evolve_50000.txt"); break;
    }
    
    FILE *fp = fopen(fnombre, "w");
    fprintf(fp, "Agent,Generation,Proportion\n");
    for (j = 0; j < 1001; j++) {
        for (i = 0; i < 25; i++) {
            fprintf(fp, "%s,%i,%lf\n", agents[i], j, proportion[j][i]);
        }
    }
    fclose(fp);
    
    // rankings
    bool done[25];
    for (i = 0; i < 25; i++) {
        done[i] = false;
    }
    int mx;
    double vs[25];
    for (i = 0; i < 25; i++) {
        vs[i] = vals[i];
    }
    mx = getMax(vals, done);
    done[mx] = true;
    ranks[mx] = 1;
    
    for (j = 1; j < 25; j++) {
        for (i = 0; i < 25; i++) {
            if (!done[i])
                proportion[0][i] = 1.0 / (25 - j);
            else
                proportion[0][i] = 0.0;
            
            //printf("%.2lf ", proportion[0][i]);
        }
        //printf("\n");

        evolve(averages[index], 1000, proportion);
        for (i = 0; i < 25; i++) {
            vs[i] = 0.0;
            for (k = 0; k < 1001; k++) {
                vs[i] += proportion[k][i];
            }
            vs[i] /= 1001;
        }
        
        mx = getMax(vs, done);
        done[mx] = true;
        ranks[mx] = j+1;
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
}


void subRR(int index, int *group, double *results, int num, int top[2]) {
    int i, j;
    
    for (i = 0; i < num; i++)
        results[i] = 0.0;
    
    for (i = 0; i < num; i++) {
        for (j = 0; j < num; j++) {
            results[i] += averages[index][group[i]][group[j]];
        }
    }

    top[0] = 0;
    for (i = 0; i < num; i++) {
        results[i] /= num;
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
}


int runOff1(int index, int winners[4]) {

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
    subRR(index, g1, r1, 6, top1);
    subRR(index, g2, r2, 6, top2);
    subRR(index, g3, r3, 6, top3);
    subRR(index, g4, r4, 7, top4);

    winners[0] = g1[top1[0]];
    winners[1] = g2[top2[0]];
    winners[2] = g3[top3[0]];
    winners[3] = g4[top4[0]];
    
    subRR(index, winners, rw, 4, topw);

    return winners[topw[0]];
}

void runOffs1(int index, double vals1[25], double vals2[25], int ranks[25]) {
    int i, j;
    int w;
    int numWins[25];
    int winners[4];
    int advance[25];

    for (i = 0; i < 25; i++)
        numWins[i] = advance[i] = 0;
    
    int num = 10000;
    for (i = 0; i < num; i++) {
        w = runOff1(index, winners);
        numWins[w] ++;
        for (j = 0; j < 4; j++)
            advance[winners[j]] ++;
    }

    for (i = 0; i < 25; i++) {
        vals1[i] = advance[i] / (double)num;
        vals2[i] = numWins[i] / (double)num;
    }
    
    doPlacement(ranks, vals1, vals2);
}


int runOff2(int index, int winners[8], int finalists[2]) {
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
    subRR(index, g1, r1, 6, top1);
    subRR(index, g2, r2, 6, top2);
    subRR(index, g3, r3, 6, top3);
    subRR(index, g4, r4, 7, top4);

    winners[0] = g1[top1[0]];
    winners[1] = g2[top2[0]];
    winners[2] = g3[top3[0]];
    winners[3] = g4[top4[0]];
    winners[4] = g1[top1[1]];
    winners[5] = g2[top2[1]];
    winners[6] = g3[top3[1]];
    winners[7] = g4[top4[1]];
    
    subRR(index, winners, rw, 8, topw);
    finalists[0] = winners[topw[0]];
    finalists[1] = winners[topw[1]];

    subRR(index, finalists, rf, 2, topf);

    return finalists[topf[0]];
}

void runOffs2(int index, double vals1[25], double vals2[25], double vals3[25], int ranks[25]) {
    int w;
    int winners[8], finalists[2];
    int i, j;
    int numWins[25];
    int advance1[25], advance2[25];

    for (i = 0; i < 25; i++)
        numWins[i] = advance1[i] = advance2[i] = 0;
    
    int num = 10000;
    for (i = 0; i < num; i++) {
        w = runOff2(index, winners, finalists);
        numWins[w] ++;
        for (j = 0; j < 8; j++)
            advance1[winners[j]] ++;
        for (j = 0; j < 2; j++)
            advance2[finalists[j]] ++;
    }

    for (i = 0; i < 25; i++) {
        vals1[i] = advance1[i] / (double)num;
        vals2[i] = advance2[i] / (double)num;
        vals3[i] = numWins[i] / (double)num;
        //printf("%s:\t\t%.4lf\t%.4lf\t%.4lf\n", agents[i], advance1[i] / (double)num, advance2[i] / (double)num, numWins[i] / (double)num);
    }
    
    doPlacement(ranks, vals1, vals2, vals3);
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

const char *toStrng(int index) {
    switch (index) {
        case 0: return "Static";
        case 1: return "RL";
        case 2: return "Expert";
        case 3: return "Other";
    }
    
    return "Nothing";
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
        case 8: return "MBRL-1";
        case 9: return "wolf";
        case 10: return "CJAL";
        case 11: return "Fict. Play";
        case 12: return "WMA";
        case 13: return "manipulator-gf";
        case 14: return "Gf (gTFT)";
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

void createMaster() {
    FILE *masterfp = fopen("master.csv", "w");

    fprintf(masterfp, "Category,Game,GameClass,Algorithm,Partner,PartnerClass,v100,v1000,v50000,d100,d1000,d50000,p100,p1000,p50000\n");
    int i, j, k, m;
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 144; j++) {
            for (k = 0; k < 26; k++) {
                for (m = 0; m < 25; m++) {
                    gameStdev[0][i][j] = gameStdev[1][i][j] = gameStdev[2][i][j] = 1.0;
                    partnerStdev[0][m] = partnerStdev[1][m] = partnerStdev[2][m] = 1.0;
                    fprintf(masterfp, "%i,%i,%s,%s,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                                i, j, strGame(reverseGameMap[j][2]), agentsStr(k), agentsStr(m), toStrng(agentClass(m)),
                                scores100[i][j][k][m], scores1000[i][j][k][m], scores50000[i][j][k][m],
                                (scores100[i][j][k][m]-gameAverages[0][i][j]) / gameStdev[0][i][j],
                                (scores1000[i][j][k][m]-gameAverages[1][i][j]) / gameStdev[1][i][j],
                                (scores50000[i][j][k][m]-gameAverages[2][i][j]) / gameStdev[2][i][j],
                                (scores100[i][j][k][m]-partnerAverages[0][m]) / partnerStdev[0][m],
                                (scores1000[i][j][k][m]-partnerAverages[1][m]) / partnerStdev[1][m],
                                (scores50000[i][j][k][m]-partnerAverages[2][m]) / partnerStdev[2][m]);
                }
            }
        }
    }

    fclose(masterfp);
}

double theVar(double lst[5*144*25], int count, double m) {
    int i;
    double sum = 0.0;
    
    for (i = 0; i < count; i++) {
        sum += (lst[i] - m) * (lst[i] - m);
    }

    return sum / count;
}


double theMean(double lst[5*144*25], int count) {
    double sum = 0.0;
    int i;
    
    for (i = 0; i < count; i++) {
        sum += lst[i];
    }

    return sum / count;
}

bool ttest(int a1, int a2, int lengthIndex, int gameFamily, int partnerFamily) {
    int g, ind, a, count;
    double l1[5*144*25], l2[5*144*25];
    
    count = 0;
    for (g = 0; g < 5; g++) {
        for (ind = 0; ind < 144; ind++) {
            if ((gameFamily != 6) && (reverseGameMap[ind][2] != gameFamily))
                continue;
            
            for (a = 0; a < 25; a++) {
                if ((partnerFamily != 4) && (agentClass(a) != partnerFamily))
                    continue;
                
                if (lengthIndex == 0) {
                    l1[count] = scores100[g][ind][a1][a] - gameAverages[0][g][ind];
                    l2[count] = scores100[g][ind][a2][a] - gameAverages[0][g][ind];
                }
                else if (lengthIndex == 1) {
                    l1[count] = scores1000[g][ind][a1][a] - gameAverages[1][g][ind];
                    l2[count] = scores1000[g][ind][a2][a] - gameAverages[1][g][ind];
                }
                else {
                    l1[count] = scores50000[g][ind][a1][a] - gameAverages[2][g][ind];
                    l2[count] = scores50000[g][ind][a2][a] - gameAverages[2][g][ind];
                }
                
                count++;
            }
        }
    }
    
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);
    
    //printf("E[%s] = %lf; Var = %lf\n", agents[a1], m1, var1);
    //printf("E[%s] = %lf; Var = %lf\n", agents[a2], m2, var2);

    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);

    if ((t < -1.959) && (t > -1.963)) {
        printf("questionable: %s vs %s, %i, %i, %i (%lf; %lf)\n", agents[a1], agents[a2], lengthIndex, gameFamily, partnerFamily, t, df);
    }

    if (t < -1.9605)
        return true;

    return false;
}


bool ttestCrandall(int a1, int a2, int lengthIndex, int gameFamily, int partnerFamily) {
    int g, ind, a, count;
    double l1[5*144*25], l2[5*144*25];
    
    count = 0;
    for (g = 0; g < 5; g++) {
        for (ind = 0; ind < 144; ind++) {
            if ((crandallFamily[g][ind] != gameFamily))
                continue;
            
            for (a = 0; a < 25; a++) {
                if ((partnerFamily != 4) && (agentClass(a) != partnerFamily))
                    continue;
                
                if (lengthIndex == 0) {
                    l1[count] = scores100[g][ind][a1][a] - gameAverages[0][g][ind];
                    l2[count] = scores100[g][ind][a2][a] - gameAverages[0][g][ind];
                }
                else if (lengthIndex == 1) {
                    l1[count] = scores1000[g][ind][a1][a] - gameAverages[1][g][ind];
                    l2[count] = scores1000[g][ind][a2][a] - gameAverages[1][g][ind];
                }
                else {
                    l1[count] = scores50000[g][ind][a1][a] - gameAverages[2][g][ind];
                    l2[count] = scores50000[g][ind][a2][a] - gameAverages[2][g][ind];
                }
                
                count++;
            }
        }
    }
    
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);
    
    //printf("E[%s] = %lf; Var = %lf\n", agents[a1], m1, var1);
    //printf("E[%s] = %lf; Var = %lf\n", agents[a2], m2, var2);

    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);

    if ((t < -1.959) && (t > -1.963)) {
        printf("questionable: %s vs %s, %i, %i, %i (%lf; %lf)\n", agents[a1], agents[a2], lengthIndex, gameFamily, partnerFamily, t, df);
    }

    if (t < -1.9605)
        return true;

    return false;
}


bool ttest2(int a1, int a2, int lengthIndex, int gameFamily, int a) {
    int g, ind, count;
    double l1[5*144*25], l2[5*144*25];
    
    count = 0;
    for (g = 0; g < 5; g++) {
        for (ind = 0; ind < 144; ind++) {
            if ((gameFamily != 6) && (reverseGameMap[ind][2] != gameFamily))
                continue;
            
            if (lengthIndex == 0) {
                l1[count] = scores100[g][ind][a1][a] - gameAverages[0][g][ind];
                l2[count] = scores100[g][ind][a2][a] - gameAverages[0][g][ind];
            }
            else if (lengthIndex == 1) {
                l1[count] = scores1000[g][ind][a1][a] - gameAverages[1][g][ind];
                l2[count] = scores1000[g][ind][a2][a] - gameAverages[1][g][ind];
            }
            else {
                l1[count] = scores50000[g][ind][a1][a] - gameAverages[2][g][ind];
                l2[count] = scores50000[g][ind][a2][a] - gameAverages[2][g][ind];
            }
            
            count++;
        }
    }
    
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);
    
    //printf("E[%s] = %lf; Var = %lf\n", agents[a1], m1, var1);
    //printf("E[%s] = %lf; Var = %lf\n", agents[a2], m2, var2);

    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);

    if ((t < -1.959) && (t > -1.963)) {
        printf("questionable: %s vs %s, %i, %i, %i (%lf; %lf)\n", agents[a1], agents[a2], lengthIndex, gameFamily, a, t, df);
    }

    if (t < -1.962)
        return true;

    return false;
}

bool ttestAlg(int a1, int a2, int k, int lengthIndex) {
    int g, ind, count;
    double l1[5*144*25*3], l2[5*144*25*3];
    
    count = 0;
    for (g = 0; g < 5; g++) {
        for (ind = 0; ind < 144; ind++) {
            //for (a = 0; a < 25; a++) {
                //if (lengthIndex == 0) {
                    l1[count] = scores100[g][ind][a1][k] - gameAverages[0][g][ind];
                    l2[count] = scores100[g][ind][a2][k] - gameAverages[0][g][ind];
                    count++;
                //}
                //else if (lengthIndex == 1) {
                    l1[count] = scores1000[g][ind][a1][k] - gameAverages[1][g][ind];
                    l2[count] = scores1000[g][ind][a2][k] - gameAverages[1][g][ind];
                    count++;
                //}
                //else {
                    l1[count] = scores50000[g][ind][a1][k] - gameAverages[2][g][ind];
                    l2[count] = scores50000[g][ind][a2][k] - gameAverages[2][g][ind];
                    count++;
                //}
                
            
            //}
        }
    }
    
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);
    
    //printf("E[%s] = %lf; Var = %lf\n", agents[a1], m1, var1);
    //printf("E[%s] = %lf; Var = %lf\n", agents[a2], m2, var2);
    
    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);
    
    if ((t < -1.959) && (t > -1.963)) {
        printf("questionable: %s vs %s, %i, (%lf; %lf)\n", agents[a1], agents[a2], lengthIndex, t, df);
    }
    
    if (t < -1.9605)
        return true;
    
    return false;
}

void theCaseAlg(int lengthIndex, int rank[25][25]) {
    int i, j, k;
    int count;
    for (k = 0; k < 25; k++) {
        for (i = 0; i < 25; i++) {
            count = 0;
            for (j = 0; j < 25; j++) {
                if (i == j)
                    continue;
                
                count += 1-(int)(ttestAlg(i, j, k, lengthIndex));
            }
            //printf("%s: %i\n", agents[i], 25 - count);
            rank[i][k] = 25 - count;
        }
    }
}

bool ttestGame(int a1, int a2, int ind, int lengthIndex) {
    int g, i, count;
    double l1[5*144*25], l2[5*144*25];
    
    count = 0;
    
    for (g = 0; g < 5; g++) {
        for (i = 0; i < 25; i++) {
            if (lengthIndex == 0) {
                l1[count] = scores100[g][ind][a1][i] - gameAverages[0][g][ind];
                l2[count] = scores100[g][ind][a2][i] - gameAverages[0][g][ind];
            }
            else if (lengthIndex == 1) {
                l1[count] = scores1000[g][ind][a1][i] - gameAverages[1][g][ind];
                l2[count] = scores1000[g][ind][a2][i] - gameAverages[1][g][ind];
            }
            else {
                l1[count] = scores50000[g][ind][a1][i] - gameAverages[2][g][ind];
                l2[count] = scores50000[g][ind][a2][i] - gameAverages[2][g][ind];
            }
            
            count++;
        }
    }
    
    //printf("count = %i\n", count);
    
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);
    
    //printf("E[%s] = %lf; Var = %lf\n", agents[a1], m1, var1);
    //printf("E[%s] = %lf; Var = %lf\n", agents[a2], m2, var2);
    
    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);
    
    //if ((t < -1.959) && (t > -1.963)) {
    //    printf("questionable: %s vs %s, %i, (%lf; %lf)\n", agents[a1], agents[a2], lengthIndex, t, df);
    //}
    
    //if (t < -1.9605)
    //    return true;
    if (t < -1.972) {
        //printf("diff\n");
        return true;
    }
    //printf("not diff\n");
    
    return false;
}

void theCaseGame(int lengthIndex, int rank[25][144]) {
    int i, j, ind, g;
    int count;
    
    for (ind = 0; ind < 144; ind++) {
        for (i = 0; i < 25; i++) {
            count = 0;
            //for (g = 0; g < 5; g++) {
                for (j = 0; j < 25; j++) {
                    if (i == j)
                        continue;
                    
                    count += 1-(int)(ttestGame(i, j, ind, lengthIndex));
                }
            //}
            //printf("%s: %i\n", agents[i], 25 - count);
            rank[i][ind] = 25 - count;
        }
    }
}

void theCaseCrandall(int lengthIndex, int gameFamily, int partnerFamily, int rank[25]) {
    int i, j;
    int count;
    for (i = 0; i < 25; i++) {
        count = 0;
        for (j = 0; j < 25; j++) {
            if (i == j)
                continue;
            
            count += 1-(int)(ttestCrandall(i, j, lengthIndex, gameFamily, partnerFamily));
        }
        //printf("%s: %i\n", agents[i], 25 - count);
        rank[i] = 25 - count;
    }
}

void theCase(int lengthIndex, int gameFamily, int partnerFamily, int rank[25]) {
    int i, j;
    int count;
    for (i = 0; i < 25; i++) {
        count = 0;
        for (j = 0; j < 25; j++) {
            if (i == j)
                continue;
            
            count += 1-(int)(ttest(i, j, lengthIndex, gameFamily, partnerFamily));
        }
        //printf("%s: %i\n", agents[i], 25 - count);
        rank[i] = 25 - count;
    }
}

void theCaseSingle(int lengthIndex, int partner, int rank[25]) {
    int i, j;
    int count;
    for (i = 0; i < 25; i++) {
        count = 0;
        for (j = 0; j < 25; j++) {
            if (i == j)
                continue;
            
            count += 1-(int)(ttest2(i, j, lengthIndex, 6, partner));
        }
        //printf("%s: %i\n", agents[i], 25 - count);
        rank[i] = 25 - count;
    }
}

void getWins2(int index, double vals[25], int ranks[25]) {
    int placement[25][25];
    int i, j;

    int counts[25][25];
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++)
            counts[i][j] = 0;
    }
    
    for (i = 0; i < 25; i++) {
        theCaseSingle(index, i, ranks);
        
        for (j = 0; j < 25; j++)
            counts[j][ranks[j]-1] ++;
    }
    
    for (i = 0; i < 25; i++) {
        vals[i] = counts[i][0] / 25.0;
        printf("%s:\t", agents[i]);
        for (j = 0; j < 25; j++) {
            printf("%i ", counts[i][j]);
        }
        printf("\t%lf\n", vals[i]);
    }
/*
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 25; j++) {
            counts[i][placement[j][i]-1]++;
        }
        vals[i] = counts[i][0] / 25.0;
    }
*/
    doPlacement(ranks, counts);

}

bool isLower(double l1[5*144], double l2[5*144], int count) {
    double m1, m2;
    m1 = theMean(l1, count);
    m2 = theMean(l2, count);
    
    double var1, var2;
    var1 = theVar(l1, count, m1);
    var2 = theVar(l2, count, m2);

    double t = (m1 - m2) / sqrt((var1 / count) + (var2 / count));
    //printf("t statistic = %lf\n", t);
    double o1 = (var1/count), o2 = (var2/count);
    double df = (o1 + o2) * (o1 + o2);
    double num = (o1*o1)/(count-1) + (o2*o2)/(count-2);
    df /= num;
    //printf("df = %lf\n", df);

    if ((t < -1.959) && (t > -1.963)) {
        printf("questionable: (%lf; %lf)\n", t, df);
    }

    if (t < -1.962)
        return true;

    return false;
}

void getWorst2(int index, double vals[25], int rank[25]) {
    double list[25][5*144];
    int culprit[25];
    double againstAve[25];
    int i, j;
    
    for (j = 0; j < 25; j++) {
        againstAve[j] = 0.0;
        for (i = 0; i < 25; i++) {
            againstAve[j] += averages[index][i][j];
        }
        againstAve[j] /= 25.0;
    }

    double v;
    for (i = 0; i < 25; i++) {
        vals[i] = 99999;
        for (j = 0; j < 25; j++) {
            v = averages[index][i][j] - againstAve[j];
            if (v < vals[i]) {
                vals[i] = v;
                culprit[i] = j;
            }
        }
    }
    
    int g;
    int count;
    for (i = 0; i < 25; i++) {
        count = 0;
        for (g = 0; g < 5; g++) {
            for (j = 0; j < 144; j++) {
                if (index == 0)
                    list[i][count] = scores100[g][j][i][culprit[i]] - gameAverages[0][g][j];//againstAve[culprit[i]];
                else if (index == 1)
                    list[i][count] = scores1000[g][j][i][culprit[i]] - gameAverages[1][g][j];//againstAve[culprit[i]];
                else
                    list[i][count] = scores50000[g][j][i][culprit[i]] - gameAverages[2][g][j];//againstAve[culprit[i]];
                count++;
            }
        }
    }
    
    for (i = 0; i < 25; i++) {
        rank[i] = 1;
        for (j = 0; j < 25; j++) {
            if (i == j)
                continue;

            if (isLower(list[i], list[j], count)) {
                //printf("%s\n", agents[j]);
                rank[i] ++;
            }
        }
    }
    
    
    for (i = 0; i < 25; i++) {
        printf("%.10s:\t%lf\t%i\n", agents[i], theMean(list[i], count), rank[i]);
    }
}

void printG(Cell _M[NumDecisionPointsMax][NumDecisionPointsMax]) {
    int i, j;
    
    printf("   |      ");
    
    for (i = 0; i < 2; i++)
        printf("%i      |      ", i);
    printf("\n");
    for (i = 0; i < 2; i++) {
        printf("--------------------------------------\n %i | ", i);
        for (j = 0; j < 2; j++) {
            printf("%.2lf , %.2lf | ", _M[i][j].r1, _M[i][j].r2);
        }
        printf("\n");
    }
    printf("--------------------------------------\n\n");
}

void fprintGame(FILE *fp, int m[2][2][2]) {
    int i, j;
    
    //printf("aqui\n");
    
    fprintf(fp, "   |   0   |   1   |\n");
    fprintf(fp, "--------------------\n");
    for (i = 0; i < 2; i++) {
        fprintf(fp, " 0 |");
        for (j = 0; j < 2; j++) {
            fprintf(fp, " %i , %i |", m[0][i][j], m[1][i][j]);
        }
        fprintf(fp, "\n--------------------\n");
    }
    fprintf(fp, "\n");
}

double NB(int index) {
	Cell Payoffs[NumDecisionPointsMax][NumDecisionPointsMax];
	int c[2], a1[2], a2[2];
    double bargain1, bargain2;
	
    int m[2][2][2], firstrow, firstcol;
    size_t len;
    
	int i, j, g;
	double p1, p2;
	
    char filename[1024];
    char fnm[1024], linea[1024], linea2[1024];
    sprintf(fnm, "../games/NBS/%i.txt", gameMap[index][0]);
    FILE *fpw1 = fopen(fnm, "w");
    FILE *fpw2;
    if (gameMap[index][0] != gameMap[index][1]) {
        sprintf(fnm, "../games/NBS/%i.txt", gameMap[index][1]);
        fpw2 = fopen(fnm, "w");
    }
    
    // read in the base matrix and row and col switches
    sprintf(filename, "../gameClass/cg_standard_%i.txt", index+1);
    //printf("filename: %s\n", filename); fflush(stdout);
    FILE *fpBase = fopen(filename, "r");

    for (i = 0; i < 8; i++) {
        fgets(linea, 1024, fpBase);
        //printf("%s", linea);
    }
    
    for (j = 0; j < 2; j++) {
        fscanf(fpBase, "%s %s", linea, linea2);
        //printf("%s %s ", linea, linea2);
        for (i = 0; i < 2; i++) {
            fscanf(fpBase, "%i %s", &(m[0][j][i]), linea2);
            //printf("%i %s", m[0][j][i], linea2);
            fscanf(fpBase, "%i %s", &(m[1][j][i]), linea2);
            //printf("%i, %i\n", m[0][j][i], m[1][j][i]);
            //printf("%s\n", linea2);
        }
        fgets(linea, 1024, fpBase);
        fgets(linea, 1024, fpBase);
        //printf("%s\n", linea);
    }
    
    fprintGame(fpw1, m);
    if (gameMap[index][0] != gameMap[index][1])
        fprintGame(fpw2, m);
    
    
    fscanf(fpBase, "%s %s", linea, linea2);
    fscanf(fpBase, "%i", &firstrow);
    fscanf(fpBase, "%s %s", linea, linea2);
    fscanf(fpBase, "%i", &firstcol);
    //printf("%i, %i\n", firstrow, firstcol);
    
    fclose(fpBase);
    
    
    for (g = 0; g < 5; g++) {
        sprintf(filename, "../games/%s_%i.txt", games[g], index+1);
        printf("filename: %s (%i, %i)\n", filename, gameMap[index][0], gameMap[index][1]);
        FILE *fp = fopen(filename, "r");
        
        int npls, nacts1, nacts2;
        fscanf(fp, "%i", &npls);
        fscanf(fp, "%i", &nacts1);
        fscanf(fp, "%i", &nacts2);
        
        int count = 0;
        for (i = 0; i < nacts1; i++) {
            for (j = 0; j < nacts2; j++) {
                fscanf(fp, "%lf %lf", &(Payoffs[j][i].r1), &(Payoffs[j][i].r2));
                count ++;
            }
        }
        fclose(fp);
        
        printG(Payoffs);
        
        Littman(Payoffs, c, a1, a2, nacts1);
        
        //printf("c: %i, %i\n", c[0], c[1]);
        //printf("a1: %i, %i\n", a1[0], a1[1]);
        //printf("a2: %i, %i\n", a2[0], a2[1]);
        
        p1 = (Payoffs[a1[0]][a2[0]].r1 * c[0]) + (Payoffs[a1[1]][a2[1]].r1 * c[1]);
        p1 /= c[0] + c[1];
        p2 = (Payoffs[a1[0]][a2[0]].r2 * c[0]) + (Payoffs[a1[1]][a2[1]].r2 * c[1]);
        p2 /= c[0] + c[1];
        bargain1 = p1;
        bargain2 = p2;

        printf("firstrow = %i\n", firstrow);
        printf("firstcol = %i\n", firstcol);
        if (firstrow == 1) {
            a1[0] = 1 - a1[0];
            a1[1] = 1 - a1[1];
        }
        if (firstcol == 0) {
            a2[0] = 1 - a2[0];
            a2[1] = 1 - a2[1];
        }

        //printf("\n\nNB = (%lf, %lf)\n", bargain1, bargain2); fflush(stdout);
        
        if ((c[0] != 0) && (c[1] != 0)) {
            fprintf(fpw1, "%.5s:\t<%i, %i> (%i) and <%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], c[0], a1[1], a2[1], c[1], bargain1, bargain2);
            printf("0: %.5s:\t<%i, %i> (%i) and <%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], c[0], a1[1], a2[1], c[1], bargain1, bargain2);
            //if (gameMap[index][0] != gameMap[index][1])
            //    fprintf(fpw2, "%.5s:\t<%i, %i> and <%i, %i> -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], a1[1], a2[1], bargain1, bargain2);
        }
        else if ((c[0] == 0) && (c[1] != 0)) {
            fprintf(fpw1, "%.5s:\t<%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[1], a2[1], c[1], bargain1, bargain2);
            //printf("1: %.5s:\t<%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[1], a2[1], c[1], bargain1, bargain2);
            //if (gameMap[index][0] != gameMap[index][1])
            //    fprintf(fpw2, "%.5s:\t<%i, %i> -> (%.3lf, %.3lf)\n", games[g]+3, a1[1], a2[1], bargain1, bargain2);
        }
        else {
            fprintf(fpw1, "%.5s:\t<%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], c[0], bargain1, bargain2);
            //printf("2: %.5s:\t<%i, %i> (%i) -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], c[0], bargain1, bargain2);
            //if (gameMap[index][0] != gameMap[index][1])
            //    fprintf(fpw2, "%.5s:\t<%i, %i> -> (%.3lf, %.3lf)\n", games[g]+3, a1[0], a2[0], bargain1, bargain2);
        }

        //printf("%s: <%i, %i> and <%i, %i> -> %lf\t%lf\n", games[g], a1[0], a2[0], a1[1], a2[1], bargain1, bargain2);
    }
    fclose(fpw1);
    if (gameMap[index][0] != gameMap[index][1])
        fclose(fpw2);
    
    return (bargain1 + bargain2) / 2.0;
}

void readSelfScores() {
    int g, index, i, j, s;
    char fnombre[1024], dummy[1024];
    FILE *fp;
    double val;
    double tmp100, tmp1000, tmp50000;
    
    for (g = 0; g < 1; g++) {
        printf("reading %s\n", games[g]);
        for (index = 0; index < 144; index++) {
            for (i = 0; i < 25; i++) {
                for (j = 0; j < 25; j++) {
                    if (i != j)
                        continue;
                    tmp100 = tmp1000 = tmp50000 = 0.0;
                    scores50000[g][index][i][j] = scores1000[g][index][i][j] = scores100[g][index][i][j] = 0.0;
                    for (s = 0; s < 10; s++) {
                        if (reverseGameMap[index][1] == 0) {
                            sprintf(fnombre, "../results/%s_%i_%s_%s_%i.txt", games[g], reverseGameMap[index][0], agents[i], agents[j], s);
                            //printf("%s\n", fnombre);
                            fp = fopen(fnombre, "r");
                            
                            fscanf(fp, "%s %lf %lf", dummy, &(tmp50000), &val);
                            scores50000[g][index][i][j] += tmp50000;
                            fscanf(fp, "%s %lf %lf", dummy, &(tmp100), &val);
                            scores100[g][index][i][j] += tmp100;
                            fscanf(fp, "%s %lf %lf", dummy, &(tmp1000), &val);
                            scores1000[g][index][i][j] += tmp1000;
                            //countScores[g][index][i][j] ++;
                            
                            fclose(fp);
                        }
                        else {
                            sprintf(fnombre, "../results/%s_%i_%s_%s_%i.txt", games[g], reverseGameMap[index][0], agents[j], agents[i], s);
                            //printf("%s\n", fnombre);
                            fp = fopen(fnombre, "r");
                            
                            fscanf(fp, "%s %lf %lf", dummy, &val, &(tmp50000));
                            scores50000[g][index][i][j] += tmp50000;
                            fscanf(fp, "%s %lf %lf", dummy, &val, &(tmp100));
                            scores100[g][index][i][j] += tmp100;
                            fscanf(fp, "%s %lf %lf", dummy, &val, &(tmp1000));
                            scores1000[g][index][i][j] += tmp1000;
                            //countScores[g][index][i][j] ++;
                            
                            fclose(fp);
                        }
                    }
                    scores100[g][index][i][j] /= 10.0;
                    scores1000[g][index][i][j] /= 10.0;
                    scores50000[g][index][i][j] /= 10.0;
                }
            }
        }
    }
}

void getNBSs() {
    int g, index;
    char filename[1024];
    int i, j;
    Cell Payoffs[NumDecisionPointsMax][NumDecisionPointsMax];
    int c[2], a1[2], a2[2];
    double p1, p2;
    
    for (g = 0; g < 5; g++) {
        for (index = 0; index < 144; index++) {
            sprintf(filename, "../games/%s_%i.txt", games[g], reverseGameMap[index][0]);
            //printf("filename: %s (%i, %i)\n", filename, gameMap[index][0], gameMap[index][1]);
            FILE *fp = fopen(filename, "r");
            
            int npls, nacts1, nacts2;
            fscanf(fp, "%i", &npls);
            fscanf(fp, "%i", &nacts1);
            fscanf(fp, "%i", &nacts2);
            
            int count = 0;
            for (i = 0; i < nacts1; i++) {
                for (j = 0; j < nacts2; j++) {
                    fscanf(fp, "%lf %lf", &(Payoffs[j][i].r1), &(Payoffs[j][i].r2));
                    count ++;
                }
            }
            fclose(fp);
            
            //printG(Payoffs);
/*
            Littman(Payoffs, c, a1, a2, nacts1);
            
            p1 = (Payoffs[a1[0]][a2[0]].r1 * c[0]) + (Payoffs[a1[1]][a2[1]].r1 * c[1]);
            p1 /= c[0] + c[1];
            p2 = (Payoffs[a1[0]][a2[0]].r2 * c[0]) + (Payoffs[a1[1]][a2[1]].r2 * c[1]);
            p2 /= c[0] + c[1];
*/
            
            
            if (reverseGameMap[index][1] == 0)
                theNBSs[g][index] = p1;
            else
                theNBSs[g][index] = p2;
        }
    }
}

// Definition of competence: (1) Within 2% of the value of the NBS if someone else gets there
//                           (2) (Otherwise) Within 2% of the highest score
bool isCompetent(double valNBS, int agent, double scores[25]) {
    double delta = 0.05;
    double required = 1.0 - delta;
    
    //printf("\t%lf\n", valNBS);
    
    if (scores[agent] >= (required * valNBS))
        return true;
    
    // didn't get there; see if someelse did
    int i;
    double high = 0.0;
    for (i = 0; i < 25; i++) {
        if (i == agent)
            continue;
        
        if (high < scores[i])
            high = scores[i];
        if (scores[i] >= valNBS)
            return false;
    }
    
    // nobody got there; see if agent is within delta of the high score
    if (scores[agent] >= (required * high))
        return true;
    
    return false;
}

void assessCompetence_Game() {
    int g, index;
    double puntos[25];
    int count = 0;
    int competence[25];
    
    for (index = 0; index < 25; index++) {
        competence[index] = 0;
    }
    
    for (g = 0; g < 1; g++) {
        for (index = 0; index < 144; index++) {
            for (int n = 0; n < 25; n++) {
                puntos[n] = scores50000[g][index][n][n];
                //puntos[n] = scores1000[g][index][n][n];
            }
            
            
            for (int n = 0; n < 25; n++) {
            //    printf("%.6s = %lf (%i)\n", agents[n], puntos[n], isCompetent(theNBSs[g][index], n, puntos));
                if (isCompetent(theNBSs[g][index], n, puntos)) {
                    competence[n] ++;
                }
            }
            count++;
        }
    }
    
    for (index = 0; index < 25; index++) {
        printf("%.6s = %lf\n", agents[index], competence[index] / (double)count);
    }
}



int main(int argc, char *argv[]) {
    nameAgents();
    nameGames();
    createGameMap();
    readScores();
    //readSelfScores();
    //getNBSs();

    //assessCompetence_Game();

    //return 0;
    
    int ranker[25][25];
    theCaseAlg(2, ranker);
    int best;
    double scores[25][2];
    for (int i = 0; i < 25; i++) {
        //printf("%s\t", agents[i]);
        best = 0;
        for (int j = 0; j < 25; j++) {
            //printf("%i\t", ranker[i][j]);
            if (ranker[i][j] <= 1) {
                best++;
            }
        }
        scores[i][0] = best / 25.0;
    }
    
    int ranker2[25][144];
    theCaseGame(2, ranker2);
    printf("\n");
    for (int i = 0; i < 25; i++) {
        best = 0;
        for (int j = 0; j < 144; j++) {
            if (ranker[i][j] <= 3) {
                best ++;
            }
            //printf("%s\t%i\n", agents[i], ranker2[i][0]);
        }
        scores[i][1] = best / 144.0;
        printf("%.6s\t%lf, %lf\n", agents[i], scores[i][0], scores[i][1]);
    }
    
    return 0;
    
    // all stuff done prior to summer 2016
    
    srand(time(NULL));

	nameAgents();
    nameGames();
    //writeLaTeXTable(50000);
    //writeLaTeXTable_ranksummary(100);
    //writeLaTeXTable_ranksummary_all();
    //writeLaTeXTable_bygame_partner2();
    //writeLaTeXTable_byPartnerFamily();
    writeLaTeXTable_byGameFamily();
    return 0;


    createGameMap();
    readScores();
    
    int i, j;
    int count = 0;
    int rank[25][25];
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            theCaseCrandall(j, i, 4, rank[count]);
            count ++;
        }
    }
    
    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (j = 0; j < count; j++) {
            printf("%i\t", rank[j][i]);
        }
        printf("\n");
    }

    //createGameMap();
    //for (int j = 0; j < 78; j++)
    //    NB(j);
    //return 0;

/*    for (i = 0; i < 6; i++) {
        for (j = 0; j < 3; j++) {
            theCase(j, i, 4, rank[count]);
            count++;
        }
    }
*/
/*    createGameMap();
    readScores();

    int i, j;
    int rank[25][25];
    int count = 0;
    for (j = 0; j < 3; j++) {
        theCase(j, 6, 4, rank[count]);
        count++;
    }

    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (j = 0; j < count; j++) {
            printf("%i\t", rank[j][i]);
        }
        printf("\n");
    }
*/
/*    double vals[25];
    getWins2(2, vals, rank[0]);
    
    printf("\n");
    for (i = 0; i < 25; i++)
        printf("%s\t%lf\t%i\n", agents[i], vals[i], rank[0][i]);
*/
    
/*
    int i, j;
    double results[3][10][25];
    int ranks[3][10][25];

    createGameMap();
    readScores();
    for (j = 0; j < 6; j++) {
        computeAverages(j);
        
        for (i = 0; i < 3; i++)
            replicator(i, results[i][j], ranks[i][j]);
    }

    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (j = 0; j < 6; j++) {
            printf("%i\t%i\t%i\t", ranks[0][j][i], ranks[1][j][i], ranks[2][j][i]);
        }
        printf("\n");
    }
*/
/*
    // this code creates master.csv
    int i, j;
    double results[3][10][25];
    int ranks[3][6][25];

    createGameMap();
    readScores();
    
    computeAveragesPerGame();
    computeAveragesPerPartner();
    createMaster();
*/
/*
    // this code computes performance against each of the four classes of algorithms
    computeAveragesAll();
    for (i = 0; i < 3; i++)
        totalAverages_selected(cSTATIC, i, results[i][0], ranks[i][0]);
    for (i = 0; i < 3; i++)
        totalAverages_selected(cRL, i, results[i][1], ranks[i][1]);
    for (i = 0; i < 3; i++)
        totalAverages_selected(cEXPERT, i, results[i][2], ranks[i][2]);
    for (i = 0; i < 3; i++)
        totalAverages_selected(cOTHER, i, results[i][3], ranks[i][3]);
    
    for (i = 0; i < 25; i++) {
        printf("%s\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", agents[i],
            ranks[0][0][i], ranks[1][0][i], ranks[2][0][i],
            ranks[0][1][i], ranks[1][1][i], ranks[2][1][i],
            ranks[0][2][i], ranks[1][2][i], ranks[2][2][i],
            ranks[0][3][i], ranks[1][3][i], ranks[2][3][i]);
    }
*/
/*
    // this code computes performance in each of the six classes of games (designaged by Robison and goforth)
    for (j = 0; j < 6; j++) {
        computeAverages(j);
        for (i = 0; i < 3; i++)
            totalAverages(i, results[i][j], ranks[i][j]);
    }

    for (i = 0; i < 25; i++) {
        printf("%s\t", agents[i]);
        for (j = 0; j < 6; j++) {
            printf("%i\t%i\t%i\t", ranks[0][j][i], ranks[1][j][i], ranks[2][j][i]);
        }
        printf("\n");
    }
*/
    /*
    // this code analyzes performance across all games with respect to the 6 metrics
    createGameMap();
    readScores();
    computeAveragesAll();
//    outputAverages();

    double results[3][10][25];
    int ranks[3][6][25];
    int i, j;
    for (i = 0; i < 3; i++)
        totalAverages(i, results[i][0], ranks[i][0]);

    for (i = 0; i < 3; i++)
        getWins(i, results[i][1], ranks[i][1]);
    
    for (i = 0; i < 3; i++)
        getWorst(i, results[i][2], ranks[i][2]);
    
    for (i = 0; i < 3; i++)
        replicator(i, results[i][3], ranks[i][3]);

    for (i = 0; i < 3; i++)
        runOffs1(i, results[i][4], results[i][5], ranks[i][4]);

    for (i = 0; i < 3; i++)
        runOffs2(i, results[i][6], results[i][7], results[i][8], ranks[i][5]);

    for (j = 0; j < 3; j++) {
        printf("\n\n");
        for (i = 0; i < 25; i++) {
            printf("%s\t%.3lf\t%i\t%.2lf\t%i\t%.3lf\t%i\t%.3lf\t%i\t%lf\t%lf\t%i\t%lf\t%lf\t%lf\t%i\n", agents[i], results[j][0][i], ranks[j][0][i],
                                                    results[j][1][i], ranks[j][1][i],
                                                    results[j][2][i], ranks[j][2][i],
                                                    results[j][3][i], ranks[j][3][i],
                                                    results[j][4][i], results[j][5][i], ranks[j][4][i],
                                                    results[j][6][i], results[j][7][i], results[j][8][i], ranks[j][5][i]);
        }
    }
    
    for (i = 0; i < 25; i++) {
        printf("%s\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", agents[i],
                ranks[0][0][i], ranks[1][0][i], ranks[2][0][i],
                ranks[0][1][i], ranks[1][1][i], ranks[2][1][i],
                ranks[0][2][i], ranks[1][2][i], ranks[2][2][i],
                ranks[0][3][i], ranks[1][3][i], ranks[2][3][i],
                ranks[0][4][i], ranks[1][4][i], ranks[2][4][i],
                ranks[0][5][i], ranks[1][5][i], ranks[2][5][i]);
    }
*/

/*  // new getWorst code that I eventually threw away -- like comparing apples to oranges
    createGameMap();
    readScores();
    computeAveragesPerGame();
    
    int rank[20][25];
    int count = 0;
    int i, j;
    
    computeAveragesAll();
    double vals[25];
    getWorst2(2, vals, rank[0]);
*/

    return 0;
}