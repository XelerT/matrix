#include "config.hpp"

#include "chain.hpp"

using namespace matrixes;

TEST (matrix_chain_dummy_mul, E_E_mul)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector E_data {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};

        matrix_t<int> matrix {3, 3, data.begin(), data.end()};

        std::vector<std::shared_ptr<imatrix_t>> matrixes {std::shared_ptr<imatrix_t>{new matrix_t<int> {matrix}}, std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin(), E_data.end()}}, 
                                                                                      std::shared_ptr<imatrix_t>{new matrix_t<int> {3, 3, E_data.begin(), E_data.end()}}};
        EXPECT_EQ(matrix[0].get_size(), 3);
        matrix_chain_t<int> chain {matrixes};

        std::shared_ptr<imatrix_t> ans_matrix = chain.dummy_mul();

        EXPECT_EQ(ans_matrix->get_n_cols(), 3);
        EXPECT_EQ(ans_matrix->get_n_rows(), 3);

        ASSERT_TRUE(matrix == *static_cast<matrix_t<int>*>(ans_matrix.get()));
}