#include "huffman.h"

#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

struct HuffmanNode {
    char c;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(const char c, const int f) {
        this->c = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    HuffmanNode(HuffmanNode* l, HuffmanNode* r) {
        c = '\0';
        freq = (l->freq + r->freq);
        left = l;
        right = r;
    }
};

struct Compare {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        return a->freq > b->freq;
    }
};

void buildRecursive(HuffmanNode* node, const string& prefix, map<char, string>& codes) {
    if (node == nullptr) return;

    if (node->left == nullptr and node->right == nullptr) {
        if (prefix.empty()) {
            codes[node->c] = "0";
        }
        else {
            codes[node->c] = prefix;
        }
        return;
    }

    buildRecursive(node->left, prefix + "0", codes);
    buildRecursive(node->right, prefix + "1", codes);
}

void deleteTree(const HuffmanNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

string Huffman::compress(const string& s) {
    codes.clear();
    if (s.empty()) return "";

    unordered_map<char, int> freq;
    for (char c : s) {
        freq[c]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto it : freq) {
        pq.push(new HuffmanNode(it.first, it.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        pq.push(new HuffmanNode(left, right));
    }

    HuffmanNode* root = pq.top();
    buildRecursive(root, "", codes);

    string encoded;
    encoded.reserve(s.size());
    for (char i : s) {
        encoded += codes[i];
    }

    deleteTree(root);
    return encoded;
}

const map<char, string>& Huffman::getCodes(){
    return codes;
}
