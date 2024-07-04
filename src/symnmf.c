#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.c"
#include "matrix.h"

#define MAX_ROW_LEN 1024


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


Matrix readData(const char* filename, int n, int d) {
    Matrix X;
    FILE *file;
    char *token;
    int row, col;
    char line[MAX_ROW_LEN];

    X = createZeroMatrix(n, d);

    file = fopen(filename, "r");
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


Matrix ddg(Matrix A){
    int diag;
    Matrix D;

    D = createZeroMatrix(A.rows, A.cols);

    for (diag = 0; diag < A.rows; diag++){
        D.data[diag][diag] = sumRow(A, diag);
    }

    return D;
}


Matrix norm(Matrix D, Matrix A){
    Matrix W, T;

    T = powerDiagMatrix(D, (-0.5));
    W = multiplyMatrix(T, A);
    W = multiplyMatrix(W, T);

    freeMatrix(T);

    return W;
}


Matrix symnmf(char *goal, char *fileName){
    int n, d;
    Matrix X;
    Matrix A;
    Matrix D;
    Matrix W;

    getDimension(fileName, &n, &d);

    X = readData(fileName, n, d);

    if (strcmp(goal,"sym") == 0){
        A = sym(X);
        freeMatrix(X);  // Free X before returning A
        return A;
    }
    if (strcmp(goal,"ddg") == 0){
        A = sym(X);
        D = ddg(A);
        freeMatrix(X);  // Free X before returning D
        freeMatrix(A);  // Free A before returning D
        return D;
    }
    if (strcmp(goal,"norm") == 0){
        A = sym(X);
        D = ddg(A);
        W = norm(D, A);
        freeMatrix(X);  // Free X before returning W
        freeMatrix(A);  // Free A before returning W
        freeMatrix(D);  // Free D before returning W
        return W;
    }

    freeMatrix(X);
    return X;
}



void update_H(Matrix H_current, Matrix H_new, Matrix W) {
    Matrix nominator = multiplyMatrix(W, H_current);
    Matrix denominator = multiplyMatrix(multiplyMatrix(H_current, transposeMatrix(H_current)), H_current);
    double beta = 0.5;

    for (int i = 0; i < H_current.rows; i++) {
        for (int j = 0; j < H_current.cols; j++) {
            H_new.data[i][j] = H_current.data[i][j] * (1 - beta + beta * (nominator.data[i][j] / denominator.data[i][j]));
        }
    }

    freeMatrix(nominator);
    freeMatrix(denominator);
}

Matrix converge_H(Matrix H, Matrix W, double eps, int iter) {
    Matrix H_new = createZeroMatrix(H.rows, H.cols);
    Matrix H_current = H;
    Matrix temp;
    int k;

    for (k = 0; k < iter; k++) {
        update_H(H_current, H_new, W);
        if (frobeniusNorm(H_new, H_current) < eps) {
            printf("Converged after %d iterations.\n", k + 1);
            break;
        }
        temp = H_current;
        H_current = H_new;
        H_new = temp;
    }

    return H_current;
}

int main(int argc, char *argv[]) {
    int n, d;
    Matrix X;
    Matrix A;
    Matrix D;
    Matrix W;
    char *goal;
    const char *fileName;

    if (argc > 0){
        goal = argv[1];
        fileName = argv[2];
    }

    getDimension(fileName, &n, &d);

    X = readData(fileName, n, d);

    if ((strcmp(goal,"sym") == 0) || (strcmp(goal,"ddg") == 0) || (strcmp(goal,"norm") == 0)) {
        A = sym(X);

        if ((strcmp(goal,"ddg") == 0) || (strcmp(goal,"norm") == 0)){
            D = ddg(A);

            if (strcmp(goal,"norm") == 0){
                W = norm(D, A);
            }
        }

        printf("current test for matrix:\n");
        printMatrix(X);
        freeMatrix(X);  // X is freed here

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


