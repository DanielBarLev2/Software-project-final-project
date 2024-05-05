#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int dimension;
    double *components;  
    int centroid;
} Vector;

Vector createVector(int dimension, double *values);
Vector add(Vector vec1, Vector vec2);
Vector multiplyScalar(Vector vec, double scalar);
double euclidean_distance(Vector vec1, Vector vec2);
void printVector(Vector vec);

#endif
