#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

Matrix createMatrix(int rows, int cols, double **values) {
    Matrix matrix;
    int i, j;

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (double **)malloc(rows * sizeof(double *));

    if (matrix.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < rows; i++) {
        matrix.data[i] = (double *)malloc(cols * sizeof(double));
        if (matrix.data[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        if (values != NULL) {
            for (j = 0; j < cols; j++) {
                matrix.data[i][j] = values[i][j];
            }
        } else {
            for (j = 0; j < cols; j++) {
                matrix.data[i][j] = 0.0;
            }
        }
    }

    return matrix;
}


Matrix createZeroMatrix(int rows, int cols) {
    Matrix mat;
    int i, j;

    mat.rows = rows;
    mat.cols = cols;
    mat.data = (double **)malloc(rows * sizeof(double *));
    if (mat.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < rows; i++) {
        mat.data[i] = (double *)malloc(cols * sizeof(double));
        if (mat.data[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < cols; j++) {
            mat.data[i][j] = 0.0;
        }
    }

    return mat;
}


void freeMatrix(Matrix matrix) {
    int i;
    for (i = 0; i < matrix.rows; i++) {
        free(matrix.data[i]);
    }
    free(matrix.data);
}


Matrix addMatrix(Matrix matrix1, Matrix matrix2) {
    Matrix result;
    int i, j;

    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        fprintf(stderr, "Matrices must have the same dimensions for addition\n");
        exit(EXIT_FAILURE);
    }

    result = createMatrix(matrix1.rows, matrix1.cols, NULL);

    for (i = 0; i < matrix1.rows; i++) {
        for (j = 0; j < matrix1.cols; j++) {
            result.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
        }
    }

    return result;
}


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


double sumRow(Matrix matrix, int row) {
    double sum = 0.0;
    int j;

    for (j = 0; j < matrix.cols; j++) {
        sum += matrix.data[row][j];
    }
    return sum;
}


double sumColumn(Matrix matrix, int col) {
    double sum = 0.0;
    int i;

    for (i = 0; i < matrix.rows; i++) {
        sum += matrix.data[i][col];
    }
    return sum;
}


double squaredEuclideanDistance(double *vector1, double *vector2, int size) {
    double sum = 0.0;
    int i;
    for (i = 0; i < size; i++) {
        double diff = vector1[i] - vector2[i];
        sum += diff * diff;
    }
    return sum;
}


Matrix powerDiagMatrix(Matrix matrix, double power) {
    Matrix result;
    int i;

    result = createMatrix(matrix.rows, matrix.cols, NULL);

    for (i = 0; i < matrix.rows; i++) {
        result.data[i][i] = pow(matrix.data[i][i], power);
    }
    return result;
}


Matrix multiplyMatrix(Matrix matrix1, Matrix matrix2) {
    int i, j, k;
    Matrix result;

    if (matrix1.cols != matrix2.rows) {
        fprintf(stderr, "Number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication\n");
        exit(EXIT_FAILURE);
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