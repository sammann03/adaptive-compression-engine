#include "greedy.h"
using namespace std;

string greedySelect(const double entropy, double repetitionRatio) {
    if (entropy < 1.5 and repetitionRatio > 0.30) return "RLE";
    else if (entropy >= 1.5 and entropy <= 3.0) return "Huffman";
    else if (repetitionRatio > 0.10) return "LZ77";
    return "ShannonFano";
}
