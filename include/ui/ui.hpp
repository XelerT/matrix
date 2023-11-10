#pragma once

#include <vector>
#include <utility>

std::vector<std::pair<char, std::pair<int, int>>>
get_user_data ();

std::vector<int> parse_input_data (std::vector<std::pair<char, std::pair<int, int>>> &input);