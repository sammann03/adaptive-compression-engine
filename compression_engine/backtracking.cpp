#include "backtracking.h"

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

namespace {
const vector<string> algorithms = {"RLE", "Huffman", "LZ77", "ShannonFano"};
const int INF = 1e9;

void addIfMissing(vector<string>& list, const string& algorithm){
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

void backtrack(const string& text,
               int minSegmentLength,
               int maxSegmentLength,
               int index,
               vector<SegmentCompressionResult>& path,
               int currentCost,
               CompressionPlan& answer){
    const int n = static_cast<int>(text.size());
    if(index >= n){
        if(currentCost < answer.totalCompressedSize){
            answer.segments = path;
            answer.totalCompressedSize = currentCost;
        }
        return;
    }

    if(currentCost >= answer.totalCompressedSize) return;

    const int minEnd = index + minSegmentLength - 1;
    int maxEnd;
    if(index + maxSegmentLength - 1 < n - 1){
        maxEnd = index + maxSegmentLength - 1;
    }
    else{
        maxEnd = n - 1;
    }

    bool explored = false;
    for(int end = minEnd; end <= maxEnd; end++){
        if(end < index) continue;
        explored = true;
        SegmentCompressionResult segment = chooseBestSegment(text, index, end);
        path.push_back(segment);
        backtrack(text, minSegmentLength, maxSegmentLength, end + 1, path, currentCost + segment.compressedSize, answer);
        path.pop_back();
    }

    if(!explored){
        SegmentCompressionResult segment = chooseBestSegment(text, index, n - 1);
        path.push_back(segment);
        backtrack(text, minSegmentLength, maxSegmentLength, n, path, currentCost + segment.compressedSize, answer);
        path.pop_back();
    }
}
}

CompressionPlan compressWithBacktracking(const string& text, int minSegmentLength, int maxSegmentLength){
    CompressionPlan answer{};
    answer.totalOriginalSize = static_cast<int>(text.size());
    answer.totalCompressedSize = INF;

    if(text.empty()){
        answer.totalCompressedSize = 0;
        return answer;
    }

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

    vector<SegmentCompressionResult> path;
    backtrack(text, minLen, maxLen, 0, path, 0, answer);

    if(answer.totalCompressedSize == INF){
        answer.totalCompressedSize = 0;
    }

    return answer;
}
