#pragma once

#include <vector>
#include <memory>
#include <iterator>

#include "imatrix.hpp"

namespace matrixes
{
        template <typename T>
        class matrix_container_t
        {
                protected:
                        std::vector<row_t<T>*> rows;
                        size_t n_rows;
                        size_t n_cols;

                        matrix_container_t (size_t n_rows_, size_t n_cols_):
                                rows(n_rows_), n_rows(n_rows_), n_cols(n_cols_)
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        try {
                                                rows[i] = static_cast<row_t<T>*>(::operator new(sizeof(row_t<T>)));
                                        } catch (std::bad_alloc &ba) {
                                                while (i) {
                                                        --i;
                                                        ::operator delete(rows[i]);
                                                }
                                                n_rows = 0;
                                                n_cols = 0;
                                                throw std::bad_alloc();
                                        }
                                }
                        }

                        matrix_container_t (std::vector<row_t<T>*> &&rows_):
                                rows(rows_), n_rows(rows.size()), n_cols(rows[0]->get_size()) {}

                        ~matrix_container_t ()
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        destroy(rows[i]);
                                        ::operator delete(rows[i]);
                                }
                        }

                        matrix_container_t (matrix_container_t &rhs_) = delete;
                        matrix_container_t operator=(matrix_container_t &rhs) = delete;

                        matrix_container_t (matrix_container_t &&rhs_) noexcept :
                                rows(rhs_.rows), n_rows(rhs_.n_rows), n_cols(rhs_.n_cols)
                        {
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

        template <typename T>
        class matrix_t : private matrix_container_t<T>, public imatrix_t<T>
        {
                protected:
                        using matrix_container_t<T>::rows;
                        using matrix_container_t<T>::n_rows;
                        using matrix_container_t<T>::n_cols;

                public:
                        template <typename It>
                        matrix_t (size_t n_rows_, size_t n_cols_, It begin_):
                                matrix_container_t<T>(n_rows_, n_cols_)
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        construct(rows[i], std::move(row_t<T>(begin_, begin_ + n_cols)));
                                        begin_ += n_cols;
                                }
                        }

                        matrix_t (std::vector<row_t<T>*> &&rows_):
                                matrix_container_t<T>{std::move(rows_)} {}

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

                        row_t<T>& operator[] (size_t n_row_) override
                        {
                                if (n_row_ >= n_rows)
                                        throw std::out_of_range("Wrong number of row.");
                                return *rows[n_row_];
                        }

                        const row_t<T>& operator[] (size_t n_row_) const override
                        { 
                                if (n_row_ >= n_rows)
                                        throw std::out_of_range("Wrong number of row.");
                                return *rows[n_row_];
                        }

                        size_t get_n_cols () const override { return n_cols; }
                        size_t get_n_rows () const override { return n_rows; }
                        void swap (size_t index1, size_t index2);
                        imatrix_t<T>* mul (imatrix_t<T> &rhs_) const override;
                        imatrix_t<T>* power_zero () const override;
                        row_t<T> mul (row_t<T> &rhs_) const override;

                        static matrix_t<T> gen_random (size_t n_rows, size_t n_cols);
                        row_t<T> power_iteration (int n_iters) const;


                        void dump () const;
        };

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

        template <typename T>
        void matrix_t<T>::swap (size_t index1, size_t index2)
        {
                auto temp    = rows[index1];
                rows[index1] = rows[index2];
                rows[index2] = temp; 
        }

        template <typename T>
        imatrix_t<T>* matrix_t<T>::mul (imatrix_t<T> &rhs_) const
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

                return new matrix_t {n_rows, rhs_.get_n_cols(), new_elems.begin()};
        }

template <typename T>
imatrix_t<T>* matrix_t<T>::power_zero () const
{
        size_t n_elems = n_cols * n_rows;
        
        std::vector<T> elems (n_elems);
        for (size_t i = 0; i < n_elems; i += 1 + n_cols)
                elems[i] = 1;

        return new matrix_t {n_rows, n_rows, elems.begin()};
}

        template <typename T>
        row_t<T> matrix_t<T>::mul (row_t<T> &rhs_) const
        {
                auto n_rows = get_n_rows();
                if (n_rows != rhs_.get_size())
                        throw std::out_of_range("Wrong matrix and row dimensions.");
                        
                std::vector<T> new_elems {};
                auto n_cols = get_n_cols();
                for (size_t i = 0; i < n_rows; i++) {
                        T temp {};
                        for (size_t j = 0; j < n_cols; j++)
                                temp += (*this)[i][j] * rhs_[j];                                
                        new_elems.push_back(temp);
                }

                return row_t<T> {new_elems.begin(), new_elems.end()};
        }

        template <typename T> matrix_t<T> 
        matrix_t<T>::gen_random (size_t n_rows, size_t n_cols)
        {
                std::vector<row_t<T>*> rows {};
                rows.reserve(n_rows);
                try {
                        for (size_t i = 0; i < n_rows; i++)
                                rows.push_back(new row_t<T> {row_t<T>::gen_random(n_cols)});
                } catch (...) {
                        for (auto&& row : rows)
                                delete row;
                        throw;
                }

                return matrix_t<T> {std::move(rows)};
        }

        template <typename T>
        row_t<T> matrix_t<T>::power_iteration (int n_iters) const
        {
                auto b_k = row_t<T>::gen_random(get_n_cols());

                for (int i = 0; i < n_iters; i++) {
                        auto b_k1 = this->mul(b_k);
                        b_k = b_k1 / b_k1.module();
                }
                return b_k;
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

//-------------------------------------------~~~~~~Binary Operators and Utils~~~~~~--------------------------------------------------------------------

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

        template <typename T, typename F>
        matrix_t<T> perform_oper (const matrix_t<T> &lhs, 
                                  const matrix_t<T> &rhs, F oper) 
        {
                auto new_rows = perform_oper(reinterpret_cast<const imatrix_t<T>*>(&lhs), 
                                             reinterpret_cast<const imatrix_t<T>*>(&rhs),
                                             oper);
                return matrix_t<T>{std::move(new_rows)};
        }

        template <typename T>
        matrix_t<T> operator+ (const matrix_t<T> &lhs, const matrix_t<T> &rhs) 
        {
                return perform_oper(lhs, rhs, [] (T l, T r) { return l + r; });
        }

        template <typename T>
        matrix_t<T> operator- (const matrix_t<T> &lhs, const matrix_t<T> &rhs) 
        {
                return perform_oper(lhs, rhs, [] (T l, T r) { return l - r; });
        }
}