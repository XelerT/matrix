#include <boost/program_options.hpp>
#include <iostream>

#include "cmd_parser.hpp"

namespace po = boost::program_options;

arguments_t parse_cmd_args (int n_args, const char **args)
{
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h",   "display help info and exit")
                ("lvl1", po::value<bool>(), "Run first lvl of matrixes(determinant calculation) (default value: false)")
                ("lvl2", po::value<bool>(), "Run second lvl of matrixes(find optimal chain multiplication) (default value: false)")
                ("mul", po::value<bool>(), "Run matrix chain multiplication mode (default value: false; only with lvl2)")
                ;

        po::variables_map var_map {};
        po::store(po::parse_command_line(n_args, args, desc), var_map);
        po::notify(var_map);

        arguments_t parsed_args {};

        if (var_map.count("help")) { 
                std::cout << desc << std::endl; 
                parsed_args.parsed = false; 
                
                return parsed_args; 
        }

        if (var_map.count("lvl1")) { parsed_args.lvl1          = var_map["lvl1"].as<bool>(); }
        if (var_map.count("lvl2")) { parsed_args.lvl2          = var_map["lvl2"].as<bool>(); }
        if (var_map.count("mul"))  { parsed_args.multiply_mxes = var_map["mul"].as<bool>(); }

        return parsed_args;
}