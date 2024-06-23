#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <numpy/arrayobject.h> 

#include "matrix.h"
#include "symnmf.c"


/* Function to convert NumPy array to Matrix structure */
static Matrix convert_numpy_to_matrix(PyArrayObject *array) {
    int rows = (int)PyArray_DIM(array, 0);
    int cols = (int)PyArray_DIM(array, 1);
    int i, j;
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (double **)malloc(rows * sizeof(double *));

    if (matrix.data == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed for matrix rows.");
        return matrix;
    }

    for (i = 0; i < rows; ++i) {
        matrix.data[i] = (double *)malloc(cols * sizeof(double));
        if (matrix.data[i] == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed for matrix columns.");
            freeMatrix(matrix); // Cleanup already allocated rows
            matrix.rows = 0;
            return matrix;
        }
        for (j = 0; j < cols; ++j) {
            matrix.data[i][j] = *(double *)PyArray_GETPTR2(array, i, j);
        }
    }

    return matrix;
}


/* Function to convert Matrix to Python object */
static PyObject* convert_matrix_to_python(Matrix outputMatrix) {
    int i, j;
    PyObject *pyOutputMatrixObj, *pyRow, *pyValue;

    pyOutputMatrixObj = PyList_New(outputMatrix.rows);
    if (!pyOutputMatrixObj) {
        PyErr_SetString(PyExc_RuntimeError, "Unable to create Python list for matrix.");
        return NULL;
    }

    for (i = 0; i < outputMatrix.rows; ++i) {
        pyRow = PyList_New(outputMatrix.cols);
        if (!pyRow) {
            Py_DECREF(pyOutputMatrixObj);
            PyErr_SetString(PyExc_RuntimeError, "Unable to create Python list for row.");
            return NULL;
        }

        for (j = 0; j < outputMatrix.cols; ++j) {
            pyValue = PyFloat_FromDouble(outputMatrix.data[i][j]);
            if (!pyValue) {
                Py_DECREF(pyOutputMatrixObj);
                Py_DECREF(pyRow);
                PyErr_SetString(PyExc_RuntimeError, "Unable to convert double to Python float.");
                return NULL;
            }
            PyList_SET_ITEM(pyRow, j, pyValue);
        }
        PyList_SET_ITEM(pyOutputMatrixObj, i, pyRow);
    }

    return pyOutputMatrixObj;
}


/* C function wrapper for converge_H */
static PyObject* converge_h_c(PyObject* self, PyObject* args) {
    PyObject *h_obj, *w_obj;
    PyArrayObject *h_array, *w_array;
    double eps;
    int iter;
    Matrix h_matrix, w_matrix, result_matrix;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "OOdi", &h_obj, &w_obj, &eps, &iter)) {
        return NULL; /* Parsing error */
    }

    /* Convert NumPy arrays to Matrix */
    h_array = PyArray_FROM_OTF(h_obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    w_array = PyArray_FROM_OTF(w_obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    if (h_array == NULL || w_array == NULL) {
        Py_XDECREF(h_array);
        Py_XDECREF(w_array);
        PyErr_SetString(PyExc_TypeError, "Failed to convert input to NumPy arrays.");
        return NULL;
    }

    h_matrix = convert_numpy_to_matrix(h_array);
    w_matrix = convert_numpy_to_matrix(w_array);

    /* Call converge_H function from symnmf.c */
    result_matrix = converge_H(h_matrix, w_matrix, eps, &iter);

    /* Convert Matrix to NumPy array */
    PyObject *pyResultObj = convert_matrix_to_numpy(result_matrix);

    /* Free allocated memory for Matrices */
    freeMatrix(h_matrix);
    freeMatrix(w_matrix);
    freeMatrix(result_matrix);

    return pyResultObj;
}



/* C function wrapper for symnmf */
static PyObject* symnmf_c(PyObject* self, PyObject* args) {
    char *goal, *fileName;
    PyObject *pyOutputMatrixObj;
    Matrix outputMatrix;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ss", &goal, &fileName)) {
        return NULL; /* Parsing error */
    }
    
    /* Call symnmf function */
    outputMatrix = symnmf(goal, fileName);

    /* Convert Matrix to Python object */
    pyOutputMatrixObj = convert_matrix_to_python(outputMatrix);

    return pyOutputMatrixObj;
}

/* Method definitions */
static PyMethodDef methods[] = {
    {"symnmf_c",
      (PyCFunction) symnmf_c,
      METH_VARARGS,
      PyDoc_STR("C implementation to symmetry matrix non-negative matrix factorization.\n\n"
                "Arguments:\n"
                "goal (str): i. sym: Calculate and output the similarity matrix.\n"
                            "ii. ddg: Calculate and output the Diagonal Degree Matrix.\n"
                            "iii. norm: Calculate and output the normalized similarity matrix.\n"
                "fileName (str): The filename.\n\n"
                "Returns:\n"
                "list: The output matrix as a nested Python list.")},
    {NULL, NULL, 0, NULL}
};

/* Module initialization */
static struct PyModuleDef symnmfmoudle = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",
    "A Python module that implements symmetry matrix non-negative matrix factorization.",
    -1,
    methods
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_mysymnmf() {
    return PyModule_Create(&symnmfmoudle);
}
