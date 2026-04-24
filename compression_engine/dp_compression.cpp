#include "dp_compression.h"

#include "../algorithms/huffman.h"
#include "../algorithms/lz77.h"
#include "../algorithms/rle.h"
#include "../algorithms/shannon_fano.h"
#include "../calculations/entropy.h"
#include "../calculations/pattern.h"
#include "greedy.h"

#include <string>
#include <vector>
using namespace std;

const vector<string> algorithms = {"RLE", "Huffman", "LZ77", "ShannonFano"};
const int INF = 1e9;

void addIfMissing(vector<string>& list, const string& algorithm) {
    for(const string& current : list){
        if(current == algorithm) return;
    }
    list.push_back(algorithm);
}

vector<string> chooseCandidates(const string& segment){
    vector<string> candidates;

    const double entropy = calculateEntropy(segment);
    const double repetition = calculateRepetition(segment);
    const double pattern = patternScore(segment);

    const string greedyChoice = greedySelect(entropy, repetition);
    addIfMissing(candidates, greedyChoice);

    if(repetition > 0.35 || pattern > 0.55) addIfMissing(candidates, "RLE");
    if(entropy >= 1.2 && entropy <= 3.5) addIfMissing(candidates, "Huffman");
    if(repetition > 0.10 || pattern > 0.35) addIfMissing(candidates, "LZ77");
    if(entropy > 3.2 && pattern < 0.25) addIfMissing(candidates, "ShannonFano");

    if(candidates.size() < 2){
        for(const string& algorithm : algorithms){
            addIfMissing(candidates, algorithm);
            if(candidates.size() >= 2) break;
        }
    }

    return candidates;
}

string runCompression(const string& algorithm, const string& text){
    if(algorithm == "RLE"){
        RLE rle;
        return rle.compress(text);
    }
    if(algorithm == "Huffman"){
        Huffman huffman;
        return huffman.compress(text);
    }
    if(algorithm == "LZ77"){
        LZ77 lz77;
        return lz77.compress(text);
    }
    ShannonFano shannonFano;
    return shannonFano.compress(text);
}

SegmentCompressionResult chooseBestSegment(const string& text, int start, int end){
    const string segment = text.substr(start, end - start + 1);
    const vector<string> candidates = chooseCandidates(segment);

    SegmentCompressionResult bestSegment{};
    bestSegment.startIndex = start;
    bestSegment.endIndex = end;
    bestSegment.originalSegment = segment;
    bestSegment.originalSize = static_cast<int>(segment.size());
    bestSegment.algorithmUsed = "None";
    bestSegment.compressedSegment = segment;
    bestSegment.compressedSize = bestSegment.originalSize;

    for(const string& algorithm : candidates){
        const string compressed = runCompression(algorithm, segment);
        const int compressedSize = static_cast<int>(compressed.size());
        if(compressedSize < bestSegment.compressedSize){
            bestSegment.algorithmUsed = algorithm;
            bestSegment.compressedSegment = compressed;
            bestSegment.compressedSize = compressedSize;
        }
    }

    return bestSegment;
}

CompressionPlan compressWithDP(const string& text, int minSegmentLength, int maxSegmentLength){
    CompressionPlan plan{};
    plan.totalOriginalSize = static_cast<int>(text.size());

    if(text.empty()){
        plan.totalCompressedSize = 0;
        return plan;
    }

    const int n = static_cast<int>(text.size());
    int minLen;
    if(minSegmentLength > 1){
        minLen = minSegmentLength;
    }
    else{
        minLen = 1;
    }

    int maxLen;
    if(maxSegmentLength >= minLen){
        maxLen = maxSegmentLength;
    }
    else{
        maxLen = minLen;
    }

    vector<int> bestCost(n + 1, INF);
    vector<int> nextCut(n, -1);
    vector<SegmentCompressionResult> segmentAt(n);
    bestCost[n] = 0;

    for(int i = n - 1; i >= 0; i--){
        const int minEnd = i + minLen - 1;
        int maxEnd;
        if(i + maxLen - 1 < n - 1){
            maxEnd = i + maxLen - 1;
        }
        else{
            maxEnd = n - 1;
        }

        for(int end = minEnd; end <= maxEnd; end++){
            if(end < i) continue;
            const int next = end + 1;
            if(next <= n && bestCost[next] == INF) continue;

            const SegmentCompressionResult segmentChoice = chooseBestSegment(text, i, end);
            const int totalCost = segmentChoice.compressedSize + bestCost[next];

            if(totalCost < bestCost[i]){
                bestCost[i] = totalCost;
                nextCut[i] = next;
                segmentAt[i] = segmentChoice;
            }
        }

        if(nextCut[i] == -1){
            const SegmentCompressionResult segmentChoice = chooseBestSegment(text, i, n - 1);
            if(segmentChoice.compressedSize < bestCost[i]){
                bestCost[i] = segmentChoice.compressedSize;
                nextCut[i] = n;
                segmentAt[i] = segmentChoice;
            }
        }
    }

    int current = 0;
    while(current < n && nextCut[current] != -1){
        plan.segments.push_back(segmentAt[current]);
        current = nextCut[current];
    }

    plan.totalCompressedSize = 0;
    for(const SegmentCompressionResult& segment : plan.segments){
        plan.totalCompressedSize += segment.compressedSize;
    }

    return plan;
}
