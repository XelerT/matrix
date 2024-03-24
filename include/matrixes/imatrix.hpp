#pragma once

#include <vector>
#include <iostream>
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
                virtual imatrix_t<T>* clone () const = 0;
                        
                virtual size_t get_n_cols () const = 0;
                virtual size_t get_n_rows () const = 0;

                virtual       row_t<T>& operator[] (size_t n_row_)       = 0;
                virtual const row_t<T>& operator[] (size_t n_row_) const = 0;
                        
                virtual ~imatrix_t () {}
};

//----------------------------------------------~~~~~~Utils~~~~~~----------------------------------------------------------------------------

template <typename T> std::unique_ptr<imatrix_t<T>>
power (const std::unique_ptr<imatrix_t<T>> &mtx, int n)
{
        using imtrx_uptr = std::unique_ptr<imatrix_t<T>>;
        std::vector<imatrix_t<T>*> matrixes {mtx->clone()};

        for (int i = n, size = 0; i > 1; i /= 2, size++) {
                auto temp = matrixes[size]->mul(*matrixes[size]);
                if (i % 2) {
                        matrixes.push_back(mtx->mul(*temp));
                        delete temp;
                } else {
                        matrixes.push_back(temp);
                }
        }

        auto last_mtx_it = matrixes.end() - 1;
        for (auto it = matrixes.begin(); it != last_mtx_it; ++it) {
                delete *it;
        }
        return imtrx_uptr {*last_mtx_it};
}

template <typename T> std::unique_ptr<imatrix_t<T>>
rec_power (const std::unique_ptr<imatrix_t<T>> &mtx, int n)
{
        using imtrx_uptr = std::unique_ptr<imatrix_t<T>>;
        if (!n)
                return imtrx_uptr {mtx->power_zero()};

        imtrx_uptr x {power(mtx, n / 2)};
        imtrx_uptr y {x->mul(*x)};

        if (n % 2)
                return imtrx_uptr {mtx->mul(*y)};
        else
                return imtrx_uptr {x->mul(*x)};
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