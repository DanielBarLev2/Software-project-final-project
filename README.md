# Software-project-final-project
In this project we will implement a clustering algorithm that is based on symmetric Non-negative Matrix Factorization (symNMF). We will further apply it to several datasets and compare to Kmeans. 

to compile C code in Linux env:
gcc -ansi -Wall -Wextra -Werror -pedantic-errors symnmf.c matrix.c -o symnmf -lm

to run in py
python3 symnmf.py 0 "sym" /a/home/cc/students/cs/danielbarlev/Software-project-final-project/data/input_7.txt
