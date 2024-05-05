#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "vector.c"

#define MAX_ROW_LEN 1024

void getDimension(const char *fileName, int* n, int* d);
Vector *convertToVectors(const char* filename, int n, int d);


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


// This function reads data from a file, tokenizes each line, and stores the values as components of Vector objects.
Vector* convertToVectors(const char* filename, int n, int d) {
    int i = 0;
    Vector *vectorsList;
    char line[MAX_ROW_LEN];

    vectorsList = malloc(n * sizeof(Vector));

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    if (vectorsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " ");
        int dimension = 0;
        double *components = malloc(d * sizeof(double)); 
        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        while (token != NULL) {
            components[dimension++] = atof(token);
            token = strtok(NULL, " ");
        }

        vectorsList[i].dimension = dimension;
        vectorsList[i].components = components;
        vectorsList[i].centroid = 0;
        i++;
    }

    fclose(file);

    return vectorsList;
}


int sym(){
    return 0;
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

    // for CMD testing
    // goal = argv[1];
    // fileName = argv[2];

    // for internal testing
    goal = "sym";
    fileName = "C:/Tau/Software-Project/Software-project-final-project/data/input_1.txt";

    getDimension(fileName, &n, &d);

    vectorList = convertToVectors(fileName, n, d);

    if (strcmp(goal,"sym") == 0){
        sym();
    }
    if (strcmp(goal,"ddg") == 0){
        ddg();
    }

    if (strcmp(goal,"norm") == 0){
        norm();
    }

     // Print final centroids
     for (int i = 0; i < n; i++) {
         printVector(vectorList[i]);
     }
     printf("\n");

    free(vectorList);

    return 0;
}