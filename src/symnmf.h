#ifndef SYMNMF_H
#define SYMNMF_H

#include "matrix.h"

void getDimension(const char *fileName, int* n, int* d);
Matrix readData(const char* filename, int n, int d);
Matrix sym(Matrix X);
Matrix ddg(Matrix A);
Matrix norm(Matrix D, Matrix A);
Matrix update_H(Matrix H_current, Matrix H_new, Matrix W); 
Matrix converge_H(Matrix H, Matrix W, double eps, int iter);
Matrix symnmf(char *goal, char *fileName);

#endif /* SYM_NMF_H */