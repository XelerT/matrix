#include "test_lazy.hpp"
#include "matrixes/lazy.hpp"

using namespace matrixes;

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
}