#include <iostream>
#include <chrono>

#include "matrix.hpp"
#include "sq_matrix.hpp"
#include "chain.hpp"
#include "ui.hpp"

#include "cmd_parser.hpp"

using namespace std;
using namespace matrixes;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main (int argc, const char *argv[])
{
        arguments_t parsed_args = parse_cmd_args(argc, argv);
        if (!parsed_args.parsed)
                return 0;

        if (parsed_args.lvl1) {
                auto n_elems_data = get_user_data();
                sq_matrix_t<double> matrix {n_elems_data.first, 
                                            n_elems_data.second.begin(), 
                                            };
                auto det_status = matrix.det();

                return 0;
        }

        if (parsed_args.multiply_mxes) {
                auto elems = get_matrixes();
                auto chain = create_matrix_chain(elems);

                auto t1 = high_resolution_clock::now();
                auto ans_matrix = chain.optimal_mul();
                auto t2 = high_resolution_clock::now();

                duration<double, std::milli> delta_time = t2 - t1;        
                cout << "optimal_mul result: " << delta_time.count() <<  "ms\n";
                
                t1 = high_resolution_clock::now();        
                auto dummy_ans_matrix = chain.dummy_mul();
                t2 = high_resolution_clock::now();

                delta_time = t2 - t1;      
                cout << "dummy_mul result: " << delta_time.count() << " ms\n";
        } else {
                auto n_elems_data = get_user_chain_data();
                matrix_chain_t<double> chain {};
                chain.print_optimal_sequence(n_elems_data.second);
        }

        return 0;
}