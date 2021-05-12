//
// Created by rne on 07.05.21.
//

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <regex>
using std::regex;
using std::regex_match;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "util.h"

namespace util {
    vector<string> splitString(string const &str, string const &delimiter) {
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

    string readWithPrompt(string const &prompt)
    {
        string input;
        cout << prompt;
        cin >> input;
        return input;
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

#if defined(__linux__)
        strncpy(dest, src.c_str(), maxSize);
        return 0;
#else
        return strncpy_s(dest, size, src.c_str(), maxSize);
#endif
    }
}