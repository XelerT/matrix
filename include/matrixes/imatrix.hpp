#pragma once

#include "row.hpp"

namespace matrixes
{

template <typename T>
class imatrix_t 
{
        public:
                virtual imatrix_t<T>* mul (imatrix_t<T> &rhs_) const = 0;
                virtual row_t<T>      mul (row_t<T> &rhs_)     const = 0;

                virtual imatrix_t<T>* power_zero () const = 0;
                        
                virtual size_t get_n_cols () const = 0;
                virtual size_t get_n_rows () const = 0;

                virtual       row_t<T>& operator[] (size_t n_row_)       = 0;
                virtual const row_t<T>& operator[] (size_t n_row_) const = 0;
                        
                virtual ~imatrix_t () {}
};

//----------------------------------------------~~~~~~Utils~~~~~~----------------------------------------------------------------------------

template <typename T>
imatrix_t<T>* power (const imatrix_t<T> *mtx, int n)
{
        if (!n)
                return mtx->power_zero();

        imatrix_t<T> *x = power(mtx, n / 2);
        imatrix_t<T> *y = x->mul(*x);

        if (n % 2)
                return mtx->mul(*y);
        else
                return x->mul(*x);
}

template <typename T, typename F>
std::vector<row_t<T>*> perform_oper (const imatrix_t<T> *lhs, 
                                     const imatrix_t<T> *rhs, F oper) 
{
        auto lhs_n_rows = lhs->get_n_rows();
        auto lhs_n_cols = lhs->get_n_cols();
        std::vector<row_t<T>*> new_rows {};
        
        bool res = lhs_n_rows == rhs->get_n_rows() && lhs_n_cols == rhs->get_n_cols();
        if (res) {
                for (size_t i = 0; i < lhs_n_rows; i++) {
                        try {
                                auto row = perform_oper((*lhs)[i], (*rhs)[i], oper);
                                new_rows.push_back(row);
                        } catch (...) {
                                for (auto& row : new_rows)
                                        delete row;
                                throw;
                        }
                } 
        } else {
                throw std::out_of_range("Lazy matrixes with different sizes.");
        }
        return new_rows; 
}

}