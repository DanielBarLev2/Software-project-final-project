import sys
import numpy as np
from kmeans import kmeans
from symnmf import read_data, symNMF
from sklearn.metrics import silhouette_score


if len(sys.argv) != 3:
    print("Usage: python3 analysis.py <k> <file_name>")
    sys.exit(1)
    
k = int(sys.argv[1])
file_name = sys.argv[2]


for index in range(1, 2):
    print("test: ", index)
    
    path = f'/a/home/cc/students/cs/danielbarlev/Software-project-final-project/data/input_{index}.txt'
    x = read_data(file_name=path)
    n, d = x.shape
    k = 3

    kmeans_labels = kmeans(input_data=path, k=k, n=n, d=d)
    kmeans_silhouette_score = silhouette_score(x, kmeans_labels)
    print(f"KMeans Silhouette Score: {kmeans_silhouette_score}")

    symNMF_labels = symNMF(x=x, k=k, n=n)
    kmeans_silhouette_score = silhouette_score(x, kmeans_labels)
    print(f"KMeans Silhouette Score: {kmeans_silhouette_score}")
    