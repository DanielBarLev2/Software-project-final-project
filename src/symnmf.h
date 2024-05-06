#ifndef SYMNMF_H
#define SYMNMF_H

#include "matrix.h"

void getDimension(const char *fileName, int* n, int* d);
Matrix readData(const char* filename, int n, int d);
Matrix sym(Matrix X);
Matrix ddg(Matrix A);
Matrix norm(Matrix D, Matrix A);
Matrix symnmf(char *goal, char *fileName);

#endif /* SYM_NMF_H */