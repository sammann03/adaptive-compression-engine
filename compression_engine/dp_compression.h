#ifndef DP_COMPRESSION_H
#define DP_COMPRESSION_H

#include <string>
#include <vector>
using namespace std;

struct SegmentCompressionResult {
    int startIndex;
    int endIndex;
    string algorithmUsed;
    string originalSegment;
    string compressedSegment;
    int originalSize;
    int compressedSize;
};

struct CompressionPlan {
    vector<SegmentCompressionResult> segments;
    int totalOriginalSize;
    int totalCompressedSize;
};

CompressionPlan compressWithDP(const string& text, int minSegmentLength = 6, int maxSegmentLength = 30);



#endif 