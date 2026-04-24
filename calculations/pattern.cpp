#include "pattern.h"

#include <vector>
using namespace std;

vector<int> buildLPS(const string& pattern) {
    int m = static_cast<int>(pattern.size());
    vector<int> lps(m, 0);
    int length = 0;

    for (int i = 1; i < m; ) {
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
    int n = static_cast<int>(s.size());
    int m = static_cast<int>(pattern.size());

    if (n == 0 || m == 0 || m > n) {
        return 0;
    }

    vector<int> lps = buildLPS(pattern);
    int i = 0;
    int j = 0;
    int count = 0;

    while (i < n) {
        if (s[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
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
