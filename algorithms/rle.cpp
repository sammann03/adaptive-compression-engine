#include "rle.h"

#include <string>
using namespace std;

string RLE::compress(const string &s) {
    if (s.empty()) return "";

    string compressed;
    char current = s[0];
    int count = 1;

    for (char i : s) {
        if (i == current) count++;
        else {
            compressed += current;
            compressed += to_string(count);

            current = i;
            count = 1;
        }
    }

    compressed += current;
    compressed += to_string(count);

    return compressed;
}
