#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.c"
#include "matrix.h"

#define MAX_ROW_LEN 1024 /* Max dim for data points. */


/* 
 * Function to get the dimensions of the matrix from the file 
 * Input: fileName - name of the file containing the matrix
 * Output: n - number of rows
 *         d - number of columns
 */
void getDimension(const char *fileName, int* n, int* d) {
    char line[MAX_ROW_LEN];
    char *token;
    FILE *file;

    *n = 0, *d = 0;

    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("An Error Has Occurred");
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



/* 
 * Function to read data from a file and return it as a matrix 
 * Input: filename - name of the file containing the data
 *        n - number of rows
 *        d - number of columns
 * Return: Matrix - the data points from the file as a matrix
 */
Matrix readData(const char* filename, int n, int d) {
    char line[MAX_ROW_LEN];
    int row, col;
    char *token;
    FILE *file;
    Matrix X;

    X = createZeroMatrix(n, d);

    file = fopen(filename, "r");

    if (file == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }

    row = 0;

    /* Reading file line by line by tokenizing. */
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


/* 
 * Function to compute the similarity matrix 
 * Input: X - data matrix (n x d)
 * Return: Matrix - similarity matrix (n x n)
 */
Matrix sym(Matrix X){
    double *currentVector, *otherVector;
    int current, other;
    double distance;
    Matrix A;

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


/* 
 * Function to compute the diagonal degree matrix 
 * Input: A - similarity matrix (n x n)
 * Return: Matrix - diagonal degree matrix (n x n)
 */
Matrix ddg(Matrix A){
    int diag;
    Matrix D;

    D = createZeroMatrix(A.rows, A.cols);

    for (diag = 0; diag < A.rows; diag++){
        D.data[diag][diag] = sumRow(A, diag);
    }

    return D;
}

/* 
 * Function to compute the normalized Laplacian matrix 
 * Input: D - diagonal degree matrix (n x n)
 *        A - similarity matrix (n x n)
 * Return: Matrix - normalized Laplacian matrix (n x n)
 */
Matrix norm(Matrix D, Matrix A){
    Matrix W, T;

    T = powerDiagMatrix(D, (-0.5));
    W = multiplyMatrix(T, A);
    W = multiplyMatrix(W, T);

    freeMatrix(T);

    return W;
}

/* 
 * Python wrapper function for different goals:
 * Input: goal - goal type:
            i. sym: Calculate and output the Similarity Matrix
            ii. ddg: Calculate and output the Diagonal Degree Matrix
            iii. norm: Calculate and output the Normalized Similarity Matrix
 *        fileName - name of the file containing the data
 * Return: Matrix - the result based on the specified goal
 */
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
        freeMatrix(X);  
        return A;
    }
    if (strcmp(goal,"ddg") == 0){
        A = sym(X);
        D = ddg(A);
        freeMatrix(X);  
        freeMatrix(A);  
        return D;
    }
    if (strcmp(goal,"norm") == 0){
        A = sym(X);
        D = ddg(A);
        W = norm(D, A);
        freeMatrix(X);  
        freeMatrix(A);  
        freeMatrix(D);  
        return W;
    }

    freeMatrix(X);
    return X;
}


/* 
 * Python wrapper helper function to update H matrix for convarge_H
 * Input: H_current - current H matrix (n x k)
 *        W - weight matrix (n x n)
 * Return: Matrix - updated H matrix (n x k)
 */
Matrix update_H(Matrix H_current, Matrix W) {
    Matrix denominator = multiplyMatrix(multiplyMatrix(H_current, transposeMatrix(H_current)), H_current);
    Matrix H_new = createZeroMatrix(H_current.rows, H_current.cols);
    Matrix nominator = multiplyMatrix(W, H_current);
    double beta = 0.5;
    int i, j;

    for (i = 0; i < H_current.rows; i++) {
        for (j = 0; j < H_current.cols; j++) {
            H_new.data[i][j] = H_current.data[i][j] * (1 - beta + beta * (nominator.data[i][j] / denominator.data[i][j]));
        }
    }

    freeMatrix(nominator);
    freeMatrix(denominator);

    return H_new;
}


/* 
 * Python wrapper function to iteratively update H matrix until convergence 
 * Input: H - initial H matrix (n x k)
 *        W - weight matrix (n x n)
 *        eps - convergence threshold. def = 0.0001
 *        iter - maximum number of iterations. def = 300
 * Return: Matrix - converged H matrix (n x k)
 */
Matrix converge_H(Matrix H, Matrix W, double eps, int iter) {
    Matrix H_new = createZeroMatrix(H.rows, H.cols);
    int k;

    for (k = 0; k < iter; k++) {
        H_new = update_H(H, W);

        if (frobeniusNorm(H_new, H) < eps) {
            printf("Converged after %d iterations.\n", k + 1);
            break;
        }
        
        H = H_new;
    }

    return H_new;
}


/* 
 * Main function to run different goals based on input arguments 
 * Input: argc - number of command-line arguments
 *        argv - array of command-line arguments
 * Return: int - exit status
 */
int main(int argc, char *argv[]) {
    const char *fileName;
    char *goal;
    int n, d;
    Matrix X;
    Matrix A;
    Matrix D;
    Matrix W;

    if (argc > 0){
        goal = argv[1];
        fileName = argv[2];
    }
    else{
         printf("An Error Has Occurred\n");
         exit(1);
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
        

            if (strcmp(goal, "sym") == 0){
                    printMatrix(A);
                    freeMatrix(A);
            }
            else if (strcmp(goal, "ddg") == 0){
                    printMatrix(D);
                    freeMatrix(D);
                    freeMatrix(A); 
            } 
            else if (strcmp(goal, "norm") == 0){
                    printMatrix(W);
                    freeMatrix(W);
                    freeMatrix(D); 
                    freeMatrix(A); 
        }

    } 
    else{
        printf("An Error Has Occurred");
        exit(1);
    }

    return 0;
}
