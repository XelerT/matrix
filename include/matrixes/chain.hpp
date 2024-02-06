#pragma once

#include <list>
#include <cassert>

#include "utils/utils.hpp"
#include "utils/std.hpp"
#include "matrix.hpp"
#include "sq_matrix.hpp"

namespace matrixes
{
        template <typename T>
        class matrix_chain_t
        {
                public:
                        using imatrix_shptr   = std::shared_ptr<imatrix_t<T>>;
                        using index_matrix_shptr = std::shared_ptr<sq_matrix_t<size_t>>;

                private:
                        std::vector<imatrix_shptr> chain {};
                        std::list<size_t> optimal_sequence {};
                        index_matrix_shptr boarder_indexes {};
                        bool found_optimal = false;

                        inline void save_optimal_sequence ();
                        inline void set_optimal_sequence  (size_t i, size_t j);
                        inline void set_default_sequence ();

                public:
                        matrix_chain_t () = default;
                        matrix_chain_t (std::vector<imatrix_shptr> chain_) : chain(chain_) {}

                        void add_matrix (imatrix_t<T> *rhs_) { assert(rhs_); 
                                                               chain.push_back(std::move(imatrix_shptr(rhs_))); 
                                                               found_optimal = false; }
                        size_t get_chain_length () const { return chain.size(); }

                        inline void print_optimal_sequence ();
                        inline void print_optimal_sequence (std::vector<size_t> &matrix_sizes);

                        inline void find_optimal_sequence ();
                        inline void find_optimal_sequence (std::vector<size_t> &matrix_sizes);
                        const std::list<size_t>& get_optimal_sequence () const { return optimal_sequence; }

                        inline imatrix_shptr optimal_mul ();
                        inline imatrix_shptr optimal_mul_req (size_t i, size_t j);
                        inline std::pair<imatrix_shptr, size_t>
                                optimal_mul_subchain (size_t start_matrix_index);
                        
                        inline imatrix_shptr dummy_mul ();
                        inline void compare_multiplications () const;
        };

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------
        
        template <typename T>
        inline void matrix_chain_t<T>::print_optimal_sequence ()
        {
                if (!found_optimal)
                        find_optimal_sequence();

                print(optimal_sequence);
        }

        template <typename T>
        inline void matrix_chain_t<T>::print_optimal_sequence (std::vector<size_t> &matrix_sizes)
        {
                if (!found_optimal)
                        find_optimal_sequence(matrix_sizes);

                print(optimal_sequence);
        }

        template <typename T>
        inline void matrix_chain_t<T>::find_optimal_sequence ()
        {
                size_t chain_length = get_chain_length();
                if (!chain_length)
                        throw std::out_of_range("Chain is empty.");
                std::vector<size_t> matrixes_sizes {};

                for (size_t i = 0; i < chain_length; ++i)
                        matrixes_sizes.push_back(chain[i]->get_n_rows());
                matrixes_sizes.push_back(chain[chain_length - 1]->get_n_cols());

                find_optimal_sequence(matrixes_sizes);
        }

        template <typename T>
        inline void matrix_chain_t<T>::find_optimal_sequence (std::vector<size_t> &matrixes_sizes)
        {
                size_t chain_length = matrixes_sizes.size() - 1; // -1 because of n_rows of last matrix

                std::vector<size_t> zeroes (chain_length * chain_length);
                sq_matrix_t<size_t> n_operations    {chain_length, zeroes.begin()};
                boarder_indexes.reset(new sq_matrix_t<size_t> {chain_length, zeroes.begin()});

                for (size_t l = 1; l < chain_length; l++) {
                        for (size_t i = 0; i < chain_length - l; i++) {
                                size_t j = i + l;
                                n_operations[i][j] = MAX_SIZE_T_VALUE;
                                for (size_t k = i; k < j; k++) {
                                        auto n_muls = n_operations[i][k] + n_operations[k + 1][j] + 
                                                      matrixes_sizes[i] * matrixes_sizes[k + 1] * matrixes_sizes[j + 1];
                                        if (n_muls < n_operations[i][j]) {
                                                n_operations[i][j]    = n_muls;
                                                (*boarder_indexes)[i][j] = k;
                                        }
                                }
                        }
                }
                // std::cout << "boarders:\n";
                // boarder_indexes->dump();
                // std::cout << "n_operations:\n";
                // n_operations.dump();
                // std::cout << n_operations[0][boarder_indexes->get_n_cols() - 1] << " n_operations\n";
                
                save_optimal_sequence();
                
                // std::cout << "\nsequence:\n";
                // print(optimal_sequence);
                // std::cout << "\nend\n";

                found_optimal = true;
        }

        template <typename T>
        inline std::shared_ptr<imatrix_t<T>> matrix_chain_t<T>::dummy_mul ()
        {
                if (!chain.size())
                        throw std::out_of_range("Chain is empty.");
                
                std::shared_ptr<imatrix_t<T>> temp {chain[0]};
                for (size_t i = 1; i < chain.size(); i++) {
                        temp = std::shared_ptr<imatrix_t<T>> {temp->mul(*chain[i])};
                }

                return temp;
        }

        template <typename T>
        inline std::shared_ptr<imatrix_t<T>> matrix_chain_t<T>::optimal_mul ()
        {
                if (!found_optimal)
                        find_optimal_sequence();

                return optimal_mul_req(0, chain.size() - 1);
        }

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

        template <typename T>
        inline std::shared_ptr<imatrix_t<T>> matrix_chain_t<T>::optimal_mul_req (size_t i, size_t j)
        {
                if (j > i) {
                        auto temp_lhs = optimal_mul_req(i, (*boarder_indexes)[i][j]);
                        auto temp_rhs = optimal_mul_req((*boarder_indexes)[i][j] + 1, j);
                        
                        return std::shared_ptr<imatrix_t<T>> {temp_lhs->mul(*temp_rhs)};
                }
                return chain[i];
        }

        template <typename T>
        inline void matrix_chain_t<T>::save_optimal_sequence ()
        {
                size_t n_matrixes = boarder_indexes->get_n_cols() - 1;

                set_optimal_sequence(0, n_matrixes);
        }

        template <typename T>
        inline void matrix_chain_t<T>::set_optimal_sequence (size_t i, size_t j)
        {
                if (j > i) {
                        set_optimal_sequence(i, (*boarder_indexes)[i][j]);
                        set_optimal_sequence((*boarder_indexes)[i][j] + 1, j);
                        
                        optimal_sequence.push_back((*boarder_indexes)[i][j]);
                }
        }

        template <typename T>
        inline void matrix_chain_t<T>::set_default_sequence ()
        {
                size_t n_muls = boarder_indexes->get_n_cols() - 1;
                for (size_t i = 0; i < n_muls; ++i)
                        optimal_sequence.push_back(i);
        }
}