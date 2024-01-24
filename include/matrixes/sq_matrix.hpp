#pragma once

#include <cmath>

#include "utils/utils.hpp"
#include "matrix.hpp"

namespace matrixes
{
        template <typename T>
        class sq_matrix_t : public matrix_t<T>
        {
                private:
                        T determinant {};
                        bool det_is_calculated = false;

                        std::pair<T, size_t> find_abs_max_elem_in_column (size_t index);

                        using matrix_t<T>::n_rows;
                        using matrix_t<T>::n_cols;
                public:
                        template <typename It>
                        sq_matrix_t (size_t n_rows_, It start_, It end_):
                                matrix_t<T>{n_rows_, n_rows_, start_, end_}
                        {}

                        std::pair<row_t<size_t>, status_t>
                        decompose (const T &degeneration_threshold);

                        std::pair<T, status_t> det ();
        };

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

        template <typename T>
        std::pair<row_t<size_t>, status_t> 
        sq_matrix_t<T>::decompose (const T &degeneration_threshold)
        {
                row_t<size_t> permutation {this->n_rows + 1};

                for (size_t i = 0; i <= this->n_rows; i++)
                        permutation.insert(i);
                
                for (size_t i = 0; i < this->n_rows; i++) {
                        auto max_elem_with_index = find_abs_max_elem_in_column(i);

                        if (max_elem_with_index.first < degeneration_threshold)
                                return {permutation, status_t::DEGENERATED};
                        
                        if (max_elem_with_index.second != i) {
                                permutation.swap(i, max_elem_with_index.second);
                                this->swap(i, max_elem_with_index.second);

                                permutation[this->n_cols]++;
                        }

                        for (size_t j = i + 1; j < this->n_rows; j++) {
                                (*this)[j][i] /= (*this)[i][i];

                                for (size_t k = i + 1; k < this->n_cols; k++)
                                        (*this)[j][k] -= (*this)[j][i] * (*this)[i][k];
                        }
                }

                return {permutation, status_t::OK};
        }

        template <typename T>
        std::pair<T, status_t> sq_matrix_t<T>::det () 
        {
                if (det_is_calculated)
                        return {determinant, status_t::OK};

                auto temp_matrix {*this};
                auto permutation_status = temp_matrix.decompose(THRESHOLD);

                if (permutation_status.second == status_t::DEGENERATED)
                        return {0, status_t::DEGENERATED};
                
                determinant = temp_matrix[0][0];
                for (size_t i = 1; i < this->n_cols; i++)
                        determinant *= temp_matrix[i][i];

                if ((permutation_status.first[this->n_cols] - this->n_cols) % 2 != 0)
                        determinant *= -1;
                det_is_calculated = true;
                
                return {determinant, status_t::OK};
        }

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

        template <typename T>
        std::pair<T, size_t> 
        sq_matrix_t<T>::find_abs_max_elem_in_column (size_t index)
        {
                T max_elem {};
                size_t max_elem_index {};

                for (size_t k = index; k < this->n_rows; k++) {
                        auto abs_elem = std::fabs((*this)[k][index]);
                        if (abs_elem > max_elem) {
                                max_elem       = abs_elem;
                                max_elem_index = k;
                        }
                }
                return {max_elem, max_elem_index}; 
        }

}