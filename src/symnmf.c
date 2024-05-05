#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.c"
#include "matrix.h"

#define MAX_ROW_LEN 1024

void getDimension(const char *fileName, int* n, int* d);
Matrix readData(const char* filename, int n, int d);
Matrix sym(Matrix X);
Matrix ddg(Matrix A);


void getDimension(const char *fileName, int* n, int* d) {
    FILE *file;
    char line[MAX_ROW_LEN];
    char *token;

    *n = 0, *d = 0;

    file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", fileName);
        exit(1);
    }

    if (fgets(line, sizeof(line), file) != NULL) {
        // Count numbers
        token = strtok(line, " \t\n");
        while (token != NULL) {
            (*d)++;
            token = strtok(NULL, " \t\n");
        }
        (*n)++;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        (*n)++;
    }

    fclose(file);
}

// Reads the data from the input file.
Matrix readData(const char* filename, int n, int d) {
    Matrix X;
    char *token;
    int row, col;
    char line[MAX_ROW_LEN];

    X = createZeroMatrix(n, d);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    row = 0;
    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, " ");
        col = 0;

        while (token != NULL) {
            char *endptr;
            X.data[row][col] = strtod(token, &endptr);
            token = strtok(NULL, " ");
            col++;
        }
        row++;
    }

    fclose(file);

    return X;
}


// Creates the similarity matrix from the input data.
Matrix sym(Matrix X){
    Matrix A;
    double distance;
    int current, other;
    double *currentVector, *otherVector;

    A = createZeroMatrix(X.rows, X.rows);

    for (current = 0; current < X.rows; current++){
        for (other = 0; other < X.rows; other++){
            if (current != other){
                currentVector = X.data[current];
                otherVector = X.data[other];
                distance = squaredEuclideanDistance(currentVector, otherVector, X.cols);
                A.data[current][other] =  exp((distance / -2));
            }
            else{
                A.data[current][other] = 0.0;
            }
        }
    }

    return A;
}


//Creates the diagonal degree matrix from the matrix A.
Matrix ddg(Matrix A){
    int diag;
    Matrix D;

    D = createZeroMatrix(A.rows, A.cols);

    for (diag = 0; diag < A.rows; diag++){
        D.data[diag][diag] = sumRow(A, diag);
    }

    return D;
}


int norm(){
    return 0;
}


int main(int argc, char *argv[]) {
    int n, d;
    Matrix X;
    Matrix A;
    Matrix D;
    Matrix W;
    char *goal;
    const char *fileName;

    // for CMD testing
    goal = argv[1];
    fileName = argv[2];

    // for internal testing
    goal = "ddg";
    fileName = "C:/Tau/Software-Project/Software-project-final-project/data/input_7.txt";

    getDimension(fileName, &n, &d);

    X = readData(fileName, n, d);

    if ((strcmp(goal,"sym") == 0) || (strcmp(goal,"ddg") == 0) || (strcmp(goal,"norm") == 0)) {
        A = sym(X);

        if ((strcmp(goal,"ddg") == 0) || (strcmp(goal,"norm") == 0)){
            D = ddg(A);

            if (strcmp(goal,"norm") == 0){
                norm();
            }
        }

        printf("current test for matrix:\n");
        printMatrix(X);
        freeMatrix(X);

        if (strcmp(goal,"sym") == 0){
            printMatrix(A);
            freeMatrix(A);
        }
        if (strcmp(goal,"ddg") == 0){
            printMatrix(D);
            freeMatrix(D);
        }
        if (strcmp(goal,"norm") == 0){
            printMatrix(W);
            freeMatrix(W);
        }
    }
    return 0;
}