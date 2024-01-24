#pragma once

#include "utils/utils.hpp"
#include "matrix.hpp"

namespace matrixes
{
        template <typename T>
        class matrix_chain_t
        {
                private:
                        std::vector<std::shared_ptr<imatrix_t>> chain {};
                        std::vector<size_t> optimal_sequence {};
                        bool found_optimal = false;

                        inline void find_optimal_sequence ();
                        inline void find_optimal_sequence (std::vector<size_t> &matrix_sizes);
                        inline void save_optimal_sequence (sq_matrix_t<size_t> &boarder_indexes, size_t i, size_t j);

                public:
                        void add_matrix (imatrix_t *rhs_) { chain.push_back(std::move(std::shared_ptr<imatrix_t>(rhs_))); 
                                                            found_optimal = false; }
                        size_t get_chain_length () const { return chain.size(); }

                        inline void print_optimal_sequence ();
                        inline void print_optimal_sequence (std::vector<size_t> &matrix_sizes);

                        inline imatrix_t&& optimal_mul ();
                        inline imatrix_t&& mul ();
                        inline void compare_multiplications () const;
        };

        template <typename T>
        inline void matrix_chain_t<T>::find_optimal_sequence ()
        {
                size_t chain_length = get_chain_length();
                std::vector<size_t> matrix_sizes {chain_length};

                for (size_t i = 0; i < chain_length; ++i)
                        matrix_sizes.push_back(chain[i]->get_n_rows());
                matrix_sizes.push_back(chain[chain_length - 1]->get_n_cols());
        
                find_optimal_sequence(matrix_sizes);
        }

        template <typename T>
        inline void matrix_chain_t<T>::find_optimal_sequence (std::vector<size_t> &matrix_sizes)
        {
                size_t chain_length = matrix_sizes.size() - 1; // -1 because of n_rows of last matrix

                std::vector<size_t> zeroes (chain_length * chain_length);
                sq_matrix_t<size_t> n_operations    {chain_length, zeroes.begin(), zeroes.end()};
                sq_matrix_t<size_t> boarder_indexes {chain_length, zeroes.begin(), zeroes.end()};

                for (size_t l = 1; l < chain_length; ++l) {
                        for (size_t i = 0; i < chain_length - l; ++i) {
                                size_t j = i + l;
                                n_operations[i][j] = MAX_SIZE_T_VALUE;
                                for (size_t k = i; k < j; ++k) {
                                        auto n_muls = n_operations[i][k] + n_operations[k + 1][j] + 
                                                      matrix_sizes[i] * matrix_sizes[k + 1] * matrix_sizes[j + 1];
                                        if (n_muls < n_operations[i][j]) {
                                                n_operations[i][j]    = n_muls;
                                                boarder_indexes[i][j] = k;
                                        }
                                }
                        }
                }

                save_optimal_sequence(boarder_indexes, 0, matrix_sizes.size() - 2);
        }

        template <typename T>
        inline void matrix_chain_t<T>::save_optimal_sequence (sq_matrix_t<size_t> &boarder_indexes, size_t i, size_t j)
        {
                if (j > i) {
                        save_optimal_sequence(boarder_indexes, i, boarder_indexes[i][j]);
                        save_optimal_sequence(boarder_indexes, boarder_indexes[i][j] + 1, j);
                } else {
                        optimal_sequence.push_back(i);
                }
        }
        
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
}
