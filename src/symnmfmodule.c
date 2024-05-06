#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "symnmf.c"

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

/* C function wrapper for symnmf */
static PyObject* symnmf_goal(PyObject* self, PyObject* args) {
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
static PyMethodDef symnmfMethods[] = {
    {"getGoal",
      (PyCFunction) symnmf_goal,
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
    "symnmfModule",
    "A Python module that implements symmetry matrix non-negative matrix factorization.",
    -1,
    symnmfMethods
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_sym(void) {
    PyObject *m;
    m = PyModule_Create(&symnmfmoudle);
    if (!m) {
        return NULL; /* Module creation failed */
    }
    return m;
}
