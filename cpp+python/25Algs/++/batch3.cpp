#include "defs.h"

#define ROUNDS  50000
#define Thresh  100

typedef struct {
    char game[1024];
    char player1[1024], player2[1024];
    int acts[ROUNDS][2];
    int interLength;
    double payoffs[ROUNDS][2];
    int group;
    bool coops[ROUNDS][2];
    bool mutual[ROUNDS];
    int betrayals_time[100][2];
    int accountable[100];
} anInteraction;

anInteraction theGames[150 * 3 * 4];
int numInteractions = 0;

void readInteractions(const char _game[1024], const char _a1[1024], const char _a2[1024], int numSamps, int numRounds) {
    int i, j;
    FILE *fp;
    char fnombre[1024];
    
    printf("Read Interaction: %s vs %s in %s\n", _a1, _a2, _game);
    
    for (i = 0; i < numSamps; i++) {
        sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", _game, _a1, _a2, i);
        fp = fopen(fnombre, "r");
        
        strcpy(theGames[numInteractions].game, _game);
        strcpy(theGames[numInteractions].player1, _a1);
        strcpy(theGames[numInteractions].player2, _a2);
        
        theGames[numInteractions].interLength = numRounds;
        
        for (j = 0; j < numRounds; j++) {
            fscanf(fp, "%i %i %lf %lf", &(theGames[numInteractions].acts[j][0]), &(theGames[numInteractions].acts[j][1]),
                   &(theGames[numInteractions].payoffs[j][0]), &(theGames[numInteractions].payoffs[j][1]));
            
            //printf("%i, %i, %lf, %lf\n", theGames[numInteractions].acts[j][0], theGames[numInteractions].acts[j][1],
            //       theGames[numInteractions].payoffs[j][0], theGames[numInteractions].payoffs[j][1]);
        }
        
        theGames[numInteractions].group = i;
        
        fclose(fp);
        
        numInteractions++;
    }
}

void computeCoops() {
    int i, j;
    double sum1, sum2, sum3, sum4;
    
    for (i = 0; i < numInteractions; i++) {
        //printf("%s: %s vs %s (%i)\n", theGames[i].game, theGames[i].player1, theGames[i].player2, theGames[i].group); fflush(stdout);
        for (j = 0; j < ROUNDS; j++) {
            if (!strcmp("prisoners", theGames[i].game)) {
                if (theGames[i].acts[j][0] == 0)
                    theGames[i].coops[j][0] = true;
                if (theGames[i].acts[j][1] == 0)
                    theGames[i].coops[j][1] = true;
                if ((theGames[i].acts[j][0] == 0) && (theGames[i].acts[j][1] == 0))
                    theGames[i].mutual[j] = true;
            }
            else if (!strcmp("chicken2", theGames[i].game)) {
                if (theGames[i].acts[j][0] == 1)
                    theGames[i].coops[j][0] = true;
                if (theGames[i].acts[j][1] == 1)
                    theGames[i].coops[j][1] = true;
                if ((theGames[i].acts[j][0] == 1) && (theGames[i].acts[j][1] == 1))
                    theGames[i].mutual[j] = true;
            }
            else { // blocks2
                if (j == 0) {
                    sum1 = theGames[i].payoffs[0][0] + theGames[i].payoffs[1][0];
                    sum2 = theGames[i].payoffs[0][1] + theGames[i].payoffs[1][1];
                    if ((sum1 > 1.39) && (sum2 > 1.39)) {
                        theGames[i].mutual[0] = true;
                        
                        theGames[i].coops[0][0] = theGames[i].coops[0][1] = true;
                    }
                    else {
                        theGames[i].mutual[0] = false;
                        theGames[i].coops[0][0] = theGames[i].coops[0][1] = false;
                    }
                }
                else {
                    sum1 = theGames[i].payoffs[j][0] + theGames[i].payoffs[j-1][0];
                    sum2 = theGames[i].payoffs[j][1] + theGames[i].payoffs[j-1][1];
                    if (j != ROUNDS-1) {
                        sum3 = theGames[i].payoffs[j][0] + theGames[i].payoffs[j+1][0];
                        sum4 = theGames[i].payoffs[j][1] + theGames[i].payoffs[j+1][1];
                    }
                    else {
                        sum3 = sum4 = 0.0;
                    }
                    if (((sum1 > 1.39) || sum3 > 1.39) && ((sum2 > 1.39) || (sum4 > 1.39))) {
                        theGames[i].mutual[j] = true;
                        theGames[i].coops[j][0] = theGames[i].coops[j][1] = true;
                    }
                    else {
                        theGames[i].mutual[j] = false;
                        
                        if (theGames[i].mutual[j-1]) {
                            if ((theGames[i].acts[j-1][0] == 0) && (theGames[i].acts[j-1][1] == 2)) {
                                if (theGames[i].acts[j][0] == 2)
                                    theGames[i].coops[j][0] = true;
                                else
                                    theGames[i].coops[j][0] = false;
                                
                                if (theGames[i].acts[j][1] == 0)
                                    theGames[i].coops[j][1] = true;
                                else
                                    theGames[i].coops[j][1] = false;
                            }
                            else {
                                theGames[i].coops[j][0] = theGames[i].coops[j][1] = false;
                            }
                        }
                    }
                }
            }
            
            //printf("%i: (%i, %i) -> %i\t%i,%i\n", j, theGames[i].acts[j][0], theGames[i].acts[j][1], theGames[i].mutual[j], theGames[i].coops[j][0], theGames[i].coops[j][1]);
        }
    }
}

void betrayals() {
    int i, j;
    int chances;
    
    for (i = 0; i < numInteractions; i++) {
        chances = 0;
        
        for (j = 0; j < 100; j++) {
            theGames[i].betrayals_time[j][0] = theGames[i].betrayals_time[j][1] = 0;
            theGames[i].accountable[j] = 0;
        }
        
        for (j = 2; j < ROUNDS; j++) {
            if (theGames[i].mutual[j-1] && theGames[i].mutual[j-2]) {
                if (theGames[i].coops[j][0] == false)
                    theGames[i].betrayals_time[chances][0] = 1;
                if (theGames[i].coops[j][1] == false)
                    theGames[i].betrayals_time[chances][1] = 1;
                
                theGames[i].accountable[chances] = 1;
                chances ++;
                if (chances == 100) {
                    break;
                }
            }
        }
    }
}

int getIndex(char player1[1024]) {
    if (!strcmp(player1, "S++"))
        return 0;
    else if (!strcmp(player1, "S"))
        return 1;
    else if (!strcmp(player1, "eeew"))
        return 2;
    else
        return 3;
}

void aveBetrayal() {
    int betrays[4][100];
    int counts[4][100];
    int i, j, index;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 100; j++) {
            betrays[i][j] = 0;
            counts[i][j] = 0;
        }
    }
    
    for (i = 0; i < numInteractions; i++) {
        index = getIndex(theGames[i].player1);
        for (j = 0; j < 100; j++) {
            betrays[index][j] += theGames[i].betrayals_time[j][0] + theGames[i].betrayals_time[j][1];
            counts[index][j] += 2 * theGames[i].accountable[j];
        }
    }
    
    
    for (i = 0; i < 100; i++) {
        printf("%i: ", i);
        for (j = 0; j < 4; j++) {
            if (counts[j][i] != 0)
                printf("%.3lf (%i)\t", betrays[j][i] / (double)(counts[j][i]), counts[j][i]);
            else
                printf("----\t");
        }
        printf("\n");
    }
/*
    printf("S++ : %lf (%i)\n", vals[0] / counts[0], counts[0]);
    printf("S   : %lf (%i)\n", vals[1] / counts[1], counts[1]);
    printf("EEE : %lf (%i)\n", vals[2] / counts[2], counts[2]);
    printf("Exp3: %lf (%i)\n", vals[3] / counts[3], counts[3]);
*/
}

void speed_loyalty() {
    int i, j, k;
    int speed;
    int loyalty;
    
    FILE *fp = fopen("../speed_loyalty/speed_loyalty.csv", "w");
    fprintf(fp, "Game,Algorithm,Speed,Loyalty\n");
    for (i = 0; i < numInteractions; i++) {
        speed = -1;
        loyalty = -1;
        for (j = 1; j < 50; j++) {
            if (j >= theGames[i].interLength)  // account for blocks
                break;
            
            if (theGames[i].mutual[j-1] && theGames[i].mutual[j]) {
                //printf("got here: %i ... ", j);
                speed = 50 - (j-1);
                //printf("speed = %i\n", speed);
                
                loyalty = 0;
                for (k = j+1; k < (j-1)+22; k++) {
                    if (k >= theGames[i].interLength) {
                        loyalty = 20;
                        break;
                    }
                    
                    if (theGames[i].mutual[k])
                        loyalty ++;
                    else
                        break;
                    
                }
                //printf("loyalty = %i\n", loyalty);
                break;
            }
        }
        if (!strcmp(theGames[i].player1, theGames[i].player2))
            fprintf(fp, "%s,%s,%lf,%lf\n", theGames[i].game, theGames[i].player1, speed / 50.0, loyalty / 20.0);
        else {
            if ((!strcmp(theGames[i].player1, "megaS++-")) || (!strcmp(theGames[i].player2, "megaS++-")))
                fprintf(fp, "%s,HS-,%lf,%lf\n", theGames[i].game, speed / 50.0, loyalty / 20.0);
            else
                fprintf(fp, "%s,HS,%lf,%lf\n", theGames[i].game, speed / 50.0, loyalty / 20.0);
        }
    }
    fclose(fp);
    
}

void reverseIndex(int index, char nombre[1024]) {
    switch (index) {
        case 0: strcpy(nombre, "S#-S# (no)"); break;
        case 1: strcpy(nombre, "S#-S# (yes)"); break;
        case 2: strcpy(nombre, "EEE-EEE (no)"); break;
        case 3: strcpy(nombre, "Exp3-Exp3 (no)"); break;
        case 4: strcpy(nombre, "S-S (no)"); break;
        case 5: strcpy(nombre, "Human-Human (no)"); break;
        case 6: strcpy(nombre, "Human-Human (yes)"); break;
        case 7: strcpy(nombre, "Human-S# (no)"); break;
        case 8: strcpy(nombre, "Human-S# (yes)"); break;
    }
}

void reverseAlg(int index, char nombre[1024]) {
    switch (index) {
        case 0: strcpy(nombre, "S#-S#"); break;
        case 1: strcpy(nombre, "S#-S#"); break;
        case 2: strcpy(nombre, "EEE-EEE"); break;
        case 3: strcpy(nombre, "Exp3-Exp3"); break;
        case 4: strcpy(nombre, "S-S"); break;
        case 5: strcpy(nombre, "Human-Human"); break;
        case 6: strcpy(nombre, "Human-Human"); break;
        case 7: strcpy(nombre, "Human-S#"); break;
        case 8: strcpy(nombre, "Human-S#"); break;
    }
}

int getIndex(char p1[1024], char p2[1024]) {
    if ((!strcmp(p2, "Human") && !strcmp(p1, "megaS++")) || (!strcmp(p1, "Human") && !strcmp(p2, "megaS++")))
        return 8;
    else if (!strcmp(p1, "Human-") && !strcmp(p2, "Human-"))
        return 5;
    else if (!strcmp(p1, "Human") && !strcmp(p2, "Human"))
        return 6;
    else if ((!strcmp(p2, "Human-") && !strcmp(p1, "megaS++-")) || (!strcmp(p1, "Human-") && !strcmp(p2, "megaS++-")))
        return 7;
    else if (!strcmp(p1, "megaS++-"))
        return 0;
    else if (!strcmp(p1, "megaS++"))
        return 1;
    else if (!strcmp(p1, "megaEEE-"))
        return 2;
    else if (!strcmp(p1, "megaExp3-"))
        return 3;
    else if (!strcmp(p1, "megaS-"))
        return 4;
    
    printf("problem\n");
    
    return -1;
}

int findMutualCoopPoint(int index) {
    int i;
    
    for (i = 1; i < 50; i++) {
        if (i >= theGames[index].interLength)  // account for blocks
            break;
        
        if (theGames[index].mutual[i-1] && theGames[index].mutual[i])
            return i-1;
    }
    
    return 51;
}

void computeSpeedCDFs() {
    int i, j, index, point;
    double cdfs[9][50];
    int counts[9][50];
    

    
    for (j = 0; j < 50; j++) {
        for (i = 0; i < 9; i++) {
            cdfs[i][j] = counts[i][j] = 0;
        }
    }
    
    for (i = 0; i < numInteractions; i++) {
        index = getIndex(theGames[i].player1, theGames[i].player2);
        point = findMutualCoopPoint(i);
        
        for (j = 0; j < 50; j++) {
            if (j >= point)
                cdfs[index][j]+=1;
            counts[index][j] ++;
        }
    }
    
    
    FILE *fp = fopen("../speed_loyalty/speedCDF.csv", "w");
    fprintf(fp, "Code,Algorithm,Comms,Round,Value\n");
    
    char nombre[1024];
    for (j = 0; j < 50; j+=2) {
        for (i = 0; i < 9; i++) {
            cdfs[i][j] /= counts[i][j];
            printf("%.2lf  ", cdfs[i][j]);
            reverseAlg(i, nombre);
            if ((i == 1) || (i == 6) || (i == 8))
                fprintf(fp, "-%i-,%s,Yes,%i,%lf\n", i, nombre, j+1, cdfs[i][j]);
            else
                fprintf(fp, "-%i-,%s,No,%i,%lf\n", i, nombre, j+1, cdfs[i][j]);
        }
        printf("\n");
    }
    
    fclose(fp);
}

void computeLoyaltyCDFs() {
    int i, j, index, point;
    double cdfs[9][21];
    int counts[9][21];
    
    
    
    for (j = 0; j < 21; j++) {
        for (i = 0; i < 9; i++) {
            cdfs[i][j] = counts[i][j] = 0;
        }
    }
    
    for (i = 0; i < numInteractions; i++) {
        index = getIndex(theGames[i].player1, theGames[i].player2);
        point = findMutualCoopPoint(i);
        
        if (index != 8)
            printf("wrong one: %s %s\n", theGames[i].player1, theGames[i].player2);
        
        if (point > 50)
            continue;
        
        bool off = false;
        for (j = 0; j < 21; j++) {
            if ((j + point+1) >= theGames[i].interLength) {
                if (off) {
                    for (int k = j; k < 21; k++)
                        counts[index][k] ++;
                }
                break;
            }
            
            if (!(theGames[i].mutual[j+point+1])) {
                if (j == 0) {
                    printf("really problem: %i\n", point+1);
                    exit(1);
                }
                off = true;
            }
            
            if (!off)
                cdfs[index][j]+=1;
            
            counts[index][j] ++;
        }
    }
    
    printf("\n");
    
    FILE *fp = fopen("../speed_loyalty/loyaltyCDF.csv", "w");
    fprintf(fp, "Code,Algorithm,Comms,Round,Value\n");
    
    char nombre[1024];
    for (j = 0; j < 21; j++) {
        for (i = 0; i < 9; i++) {
            cdfs[i][j] /= counts[i][j];
            printf("%.2lf  ", cdfs[i][j]);
            reverseAlg(i, nombre);
            if ((i == 1) || (i == 6) || (i == 8))
                fprintf(fp, "-%i-,%s,Yes,%i,%lf\n", i, nombre, j, cdfs[i][j]);
            else
                fprintf(fp, "-%i-,%s,No,%i,%lf\n", i, nombre, j, cdfs[i][j]);
        }
        printf("\n");
    }
    
    fclose(fp);
}


int main() {
    // read in each interaction
    readInteractions("prisoners", "megaS++-", "megaS++-", 50, 1000);
    readInteractions("prisoners", "megaS++", "megaS++", 50, 1000);
    readInteractions("prisoners", "megaS-", "megaS-", 50, 1000);
    readInteractions("prisoners", "megaEEE-", "megaEEE-", 50, 1000);
    readInteractions("prisoners", "megaExp3-", "megaExp3-", 50, 1000);
    readInteractions("prisoners", "Human-", "Human-", 10, 51);
    readInteractions("prisoners", "Human", "Human", 12, 51);
    readInteractions("prisoners", "megaS++-", "Human-", 7, 51);
    readInteractions("prisoners", "Human-", "megaS++-", 3, 51);
    readInteractions("prisoners", "megaS++", "Human", 6, 51);
    readInteractions("prisoners", "Human", "megaS++", 6, 51);

    readInteractions("chicken2", "megaS++-", "megaS++-", 50, 1000);
    readInteractions("chicken2", "megaS++", "megaS++", 50, 1000);
    readInteractions("chicken2", "megaS-", "megaS-", 50, 1000);
    readInteractions("chicken2", "megaEEE-", "megaEEE-", 50, 1000);
    readInteractions("chicken2", "megaExp3-", "megaExp3-", 50, 1000);
    readInteractions("chicken2", "Human-", "Human-", 10, 54);
    readInteractions("chicken2", "Human", "Human", 11, 54);
    readInteractions("chicken2", "megaS++-", "Human-", 5, 54);
    readInteractions("chicken2", "Human-", "megaS++-", 5, 54);
    readInteractions("chicken2", "megaS++", "Human", 6, 54);
    readInteractions("chicken2", "Human", "megaS++", 6, 54);

    readInteractions("blocks2", "megaS++-", "megaS++-", 50, 1000);
    readInteractions("blocks2", "megaS++", "megaS++", 50, 1000);
    readInteractions("blocks2", "megaS-", "megaS-", 50, 1000);
    readInteractions("blocks2", "megaEEE-", "megaEEE-", 50, 1000);
    readInteractions("blocks2", "megaExp3-", "megaExp3-", 50, 1000);
    readInteractions("blocks2", "Human-", "Human-", 10, 47);
    readInteractions("blocks2", "Human", "Human", 12, 47);
    readInteractions("blocks2", "megaS++-", "Human-", 5, 47);
    readInteractions("blocks2", "Human-", "megaS++-", 5, 47);
    readInteractions("blocks2", "megaS++", "Human", 4, 47);
    readInteractions("blocks2", "Human", "megaS++", 7, 47);

    printf("\nCompute the Coops\n");
    
    computeCoops();
    
    computeSpeedCDFs();
    computeLoyaltyCDFs();
    //speed_loyalty();
    //betrayals();
    
    //aveBetrayal();
    
    return 0;
}
