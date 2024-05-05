#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"
#include "vector.c"
#include <math.h>

#define MAX_ROW_LEN 1024

void getDimension(const char *fileName, int* n, int* d);
Vector *convertToVectors(const char* filename, int n, int d);
Vector *sym(Vector* vectorList, int n);


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
    int i = 0;
    char *token;
    int dimension;
    double *components;
    Vector *vectorList;
    char line[MAX_ROW_LEN];

    vectorList = malloc(n * sizeof(Vector));
    if (vectorList == NULL) {
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

        vectorList[i] = createVector(d, components);
        i++;
    }
    fclose(file);

    return vectorList;
}



// Creates the similarity matrix from the input data.
Vector *sym(Vector* vectorList, int n){
    double distance;
    int current, other;
    double *components;
    Vector *outputMatrix;

    outputMatrix = malloc(n * sizeof(Vector));

    if (outputMatrix == NULL) {
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
                distance = euclidean_distance(vectorList[current], vectorList[other]);
                components[other] = exp((pow(distance, 2) / - 2));
            }
            else{
                components[other] = 0;
            }
        }
        outputMatrix[current] = createVector(n, components);
        free(components);
    }

    return outputMatrix;
}


int ddg(){
    return 0;
}


int norm(){
    return 0;
}


int main(int argc, char *argv[]) {
    int n, d;
    char *goal;
    const char *fileName;
    Vector *vectorList;
    Vector *outputMatrix;

    // for CMD testing
    // goal = argv[1];
    // fileName = argv[2];

    // for internal testing
    goal = "ddg";
    fileName = "C:/Tau/Software-Project/Software-project-final-project/data/input_1.txt";

    getDimension(fileName, &n, &d);

    vectorList = convertToVectors(fileName, n, d);

    if (strcmp(goal,"sym") == 0){
        outputMatrix = sym(vectorList, n);
    }
    if (strcmp(goal,"ddg") == 0){
        ddg();
    }

    if (strcmp(goal,"norm") == 0){
        norm();
    }

     // Print
     for (int i = 0; i < n; i++) {
         printVector(vectorList[i]);
     }
     printf("\n");

    // Print
    for (int i = 0; i < n; i++) {
        printVector(outputMatrix[i]);
    }
    printf("\n");

    free(vectorList);
    free(outputMatrix);

    return 0;
}