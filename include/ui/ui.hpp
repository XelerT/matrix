#pragma once

#include <vector>
#include <utility>

std::pair<size_t, std::vector<double>>
get_user_data ();

std::pair<size_t, std::vector<size_t>>
get_user_chain_data ();