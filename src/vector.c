#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h" 


Vector createVector(int dimension, double *values) {
    Vector vec;
    int i; 
    vec.dimension = dimension;
    vec.centroid = -1; 

    if (values == NULL)
        vec.components = (double *)calloc(dimension, sizeof(double)); 
    else {
        vec.components = (double *)malloc(dimension * sizeof(double));
        if (vec.components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < dimension; i++) {
            vec.components[i] = values[i];
        }
    }
    return vec;
}


Vector add(Vector vec1, Vector vec2) {
    Vector result;
    int i;
    if (vec1.dimension != vec2.dimension) {
        fprintf(stderr, "Vectors must have the same dimension for addition\n");
        exit(EXIT_FAILURE);
    }

    result = createVector(vec1.dimension, NULL); 

    for (i = 0; i < vec1.dimension; i++) {
        result.components[i] = vec1.components[i] + vec2.components[i];
    }
    return result;
}


Vector multiplyScalar(Vector vec, double scalar) {
    Vector result;
    int i;
    result = createVector(vec.dimension, NULL); 

    for (i = 0; i < vec.dimension; i++) {
        result.components[i] = vec.components[i] * scalar;
    }
    return result;
}


double euclidean_distance(Vector vec1, Vector vec2) {
    double sum = 0.0;
    int i;
    if (vec1.dimension != vec2.dimension) {
        fprintf(stderr, "Vectors must have the same dimension for calculating Euclidean distance\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < vec1.dimension; i++) {
        double diff = vec1.components[i] - vec2.components[i];
        sum += diff * diff;
    }
    return sqrt(sum); 
}


void printVector(Vector vec) {
    int i;
    printf("(");

    for (i = 0; i < vec.dimension; i++) {
        printf("%.4f", vec.components[i]);
        if (i < vec.dimension - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}
