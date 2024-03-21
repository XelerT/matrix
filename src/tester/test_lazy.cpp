#include "test_lazy.hpp"
#include "matrixes/lazy.hpp"

using namespace matrixes;

void test_power_iters (std::ostream &os)
{
        MEMORY_TRACKER.dump("Allocated memory: ", os);
        lazy_matrix_t<int> matrix = lazy_matrix_t<int>::gen_random(10, 10);
        MEMORY_TRACKER.dump("Allocated memory: ", os);
        matrix.dump();
}

void test_lazy_matrix (std::ostream &os)
{
        std::vector data {1, 0, 2, 
                          0, 3, 0,
                          4, 0, 5};
        std::vector data2 {9, 8, 7,
                            6, 5, 4,
                            3, 2, 1};

        MEMORY_TRACKER.dump("Allocated memory: ", os);
        lazy_matrix_t<int> matrix {3, 3, data.begin()};
        lazy_matrix_t<int> matrix2 {3, 3, data2.begin()};
        
        MEMORY_TRACKER.dump("Allocated memory: ", os);
        lazy_matrix_t<int> copied_matrix1 {matrix};
        lazy_matrix_t<int> copied_matrix2 {matrix};

        MEMORY_TRACKER.dump("Allocated memory before mtx multiply: ", os);
        copied_matrix1 *= matrix2; 
        MEMORY_TRACKER.dump("Allocated memory after mtx multiply: ", os);

        MEMORY_TRACKER.dump("Allocated memory before int multiply: ", os);
        copied_matrix2 *= 5; 
        MEMORY_TRACKER.dump("Allocated memory after int multiply: ", os);

        MEMORY_TRACKER.dump("Allocated memory before int multiply: ", os);
        copied_matrix2 *= 0;
        MEMORY_TRACKER.dump("Allocated memory after int multiply: ", os);


        auto copied_matrix3 = matrix;
        MEMORY_TRACKER.dump("Copied_matrix3 and matrix share a common buffer--> ", os);
        copied_matrix1 = copied_matrix3;
        MEMORY_TRACKER.dump("the old buffer copied_matrix1 is freed; matrix," \
                            "Copied_matrix3 and copied_matrix1 share the common one--> ", os);
        matrix += matrix2;
        MEMORY_TRACKER.dump("now matrix has a new buffer, copied_matrix1 and " \
                            "copied_matrix3 share a common buffer that used to be matrix's buffer--> ", os);

        test_power_iters(os);
}

