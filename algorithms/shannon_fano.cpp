#include "shannon_fano.h"

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

struct SF {
    char c;
    int freq;
};

bool sortByFrequency(SF* a, SF* b) {
    return a->freq < b->freq;
}

void assignCodes(vector<SF>& items, int left, int right, map<char, string>& codes) {
    if (left > right) return;
    if (left == right) {
        if (codes[items[left].c].empty()) {
            codes[items[left].c] = "0";
        }
        return;
    }

    int total = 0;
    for (int i = left; i <= right; i++) {
        total += items[i].freq;
    }

    int running = 0;
    int split = left;
    int bestDifference = total;

    for (int i = left; i <= right; i++) {
        running += items[i].freq;
        int diff = abs(total - 2 * running);
        if (diff < bestDifference) {
            bestDifference = diff;
            split = i;
        }
    }

    for (int i = left; i <= split; i++) {
        codes[items[i].c] = "0";
    }
    for (int i = split + 1; i <=  right; i++) {
        codes[items[i].c] = "1";
    }

    assignCodes(items, left, split, codes);
    assignCodes(items, split + 1, right, codes);
}

string ShannonFano::compress(const string &s) {
    codes.clear();
    if (s.empty()) return "";

    unordered_map<char, int> freq;
    for (char c : s) {
        freq[c]++;
    }

    vector<SF> items;
    for (auto it : freq) {
        SF item{};
        item.c = it.first;
        item.freq = it.second;
        items.push_back(item);
    }

    sort(items.begin(), items.end(), sortByFrequency());
    assignCodes(items, 0, static_cast<int>(items.size() - 1), codes);

    string encoded;
    encoded.reserve(s.size());
    for (char i : s) {
        encoded += codes[i];
    }

    return encoded;
}

const map<char, string>& ShannonFano::getCodes(){
    return codes;
}
