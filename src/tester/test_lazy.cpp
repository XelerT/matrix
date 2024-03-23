#include "test_lazy.hpp"
#include "matrixes/lazy.hpp"
#include "matrixes/matrix.hpp"

using namespace matrixes;

void test_stupid_raise2power (std::ostream &os) 
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        lazy_matrix_t<int> lazy {3, 3, data.begin()};
        matrix_t<int> matrix {3, 3, data.begin()};

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto x = power(&lazy, 2);
        MEMORY_TRACKER.dump(os, "After lazy matrixes");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto y = power(&matrix, 2);
        MEMORY_TRACKER.dump(os, "After std matrixes");
}

void test_power_iters (std::ostream &os)
{
        MEMORY_TRACKER.dump(os, "Before lazy matrixes");
        lazy_matrix_t<int> lazy_matrix = lazy_matrix_t<int>::gen_random(10, 100);

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto eigen_vec1 = lazy_matrix.power_iteration(1000);
        MEMORY_TRACKER.dump(os, "\nLazy matrix's result:");

        MEMORY_TRACKER.reset_used_mem_cnt();
        matrix_t<int> matrix = matrix_t<int>::gen_random(10, 100);
        MEMORY_TRACKER.dump(os, "\nMatrix's generation:");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto eigen_vec2 = matrix.power_iteration(1000);
        MEMORY_TRACKER.dump(os, "\nMatrix's results:");
}

void test_lazy_matrix (std::ostream &os)
{
        // std::vector data {1, 0, 2, 
        //                   0, 3, 0,
        //                   4, 0, 5};
        // std::vector data2 {9, 8, 7,
        //                     6, 5, 4,
        //                     3, 2, 1};

        // MEMORY_TRACKER.dump(os);
        // lazy_matrix_t<int> matrix {3, 3, data.begin()};
        // lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        
        // MEMORY_TRACKER.dump(os);
        // lazy_matrix_t<int> copied_matrix1 {matrix};
        // lazy_matrix_t<int> copied_matrix2 {matrix};

        // MEMORY_TRACKER.dump(os, "Before mtx multiply: ");
        // copied_matrix1 *= matrix2; 
        // MEMORY_TRACKER.dump(os, "After mtx multiply: ");

        // MEMORY_TRACKER.dump(os, "Before int multiply: ");
        // copied_matrix2 *= 5; 
        // MEMORY_TRACKER.dump(os, "After int multiply: ");

        // MEMORY_TRACKER.dump(os, "Before int multiply: ");
        // copied_matrix2 *= 0;
        // MEMORY_TRACKER.dump(os, "After int multiply: ");

        // auto copied_matrix3 = matrix;
        // MEMORY_TRACKER.dump(os, "Copied_matrix3 and matrix share a common buffer--> ");
        // copied_matrix1 = copied_matrix3;
        // MEMORY_TRACKER.dump(os, "the old buffer copied_matrix1 is freed; matrix," \
        //                         "Copied_matrix3 and copied_matrix1 share the common one--> ");
        // matrix += matrix2;
        // MEMORY_TRACKER.dump(os, "now matrix has a new buffer, copied_matrix1 and " \
        //                         "copied_matrix3 share a common buffer that used to be matrix's buffer--> ");

        // test_power_iters(os);

        test_stupid_raise2power(os);
}

