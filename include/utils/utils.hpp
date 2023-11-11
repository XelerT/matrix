#pragma once

enum class status_t
{
        OK,
        ERROR,
        WARNING,
        DEGENERATED
};

const double THRESHOLD = 1e-5;