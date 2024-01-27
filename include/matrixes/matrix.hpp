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
                                                n_rows = 0;
                                                n_cols = 0;
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
                                if (n_rows)
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
                                std::swap(rows,   rhs_.rows);
                                std::swap(n_rows, rhs_.n_rows);
                                std::swap(n_cols, rhs_.n_cols);

                                return *this;
                        }
        };

        class imatrix_t 
        {
                public:
                        virtual imatrix_t* mul (imatrix_t &rhs_) const = 0;
                        virtual size_t get_n_cols () const = 0;
                        virtual size_t get_n_rows () const = 0;
                        virtual ~imatrix_t () {}
        };

        template <typename T>
        class matrix_t : private matrix_container_t<T>, public imatrix_t
        {
                protected:
                        using matrix_container_t<T>::rows;
                        using matrix_container_t<T>::n_rows;
                        using matrix_container_t<T>::n_cols;

                public:
                        template<typename It>
                        matrix_t (size_t n_rows_, size_t n_cols_, It begin_):
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
                                        construct(rows[i], *rhs_.rows[i]);
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

                        const row_t<T>& operator[] (size_t n_row_) const
                        { 
                                if (n_row_ >= n_rows)
                                        throw std::out_of_range("Wrong number of row.");
                                return *rows[n_row_];
                        }

                        size_t get_n_cols () const { return n_cols; }
                        size_t get_n_rows () const { return n_rows; }
                        inline void swap (size_t index1, size_t index2);
                        inline imatrix_t* mul (imatrix_t &rhs_) const;

                        inline void dump () const;
        };

        template <typename T>
        inline void matrix_t<T>::swap (size_t index1, size_t index2)
        {
                auto temp    = rows[index1];
                rows[index1] = rows[index2];
                rows[index2] = temp; 
        }

        template <typename T>
        inline imatrix_t* matrix_t<T>::mul (imatrix_t &rhs_) const
        {
                if (n_cols != static_cast<matrix_t<T>&>(rhs_).n_rows)
                        throw std::out_of_range("Wrong matrixes dimensions.");
                
                std::vector<T> new_elems {};
                for (size_t i = 0; i < n_rows; i++) {
                        for (size_t k = 0; k < static_cast<matrix_t<T>&>(rhs_).n_cols; k++) {
                                T temp {};
                                for (size_t j = 0; j < n_cols; j++) {
                                        temp += (*this)[i][j] * (static_cast<matrix_t<T>&>(rhs_))[j][k];                                
                                }
                                new_elems.push_back(temp);
                        }
                }

                return new matrix_t {n_rows, static_cast<matrix_t<T>&>(rhs_).n_cols, new_elems.begin()};
        }

        template <typename T>
        inline void matrix_t<T>::dump () const
        {
                for (size_t i = 0; i < n_rows; i++) {
                        std::cout << "[ ";
                        rows[i]->dump();
                        std::cout << " ]\n";
                }
        }

        template <typename T>
        bool operator== (const matrix_t<T>& lhs, const matrix_t<T>& rhs) {
                auto lhs_n_rows = lhs.get_n_rows();
                auto lhs_n_cols = lhs.get_n_cols();
        
                bool res = lhs_n_rows == rhs.get_n_rows() && lhs_n_cols == rhs.get_n_cols();

                if (res)
                        for (size_t i = 0; i < lhs_n_rows; i++)
                                for (size_t j = 0; j < lhs_n_cols; j++)
                                        if (lhs[i][j] != rhs[i][j])
                                                return false;
                return res;
        }
}