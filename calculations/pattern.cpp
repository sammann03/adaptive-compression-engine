#include "pattern.h"

#include <vector>
using namespace std;

vector<int> buildLPS(const string& pattern) {
    vector<int> lps(pattern.size(), 0);
    int length = 0;

    for (int i = 0; i < pattern.size(); ) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        }
        else if (length != 0) length = lps[length - 1];
        else {
            lps[i] = 0;
            i++;
        }
    }

    return lps;
}

int kmpCount(const string &s, const string &pattern) {
    if (s.empty() || pattern.empty() || pattern.size() > s.size()) {
        return 0;
    }

    vector<int> lps = buildLPS(pattern);
    int i = 0;
    int j = 0;
    int count = 0;

    while (i < s.size()) {
        if (s[i] == pattern[j]) {
            i++;
            j++;
            if (j == pattern.size()) {
                count++;
                j = lps[j - 1];
            }
        }
        else if (j != 0) j = lps[j - 1];
        else i++;
    }

    return count;
}

double patternScore(const string &s) {
    if (s.size() < 6) return 0.0;

    vector<string> samples;
    samples.push_back(s.substr(0, 2));
    samples.push_back(s.substr(0, 3));
    samples.push_back(s.substr(s.size() / 2, 2));

    double total = 0.0;
    for (const auto & sample : samples) {
        int count = kmpCount(s, sample);
        total += static_cast<double>(count * sample.size()) / static_cast<double>(s.size());
    }

    double score = total / static_cast<double>(samples.size());
    if (score >= 1.0) return 1.0;
    return score;
}
