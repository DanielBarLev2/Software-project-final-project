import pandas as pd
import numpy as np
import sys
import mysymnmf


def sys_arguments():
    """
    Parses command-line arguments.

    Expects three sys arguments:
    1. K (int): Number of required clusters.
    2. goal (str): Can get the following values:
        i. symnmf: Perform full the symNMF.
        ii. sym: Calculate and output the similarity matrix.
        iii. ddg: Calculate and output the Diagonal Degree Matrix.
        iv. norm: Calculate and output the normalized similarity matrix.
    3. file_name (str): The path to the Input file, it will contain N data points for all above
        goals, the file extension is .txt

    :return: k, goal, file_name
    """
    k, goal, file_name = None, None, None
    
    if len(sys.argv) != 4:
        raise ValueError(len(sys.argv), " are not enough cmd arguments")

    try:
        k = int(sys.argv[1])
        goal = sys.argv[2]
        file_name = sys.argv[3]

        if goal not in ["symnmf", "sym", "ddg", "norm"]:
            raise ValueError("Not a valid goal")

    except ValueError:
        print("Invalid arguments")
        sys.exit(1)

    return k, goal, file_name


def read_data(file_name: str) -> np.ndarray:
    """
    Reads the data from the input file.
    :param file_name: The name of the text file containing the data.
    :return:The data stored in the file as a NumPy array.
    """
    with open(file_name, 'r') as f:
        lines = f.readlines()[0:]  # Skip the first line
        data = [[float(x) for x in line.split()] for line in lines]
        return np.array(data)


def similarity_matrix(X: np.ndarray, n: int) -> np.ndarray:
    """
    Creates the similarity matrix from the input data.
    :param X: input data.
    :param n: number of data points
    :return: the similarity matrix.
    """
    # broadcasts x(n, d) to x(n, 1, d) to perform pairwise subtraction between every pair.
    # then raises to the second power and sums over the last dimension d.
    x_norm = ((X[:, np.newaxis] - X) ** 2).sum(axis=-1)
    A = np.exp(-x_norm / 2)
    np.fill_diagonal(A, val=0)

    return A


def diag_degree_matrix(A: np.ndarray) -> np.ndarray:
    """
    Creates the diagonal degree matrix from the matrix A.
    :param A: the similarity matrix.
    :return: the diagonal degree matrix
    """
    D = np.zeros_like(A)
    np.fill_diagonal(D, A.sum(axis=1))
    return D


def normalized_similarity_matrix(A: np.ndarray, D: np.ndarray) -> np.ndarray:
    """
    Creates the normalized similarity matrix from the similarity matrix and from diagonal degree matrix.
    :param A: similarity matrix
    :param D: diagonal degree matrix
    :return: normalized similarity matrix
    """
    D_sqrt_inverse = np.zeros_like(D)
    np.fill_diagonal(D_sqrt_inverse, np.sqrt(np.diag(D)))
    D_sqrt_inverse = np.linalg.matrix_power(D_sqrt_inverse, -1)

    W = np.matmul(D_sqrt_inverse, A)
    W = np.matmul(W, D_sqrt_inverse)

    return W


def h_initialization(k: int, n: int, m: float) -> np.ndarray:
    """
    Randomly initialize H with values from the interval [0, 2 ∗ sqrt(m/k)].
    :param k: number of clusters
    :param n: number of data points
    :param m: the average of all entries of W.
    :return: lower dimension non-negative matrix H [n×k].
    """
    np.random.seed(0)
    np.random.uniform()

    upper_bound = 2 * np.sqrt(m / k)

    h = np.random.uniform(low=0, high=upper_bound, size=(n, k))

    return h


if __name__ == "__main__":
     
    # file_name = "C:\Tau\Software-Project\Software-project-final-project\data\input_7.txt"
    k, goal, file_name = sys_arguments()

    # file_name = "/a/home/cc/students/cs/danielbarlev/Software-project-final-project/data/input_7.txt"
    x = read_data(file_name=file_name)
    n, d = x.shape    

    # @todo: implement logic for goal selection and ouput.

    A1 = similarity_matrix(x, n)

    print(A1)
    # A1 = diag_degree_matrix(A1)
    # A2 = mysymnmf.symnmf_c("ddg", file_name)

    print("Done with no errors :)")
    
    
    
    
    