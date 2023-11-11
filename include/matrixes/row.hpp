#pragma once 

#include <vector>

namespace matrixes
{
        template <typename T>
        class row_t 
        {
                private:
                        size_t length {};
                        std::vector<T> data {};

                public:
                        #pragma GCC diagnostic ignored "-Weffc++"
                        template<typename It>
                        row_t (It start_, It end_)
                        {
                                data.insert(data.end(), start_, end_);
                                length = data.size();
                        };
                        #pragma GCC diagnostic warning "-Weffc++"

                        row_t () = default;

                        void insert (T &elem) 
                        {
                                data.push_back(elem);
                                length++;
                        }

                        void insert (T &elem, size_t index)
                        {
                                if (index < length)
                                        data.insert(elem, index);
                                else
                                        throw std::out_of_range();
                        }

                        row_t& operator+=(const row_t &rhs_) {
                                if (length != rhs_.length)
                                        throw std::runtime_error("Can not sum rows with different lengths.");
                                for (size_t i = 0; i < length; i++)
                                        data[i] += rhs_.data[i];

                                return *this;
                        }

                        row_t& operator-=(const row_t &rhs_) {
                                if (length != rhs_.length)
                                        throw std::runtime_error("Can not sub rows with different lengths.");
                                for (size_t i = 0; i < length; i++)
                                        data[i] -= rhs_.data[i];

                                return *this;
                        }

                        T& operator[] (size_t index_) { return data[index_]; }

                        size_t get_length () const { return length; }
        };

        template <typename T>
        row_t<T> operator+(const row_t<T> &rhs_, const row_t<T> &lhs_) {
                row_t tmp(rhs_); tmp += lhs_; return tmp;
        }

        template <typename T>
        row_t<T> operator-(const row_t<T> &rhs_, const row_t<T> &lhs_) {
                row_t tmp(rhs_); tmp -= lhs_; return tmp;
        }
}