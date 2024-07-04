import sys
import numpy as np
import mysymnmf as symnmf


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


def update_H_until_convergence(H, W, epsilon=1e-5, max_iterations=100):
    """
    Update H using the given rule until convergence criteria are met.

    Args:
    - W: Matrix W (numpy array)
    - H: Initial matrix H (numpy array)
    - beta: Update parameter (default is 0.5)
    - max_iterations: Maximum number of iterations (default is 100)
    - epsilon: Convergence threshold (default is 1e-5)

    Returns:
    - Updated matrix H after convergence or reaching max iterations.
    """
    beta=0.5
    prev_H = H.copy()
    for t in range(max_iterations):
        WH = np.dot(W, H)
        HHTH = np.dot(np.dot(H, H.T), H)
        H = H * (1 - beta + beta * (WH / HHTH))

        # Check convergence criteria
        diff_norm = np.linalg.norm(H - prev_H, 'fro')  # Frobenius norm
        if diff_norm < epsilon:
            print(f"Converged after {t+1} iterations.")
            break

        prev_H = H.copy()

    else:
        print(f"Warning: Max iterations ({max_iterations}) reached without convergence.")

    return H


def print_np_list(np_list):
    for row in np_list:
        print(",".join(f"{value:.4f}" for value in row))
        

def calculate_similarity(matrix1, matrix2):
    """
    Calculate the similarity between two matrices by value up to 4 digits and return it as a percentage.

    Parameters:
    - matrix1: First matrix (list of lists or numpy array)
    - matrix2: Second matrix (list of lists or numpy array)

    Returns:
    - similarity_percentage: Similarity percentage between the two matrices
    """
    # Convert lists to numpy arrays if they are not already
    if not isinstance(matrix1, np.ndarray):
        matrix1 = np.array(matrix1)
    if not isinstance(matrix2, np.ndarray):
        matrix2 = np.array(matrix2)

    # Check if matrices have the same shape
    if matrix1.shape != matrix2.shape:
        raise ValueError("Matrices must have the same shape to calculate similarity.")

    # Round the matrices to 4 decimal places
    matrix1_rounded = np.round(matrix1, 4)
    matrix2_rounded = np.round(matrix2, 4)
    
    # Compare element-wise and calculate the number of similar elements
    similar_elements = np.sum(matrix1_rounded == matrix2_rounded)
    total_elements = matrix1.size

    # Calculate similarity percentage
    similarity_percentage = (similar_elements / total_elements) * 100

    return similarity_percentage


if __name__ == "__main__":
     
    k, goal, file_name = sys_arguments()
    
    x = read_data(file_name=file_name)
    n, d = x.shape    

    # @todo: implement logic for goal selection and ouput.

    Apy = similarity_matrix(x, n)
    Dpy = diag_degree_matrix(Apy)
    Wpy = normalized_similarity_matrix(Apy, Dpy)
    
    A2, D2, W2 = 0, 0, 0
               
    A = symnmf.symnmf_c('sym', x)
    D = symnmf.symnmf_c('ddg', x)
    W = symnmf.symnmf_c('norm', x)
    
    H_init = h_initialization(k=3, n=n, m=np.mean(W))
    print("\n\ninit run: ")
    print_np_list(H_init)
    
    input("??")
    
    print("\n\nPy: ",)
    Hpy = update_H_until_convergence(H_init, Wpy, 0.001, 100)
    print_np_list(Hpy)
    
    input("??")
    
    print("\n\nC: ")
    H = symnmf.converge_h_c(H_init, W, 0.001, 100)
    print_np_list(H)
    
    print("\n\n\n\n")
    
    print(f'A sim: {calculate_similarity(Apy, A)}')
    print(f'D sim: {calculate_similarity(Dpy, D)}')
    print(f'W sim: {calculate_similarity(Wpy, W)}')
    print(f'H sim: {calculate_similarity(Hpy, H)}')

    print("Done with no errors :)")
    
    
    
    
    