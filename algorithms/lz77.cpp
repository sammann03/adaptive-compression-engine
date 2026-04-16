#include "lz77.h"

#include <iostream>
using namespace std;

LZ77::LZ77(const int windowSize) {
    this->windowSize = windowSize;
}

string LZ77::compress(const string &s) {
    if (s.empty()) return "";

    string compressed;
    int n = static_cast<int>(s.size());
    int i = 0;

    while (i < n) {
        int bestLength = 0;
        int bestOffset = 0;

        int start = i - windowSize;
        if (start < 0) start = 0;

        for (int j = start; j < i; j++) {
            int length = 0;;
            while (i + length < n and j + length < i and s[j + length] == s[i + length]) length++;
            if (length > bestLength) {
                bestLength = length;
                bestOffset = i - j;
            }
        }

        char nextChar;
        if (i + bestLength < n) {
            nextChar = s[i + bestLength];
        } else {
            nextChar = '#';
        }

        compressed += "(";
        compressed += to_string(bestOffset);
        compressed += ",";
        compressed += to_string(bestLength);
        compressed += ",";
        compressed += nextChar;
        compressed += ")";
        i += bestLength + 1;
    }
    
    return compressed;
}