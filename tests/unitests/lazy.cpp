#include "config.hpp"

#include "lazy.hpp"

using namespace matrixes;

TEST (lazy_matrix, init)
{
        std::vector data {1, 2, 3, 4};

        lazy_matrix_t<int> matrix {2, 2, data.begin()};

        EXPECT_EQ(matrix.get_n_cols(), 2);
        EXPECT_EQ(matrix.get_n_rows(), 2);
        EXPECT_EQ(matrix.get_n_cols(), matrix[0].get_size());

        EXPECT_EQ(matrix[0][0], 1);
        EXPECT_EQ(matrix[0][1], 2);
        EXPECT_EQ(matrix[1][0], 3);
        EXPECT_EQ(matrix[1][1], 4);
}

TEST (lazy_matrix, copy_ctor)
{
        std::vector data {1, 2, 3, 4};

        lazy_matrix_t<int> matrix {2, 2, data.begin()};
        lazy_matrix_t cp_matrix {matrix};

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}

TEST (lazy_matrix, copy_ass)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        lazy_matrix_t<int> matrix {2, 2, data.begin()};
        lazy_matrix_t<int> cp_matrix {3, 2, data2.begin()};
        cp_matrix = matrix;

        EXPECT_EQ(cp_matrix.get_n_cols(), 2);
        EXPECT_EQ(cp_matrix.get_n_rows(), 2);

        EXPECT_EQ(cp_matrix[0][0], 1);
        EXPECT_EQ(cp_matrix[0][1], 2);
        EXPECT_EQ(cp_matrix[1][0], 3);
        EXPECT_EQ(cp_matrix[1][1], 4);
}

TEST (lazy_matrix, equal_oper_diff_sizes)
{
        std::vector data {1, 2, 3, 4};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        lazy_matrix_t<int> matrix  {2, 2, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_FALSE(matrix == matrix2);
}

TEST (lazy_matrix, equal_oper_diff_values)
{
        std::vector data  {1, 2, 3, 4, 0, 0};
        std::vector data2 {4, 5, 9, 7, 1, 2};

        lazy_matrix_t<int> matrix  {3, 2, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_FALSE(matrix == matrix2);
}

TEST (lazy_matrix, equal_oper_equal_matrixes)
{
        std::vector data  {1, 2, 3, 4, 0, 0};
        std::vector data2 {1, 2, 3, 4, 0, 0};

        lazy_matrix_t<int> matrix  {3, 2, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 2, data2.begin()};
        
        ASSERT_TRUE(matrix == matrix2);
}

TEST (lazy_matrix, multiply_E) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector E_data {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix_E {3, 3, E_data.begin()};

        std::shared_ptr<lazy_matrix_t<int>> ans {static_cast<lazy_matrix_t<int>*>(matrix.mul(matrix_E))};
        ASSERT_TRUE(*ans == matrix);
}

TEST (lazy_matrix, multiply) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};
        std::vector data3 {15, 12, 9,
                            18, 15, 12,
                            51, 42, 33};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix3 {3, 3, data3.begin()};

        std::shared_ptr<lazy_matrix_t<int>> ans {static_cast<lazy_matrix_t<int>*>(matrix.mul(matrix2))};
        ASSERT_TRUE(*ans == matrix3);
}

TEST (laziness_of_lazy_matrix, copy_ctor) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix2 {matrix};

        ASSERT_TRUE(matrix == matrix2);
        ASSERT_TRUE(matrix.get_linked_with() == 2);
        ASSERT_TRUE(matrix2.get_linked_with() == 2);
}


TEST (laziness_of_lazy_matrix, copy_ctor_mul) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};
        std::vector data3 {15, 12, 9,
                            18, 15, 12,
                            51, 42, 33};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> copied_matrix {matrix};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix3 {3, 3, data3.begin()};
        
        copied_matrix *= matrix2; 

        ASSERT_TRUE(copied_matrix == matrix3);

        ASSERT_TRUE(matrix.get_linked_with() == 1);
        ASSERT_TRUE(matrix2.get_linked_with() == 1);
}

TEST (laziness_of_lazy_matrix, copy_ass_mul) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};
        std::vector data3 {15, 12, 9,
                            18, 15, 12,
                            51, 42, 33};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> copied_matrix {3, 3, data2.begin()};
        copied_matrix = matrix;
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix3 {3, 3, data3.begin()};

        ASSERT_EQ(matrix.get_linked_with(), 2);
        ASSERT_EQ(copied_matrix.get_linked_with(), 2);
        
        copied_matrix *= matrix2; 

        ASSERT_TRUE(copied_matrix == matrix3);

        ASSERT_EQ(matrix.get_linked_with(), 1);
        ASSERT_EQ(matrix2.get_linked_with(), 1);
}

TEST (laziness_of_lazy_matrix, multiple_copy_ass__mul) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};
        std::vector data3 {15, 12, 9,
                            18, 15, 12,
                            51, 42, 33};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> copied_matrix {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix3 {3, 3, data3.begin()};

        copied_matrix = matrix;

        ASSERT_EQ(matrix.get_linked_with(), 2);
        ASSERT_EQ(copied_matrix.get_linked_with(), 2);
        
        auto copied_matrix2 = matrix;

        ASSERT_EQ(matrix.get_linked_with(), 3);
        ASSERT_EQ(copied_matrix.get_linked_with(), 3);

        matrix = matrix3;

        copied_matrix *= matrix2; 

        ASSERT_TRUE(copied_matrix == matrix3);
        matrix3 = matrix2;

        ASSERT_EQ(matrix.get_linked_with(), 1);
        ASSERT_EQ(matrix2.get_linked_with(), 2);
}

TEST (laziness_of_lazy_matrix, copy_ass2itself) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> copied_matrix {3, 3, data2.begin()};

        copied_matrix = matrix;
        matrix = copied_matrix;
        copied_matrix = matrix;
        matrix = copied_matrix;
        copied_matrix = matrix;
        matrix = copied_matrix;
        copied_matrix = matrix;
        matrix = copied_matrix;

        ASSERT_EQ(matrix.get_linked_with(), 2);
        ASSERT_EQ(copied_matrix.get_linked_with(), 2);
}

TEST (laziness_of_lazy_matrix, copy_ass_add) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};
        std::vector data3 {10, 8, 9,
                            6, 8, 4,
                            7, 2, 6};

        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> copied_matrix {matrix};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        lazy_matrix_t<int> matrix3 {3, 3, data3.begin()};
        
        copied_matrix += matrix2; 

        ASSERT_TRUE(copied_matrix == matrix3);

        ASSERT_TRUE(matrix.get_linked_with() == 1);
        ASSERT_TRUE(matrix2.get_linked_with() == 1);
}