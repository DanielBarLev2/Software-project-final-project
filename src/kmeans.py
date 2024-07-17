from Vector import Vector
import numpy as np


def kmeans(input_data: str, k: int, n: int, d: int, max_iter=300):
    """
    Cluster data into K groups based on similarities between data points, using Euclidean distance.
    :param input_data: Text file containing the data; assuming valid
    :param k: Number of clusters
    :param n: Number of data points
    :param d: Data dimension
    :param max_iter: Maximum iterations of optimization
    """

    test_validation(k=k, n=n, d=d, max_iter=max_iter)

    # Select the first k elements and assign them as centroids
    with open(input_data, 'r') as file:
        vector_list = convert_to_vectors(input_data=file)

    centroids_list = initialize_centroids(vector_list=vector_list, k=k)

    # Main loop
    for iter in range(max_iter):

        for vector in vector_list:
            compute_min_distance(vector=vector, centroids_list=centroids_list)

        updated_centroids_list = update_centroids(vector_list=vector_list, d=d, k=k)

        if is_converged(centroids_list=centroids_list, updated_centroids_list=updated_centroids_list):
            break
        centroids_list = updated_centroids_list

    # Print the centroid points
    # for centroid in centroids_list:
    #     print(centroid)
    
    labels = []
    for vector in vector_list:
        labels.append(vector.centroid)
    
    return np.array(labels)


def test_validation(k: int, n: int, d: int, max_iter: int):
    """
    Validates the input parameters according to the requirements
    :param k: Number of clusters
    :param n: Number of data points
    :param d: Data dimension
    :param max_iter: Maximum iterations of optimization
    """
    if not (1 < k < n):
        raise ValueError("Invalid number of clusters!")

    if n < 1:
        raise ValueError("Invalid number of points!")

    if not isinstance(d, int):
        raise ValueError("Invalid dimension of point!")

    if not (1 < max_iter < 1000):
        raise ValueError("Invalid maximum iteration!")


def convert_to_vectors(input_data) -> list:
    """
    Convert rows from a text file, line by line, to Vector Class instances.
    :param input_data: Text file containing the data; assuming valid
    :return: Vector list of Vector Class
    """
    vectors = []

    for line in input_data:
        components = [float(x) for x in line.strip().split(',')]
        vector = Vector(*components)
        vectors.append(vector)

    return vectors


def initialize_centroids(vector_list: list, k: int) -> list:
    """
    Randomly choose vectors to initialize centroids.
    :param vector_list: Vector list of Vector Class
    :param k: Number of clustering
    :return: Random sample data vectors from a vector list
    """
    return vector_list[:k]


def compute_min_distance(vector: Vector, centroids_list: list) -> float:
    """
    Computes the distances to all centroids.
    :param vector: Vector
    :param centroids_list: Centroids list
    :return: The closest centroid number
    """
    min_distance = float('inf')

    for index, centroid in enumerate(centroids_list):
        distance = vector.euclidean_distance(centroid)

        if distance < min_distance:
            min_distance = distance
            vector.centroid = index

    return min_distance


def update_centroids(vector_list: list, d: int, k: int) -> list:
    """
    Compute the mean of each vector cluster.
    :param vector_list: Vector list of Vector Class
    :param d: Data dimension
    :param k: Number of a clustering
    :return: New centroid vector list according to the mean of each cluster data point
    """
    updated_centroids_list = []
    k_numbers = [0] * k

    # Initialize mean centroids
    for _ in range(k):
        updated_centroids_list.append(Vector(*([0] * d)))

    # Sum all vectors coordinates
    for vector in vector_list:
        updated_centroids_list[vector.centroid] += vector
        k_numbers[vector.centroid] += 1

    # Calculate mean
    for index in range(k):
        updated_centroids_list[index] = round(updated_centroids_list[index] * (1 / k_numbers[index]), 4)

    return updated_centroids_list


def is_converged(centroids_list: list, updated_centroids_list: list) -> bool:
    """
    Check if the cluster mean point is converged.
    :param centroids_list: Centroids list
    :param updated_centroids_list: New centroids list
    :return: True if the lists are equal
    """
    for index in range(len(centroids_list)):
        if not centroids_list[index].__eq__(updated_centroids_list[index]):
            return False
    return True
