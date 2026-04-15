#ifndef SHANNON_FANO_H
#define SHANNON_FANO_H

#include <map>
#include <string>
using namespace std;

class ShannonFano {
private:
    map<char, string> codes;

public:
    string compress(const string& s);
    const map<char, string>& getCodes();
};

#endif