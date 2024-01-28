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
                private:
                        std::vector<std::shared_ptr<imatrix_t>> chain {};
                        std::list<size_t> optimal_sequence {};
                        bool found_optimal = false;

                        inline void save_optimal_sequence (sq_matrix_t<size_t> &boarder_indexes);
                        inline void set_optimal_sequence  (sq_matrix_t<size_t> &boarder_indexes, size_t i, size_t j);
                        inline void swap_first_second_matrixes_in_sequence ();
                public:
                        matrix_chain_t () = default;
                        matrix_chain_t (std::vector<std::shared_ptr<imatrix_t>> chain_) : chain(chain_) {}

                        void add_matrix (imatrix_t *rhs_) { assert(rhs_); chain.push_back(std::move(std::shared_ptr<imatrix_t>(rhs_))); 
                                                            found_optimal = false; }
                        size_t get_chain_length () const { return chain.size(); }

                        inline void print_optimal_sequence ();
                        inline void print_optimal_sequence (std::vector<size_t> &matrix_sizes);

                        inline void find_optimal_sequence ();
                        inline void find_optimal_sequence (std::vector<size_t> &matrix_sizes);
                        const std::list<size_t>& get_optimal_sequence () const { return optimal_sequence; }

                        inline std::shared_ptr<imatrix_t> optimal_mul ();
                        inline std::pair<std::shared_ptr<imatrix_t>, size_t>
                                optimal_mul_subchain (size_t start_matrix_index);
                        
                        inline std::shared_ptr<imatrix_t> dummy_mul ();
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
                sq_matrix_t<size_t> boarder_indexes {chain_length, zeroes.begin()};

                for (size_t l = 1; l < chain_length; l++) {
                        for (size_t i = 0; i < chain_length - l; i++) {
                                size_t j = i + l;
                                n_operations[i][j] = MAX_SIZE_T_VALUE;
                                for (size_t k = i; k < j; k++) {
                                        auto n_muls = n_operations[i][k] + n_operations[k + 1][j] + 
                                                      matrixes_sizes[i] * matrixes_sizes[k + 1] * matrixes_sizes[j + 1];
                                        if (n_muls < n_operations[i][j]) {
                                                n_operations[i][j]    = n_muls;
                                                boarder_indexes[i][j] = k;
                                        }
                                }
                        }
                }
                // std::cout << "boarders:\n";
                // boarder_indexes.dump();
                // std::cout << "n_operations:\n";
                // n_operations.dump();
                // std::cout << n_operations[0][boarder_indexes.get_n_cols() - 1] << " n_operations\n";
                
                save_optimal_sequence(boarder_indexes);
                // std::cout << "\nsequence:\n";
                // print(optimal_sequence);
                // std::cout << "\nend\n";

                found_optimal = true;
        }

        template <typename T>
        inline std::shared_ptr<imatrix_t> matrix_chain_t<T>::dummy_mul ()
        {
                if (!chain.size())
                        throw std::out_of_range("Chain is empty.");
                
                std::shared_ptr<imatrix_t> temp {chain[0]};
                for (size_t i = 1; i < chain.size(); i++) {
                        temp = std::shared_ptr<imatrix_t> {temp->mul(*chain[i])};
                }

                return temp;
        }

        template <typename T>
        inline std::shared_ptr<imatrix_t> matrix_chain_t<T>::optimal_mul ()
        {
                if (!found_optimal)
                        find_optimal_sequence();



                auto prev_it = optimal_sequence.begin();
                std::shared_ptr<imatrix_t> temp {chain[*prev_it]};
                
                for (auto it = std::next(prev_it); it != optimal_sequence.end(); ++it, ++prev_it) {
                        try {
                                if (*it < *prev_it) {
                                        temp = std::shared_ptr<imatrix_t> {chain[*it]->mul(*temp)};
                                } else if (*it - *prev_it > 2 && std::distance(it, optimal_sequence.end()) != 1) {
                                        std::cout << "HERE\n";
                                        auto sub_chain_mul_chain_len = optimal_mul_subchain(std::distance(optimal_sequence.begin(), it));                                         
                                        temp = std::shared_ptr<imatrix_t> {temp->mul(*sub_chain_mul_chain_len.first)};
                                        it = std::next(it, sub_chain_mul_chain_len.second);
                                        std::cout << "HERE2\n";
                                } else {
                                        temp = std::shared_ptr<imatrix_t> {temp->mul(*chain[*it])};
                                }
                        } catch (std::out_of_range &ore) {
                                print_optimal_sequence();
                                std::cout << "\nCached: out of range for this sequence--^\n";
                                throw;
                        }
                }

                return temp;
        }

        template <typename T>
        inline std::pair<std::shared_ptr<imatrix_t>, size_t>
        matrix_chain_t<T>::optimal_mul_subchain (size_t start_matrix_index)
        {
                if (!found_optimal)
                        find_optimal_sequence();

                auto prev_it = std::next(optimal_sequence.begin(), start_matrix_index);
                std::shared_ptr<imatrix_t> temp {chain[*prev_it]};
                auto it = std::next(prev_it);

                for (; it != optimal_sequence.end(); ++it, ++prev_it) {
                        if (*it < *prev_it) {
                                        temp = std::shared_ptr<imatrix_t> {chain[*it]->mul(*temp)};
                        } else if (*it - *prev_it > 2 && std::distance(it, optimal_sequence.end()) != 1) {
                                std::cout << "HERE\n";
                                auto sub_chain_mul_chain_len = optimal_mul_subchain(std::distance(optimal_sequence.begin(), it));                                         
                                temp = std::shared_ptr<imatrix_t> {temp->mul(*sub_chain_mul_chain_len.first)};
                                it = std::next(it, sub_chain_mul_chain_len.second);
                                std::cout << "HERE2\n";
                        } else {
                                        temp = std::shared_ptr<imatrix_t> {temp->mul(*chain[*it])};
                        }
                }
                return {temp, std::distance(it, optimal_sequence.begin())};
        }


//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

        template <typename T>
        inline void matrix_chain_t<T>::save_optimal_sequence (sq_matrix_t<size_t> &boarder_indexes)
        {
                size_t n_matrixes = boarder_indexes.get_n_cols() - 1;

                set_optimal_sequence(boarder_indexes, 0, n_matrixes);
        }

        template <typename T>
        inline void matrix_chain_t<T>::set_optimal_sequence (sq_matrix_t<size_t> &boarder_indexes, size_t i, size_t j)
        {
                if (j > i) {
                        set_optimal_sequence(boarder_indexes, i, boarder_indexes[i][j]);
                        set_optimal_sequence(boarder_indexes, boarder_indexes[i][j] + 1, j);

                        optimal_sequence.push_back(boarder_indexes[i][j] + 1);
                        if (boarder_indexes[i][j] == 0)
                                optimal_sequence.push_back(0);
                }
        }

        template <typename T>
        inline void matrix_chain_t<T>::swap_first_second_matrixes_in_sequence ()
        {
                auto first_index  = *optimal_sequence.begin();
                optimal_sequence.pop_front();
                auto second_index = *optimal_sequence.begin();
                optimal_sequence.pop_front();

                optimal_sequence.push_front(first_index);
                optimal_sequence.push_front(second_index);
        }
}