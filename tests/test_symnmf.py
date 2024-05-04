from src.symnmf import create_similarity_matrix, read_data
import pytest
import numpy as np


@pytest.fixture
def test_read_data():
    x = read_data(file_name="C:\Tau\Software-Project\Software-project-final-project\data\input_1.txt")
    assert np.array_equal(np.array([[1, 2, 3],
                                    [4, 5, 6],
                                    [7, 8, 9]]), x)


def test_create_similarity_matrix():
    a1 = create_similarity_matrix(x=np.array([[1, 2, 3],
                                              [4, 5, 6]]), n=2)

    a2 = create_similarity_matrix(x=np.array([[1, 2, 3],
                                              [4, 5, 6],
                                              [7, 8, 9]]), n=3)

    assert np.array_equal(a1, np.array([0, 27,
                                        27, 0]))

    assert np.array_equal(a2, np.array([0, 27, 108,
                                        27, 0, 27,
                                        108, 27, 0]))


def main():
    test_read_data()
    test_create_similarity_matrix()


if __name__ == '__main__':
    main()