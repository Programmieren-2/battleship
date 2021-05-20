//
// Created by rne on 07.05.21.
//

#include <algorithm>
using std::copy;

#include <array>
using std::array;
using std::begin;
using std::end;

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <optional>
using std::optional;

#include <regex>
using std::regex;
using std::regex_match;

#include <stdexcept>
using std::domain_error;
using std::length_error;

#include <string>
using std::getline;
using std::string;

#include <vector>
using std::vector;

#include <boost/program_options.hpp>
using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::parse_command_line;
using boost::program_options::store;
using boost::program_options::unknown_option;
using boost::program_options::variables_map;

#include "util.h"

namespace util {
    static bool isWhitespace(char const character)
    {
        switch (character) {
            case ' ':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    vector<string> splitString(string const &str, string const &delimiter)
    {
        vector<string> result;
        string processed;
        string lookahead;

        for (char const character : str) {
            lookahead += character;

            if (lookahead.length() == delimiter.length()) {
                if (lookahead == delimiter) {
                    result.push_back(processed);
                    lookahead = "";
                    processed = "";
                } else {
                    processed += lookahead.substr(0, 1);
                    lookahead = lookahead.substr(1, lookahead.length() - 1);
                }
            }
        }

        if (!processed.empty())
            result.push_back(processed);

        return result;
    }

    vector<string> splitString(string const &str)
    {
        vector<string> result;
        string item;

        for (char const character : str) {
            if (isWhitespace(character)) {
                if (!item.empty()) {
                    result.push_back(item);
                    item = "";
                }
            } else {
                item += character;
            }
        }

        if (!item.empty())
            result.push_back(item);

        return result;
    }

    string readWithPrompt(string const &prompt)
    {
        string result;
        cout << prompt;
        getline(cin, result);
        return result;
    }

    vector<string> readCommandLine(string const &prompt)
    {
        return splitString(readWithPrompt(prompt));
    }

    bool isExitCommand(string const &command)
    {
        return contains({"exit", "quit"}, command);
    }

    bool isNumber(string const &str)
    {
        const regex number("^\\d{1,6}$");
        return regex_match(str, number);
    }

    int copyString(char *dest, string const &src, size_t size)
    {
        size_t maxSize = size - 1;  // Reserve for terminating NULL byte.

        if (src.size() > maxSize)
            cerr << "String exceeds " << maxSize << " bytes, so it will be truncated.\n";

#ifdef __windows__
        return strncpy_s(dest, size, src.c_str(), maxSize);
#else
        strncpy(dest, src.c_str(), maxSize);
        return 0;
#endif
    }

    optional<variables_map> parseArgDesc(int argc, const char *argv[], options_description const &desc)
    {
        optional<variables_map> args;
        variables_map varMap;

        try {
            store(parse_command_line(argc, argv, desc), varMap);
        } catch (unknown_option const &error) {
            cerr << "Invalid option: " << error.get_option_name() << "\n";
            cerr << desc << "\n";
            return args;
        }

        notify(varMap);

        if (varMap.count("help")) {
            cout << desc << "\n";
            return args;
        }

        return args = varMap;
    }
}