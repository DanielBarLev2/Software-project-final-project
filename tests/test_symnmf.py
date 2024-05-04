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

    assert np.array_equal(a1, a1_correct)

    assert np.array_equal(a2, a2_correct)


def main():
    test_read_data()
    test_create_similarity_matrix()


if __name__ == '__main__':
    main()