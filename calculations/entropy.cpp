#include "entropy.h"

#include <cmath>
#include <unordered_map>
using namespace std;

double calculateEntropy(const string &s) {
    if (s.empty()) return 0.0;

    unordered_map<char, int> freq;
    for (char c : s) {
        freq[c]++;
    }

    const auto n = static_cast<double>(s.size());
    double entropy = 0.0;

    for (auto it : freq) {
        double p = it.second / n;
        entropy += -p * log2(p);
    }

    return entropy;
}

double calculateRepetition(const string &s) {
    if (s.size() < 2) return 0.0;
    int repeated = 0;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == s[i - 1]) {
            repeated++;
        }
    }

    return static_cast<double>(repeated) / static_cast<double>(s.size() - 1);
}
