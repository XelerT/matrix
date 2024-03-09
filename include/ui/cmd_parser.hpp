#pragma once

#include "utils/utils.hpp"

struct arguments_t
{
        bool parsed = true;
        bool lvl1   = false;
        bool lvl2   = false;
        bool multiply_mxes = false;

};

arguments_t parse_cmd_args (int n_args, const char **args);