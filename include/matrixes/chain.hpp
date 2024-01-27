#pragma once

#include <list>

#include "utils/utils.hpp"
#include "utils/std.hpp"
#include "matrix.hpp"

namespace matrixes
{
        template <typename T>
        class chain_sequence_node_t 
        {
                public:
                        using child_ptr = std::unique_ptr<chain_sequence_node_t<T>>;
                        size_t matrix_index = MAX_SIZE_T_VALUE;

                        child_ptr left_child {};
                        child_ptr right_child {};

                        chain_sequence_node_t (chain_sequence_node_t<T>* left_child_, chain_sequence_node_t<T>* right_child_):
                                left_child(left_child_), right_child(right_child_) {}
                        chain_sequence_node_t (size_t matrix_index_):
                                matrix_index(matrix_index_) {} 
        };

        template <typename T>
        class matrix_chain_t
        {
                private:
                        std::vector<std::shared_ptr<imatrix_t>> chain {};
                        std::list<size_t> optimal_sequence {};
                        bool found_optimal = false;

                        inline void find_optimal_sequence ();
                        inline void find_optimal_sequence (std::vector<size_t> &matrix_sizes);
                        inline void save_optimal_sequence (std::unique_ptr<chain_sequence_node_t<T>> &root);
                        
                        inline chain_sequence_node_t<T>* create_multiplication_tree (sq_matrix_t<size_t> &boarder_indexes, size_t i, size_t j);

                public:
                        matrix_chain_t () = default;
                        matrix_chain_t (std::vector<std::shared_ptr<imatrix_t>> chain_) : chain(chain_) {}

                        void add_matrix (imatrix_t *rhs_) { chain.push_back(std::move(std::shared_ptr<imatrix_t>(rhs_))); 
                                                            found_optimal = false; }
                        size_t get_chain_length () const { return chain.size(); }

                        inline void print_optimal_sequence ();
                        inline void print_optimal_sequence (std::vector<size_t> &matrix_sizes);

                        inline std::shared_ptr<imatrix_t> optimal_mul ();
                        inline std::shared_ptr<imatrix_t> dummy_mul   ();
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
                print(optimal_sequence);
                std::cout << " optimal seq\n";

                std::shared_ptr<imatrix_t> temp {chain[0]};
                for (auto it = ++optimal_sequence.begin(); it != optimal_sequence.end(); ++it) {
                        temp = std::shared_ptr<imatrix_t> {temp->mul(*chain[*it])};
                }

                return temp;
        }

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

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
                print(matrixes_sizes);
                std::cout << " sizes\n";
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
                boarder_indexes.dump();
                std::unique_ptr<chain_sequence_node_t<T>> root {create_multiplication_tree(boarder_indexes, 0, chain_length - 1)};
                save_optimal_sequence(root);
        }

        template <typename T>
        inline void matrix_chain_t<T>::save_optimal_sequence (std::unique_ptr<chain_sequence_node_t<T>> &root)
        {
                if (root->matrix_index != MAX_SIZE_T_VALUE) {
                        std::cout << "pushed " << root->matrix_index << "\n";

                        optimal_sequence.push_back(root->matrix_index);
                } else {
                        save_optimal_sequence(root->left_child);
                        std::cout << "right\n";
                        save_optimal_sequence(root->right_child);
                }
        }

        template <typename T>
        inline chain_sequence_node_t<T>* matrix_chain_t<T>::create_multiplication_tree (sq_matrix_t<size_t> &boarder_indexes, 
                                                                                        size_t i, size_t j)
        {
                if (j == i) {
                        std::cout << i << " " << boarder_indexes[i][j] << " matrix\n";
                        return new chain_sequence_node_t<T>(i);
                }
                auto left_child  = create_multiplication_tree(boarder_indexes, i, boarder_indexes[i][j]);
                auto right_child = create_multiplication_tree(boarder_indexes, boarder_indexes[i][j] + 1, j);

                return new chain_sequence_node_t<T>(left_child, right_child);
        }
}
