#pragma once 

#include <vector>
#include <stdexcept>
#include <cmath>

#include "utils/std_vector.hpp"
#include "utils/utils.hpp"

namespace matrixes
{
        template <typename T>
        class row_container_t
        {
                protected:
                        T* data = nullptr;
                        size_t size     = 0;
                        size_t capacity = 0;

                        row_container_t (size_t cap_):
                                data(cap_ ? static_cast<T*>(::operator new(sizeof(T) * cap_)) : nullptr),
                                capacity(cap_)
                                {}
                        
                        ~row_container_t ()
                        {
                                destroy(data, data + size);
                                ::operator delete(data);
                        }
                        
                        row_container_t (row_container_t &rhs_) = delete;
                        row_container_t operator=(row_container_t &rhs_) = delete;

                        row_container_t (row_container_t &&rhs_) noexcept :
                                data(rhs_.data), size(rhs_.size), capacity(rhs_.capacity)
                        {
                                rhs_.data     = nullptr;
                                rhs_.size     = 0;
                                rhs_.capacity = 0;
                        }

                        row_container_t& operator=(row_container_t &&rhs_) noexcept
                        {
                                std::swap(data, rhs_.data);
                                std::swap(size, rhs_.size);
                                std::swap(capacity, rhs_.capacity);

                                return *this;
                        }
        };
        
        template <typename T>
        class row_t : private row_container_t<T>
        {
                using row_container_t<T>::data;
                using row_container_t<T>::size;
                using row_container_t<T>::capacity;

                public:
                        template<typename It>
                        row_t (It begin_, It end_):
                                row_container_t<T>(std::distance(begin_, end_))
                        {
                                while (begin_ != end_) {
                                        construct(data + size, *begin_);
                                        size++;
                                        begin_++;
                                }
                        }

                        row_t (size_t cap_):
                                row_container_t<T>(cap_)
                                {}

                        row_t (row_t &&rhs_) = default;
                        row_t& operator=(row_t &&rhs_) = default;

                        row_t (const row_t &rhs_):
                                row_container_t<T>(rhs_.size)
                        {
                                while (size != rhs_.size) {
                                        construct(data + size, rhs_.data[size]);
                                        size++;
                                }
                        }

                        row_t& operator=(row_t &rhs_)
                        {
                                row_t tmp {rhs_};
                                std::swap(*this, tmp);
                                return *this;
                        }

                        row_t& operator+=(const row_t &rhs_) {
                                if (size != rhs_.size)
                                        throw std::runtime_error("Cannot sum rows with different sizes.");
                                for (size_t i = 0; i < size; i++)
                                        data[i] += rhs_.data[i];

                                return *this;
                        }

                        row_t& operator-=(const row_t &rhs_) {
                                if (size != rhs_.size)
                                        throw std::runtime_error("Cannot sub rows with different sizes.");
                                for (size_t i = 0; i < size; i++)
                                        data[i] -= rhs_.data[i];

                                return *this;
                        }

                        row_t& operator*=(const T &rhs_) {
                                for (size_t i = 0; i < size; i++)
                                        data[i] *= rhs_;

                                return *this;
                        }

                        row_t& operator/=(const T &rhs_) {
                                for (size_t i = 0; i < size; i++)
                                        if (rhs_)
                                                data[i] /= rhs_;

                                return *this;
                        }

                        row_t& operator+=(const T &rhs_) {
                                for (size_t i = 0; i < size; i++)
                                        data[i] += rhs_;

                                return *this;
                        }

                        row_t& operator-=(const T &rhs_) {
                                for (size_t i = 0; i < size; i++)
                                        data[i] -= rhs_;

                                return *this;
                        }

                        T& operator[] (size_t index_)
                        {
                                if (index_ >= size)
                                        throw std::out_of_range("Wrong index of element.");
                                return data[index_];
                        }

                        const T& operator[] (size_t index_) const
                        {
                                if (index_ >= size)
                                        throw std::out_of_range("Wrong index of element.");
                                return data[index_];
                        }

                        void insert (const T &elem_)
                        {
                                if (size < capacity) {
                                        construct(data + size, elem_);
                                        size++;
                                } else {
                                        throw std::out_of_range("Cannot insert more elements in row.");
                                }
                        }

                        T module () const // need to replace with dot product?
                        { 
                                T sum {};
                                for (size_t i = 0; i < size; i++)
                                        sum += data[i] * data[i];
                                return std::sqrt(sum);
                        }

                        size_t get_size () const { return size; }
                        void swap (size_t index1, size_t index2);

                        void dump () const;

                        static row_t<T> gen_random (size_t size);
        };

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

        template <typename T>
        void row_t<T>::swap (size_t index1, size_t index2)
        {
                auto temp    = data[index1];
                data[index1] = data[index2];
                data[index2] = temp; 
        }

        template <typename T>
        void row_t<T>::dump () const
        { 
                for (size_t i = 0; i < size; i++)
                        std::cout << data[i] << " ";
        }

        template <typename T>
        row_t<T> row_t<T>::gen_random (size_t size)
        {
                std::vector<T> elems {};
                elems.reserve(size);
                for (size_t i = 0; i < size; i++)
                        elems.push_back(get_random_num<T>());

                return row_t<T>(elems.begin(), elems.end());
        }

        template <typename T>
        row_t<T> operator/(const row_t<T> &lhs_, const T &rhs_)
        {
                row_t tmp(lhs_);
                tmp /= rhs_;
                return tmp;
        }

        template <typename T>
        row_t<T> operator+(const row_t<T> &lhs_, const row_t<T> &rhs_)
        {
                row_t tmp(lhs_);
                tmp += rhs_;
                return tmp;
        }

        template <typename T>
        row_t<T> operator-(const row_t<T> &lhs_, const row_t<T> &rhs_)
        {
                row_t tmp(lhs_);
                tmp -= rhs_;
                return tmp;
        }

        template <typename T, typename F>
        row_t<T>* perform_oper (const row_t<T> &lhs_, const row_t<T> &rhs_, F oper)
        {
                auto size = lhs_.get_size();
                row_t<T> *row = new row_t<T>{size};
                
                for (size_t i = 0; i < size; i++)
                        row->insert(oper(lhs_[i], rhs_[i]));
                
                return row;
        }
}