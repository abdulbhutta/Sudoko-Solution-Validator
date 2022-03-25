#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int board[9][9];
int valid[10];
int subgridThread= 2;

/* structure for passing data to threads */
typedef struct {
    int row;
    int column;
} parameters; 

void getPuzzle() {
    FILE *fp;

    // Open file
    fp = fopen("puzzleSolved.txt", "r");
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

void *columnCheck(void *parameter){
    //Go through each row
    for (int i=0; i<9; i++) {
        //sorted array for the column
        int array[9] = {0};
        //Go through each column
        for (int j=0; j<9; j++){
            int value = board[j][i];
            //If the number in column is greater than 1 AND less than 9 AND the sorted array position is 0
            if (array[value-1] == 0 && value > 0 && value <= 9){
                array[value-1] = value;
            } 
            //The number is a duplicate or not in range, change column check thread to 0 as its invalid
            else {
                valid[0] = 0;
                pthread_exit(0);
            }
        }
    }

    //The column check thread is valid
    valid[0] = 1;
    pthread_exit(0);
}

void *rowCheck (void *parameter){
      //Go through each column
    for (int i=0; i<9; i++) {
        //sorted array for the row
        int array[9] = {0};
        //Go through each row
        for (int j=0; j<9; j++){
            int value = board[i][j];
            //If the number in column is less than 1 or the region is valid, exit the thread
            if (array[value-1] == 0 && value > 0){
                array[value - 1] = value;
            } 
            //The number has been 
            else {
                valid[1] = 0;
                pthread_exit(0);
            }
        }
    }

    valid[1] = 1;
    pthread_exit(0);
}

void *subgridCheck (void *parameter){
    parameters *subgrid = (parameters *) parameter;
    int row = subgrid->row;
    int column = subgrid->column;

    //initialize all values in array to 0 and then store sorted array for 3x3 grid
    int array[9] = {0};

    for (int i=row; i<row+3; i++) {
        //Go through each row
        for (int j=column; j<column+3; j++){
            int value = board[i][j];
             //If the number in column is greater than 1 AND less than 9 AND the sorted array position is 0
            if (array[value-1] == 0 && value > 0 && value <= 9){
                array[value - 1] = value;
            } 
            //The number has been 
            else {
                valid[subgridThread] = 0;
                subgridThread++;
                printf("invalid \n");
                pthread_exit(0);
            }
        }
    }
    //The subgrid is valid and add it to valid thread array
    valid[subgridThread] = 1;
    //Increment the subgrid thread 
    subgridThread++;
    pthread_exit(0);
}

int main (int argc, char *argv[]){
    //Get the puzzle to solve from the text file
    getPuzzle();
    //Create 11 threads
    pthread_t tid[11];

    //11 threads
    for (int thread = 1; thread <= 11; thread++)
        //Thread 1 is used to check if each column contains digit 1 to 9.
        if (thread == 1) {
            pthread_create(&tid[thread], NULL, columnCheck, NULL);
            pthread_join(tid[thread], NULL);
        }
        //Thead 2 is used to use if each row contains digit 1 to 9.
        else if (thread == 2){
            pthread_create(&tid[thread], NULL, rowCheck, NULL);
            pthread_join(tid[thread], NULL);
        }
        //Threads 3 - 11 or 9 threads are used to check 3x3 subgrids
        else {
            //Start column at 0
            for (int i=0; i<9; i++){
                //Start row at 0
                for (int j=0; j<9; j++){
                    //If the row and column is divided by 3 then its a 3x3 subgrid 
                    if (i%3 == 0 && j%3 == 0){
                        //Store the parameters for row and column based on the subgrid
                        parameters *parameter = (parameters *) malloc(sizeof(parameters));
                        parameter->row = i;
                        parameter->column = j;
                        //Create the threads and perform check on the subgrid
                        pthread_create(&tid[thread], NULL, subgridCheck, parameter);
                        //Wait until the thead is done
                        pthread_join(tid[thread], NULL);
                    }
                }
                //Go to the next thread
                thread++;
            }
        }

    for (int i=0; i<11; i++)
        printf("%d", valid[i]);
        
    return(0);
}

