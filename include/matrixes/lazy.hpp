#pragma once

#include <memory>
#include <vector>

#include "imatrix.hpp"

namespace matrixes
{
        template <typename T>
        class lazy_matrix_container_t
        {
                private:
                        std::vector<row_t<T>*> rows;
                        size_t n_rows;
                        size_t n_cols;

                        int linked_with;
                public:
                        lazy_matrix_container_t (size_t n_rows_, size_t n_cols_):
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

                                linked_with = 1;
                        }

                        lazy_matrix_container_t (std::vector<row_t<T>*> &&rows_):
                                rows(rows_), n_rows(rows.size()), n_cols(rows[0]->get_size()) {}

                        ~lazy_matrix_container_t ()
                        {
                                for (size_t i = 0; i < n_rows; i++) {
                                        destroy(rows[i]);
                                        ::operator delete(rows[i]);
                                }
                        }

                        lazy_matrix_container_t (lazy_matrix_container_t &rhs) = delete;
                        lazy_matrix_container_t operator=(lazy_matrix_container_t &rhs) = delete;

                        lazy_matrix_container_t (lazy_matrix_container_t &&rhs) noexcept :
                                rows(std::move(rhs.rows)), n_rows(rhs.n_rows), n_cols(rhs.n_cols),
                                linked_with(rhs.linked_with)
                        {
                                rhs.n_rows      = 0;
                                rhs.n_cols      = 0;
                                rhs.linked_with = 0;
                        }

                        lazy_matrix_container_t& operator= (lazy_matrix_container_t &&rhs) noexcept
                        {
                                std::swap(rows,   rhs.rows);
                                std::swap(n_rows, rhs.n_rows);
                                std::swap(n_cols, rhs.n_cols);
                                std::swap(linked_with, rhs.linked_with);

                                return *this;
                        }

                        void inc_linked_with () { linked_with++; }
                        void dec_linked_with () { linked_with--; }
                        int  get_linked_with () { return linked_with; }

                        size_t get_n_cols () const  { return n_cols; }
                        size_t get_n_rows () const  { return n_rows; }

                        row_t<T>* operator[] (size_t i) { return rows[i]; }

                        lazy_matrix_container_t* clone ()
                        {
                                auto temp = new lazy_matrix_container_t<T> {n_rows, n_cols};
                                for (size_t i = 0; i < n_rows; i++)
                                        construct((*temp)[i], *rows[i]);
                                
                                return temp;
                        }
        };

        template <typename T>
        class lazy_matrix_t : public imatrix_t<T>
        {
                private:
                        std::shared_ptr<lazy_matrix_container_t<T>> container;
                public:
                        template <typename It>
                        lazy_matrix_t (size_t n_rows_, size_t n_cols_, It begin_):
                                container(new lazy_matrix_container_t<T>{n_rows_, n_cols_}) 
                        {
                                for (size_t i = 0; i < n_rows_; i++) {
                                        construct((*container)[i], std::move(row_t<T>(begin_, begin_ + n_cols_)));
                                        begin_ += n_cols_;
                                }
                        }

                        lazy_matrix_t (std::vector<row_t<T>*> &&rows_):
                                container(new lazy_matrix_container_t<T>{std::move(rows_)}) {}

                        lazy_matrix_t (lazy_matrix_t &&rhs) = default;
                        lazy_matrix_t& operator=(lazy_matrix_t &&rhs) = default;

                        lazy_matrix_t (lazy_matrix_t &rhs):
                                container(rhs.container)
                        {
                                container->inc_linked_with();
                        }

                        lazy_matrix_t& operator=(lazy_matrix_t &rhs) 
                        {
                                container->dec_linked_with();
                                container = rhs.container;
                                container->inc_linked_with(); 

                                return *this;
                        }

                        row_t<T>& operator[] (size_t n_row_)
                        {
                                if (n_row_ >= get_n_rows())
                                        throw std::out_of_range("Wrong number of row.");
                                return *(*container)[n_row_];
                        }

                        const row_t<T>& operator[] (size_t n_row_) const
                        {
                                if (n_row_ >= get_n_rows())
                                        throw std::out_of_range("Wrong number of row.");
                                return *(*container)[n_row_];
                        }

                        size_t get_n_cols () const override { return container->get_n_cols(); }
                        size_t get_n_rows () const override { return container->get_n_rows(); }
                        int    get_linked_with () const { return container->get_linked_with(); }
                        void swap (size_t index1, size_t index2);
                        std::vector<T> mul_container (imatrix_t<T> &rhs_) const;
                        imatrix_t<T>*  mul (imatrix_t<T> &rhs_) const override;

                        static lazy_matrix_t<T> gen_random (size_t n_rows, size_t n_cols);

                        lazy_matrix_container_t<T>* clone_container () const { return container->clone(); }

                        void dump () const;

                        lazy_matrix_t& operator*= (lazy_matrix_t &rhs);
                        lazy_matrix_t& operator+=(const lazy_matrix_t &rhs);
                        lazy_matrix_t& operator-=(const lazy_matrix_t &rhs);
                        lazy_matrix_t& operator*=(const T &rhs);
                        lazy_matrix_t& operator/=(const T &rhs);
                        lazy_matrix_t& operator+=(const T &rhs);
                        lazy_matrix_t& operator-=(const T &rhs);

                        row_t<T> power_iteration (int n_iters) const;

                        private:
                                void check_sizes (const lazy_matrix_t &rhs) const
                                {
                                        if (get_n_rows() != rhs.get_n_rows())
                                                throw std::runtime_error("Lazy matrixes cannot operate with different sizes.");
                                }

                                void prepare_before_operate () 
                                {
                                        if (container->get_linked_with() > 1) {
                                                auto temp = container;
                                                container.reset(container->clone());
                                                temp->dec_linked_with();
                                        }
                                }
        };

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T> lazy_matrix_t<T> 
lazy_matrix_t<T>::gen_random (size_t n_rows, size_t n_cols)
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

        return lazy_matrix_t<T> {std::move(rows)};
}


template <typename T>
void lazy_matrix_t<T>::swap (size_t index1, size_t index2)
{
        auto temp         = container[index1];
        container[index1] = container[index2];
        container[index2] = temp; 
}

template <typename T>
std::vector<T> lazy_matrix_t<T>::mul_container (imatrix_t<T> &rhs_) const
{
        auto n_cols = container->get_n_cols(); 
        if (n_cols != static_cast<lazy_matrix_t<T>&>(rhs_).get_n_rows())
                throw std::out_of_range("Wrong matrixes dimensions.");
                
        std::vector<T> new_elems {};
        auto n_rows = container->get_n_rows();
        for (size_t i = 0; i < n_rows; i++) {
                auto n_cols_ = static_cast<lazy_matrix_t<T>&>(rhs_).get_n_cols();
                for (size_t k = 0; k < n_cols_; k++) {
                        T temp {};
                        for (size_t j = 0; j < n_cols; j++) {
                                temp += (*this)[i][j] * (static_cast<lazy_matrix_t<T>&>(rhs_))[j][k];                                
                        }
                        new_elems.push_back(temp);
                }
        }
        return new_elems;
}
        
template <typename T>
imatrix_t<T>* lazy_matrix_t<T>::mul (imatrix_t<T> &rhs_) const
{
        std::vector<T> new_elems = mul_container(rhs_);

        return new lazy_matrix_t {get_n_rows(), static_cast<imatrix_t<T>&>(rhs_).get_n_cols(), new_elems.begin()};
}

template <typename T>
void lazy_matrix_t<T>::dump () const
{
        auto n_rows = container->get_n_rows();
        for (size_t i = 0; i < n_rows; i++) {
                std::cout << "[ ";
                (*container)[i]->dump();
                std::cout << " ]\n";
        }
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator*= (lazy_matrix_t &rhs)
{
        check_sizes(rhs);
        auto n_cols = container->get_n_cols(); 

        if (container->get_linked_with() > 1) {
                std::vector<T> new_elems = mul_container(rhs);
                auto prev_container = container;
                container.reset(new lazy_matrix_container_t<T> {get_n_rows(), n_cols});
                        
                auto elems_it = new_elems.begin();
                auto n_rows = get_n_rows();
                for (size_t i = 0; i < n_rows; i++) {
                        construct((*container)[i], std::move(row_t<T>(elems_it, elems_it + n_cols)));
                        elems_it += n_cols;
                }
                prev_container->dec_linked_with();
        }

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator+= (const lazy_matrix_t &rhs)
{
        check_sizes(rhs);
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] += rhs[i];

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator-= (const lazy_matrix_t &rhs)
{
        check_sizes(rhs);
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] -= rhs[i];

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator*= (const T &rhs)
{
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] *= rhs;

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator/= (const T &rhs)
{
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] /= rhs;

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator+= (const T &rhs)
{
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] += rhs;

        return *this;
}

template <typename T>
lazy_matrix_t<T>& lazy_matrix_t<T>::operator-= (const T &rhs)
{
        prepare_before_operate();
                                
        size_t n_rows = get_n_rows();
        for (size_t i = 0; i < n_rows; i++)
                *(*container)[i] -= rhs;

        return *this;
}

template <typename T>
row_t<T> lazy_matrix_t<T>::power_iteration (int n_iters) const
{
        auto b_k = row_t<T>::get_random(get_n_cols());

        for (int i = 0; i < n_iters; i++) {
                auto b_k1 = *this * b_k;
                b_k = b_k1 / b_k1.module();
        }
        return b_k;
}


//-------------------------------------------~~~~~~Binary Operators and Utils~~~~~~--------------------------------------------------------------------

template <typename T>
bool operator== (const lazy_matrix_t<T> &lhs, const lazy_matrix_t<T> &rhs) 
{
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
lazy_matrix_t<T> perform_oper (const lazy_matrix_t<T> &lhs, 
                               const lazy_matrix_t<T> &rhs, F oper) 
{
        auto new_rows = perform_oper(reinterpret_cast<const imatrix_t<T>*>(&lhs), 
                                     reinterpret_cast<const imatrix_t<T>*>(&rhs),
                                     oper);
        return lazy_matrix_t<T>{std::move(new_rows)};
}

template <typename T>
lazy_matrix_t<T> operator+ (const lazy_matrix_t<T> &lhs, const lazy_matrix_t<T> &rhs) 
{
        return perform_oper(lhs, rhs, [] (T l, T r) { return l + r; });
}

template <typename T>
lazy_matrix_t<T> operator- (const lazy_matrix_t<T> &lhs, const lazy_matrix_t<T> &rhs) 
{
        return perform_oper(lhs, rhs, [] (T l, T r) { return l - r; });
}
}