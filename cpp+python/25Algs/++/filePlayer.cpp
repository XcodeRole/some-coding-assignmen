#include "filePlayer.h"

filePlayer::filePlayer() {
    printf("incomplete filePlayer constructor\n");
    exit(1);
}

filePlayer::filePlayer(int _me, int _A[NUM_PLAYERS], double ***_M, const char *filename) {
    me = _me;
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
    
    char fnombre[1024];
    
    sprintf(fnombre, "../results/raw_roundRobin_select/%s.txt", filename);
    fp = fopen(fnombre, "r");
}

filePlayer::~filePlayer() {
    fclose(fp);
}

int filePlayer::move() {
    double p[2];
    fscanf(fp, "%lf %lf\n", &(p[0]), &(p[1]));
    
    int i, j;
    int acts[2];
    
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            if ((M[0][i][j] == p[0]) && (M[1][i][j] == p[1])) {
                acts[0] = i;
                acts[1] = j;
            }
        }
    }
    
    return acts[me];
}

void filePlayer::update(int *acts) {
}