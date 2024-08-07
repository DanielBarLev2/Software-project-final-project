#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <numpy/arrayobject.h>
#include "matrix.h"
#include "symnmf.c"


/* 
 * Convert a NumPy array to a Matrix struct 
 * Input: array - NumPy array to convert
 * Return: Matrix - Converted Matrix struct
 */
static Matrix convert_numpy_to_matrix(PyArrayObject *array) {
    int rows = (int)PyArray_DIM(array, 0);
    int cols = (int)PyArray_DIM(array, 1);
    Matrix matrix;
    int i, j;

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (double **)malloc(rows * sizeof(double *));

    if (matrix.data == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
        matrix.rows = 0;
        matrix.cols = 0;
        return matrix;
    }

    for (i = 0; i < rows; ++i) {
        matrix.data[i] = (double *)malloc(cols * sizeof(double));

        if (matrix.data[i] == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");

            /* Clean up allocated memory before returning */
            for (j = 0; j < i; ++j) {
                free(matrix.data[j]);
            }

            free(matrix.data);

            matrix.rows = 0;
            matrix.cols = 0;

            return matrix;
        }

        /* Copy data from NumPy array to Matrix struct */
        for (j = 0; j < cols; ++j) {
            matrix.data[i][j] = *(double *)PyArray_GETPTR2(array, i, j);
        }
    }
    return matrix;
}


/* 
 * Convert a Matrix struct to a Python list of lists 
 * Input: outputMatrix - Matrix struct to convert
 * Return: PyObject* - Python list of lists representing the matrix
 */
static PyObject* convert_matrix_to_python(Matrix outputMatrix) {
    PyObject *pyOutputMatrixObj = PyList_New(outputMatrix.rows);
    int i, j, k;

    if (!pyOutputMatrixObj) {
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
        return NULL;
    }
    
    for (i = 0; i < outputMatrix.rows; ++i) {
        PyObject *pyRow = PyList_New(outputMatrix.cols);
        
        if (!pyRow) {
            /* Clean up if creating a row list fails */
            Py_DECREF(pyOutputMatrixObj);
            PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
            return NULL;
        }

        /* Convert each element of Matrix struct to Python float */
        for (j = 0; j < outputMatrix.cols; ++j) {
            PyObject *pyValue = PyFloat_FromDouble(outputMatrix.data[i][j]);

            if (!pyValue) {
                /* Clean up if converting double to Python float fails */
                Py_DECREF(pyRow);

                for (k = 0; k < i; ++k) {
                    Py_DECREF(PyList_GET_ITEM(pyOutputMatrixObj, k));
                }

                Py_DECREF(pyOutputMatrixObj);
                PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
                return NULL;
            }

            PyList_SET_ITEM(pyRow, j, pyValue);

        }
        PyList_SET_ITEM(pyOutputMatrixObj, i, pyRow);
    }

    return pyOutputMatrixObj;
}


/* 
 * Python wrapper function to iteratively update H matrix until convergence 
 * Input: H - initial H matrix (n x k)
 *        W - weight matrix (n x n)
 *        eps - convergence threshold. def = 0.0001
 *        iter - maximum number of iterations. def = 300
 * Return: PyObject* - converged H matrix (n x k) as a Python object
 */
static PyObject* converge_h_c(PyObject* self, PyObject* args) {
    Matrix h_matrix = {0}, w_matrix = {0}, result_matrix = {0};
    PyArrayObject *h_array = NULL, *w_array = NULL;
    PyObject *h_obj, *w_obj;
    PyObject *pyResultObj = NULL;
    double eps;
    int iter;
    
    if (!PyArg_ParseTuple(args, "OOdi", &h_obj, &w_obj, &eps, &iter)) {
        return NULL;
    }

    h_array = (PyArrayObject *)PyArray_FROM_OTF(h_obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    w_array = (PyArrayObject *)PyArray_FROM_OTF(w_obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);

    if (h_array == NULL || w_array == NULL) {
        PyErr_SetString(PyExc_TypeError, "An Error Has Occurred");
        Py_XDECREF(h_array);
        Py_XDECREF(w_array);
        return NULL;
    }

    h_matrix = convert_numpy_to_matrix(h_array);
    w_matrix = convert_numpy_to_matrix(w_array);

    if (h_matrix.data == NULL || w_matrix.data == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
        freeMatrix(h_matrix);
        freeMatrix(w_matrix);
        Py_DECREF(h_array);
        Py_DECREF(w_array);
        return NULL;
    }

    result_matrix = converge_H(h_matrix, w_matrix, eps, iter);

    if (result_matrix.data == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
        freeMatrix(h_matrix);
        freeMatrix(w_matrix);
        Py_DECREF(h_array);
        Py_DECREF(w_array);
        return NULL;
    }

    pyResultObj = convert_matrix_to_python(result_matrix);

    freeMatrix(h_matrix);
    freeMatrix(w_matrix);
    freeMatrix(result_matrix);

    /* Release NumPy arrays*/
    Py_DECREF(h_array);
    Py_DECREF(w_array);

    if (pyResultObj == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
    }

    return pyResultObj;
}


/* 
 * Python wrapper function for SymNMF operations 
 * Input: goal - the desired operation ('sym', 'ddg', 'norm')
 *        x - data matrix
 * Return: PyObject* - resulting matrix as a Python object
 */
static PyObject* symnmf_c(PyObject* self, PyObject* args) {
    char *goal;
    PyObject *x_obj;
    PyArrayObject *x_array;
    PyObject *pyOutputMatrixObj;
    Matrix outputMatrix, x_matrix;
    Matrix sym_matrix;

    if (!PyArg_ParseTuple(args, "sO", &goal, &x_obj)) {
        return NULL;
    }

    x_array = (PyArrayObject *)PyArray_FROM_OTF(x_obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    if (x_array == NULL) {
        PyErr_SetString(PyExc_TypeError, "An Error Has Occurred");
        return NULL;
    }

    x_matrix = convert_numpy_to_matrix(x_array);

    if (strcmp(goal, "sym") == 0) {
        outputMatrix = sym(x_matrix);
    }
    else if (strcmp(goal, "ddg") == 0) {
        sym_matrix = sym(x_matrix);
        outputMatrix = ddg(sym_matrix);
        freeMatrix(sym_matrix);
    } 
    else if (strcmp(goal, "norm") == 0) {
        sym_matrix = sym(x_matrix);
        outputMatrix = norm(ddg(sym_matrix), sym_matrix);
        freeMatrix(sym_matrix);
    } 
    else {
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        freeMatrix(x_matrix);
        Py_DECREF(x_array);
        return NULL;
    }

    pyOutputMatrixObj = convert_matrix_to_python(outputMatrix);

    freeMatrix(x_matrix);
    freeMatrix(outputMatrix);

    /* Release NumPy arrays */
    Py_DECREF(x_array);

    return pyOutputMatrixObj;
}


/* Methods definitions for the Python module: */
static PyMethodDef methods[] = {
    {"symnmf_c", (PyCFunction)symnmf_c, METH_VARARGS, "C implementation of symmetric non-negative matrix factorization."},
    {"converge_h_c", (PyCFunction)converge_h_c, METH_VARARGS, "Converge H using C implementation."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",
    "Symmetric Non-negative Matrix Factorization",
    -1,
    methods,
    NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_mysymnmf(void) {
    import_array(); /* For NumPy */
    return PyModule_Create(&symnmfmodule);
}
