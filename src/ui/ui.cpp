#include <iostream>
#include <chrono>

#include "ui.hpp"
#include "avl/avl.hpp"
#include "utils/std.hpp"

using namespace std;
using namespace std::chrono;

namespace
{
        int get_number (const char *str, int &val)
        {
                val = stoi(str);
                return length(val);
        }
} // anonymous namespace

vector<pair<char, pair<int, int>>>
get_user_data ()
{
        int val1 = 0;
        int val2 = 0;

        vector<pair<char, pair<int, int>>> input {};
        string input_str {};
        getline(cin, input_str);
        auto input_length = input_str.length();

        for (size_t i = 0; i < input_length; i++)
                if (input_str[i] == 'k') {
                        i += get_number(&input_str[++i], val1);
                        input.push_back({'k', {val1, 0}});
                } else if (input_str[i] == 'q') {
                        i += get_number(&input_str[++i], val1);
                        i += get_number(&input_str[++i], val2);
                        input.push_back({'q', {val1, val2}});
                }

        if (std::cin.fail() && !std::cin.eof())
                throw std::runtime_error("You need to enter \"k\" to add data and \"q\" to enter range.");

        return input;
}

vector<int> parse_input_data (vector<pair<char, pair<int, int>>> &input)
{
        int invalid_val = 0xFFFFFFFF;
        binary_trees::avl_t<int> tree{invalid_val};

        vector<int> output_data {};

        for (auto &elem : input) {
                if (elem.first == 'k') {
                        tree.insert(elem.second.first, elem.second.first);
                } else if (elem.first == 'q') {
                        // auto start = high_resolution_clock::now();
                        if (elem.second.first >= elem.second.second)
                                output_data.push_back(0);
                        else
                                output_data.push_back(tree.distance(elem.second.first, elem.second.second));
                        // auto stop = high_resolution_clock::now();

                        // auto duration = duration_cast<nanoseconds>(stop - start);
                        // cout << "Time taken by function: " << duration.count() << " nanoseconds" << endl;
                } else {
                        throw std::runtime_error("Unknown command.");
                }
        }
        // tree.graphviz_dump();

        return output_data;
}