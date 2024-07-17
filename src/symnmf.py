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
        raise ValueError(len(sys.argv), "An Error Has Occrred")

    try:
        k = int(sys.argv[1])
        goal = sys.argv[2]
        file_name = sys.argv[3]

        if goal not in ["symnmf", "sym", "ddg", "norm"]:
            raise ValueError("An Error Has Occrred")

    except ValueError:
        print("An Error Has Occrred")
        sys.exit(1)

    return k, goal, file_name


def read_data(file_name: str) -> np.ndarray:
    """
    Reads the data from the input file.
    :param file_name: The name of the text file containing the data.
    :return: The data stored in the file as a NumPy array.
    """
    with open(file_name, 'r') as f:
        lines = f.readlines()
        data = [[float(x) for x in line.strip().split(',')] for line in lines]
    return np.array(data)


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

        # Check convergence
        diff_norm = np.linalg.norm(H - prev_H, 'fro') 
        if diff_norm < epsilon:
            print(f"Converged after {t+1} iterations.")
            break

        prev_H = H.copy()

    else:
        print(f"Warning: Max iterations ({max_iterations}) reached without convergence.")

    return H


def print_np_list(np_list):
    """
    Print np list as described in 2.1.4: 
    For example:
        0.0600,0.0100
        0.0100,0.0500
        0.0100,0.0400
        0.0200,0.0400
        0.0500,0.0200
    Args:
        np_list: numpy list
    """
    for row in np_list:
        print(",".join(f"{value:.4f}" for value in row))
        

def symNMF(x, k, n, epsilon=0.0001, max_iter=300):
    W = symnmf.symnmf_c('norm', x)
    H_init = h_initialization(k=k, n=n, m=np.mean(W))
    H_final = symnmf.converge_h_c(H_init, W, epsilon, max_iter)

    labels = np.argmax(H_final, axis=1)
    
    return labels
    
    
    
def main():
    np.random.seed(0)
    
    k, goal, file_name = sys_arguments()
    x = read_data(file_name=file_name)
    
    if (goal == "symnmf"):
        n, _ = x.shape  
        W = symnmf.symnmf_c('norm', x)
        H_init = h_initialization(k=k, n=n, m=np.mean(W))
        epsilon = 0.0001
        max_iter = 300
        H_final = symnmf.converge_h_c(H_init, W, epsilon, max_iter)

        print_np_list(H_final)
        
    elif(goal == "sym"):
        A = symnmf.symnmf_c('sym', x)
        
        print_np_list(A)
        
    elif(goal == "ddg"):
        D = symnmf.symnmf_c('ddg', x)
        
        print_np_list(D)

    elif(goal == "norm"):   
        W = symnmf.symnmf_c('norm', x)
        
        print_np_list(W)
        
        
if __name__ == "__main__":
    main()
    
    
    
    
    
    
