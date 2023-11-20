#pragma once

#include <iostream>

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
