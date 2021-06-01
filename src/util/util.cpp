//
// Created by rne on 07.05.21.
//

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
using boost::program_options::invalid_command_line_syntax;
using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::parse_command_line;
using boost::program_options::store;
using boost::program_options::unknown_option;
using boost::program_options::variables_map;

#include "os.h" // Needed for __windows__ check

#include "util.h"

namespace util {
    static char const *WHITESPACE = " \t";

    static vector<string> splitStringByChars(string const &str, string const &delimiters)
    {
        vector<string> result;
        string item;

        for (auto const &chr : str) {
            if (any_of(delimiters.begin(), delimiters.end(), [chr] (auto const &delim) { return chr == delim; })) {
                result.push_back(item);
                item = "";
            } else {
                item += chr;
            }
        }

        if (!item.empty())
            result.push_back(item);

        return result;
    }

    vector<string> splitString(string const &str, string const &delimiter)
    {
        vector<string> result;
        string text = str;
        size_t pos;

        while ((pos = text.find(delimiter)) != string::npos) {
            result.push_back(text.substr(0, pos));
            text.erase(0, pos + delimiter.length());
        }

        if (!text.empty())
            result.push_back(text);

        return result;
    }

    vector<string> splitString(string const &str)
    {
        return splitStringByChars(str, WHITESPACE);
    }

    string joinStrings(vector<string> const &strings, string const &delimiter)
    {
        string result;
        size_t index = 0;

        for (auto const &str : strings) {
            result += str;

            if (++index < strings.size())
                result += delimiter;
        }

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

    optional<variables_map> parseArgDesc(int argc, char const *argv[], options_description const &desc)
    {
        variables_map varMap;

        try {
            store(parse_command_line(argc, argv, desc), varMap);
        } catch (unknown_option const &error) {
            cerr << "Invalid option: " << error.get_option_name() << "\n";
            cerr << desc << "\n";
            return {};
        } catch (invalid_command_line_syntax &error) {
            cerr << "Syntax error: " << error.what() << "\n";
            return {};
        }

        notify(varMap);

        if (varMap.count("help")) {
            cout << desc << "\n";
            return {};
        }

        return varMap;
    }
}