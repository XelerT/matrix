#include "test_lazy.hpp"
#include "matrixes/lazy.hpp"
#include "matrixes/matrix.hpp"

using namespace matrixes;

namespace
{

void test_raise2power (std::ostream &os, int dim, int n) 
{
        auto lazy = lazy_matrix_t<size_t>::gen_random(dim, dim);
        auto matrix = matrix_t<size_t>::gen_random(dim, dim);

        TESTER.dump_mem(os, "Status before multiplication.");

        TESTER.reset_used_mem_cnt();
        TESTER.start_timer();
        auto x = power(&lazy, n);
        TESTER.end_timer();
        TESTER.dump(os, "After lazy matrixes");

        TESTER.reset_used_mem_cnt();
        TESTER.start_timer();
        auto y = power(&matrix, n);
        TESTER.end_timer();
        TESTER.dump(os, "After std matrixes");
}

void test_rec_raise2power (std::ostream &os, int dim, int n) 
{
        auto lazy   = lazy_matrix_t<size_t>::gen_random(dim, dim);
        auto matrix = matrix_t<size_t>::gen_random(dim, dim);

        TESTER.dump_mem(os, "Status before multiplication.");

        TESTER.reset_used_mem_cnt();
        TESTER.start_timer();
        auto x = rec_power(&lazy, n);
        TESTER.end_timer();
        TESTER.dump(os, "After lazy matrixes");

        TESTER.reset_used_mem_cnt();
        TESTER.start_timer();
        auto y = rec_power(&matrix, n);
        TESTER.end_timer();
        TESTER.dump(os, "After std matrixes");
}

void test_power_iters (std::ostream &os)
{
        TESTER.dump_mem(os, "Before lazy matrixes");
        lazy_matrix_t<int> lazy_matrix = lazy_matrix_t<int>::gen_random(100, 100);

        TESTER.reset_used_mem_cnt();
        auto eigen_vec1 = lazy_matrix.power_iteration(1000);
        TESTER.dump_mem(os, "\nLazy matrix's result:");

        TESTER.reset_used_mem_cnt();
        matrix_t<int> matrix = matrix_t<int>::gen_random(100, 100);
        TESTER.dump_mem(os, "\nMatrix's generation:");

        TESTER.reset_used_mem_cnt();
        auto eigen_vec2 = matrix.power_iteration(1000);
        TESTER.dump_mem(os, "\nMatrix's results:");
}

void test_math_opers (std::ostream &os)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};

        TESTER.dump_mem(os);
        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        
        TESTER.dump_mem(os);
        lazy_matrix_t<int> copied_matrix1 {matrix};
        lazy_matrix_t<int> copied_matrix2 {matrix};

        TESTER.dump_mem(os, "Before mtx multiply: ");
        copied_matrix1 *= matrix2; 
        TESTER.dump_mem(os, "After mtx multiply: ");

        TESTER.dump_mem(os, "Before int multiply: ");
        copied_matrix2 *= 5; 
        TESTER.dump_mem(os, "After int multiply: ");

        TESTER.dump_mem(os, "Before int multiply: ");
        copied_matrix2 *= 0;
        TESTER.dump_mem(os, "After int multiply: ");

        auto copied_matrix3 = matrix;
        TESTER.dump_mem(os, "Copied_matrix3 and matrix share a common buffer--> ");
        copied_matrix1 = copied_matrix3;
        TESTER.dump_mem(os, "the old buffer copied_matrix1 is freed; matrix," \
                                "Copied_matrix3 and copied_matrix1 share the common one--> ");
        matrix += matrix2;
        TESTER.dump_mem(os, "now matrix has a new buffer, copied_matrix1 and " \
                                "copied_matrix3 share a common buffer that used to be matrix's buffer--> ");
}

} /* unanimous namespace */

void test_lazy_matrix (std::ostream &os)
{
        // os << CONSOLE_CLRS::BLUE << "\nTest: Math operations\n" << CONSOLE_CLRS::DEFAULT;
        // test_math_opers(os);

        // os << CONSOLE_CLRS::BLUE << "\nTest: Power operations\n" << CONSOLE_CLRS::DEFAULT;
        // test_power_iters(os);

        os << CONSOLE_CLRS::BLUE << "\nTest: Recursive raise to power\n" << CONSOLE_CLRS::DEFAULT;
        test_rec_raise2power(os, 20, 10);

        os << CONSOLE_CLRS::BLUE << "\nTest: Raise to power\n" << CONSOLE_CLRS::DEFAULT;
        test_raise2power(os, 20, 10);
}
