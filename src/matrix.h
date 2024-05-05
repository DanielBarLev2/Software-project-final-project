#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix createMatrix(int rows, int cols, double **values);
Matrix createZeroMatrix(int rows, int cols);
void freeMatrix(Matrix matrix);
Matrix addMatrix(Matrix matrix1, Matrix matrix2);
Matrix multiplyScalarMatrix(Matrix matrix, double scalar);
void printMatrix(Matrix matrix);
double sumRow(Matrix matrix, int row);
double sumColumn(Matrix matrix, int col);
double squaredEuclideanDistance(double *vector1, double *vector2, int size);

#endif /* MATRIX_H */
