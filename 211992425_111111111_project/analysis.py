import sys
from kmeans import kmeans
from symnmf import read_data, symNMF
# from sklearn.metrics import silhouette_score

def main():
    """
        Compare SymNMF to Kmeans from HW1.
        Prints the silhouette_score from the sklearn.metrics for SymNMF and Kmeans.
        A higher score indicates better-defined clusters.
    """
    
    if len(sys.argv) != 3:
        print("An Error Has Occurred")
        sys.exit(1)
    
    k = int(sys.argv[1])
    file_name = sys.argv[2]
    
    x = read_data(file_name=file_name)
    n, d = x.shape
    
    symNMF_labels = symNMF(x=x, k=k, n=n)
    # sym_silhouette_score = silhouette_score(x, symNMF_labels)
    # print(f"nmf: {sym_silhouette_score}")
    
    kmeans_labels = kmeans(input_data=file_name, k=k, n=n, d=d)
    # kmeans_silhouette_score = silhouette_score(x, kmeans_labels)
    # print(f"kmeans: {kmeans_silhouette_score}")

    # print(f"nmf: {symNMF_labels}")
    # print(f"kmeans: {kmeans_labels}")
    
    listNMF = [[] for _ in range(k)]
    listkmeans = [[] for _ in range(k)]
    # Populate listNMF based on symNMF_labels
    for j in range(len(symNMF_labels)):
        label = symNMF_labels[j]
        listNMF[label].append(j)
   

    # Populate listkmeans based on kmeans_labels
    for j in range(len(kmeans_labels)):
        label = kmeans_labels[j]
        listkmeans[label].append(j)

    for i in range(k):
        print(f'nmf: in {i}: {listNMF[i]}')
    
    for i in range(k):
        print(f'kms: in {i}: {listkmeans[i]}')
    
if __name__ == "__main__":
    main()
    
