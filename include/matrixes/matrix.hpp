#pragma once

#include <vector>
#include <memory>
#include <iterator>

#include "row.hpp"

namespace matrixes
{
        template <typename T>
        class matrix_container_t 
        {
                protected:
                        row_t<T>** rows;
                        size_t n_rows;
                        size_t n_cols;

                        matrix_container_t (size_t n_rows_, size_t n_cols_):
                                rows(n_rows_ ? static_cast<row_t<T>**>(new row_t<T>* [n_rows_]) : nullptr),
                                n_rows(n_rows_), n_cols(n_cols_)
                        {
                                for (size_t i = 0; i < n_rows_; i++) {
                                        try {
                                                rows[i] = static_cast<row_t<T>*>(::operator new(sizeof(row_t<T>)));
                                        } catch (std::bad_alloc &ba) {
                                                while (i) {
                                                        --i;
                                                        ::operator delete(rows[i]);
                                                }
                                                delete [] rows;
                                                throw std::bad_alloc();
                                        }
                                }
                        }

                        ~matrix_container_t ()
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        destroy(rows[i]);
                                        ::operator delete(rows[i]);
                                }
                                delete [] rows;
                        }

                        matrix_container_t (matrix_container_t &rhs_) = delete;
                        matrix_container_t operator=(matrix_container_t &rhs) = delete;

                        matrix_container_t (matrix_container_t &&rhs_) noexcept :
                                rows(rhs_.rows), n_rows(rhs_.n_rows), n_cols(rhs_.n_cols)
                        {
                                rhs_.rows   = nullptr;
                                rhs_.n_rows = 0;
                                rhs_.n_cols = 0;
                        }

                        matrix_container_t& operator=(matrix_container_t &&rhs_) noexcept
                        {
                                std::swap(rows, rhs_.rows);
                                std::swap(n_rows, rhs_.n_rows);
                                std::swap(n_cols, rhs_.n_cols);

                                return *this;
                        }
        };

        template <typename T>
        class matrix_t : private matrix_container_t<T>
        {
                protected:
                        using matrix_container_t<T>::rows;
                        using matrix_container_t<T>::n_rows;
                        using matrix_container_t<T>::n_cols;

                public:
                        template<typename It>
                        matrix_t (size_t n_rows_, size_t n_cols_, It begin_, It end_):
                                matrix_container_t<T>(n_rows_, n_cols_)
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        construct(rows[i], std::move(row_t<T>(begin_, begin_ + n_cols)));
                                        begin_ += n_cols;
                                }
                        };

                        matrix_t (matrix_t &&rhs) = default;
                        matrix_t& operator=(matrix_t &&rhs) = default;

                        matrix_t (matrix_t &rhs_):
                                matrix_container_t<T>(rhs_.n_rows, rhs_.n_cols)
                        {
                                for (size_t i = 0; i < n_rows; i++)
                                        construct(rows[i], std::move(*rhs_.rows[i]));
                        }

                        matrix_t& operator=(matrix_t &rhs_)
                        {
                                matrix_t tmp {rhs_};
                                std::swap(*this, tmp);
                                return *this;
                        }

                        row_t<T>& operator[] (size_t n_row_)
                        { 
                                if (n_row_ >= n_rows)
                                        throw std::out_of_range("Wrong number of row.");
                                return *rows[n_row_];
                        }

                        size_t get_n_cols () const { return n_cols; }
                        size_t get_n_rows () const { return n_rows; }
                        void swap (size_t index1, size_t index2);

                        void dump () const;
        };

        template <typename T>
        void matrix_t<T>::swap (size_t index1, size_t index2)
        {
                auto temp    = rows[index1];
                rows[index1] = rows[index2];
                rows[index2] = temp; 
        }

        template <typename T>
        void matrix_t<T>::dump () const
        {
                for (size_t i = 0; i < n_rows; i++) {
                        std::cout << "[ ";
                        rows[i]->dump();
                        std::cout << " ]\n";
                }
        }
}