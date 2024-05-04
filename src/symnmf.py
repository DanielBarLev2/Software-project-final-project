import sys
import numpy as np
import pandas as pd


def sys_arguments() -> tuple[int, str, str]:
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
    """
    k, goal, file_name = None, None, None

    if len(sys.argv) != 3:
        raise ValueError("Not enough cmd arguments")

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


if __name__ == "__main__":
    print("")