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
                public:
                        template<typename It>
                        sq_matrix_t (size_t n_rows_, It start_, It end_):
                                matrix_t<T>{n_rows_, n_rows_, start_, end_}
                        {}

                        sq_matrix_t (const sq_matrix_t &rhs_):
                                matrix_t<T>{rhs_}
                        {
                                determinant = rhs_.determinant;
                                det_is_calculated = rhs_.det_is_calculated;
                        }

                        sq_matrix_t (const sq_matrix_t &&rhs_):
                                matrix_t<T>{rhs_}
                        {
                                determinant = rhs_.determinant;
                                det_is_calculated = rhs_.det_is_calculated;
                        }

                        sq_matrix_t& operator=(const sq_matrix_t &rhs_)
                        {
                                return *this = sq_matrix_t(rhs_);
                        }

                        sq_matrix_t& operator=(sq_matrix_t &&rhs_)
                        {
                                if (this != &rhs_) {
                                        this->n_rows = rhs_.n_rows;
                                        this->n_cols = rhs_.n_cols;

                                        this->rows.swap(rhs_.rows);
                                        determinant = rhs_.determinant;
                                        det_is_calculated = rhs_.det_is_calculated;
                                }       

                                return *this;
                        }

                        ~sq_matrix_t () = default;

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
                T max_elem {};
                size_t max_elem_index {};
                row_t<size_t> permutation {};

                for (size_t i = 0; i <= this->n_rows; i++)
                        permutation.insert(i);
                
                for (size_t i = 0; i < this->n_rows; i++) {
                        max_elem = 0;
                        max_elem_index = 0;

                        for (size_t k = i; k < this->n_rows; k++) {
                                auto abs_elem = std::fabs((*this)[k][i]);
                                if (abs_elem > max_elem) {
                                        max_elem       = abs_elem;
                                        max_elem_index = k;
                                }
                        }

                        if (max_elem < degeneration_threshold)
                                return {permutation, status_t::DEGENERATED};
                        
                        if (max_elem_index != i) {
                                auto temp = permutation[i];
                                permutation[i] = permutation[max_elem_index];
                                permutation[max_elem_index] = temp;

                                this->rows[i].swap(this->rows[max_elem_index]);

                                permutation[this->n_cols]++;
                        }

                        for (size_t j = i + 1; j < this->n_cols; j++) {
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
}