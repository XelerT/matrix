#include "config.hpp"

using namespace matrix;

TEST (matrix, init)
{
        std::vector data {1, 2, 3, 4};

        matrix_t<int> matrix {2, 2, data.begin(), data.end()};

        EXPECT_EQ(matrix.get_n_cols(), 2);
        EXPECT_EQ(matrix.get_n_rows(), 2);

        EXPECT_EQ(matrix[0][0], 1);
        EXPECT_EQ(matrix[0][1], 2);
        EXPECT_EQ(matrix[1][0], 3);
        EXPECT_EQ(matrix[1][1], 4);
}

TEST (matrix, copy_ctor)
{
        std::vector data {1, 2, 3, 4};

        matrix_t<int> matrix {2, 2, data.begin(), data.end()};
        matrix_t cp_matrix {matrix};

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}

TEST (matrix, copy_ass)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        matrix_t<int> matrix {2, 2, data.begin(), data.end()};
        matrix_t<int> cp_matrix {3, 2, data2.begin(), data2.end()};
        cp_matrix = matrix;

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}
