#ifndef MATRIX_H
#define MATRIX_H

/* Define a structure for Matrix with rows, cols, and data */
typedef struct {
    int rows;       /* Number of rows in the matrix */
    int cols;       /* Number of columns in the matrix */
    double **data;  /* Pointer to the matrix data */
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
Matrix powerDiagMatrix(Matrix matrix, double power);
Matrix multiplyMatrix(Matrix matrix1, Matrix matrix2);
Matrix transposeMatrix(Matrix matrix);
double frobeniusNorm(Matrix matrix1, Matrix matrix2);

#endif /* MATRIX_H */
