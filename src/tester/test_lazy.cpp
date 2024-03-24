#include "test_lazy.hpp"
#include "matrixes/lazy.hpp"
#include "matrixes/matrix.hpp"

using namespace matrixes;

namespace
{

void test_raise2power (std::ostream &os, int dim, int n) 
{
        // std::vector<size_t> data 
        //                  {1, 0, 2, 
        //                   0, 3, 0,
        //                   4, 0, 5};
        // auto lazy   = new lazy_matrix_t<size_t> {3, 3, data.begin()};
        // auto matrix = new matrix_t<size_t> {3, 3, data.begin()};

        auto lazy = new lazy_matrix_t<size_t> {lazy_matrix_t<size_t>::gen_random(dim, dim)};
        auto matrix = new matrix_t<size_t> {matrix_t<size_t>::gen_random(dim, dim)};

        MEMORY_TRACKER.dump(os, "Status before multiplication.");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto x = power(std::unique_ptr<imatrix_t<size_t>> {lazy}, n);
        MEMORY_TRACKER.dump(os, "After lazy matrixes");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto y = power(std::unique_ptr<imatrix_t<size_t>> {matrix}, n);
        MEMORY_TRACKER.dump(os, "After std matrixes");

        // reinterpret_cast<lazy_matrix_t<size_t>*>(x.get())->dump();
        // reinterpret_cast<matrix_t<size_t>*>(y.get())->dump();
}

void test_rec_raise2power (std::ostream &os, int dim, int n) 
{
        // std::vector<size_t> data 
        //                  {1, 0, 2, 
        //                   0, 3, 0,
        //                   4, 0, 5};
        // auto lazy   = new lazy_matrix_t<size_t> {3, 3, data.begin()};
        // auto matrix = new matrix_t<size_t> {3, 3, data.begin()};

        auto lazy = new lazy_matrix_t<size_t> {lazy_matrix_t<size_t>::gen_random(dim, dim)};
        auto matrix = new matrix_t<size_t> {matrix_t<size_t>::gen_random(dim, dim)};

        MEMORY_TRACKER.dump(os, "Status before multiplication.");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto x = rec_power(std::unique_ptr<imatrix_t<size_t>> {lazy}, n);
        MEMORY_TRACKER.dump(os, "After lazy matrixes");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto y = rec_power(std::unique_ptr<imatrix_t<size_t>> {matrix}, n);
        MEMORY_TRACKER.dump(os, "After std matrixes");

        // reinterpret_cast<lazy_matrix_t<size_t>*>(x.get())->dump();
        // reinterpret_cast<matrix_t<size_t>*>(y.get())->dump();
}

void test_power_iters (std::ostream &os)
{
        MEMORY_TRACKER.dump(os, "Before lazy matrixes");
        lazy_matrix_t<int> lazy_matrix = lazy_matrix_t<int>::gen_random(100, 100);

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto eigen_vec1 = lazy_matrix.power_iteration(1000);
        MEMORY_TRACKER.dump(os, "\nLazy matrix's result:");

        MEMORY_TRACKER.reset_used_mem_cnt();
        matrix_t<int> matrix = matrix_t<int>::gen_random(100, 100);
        MEMORY_TRACKER.dump(os, "\nMatrix's generation:");

        MEMORY_TRACKER.reset_used_mem_cnt();
        auto eigen_vec2 = matrix.power_iteration(1000);
        MEMORY_TRACKER.dump(os, "\nMatrix's results:");
}

void test_math_opers (std::ostream &os)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};

        MEMORY_TRACKER.dump(os);
        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        
        MEMORY_TRACKER.dump(os);
        lazy_matrix_t<int> copied_matrix1 {matrix};
        lazy_matrix_t<int> copied_matrix2 {matrix};

        MEMORY_TRACKER.dump(os, "Before mtx multiply: ");
        copied_matrix1 *= matrix2; 
        MEMORY_TRACKER.dump(os, "After mtx multiply: ");

        MEMORY_TRACKER.dump(os, "Before int multiply: ");
        copied_matrix2 *= 5; 
        MEMORY_TRACKER.dump(os, "After int multiply: ");

        MEMORY_TRACKER.dump(os, "Before int multiply: ");
        copied_matrix2 *= 0;
        MEMORY_TRACKER.dump(os, "After int multiply: ");

        auto copied_matrix3 = matrix;
        MEMORY_TRACKER.dump(os, "Copied_matrix3 and matrix share a common buffer--> ");
        copied_matrix1 = copied_matrix3;
        MEMORY_TRACKER.dump(os, "the old buffer copied_matrix1 is freed; matrix," \
                                "Copied_matrix3 and copied_matrix1 share the common one--> ");
        matrix += matrix2;
        MEMORY_TRACKER.dump(os, "now matrix has a new buffer, copied_matrix1 and " \
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

