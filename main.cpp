#include <iostream>

#include "matrix.hpp"
#include "sq_matrix.hpp"
#include "ui.hpp"

using namespace std;
using namespace matrixes;

int main ()
{
        auto n_elems_data = get_user_data();
        // std::cout << "here\n";
        sq_matrix_t<double> matrix {n_elems_data.first, 
                                    n_elems_data.second.begin(), 
                                    n_elems_data.second.end()};
        // std::cout << "here\n";
        auto det_status = matrix.det();
        cout << det_status.first << endl;

        return 0;
}