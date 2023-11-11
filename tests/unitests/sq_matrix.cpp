#include "config.hpp"

using namespace std;
using namespace matrixes;

TEST (sq_matrix, determinant)
{
        std::vector data {3, 4, 1, 3};

        sq_matrix_t<double> matrix {2, data.begin(), data.end()};

        auto det_status = matrix.det();
        EXPECT_EQ(det_status.second, status_t::OK);
        EXPECT_NEAR(det_status.first, 5, THRESHOLD);
}