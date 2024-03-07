#pragma once

#include "row.hpp"

namespace matrixes
{

template <typename T>
class imatrix_t 
{
        public:
                virtual imatrix_t<T>* mul (imatrix_t<T> &rhs_) const = 0;
                        
                virtual size_t get_n_cols () const = 0;
                virtual size_t get_n_rows () const = 0;

                virtual       row_t<T>& operator[] (size_t n_row_)       = 0;
                virtual const row_t<T>& operator[] (size_t n_row_) const = 0;
                        
                virtual ~imatrix_t () {}
};

}