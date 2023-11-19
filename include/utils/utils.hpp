#pragma once

#include <iostream>

enum class status_t
{
        OK,
        ERROR,
        WARNING,
        DEGENERATED
};

std::ostream& operator<<(std::ostream &os, const status_t &stat)
{
        os << static_cast<std::underlying_type<status_t>::type>(stat);
        return os;
}

const double THRESHOLD = 1e-5;