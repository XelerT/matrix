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

#ifdef LVL2
        auto n_elems_data = get_user_chain_data();
        matrix_chain_t<double> chain {};
        chain.print_optimal_sequence(n_elems_data.second);
#endif /*LVL2*/

        return 0;
}