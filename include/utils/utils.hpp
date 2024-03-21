#pragma once

#include <iostream>
#include <random>

enum class status_t
{
        OK,
        ERROR,
        WARNING,
        DEGENERATED
};

// std::ostream& operator<<(std::ostream &os, const status_t &stat)
// {
//         os << static_cast<std::underlying_type<status_t>::type>(stat);
//         return os;
// }

const double THRESHOLD = 1e-5;
const size_t MAX_SIZE_T_VALUE = static_cast<size_t>(-1);

template <typename T> void construct (T *data, const T &rhs) { new (data) T(rhs); }
template <typename T> void construct (T *data, T &&rhs)
{
        new (data) T(std::move(rhs));
}

template <class T> void destroy (T *data) { data->~T(); }
template <typename It> void destroy (It begin, It end)
{
        while (begin != end) {
                destroy(begin);
                begin++;
        }
}

template <typename T> T get_random_num ()
{
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<T> dis;
        
        return dis(gen) % 10;
}
