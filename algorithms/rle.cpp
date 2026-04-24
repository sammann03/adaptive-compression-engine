#include "rle.h"

#include <string>
using namespace std;

string RLE::compress(const string &s) {
    if (s.empty()) return "";
    int n = static_cast<int>(s.size());

    string compressed;
    char current = s[0];
    int count = 1;

    for (int i = 1; i < n; i++) {
        if (s[i] == current) count++;
        else {
            compressed += current;
            compressed += to_string(count);

            current = s[i];
            count = 1;
        }
    }

    compressed += current;
    compressed += to_string(count);

    return compressed;
}
