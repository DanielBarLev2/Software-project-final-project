#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

/* This C code defines a set of functions for creating,
 * manipulating, and performing operations on matrices. */

/* Function to create a matrix with given dimensions and values.
 * If values is NULL, the matrix is initialized with zeros. */
Matrix createMatrix(int rows, int cols, double **values) {
    Matrix matrix;
    int i, j;

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (double **)malloc(rows * sizeof(double *));

    if (matrix.data == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }

    for (i = 0; i < rows; i++) {
        matrix.data[i] = (double *)malloc(cols * sizeof(double));

        if (matrix.data[i] == NULL) {
            printf("An Error Has Occurred");
            exit(1);
        }

        if (values != NULL) {

            for (j = 0; j < cols; j++) {
                matrix.data[i][j] = values[i][j];
            }

        } 
        else {

            for (j = 0; j < cols; j++) {
                matrix.data[i][j] = 0.0;
            }
        }
    }

    return matrix;
}

/* Function to create a matrix with given dimensions initialized to zeros. */
Matrix createZeroMatrix(int rows, int cols) {
    Matrix mat;
    int i, j;

    mat.rows = rows;
    mat.cols = cols;
    mat.data = (double **)malloc(rows * sizeof(double *));

    if (mat.data == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }

    for (i = 0; i < rows; i++) {
        mat.data[i] = (double *)malloc(cols * sizeof(double));

        if (mat.data[i] == NULL) {
            printf("An Error Has Occurred");
            exit(1);
        }

        for (j = 0; j < cols; j++) {
            mat.data[i][j] = 0.0;
        }
    }

    return mat;
}


/* Function to free the memory allocated for a matrix. */
void freeMatrix(Matrix matrix) {
    int i;

    for (i = 0; i < matrix.rows; i++) {
        free(matrix.data[i]);
    }

    free(matrix.data);
}


/* Function to add two matrices of the same dimensions. */
Matrix addMatrix(Matrix matrix1, Matrix matrix2) {
    Matrix result;
    int i, j;

    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        printf("An Error Has Occurred");
        exit(1);
    }

    result = createMatrix(matrix1.rows, matrix1.cols, NULL);

    for (i = 0; i < matrix1.rows; i++) {

        for (j = 0; j < matrix1.cols; j++) {
            result.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
        }
    }

    return result;
}


/* Function to add two matrices of the same dimensions. */
Matrix multiplyScalarMatrix(Matrix matrix, double scalar) {
    Matrix result;
    int i, j;

    result = createMatrix(matrix.rows, matrix.cols, NULL);

    for (i = 0; i < matrix.rows; i++) {
        for (j = 0; j < matrix.cols; j++) {
            result.data[i][j] = matrix.data[i][j] * scalar;
        }
    }
    return result;
}


/* Function to print the elements of a matrix. */
void printMatrix(Matrix matrix) {
    int i, j;

    for (i = 0; i < matrix.rows; i++) {

        for (j = 0; j < matrix.cols; j++) {

            printf("%.4f", matrix.data[i][j]);

            if (j < matrix.cols - 1)
                printf(",");
            else
                printf("\n");
        }
    }
}


/* Function to compute the sum of the elements in a specific row. */
double sumRow(Matrix matrix, int row) {
    double sum = 0.0;
    int j;

    for (j = 0; j < matrix.cols; j++) {
        sum += matrix.data[row][j];
    }

    return sum;
}

/* Function to compute the sum of the elements in a specific column. */
double sumColumn(Matrix matrix, int col) {
    double sum = 0.0;
    int i;

    for (i = 0; i < matrix.rows; i++) {
        sum += matrix.data[i][col];
    }
    return sum;
}


/* Function to compute the squared Euclidean distance between two vectors. */
double squaredEuclideanDistance(double *vector1, double *vector2, int size) {
    double sum = 0.0;
    int i;

    for (i = 0; i < size; i++) {
        double diff = vector1[i] - vector2[i];
        sum += diff * diff;
    }

    return sum;
}


/* Function to raise the diagonal elements of a matrix to a given power. /
 * Power values are from R*/
Matrix powerDiagMatrix(Matrix matrix, double power) {
    Matrix result;
    int i;

    result = createMatrix(matrix.rows, matrix.cols, NULL);

    for (i = 0; i < matrix.rows; i++) {
        result.data[i][i] = pow(matrix.data[i][i], power);
    }

    return result;
}


/* Function to multiply two matrices of right sizes. */
Matrix multiplyMatrix(Matrix matrix1, Matrix matrix2) {
    Matrix result;
    int i, j, k;

    if (matrix1.cols != matrix2.rows) {
        printf("An Error Has Occurred");
        exit(1);
    }

    result = createZeroMatrix(matrix1.rows, matrix2.cols);

    for (i = 0; i < matrix1.rows; i++) {
        for (j = 0; j < matrix2.cols; j++) {
            for (k = 0; k < matrix1.cols; k++) {
                result.data[i][j] += matrix1.data[i][k] * matrix2.data[k][j];
            }
        }
    }

    return result;
}


/* Function to transpose a matrix. */
Matrix transposeMatrix(Matrix matrix) {
    Matrix result;
    int i, j;

    result = createMatrix(matrix.cols, matrix.rows, NULL);

    for (i = 0; i < matrix.rows; i++) {
        for (j = 0; j < matrix.cols; j++) {
            result.data[j][i] = matrix.data[i][j];
        }
    }

    return result;
}


/* Function to compute the Frobenius norm between two matrices. */
double frobeniusNorm(Matrix matrix1, Matrix matrix2) {
    double norm = 0.0;
    int i, j;
    for (i = 0; i < matrix1.rows; i++) {
        for (j = 0; j < matrix1.cols; j++) {
            double diff = matrix1.data[i][j] - matrix2.data[i][j];
            norm += diff * diff;
        }
    }
    return sqrt(norm);
}
