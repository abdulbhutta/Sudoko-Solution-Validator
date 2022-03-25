#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//initialize global variables
int board[9][9];
int valid[10];
int subgridThread= 2;

/* structure for passing data to threads */
typedef struct {
    int row;
    int column;
} parameters; 

//Methods to valid all the columns
void *validateColumn(void *parameter){
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

//Methods to valid all the rows
void *validateRow (void *parameter){
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

//Method to validate all subgrids
void *validateSubgrid (void *parameter){
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

//Get the puzzle to solve from a file
void getPuzzle() {
    FILE *fp;

    // Open file
    fp = fopen("puzzle.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    //Read the files row and columns into the 2D array
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            fscanf(fp, "%1d", &board[i][j]);
        }
    }

    //Close file
    fclose(fp);
}

//Checks if the number is valid for the row and column only!
int numberCheck(int number, int row, int column) {
    //Begin the row/column from 0, 3, or 6 for 3x3 subgrid
    int beginRow = row - (row % 3);
    int beginColumn = column - (column % 3);

    //Check the whole column
    for (int i=0; i<9; i++)
        //If the number already exist, return false
        if (board[row][i] == number)
            return 0;
    
    //Check the whole row
    for (int i=0; i<9; i++)
        //If the number already exist, return false
        if (board[i][column] == number)
            return 0;
    
    //Check the current 3x3 for the current row 
    for (int i = 0; i < 3; i++)
        //Check the current 3x3 for the current column
        for (int j = 0; j < 3; j++)
            //If the number already exist in the 3x3 grid, then return false
            if (board[i + beginRow][j + beginColumn] == number)
                return 0;

    //The number is correct, return true
    return 1;
} 

//Uses recursion to solve if the number 1-9 fit the puzzle
int sudokoSolver (int row, int column){
    //If the row is 8 and column is 8, we have reached the end
    if (row == 8 && column == 8){
        return 1;
    }
    //If the column is 8, then we have reached the end of the column,
    //Increment the row and start traversing through that
    if (column == 8){
        row++;
        column=0;
    }

    //If the vallue of the current number is 1 - 9, then proceed to the next number
    if (board[row][column] > 0 && board[row][column] <= 9){
        return sudokoSolver(row, column + 1);
    }

    //Check all numbers 1 - 9 and if it is valid
    for (int i=1; i<=9; i++){
        if (numberCheck(i, row, column)) {
            board[row][column] = i;
            if (sudokoSolver(row, column + 1)) {
            return 1;
            }
        }

        //The number guessed was wrong, switch it back to 0 to find the correct number
        board[row][column] = 0;
    }
    return 0;
} 

//Write the solution to a file
void writeSolution (int board[9][9]){
    FILE *fp;

    // Open file to write the solution in
    fp = fopen("solution.txt", "w");
    if (fp == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }
    
    //Loop through the 2-D array and print it out
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            fprintf(fp, "%d ", board[i][j]);
        }
        fprintf(fp, "\n");
    }

    //Close file
    fclose(fp);
}

//Print the current board
void printBoard(int board[9][9]) {
     for (int i = 0; i < 9; i++) {
         for (int j = 0; j < 9; j++){
            printf("%d ", board[i][j]);
         }
            printf("\n");
       }
}

int main (int argc, char *argv[]){
    //Get the puzzle to solve from the text file
    getPuzzle();
    printBoard(board);
    
    //Create 11 threads
    pthread_t tid[11];
 
    if (sudokoSolver(0, 0)){
        printf("\n");
        printBoard(board);
        writeSolution(board);
    }

    else {
        printf("NOT WORKING");
    }

    /*pthread_create(&tid[0], NULL, columnOnly, NULL);
    pthread_join(tid[0], NULL);
    */

    /*
    //11 threads
    for (int thread = 1; thread <= 11; thread++)
        //Thread 1 is used to check if each column contains digit 1 to 9.
        if (thread == 1) {
            pthread_create(&tid[thread], NULL, validateColumn, NULL);
            pthread_join(tid[thread], NULL);
        }
        //Thead 2 is used to use if each row contains digit 1 to 9.
        else if (thread == 2){
            pthread_create(&tid[thread], NULL, validateRow, NULL);
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
                        pthread_create(&tid[thread], NULL, validateSubgrid, parameter);
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
    */    
    return(0);
}

