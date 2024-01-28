#include <iostream>

#include "matrix.hpp"
#include "sq_matrix.hpp"
#include "chain.hpp"
#include "ui.hpp"

using namespace std;
using namespace matrixes;

int main ()
{
#ifdef LVL1
        auto n_elems_data = get_user_data();
        sq_matrix_t<double> matrix {n_elems_data.first, 
                                    n_elems_data.second.begin(), 
                                    n_elems_data.second.end()};
        auto det_status = matrix.det();
#endif /*LVL1*/

// #define LVL2

#ifdef LVL2
        auto n_elems_data = get_user_chain_data();
        matrix_chain_t<double> chain {};
        chain.print_optimal_sequence(n_elems_data.second);
#endif /*LVL2*/

#define TESTS

#ifdef TESTS
        auto elems = get_matrixes();
        auto chain = create_matrix_chain(elems);

        cout << "optimal_mul result:\n";
        auto ans_matrix = chain.optimal_mul();
        reinterpret_cast<matrix_t<int>*>(ans_matrix.get())->dump();
        
        cout << "dummy_mul result:\n";
        auto dummy_ans_matrix = chain.dummy_mul();
        reinterpret_cast<matrix_t<int>*>(dummy_ans_matrix.get())->dump();
#endif /*TESTS*/

        return 0;
}