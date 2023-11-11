import random
import numpy as np

GEN_DATA_MAX = 0xF


def create_diag_matrix(diagonal):
        matrix = np.array([np.array([0 for i in range(len(diagonal))]) for i in range(len(diagonal))])
        for raw, i in zip(matrix, range(len(diagonal))):
                raw[i] = diagonal[i]

        return matrix


def swap_raws(diagonal_matrix, n_raw_swaps):
        for i in range(n_raw_swaps):
                n_raw1 = random.randint(0, len(diagonal_matrix) - 1)
                n_raw2 = n_raw1
                while (n_raw1 == n_raw2):
                        n_raw2 = random.randint(0, len(diagonal_matrix) - 1)

                tmp = np.array(diagonal_matrix[n_raw1])
                diagonal_matrix[n_raw1] = diagonal_matrix[n_raw2]
                diagonal_matrix[n_raw2] = tmp
                
        if (n_raw_swaps % 2 != 0):
                return -1
        return 1


def add_raws(diagonal_matrix, n_add_raws):
        for i in range(n_add_raws):
                n_raw1 = random.randint(0, len(diagonal_matrix) - 1)
                n_raw2 = n_raw1
                while (n_raw1 == n_raw2):
                        n_raw2 = random.randint(0, len(diagonal_matrix) - 1)

                diagonal_matrix[n_raw1] += diagonal_matrix[n_raw2]


def multiply_matrix_raws(diagonal_matrix, n_mult_raws):
        determinant_mult_coeff = 1
        for i in range(n_mult_raws):
                n_raw = random.randint(0, len(diagonal_matrix) - 1)
                coeff = random.randint(1, 5)
                determinant_mult_coeff *= coeff

                diagonal_matrix[n_raw] *= coeff

        return determinant_mult_coeff 


def mix_matrix(diagonal_matrix, determinant):
        n_raw_swaps = random.randint(1, len(diagonal_matrix[0]) * 10)
        n_add_raws  = random.randint(1, len(diagonal_matrix[0]) * 10)
        n_mult_raws = random.randint(1, len(diagonal_matrix[0]) * 10)

        determinant *= swap_raws(diagonal_matrix, n_raw_swaps)
        determinant *= multiply_matrix_raws(diagonal_matrix, n_mult_raws)
        add_raws(diagonal_matrix, n_add_raws)

        return diagonal_matrix, determinant


def gen_sq_matrix (size):
        diagonal = [ random.randint(-GEN_DATA_MAX, GEN_DATA_MAX) for i in range(size) ]

        determinant = 1
        for i in range(size):
                determinant *= diagonal[i]

        matrix, determinant = mix_matrix(create_diag_matrix(diagonal), determinant)

        return matrix, determinant


# arr, determinant = gen_sq_matrix(3)
# print(determinant)
# for x in arr:
#         for y in x:
#                 print(y)
