#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int board[9][9];
/* structure for passing data to threads */
typedef struct {
    int row;
    int column;
} parameters; 

void getPuzzle() {
    FILE *fp;

    // Open file
    fp = fopen("puzzle.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            fscanf(fp, "%1d", &board[i][j]);
        }
    }

    //Close file
    fclose(fp);
}


void *columnCheck(void *parameters){
    

    return NULL;
}


int main (int argc, char *argv[]){

    pthread_t tid[11];

    parameters *first = (parameters *) malloc(sizeof(parameters));
    first->row = 0;
    first->column = 0;

    pthread_create(&tid[0], NULL, columnCheck, first);


    getPuzzle();
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    return(0);
}

