#ifndef LZ77_H
#define LZ77_H

#include <string>
using namespace std;

class LZ77 {
private:
    int windowSize;

public:
    string compress(const string& s);
    explicit LZ77(int windowSize = 20);
};

#endif