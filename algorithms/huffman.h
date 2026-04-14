#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <string>
using namespace std;

class Huffman {
private:
    map<char, string> codes;

public:
    string compress(const string& input);
    const map<char, string>& getCodes();
};

#endif