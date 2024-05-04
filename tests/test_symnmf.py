from src.symnmf import similarity_matrix, read_data, diag_degree_matrix
import pytest
import numpy as np


@pytest.fixture
def test_read_data():
    x = read_data(file_name="C:\Tau\Software-Project\Software-project-final-project\data\input_1.txt")
    assert np.array_equal(np.array([[1, 2, 3],
                                    [4, 5, 6],
                                    [7, 8, 9]]), x)


def test_similarity_matrix():
    a1 = similarity_matrix(X=np.array([[1, 2, 3],
                                       [4, 5, 6]]), n=2)

    a2 = similarity_matrix(X=np.array([[1, 2, 3],
                                       [4, 5, 6],
                                       [7, 8, 9]]), n=3)

    a3 = similarity_matrix(X=np.array([[0.1, 0.2, 0.1],
                                       [0.1, 0.1, 0.1]]), n=2)



    a1_correct = np.array([[0, pytest.approx(expected=1.36096e-6, abs=1e-6)],
                           [pytest.approx(expected=1.36096e-6, abs=1e-6), 0]])

    np.fill_diagonal(a1_correct, 0)

    a2_correct = np.array([[0,
                            pytest.approx(expected=1.36096e-6, abs=1e-6),
                            pytest.approx(expected=3.532628572e-24, abs=1e-6)],
                           [pytest.approx(expected=1.36096e-6, abs=1e-6),
                            0,
                            pytest.approx(expected=1.36096e-6, abs=1e-6)],
                           [pytest.approx(expected=3.532628572e-24, abs=1e-6),
                            pytest.approx(expected=1.36096e-6, abs=1e-6),
                            0]])

    np.fill_diagonal(a2_correct, 0)

    a3_correct = np.array([[0, pytest.approx(expected=0.995012, abs=1e-5)],
                           [pytest.approx(expected=0.995012, abs=1e-5), 0]])

    assert np.array_equal(a1, a1_correct)

    assert np.array_equal(a2, a2_correct)

    assert np.array_equal(a3, a3_correct)


def test_diag_degree_matrix():
    a1 = np.array([[1, 1, 1],
                   [1, 1, 1],
                   [1, 1, 1]])

    a2 = np.array([[2, 1, 0],
                   [4, 5, 6],
                   [7, 8, 9]])

    d1 = diag_degree_matrix(a1)

    d2 = diag_degree_matrix(a2)

    d1_correct = np.array([[3, 0 , 0], [0, 3, 0], [0, 0, 3]])

    d2_correct = np.array([[3, 0, 0], [0, 15, 0], [0, 0, 24]])

    assert np.array_equal(d1, d1_correct)

    assert np.array_equal(d2, d2_correct)

def main():
    test_read_data()
    test_similarity_matrix()
    test_diag_degree_matrix()


if __name__ == '__main__':
    main()