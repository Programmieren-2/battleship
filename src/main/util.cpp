//
// Created by rne on 07.05.21.
//

#include <iostream>
using std::cin;
using std::cout;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "util.h"

namespace util {
    vector<string> splitString(string const &str, string const &delimiter) {
        vector<string> result;
        string processed = "";
        string lookahead = "";

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
        string input = "";
        cout << prompt;
        cin >> input;
        return input;
    }
}