#ifndef SYMNMF_H
#define SYMNMF_H

#include "matrix.h"

void getDimension(const char *fileName, int* n, int* d);
void update_H(Matrix H_current, Matrix H_new, Matrix W); // Add this declaration
Matrix readData(const char* filename, int n, int d);
Matrix sym(Matrix X);
Matrix ddg(Matrix A);
Matrix norm(Matrix D, Matrix A);
Matrix symnmf(char *goal, char *fileName);
Matrix converge_H(Matrix H, Matrix W, double eps, int iter);

#endif /* SYM_NMF_H */