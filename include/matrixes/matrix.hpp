#include <vector>
#include <memory>
#include <iterator>

#include "row.hpp"

namespace matrix
{
        template <typename T>
        class matrix_t 
        {
                private:
                        size_t n_rows;
                        size_t n_cols;

                        using rows_vector_t = typename std::vector<std::unique_ptr<row_t<T>>>;
                        using row_ptr_t     = typename std::unique_ptr<row_t<T>>;

                        rows_vector_t rows = {};

                public:
                        template<typename It>
                        matrix_t (size_t n_rows_, size_t n_cols_, It start_, It end_ ):
                                n_rows(n_rows_), n_cols(n_cols_)
                        {
                                for (auto it = start_; it != end_; it += n_cols)
                                        rows.push_back(row_ptr_t{new row_t<T> {it, it + n_cols}});
                        };

                        matrix_t (const matrix_t &rhs_):
                                n_rows(rhs_.n_rows), n_cols(rhs_.n_cols)
                        {
                                for (auto &row : rhs_.rows)
                                        rows.push_back(row_ptr_t{new row_t<T> {*row}});
                        }

                        matrix_t (const matrix_t &&rhs_):
                                n_rows(rhs_.n_rows), n_cols(rhs_.n_cols)
                        {
                                rows.swap(rhs_.rows);
                        }

                        matrix_t& operator=(const matrix_t &rhs_)
                        {
                                return *this = matrix_t(rhs_);
                        }

                        matrix_t& operator=(matrix_t &&rhs_)
                        {
                                if (this != &rhs_) {
                                        n_rows = rhs_.n_rows;
                                        n_cols = rhs_.n_cols;

                                        rows.swap(rhs_.rows);
                                }       
                                return *this;
                        }

                        ~matrix_t () = default;

                        row_t<T> operator[] (size_t n_row_) { return *rows[n_row_]; }

                        size_t get_n_cols () const { return n_cols; }
                        size_t get_n_rows () const { return n_rows; }

                        // void transpose () {};
                        // void negate () {};
        };
}