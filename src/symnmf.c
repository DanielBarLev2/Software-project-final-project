#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"
#include "vector.c"
#include <math.h>

#define MAX_ROW_LEN 1024

void getDimension(const char *fileName, int* n, int* d);
Vector *convertToVectors(const char* filename, int n, int d);
Vector *sym(Vector* X, int n);


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


Vector *convertToVectors(const char* filename, int n, int d) {
    Vector *X;
    int i = 0;
    char *token;
    int dimension;
    double *components;
    char line[MAX_ROW_LEN];

    X = malloc(n * sizeof(Vector));
    if (X == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        dimension = 0;
        token = strtok(line, " ");
        components = malloc(d * sizeof(double));
        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        while (token != NULL) {
            char *endptr;
            components[dimension++] = strtod(token, &endptr);
            token = strtok(NULL, " ");
        }

        X[i] = createVector(d, components);
        i++;
    }
    fclose(file);

    return X;
}


// Creates the similarity matrix from the input data.
Vector *sym(Vector* X, int n){
    double distance;
    int current, other;
    double *components;
    Vector *A;

    A = malloc(n * sizeof(Vector));

    if (A == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (current = 0; current < n; current++){
        components = malloc(n * sizeof(double));

        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (other = 0; other < n; other++){

            if (current != other){
                distance = euclidean_distance(X[current], X[other]);
                components[other] = exp((pow(distance, 2) / - 2));
            }
            else{
                components[other] = 0;
            }
        }
        A[current] = createVector(n, components);
        free(components);
    }

    return A;
}


//Creates the diagonal degree matrix from the matrix A.
Vector *ddg(Vector* A, int n){
    double *components, rowSum;
    int row, col;
    Vector *D;

    D = malloc(n * sizeof(Vector));

    for (row = 0; row < n; row++){
        components = malloc(n * sizeof(double));
        rowSum = 0;

        for (col = 0; col < n; col ++){
            rowSum += A[row].components[col];
        }

        for (col = 0; col < n; col++){
            if (col == row){
                components[col] = rowSum;
            }
            else{
               components[col] = 0.0;
            }
        }

        D[row] = createVector(n, components);

        free(components);
    }

    return D;
}


int norm(){
    return 0;
}


int main(int argc, char *argv[]) {
    int n, d;
    Vector *X;
    Vector *A;
    Vector *D;
    Vector *W;
    char *goal;
    const char *fileName;

    // for CMD testing
    goal = argv[1];
    fileName = argv[2];

    // for internal testing
    goal = "ddg";
    fileName = "C:/Tau/Software-Project/Software-project-final-project/data/input_7.txt";

    getDimension(fileName, &n, &d);

    X = convertToVectors(fileName, n, d);

    if (strcmp(goal,"sym") == 0){
        A = sym(X, n);
    }
    if (strcmp(goal,"ddg") == 0){
        A = sym(X, n);
        D = ddg(A, n);
    }

    if (strcmp(goal,"norm") == 0){
        norm();
    }

     // Print
     for (int i = 0; i < n; i++) {
         printVector(X[i]);
     }
     printf("\n");

    // Print
    for (int i = 0; i < n; i++) {
        printVector(D[i]);
    }
    printf("\n");

    free(X);
    free(A);
    free(D);

    return 0;
}