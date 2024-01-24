#include <iostream>
#include <chrono>

#include "ui.hpp"
#include "utils/std.hpp"

using namespace std;
using namespace std::chrono;

pair<size_t, vector<double>>
get_user_data ()
{
        size_t n_dimensions;
        cin >> n_dimensions;
        size_t n_elems = n_dimensions * n_dimensions;

        vector<double> elems {};
        double input;

        while (elems.size() != n_elems && std::cin >> input)
                elems.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return {n_dimensions, elems};
}

pair<size_t, vector<size_t>>
get_user_chain_data ()
{
        size_t n_matrixes;
        cin >> n_matrixes;

        vector<size_t> sizes {};
        size_t input;

        while (sizes.size() != n_matrixes && std::cin >> input)
                sizes.push_back(input);

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter digits.");

        return {n_matrixes, sizes};
}
