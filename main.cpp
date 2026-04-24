#include "compression_engine/backtracking.h"
#include "compression_engine/dp_compression.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

void printPlan(const CompressionPlan& plan){
    cout << "\nSegments and chosen algorithms:\n";
    int totalSegments = static_cast<int>(plan.segments.size());
    for(int i = 0; i < totalSegments; i++){
        const SegmentCompressionResult& segment = plan.segments[i];
        cout << "  Segment " << i + 1 << " [" << segment.startIndex << ", " << segment.endIndex << "]"
             << " -> " << segment.algorithmUsed << " | original: " << segment.originalSize
             << ", compressed: " << segment.compressedSize << "\n";
    }
}

void printSummary(const CompressionPlan& plan, double timeMs){
    cout << "\nOriginal text size: " << plan.totalOriginalSize << " bytes\n";
    cout << "Reduced text size: " << plan.totalCompressedSize << " bytes\n";

    double ratio = 0.0;
    if(plan.totalOriginalSize > 0){
        ratio = (100.0 * static_cast<double>(plan.totalCompressedSize)) / static_cast<double>(plan.totalOriginalSize);
    }

    cout << "Compression ratio: " << fixed << setprecision(2) << ratio << "%\n";
    cout << "Time taken: " << fixed << setprecision(3) << timeMs << " ms\n";
}

int main(){
    cout << "Adaptive Compression Engine Demo\n";
    cout << "Enter text to compress: ";

    string text;
    getline(cin, text);

    if(text.empty()){
        cout << "No input provided.\n";
        return 0;
    }

    cout << "\nChoose mode:\n";
    cout << "1. Dynamic Programming\n";
    cout << "2. Backtracking\n";
    cout << "Choice: ";

    int mode = 1;
    string modeInput;
    getline(cin, modeInput);
    string cleanedModeInput;
    for(char c : modeInput){
        if(c != ' ' && c != '\t' && c != '\r') cleanedModeInput += c;
    }

    if(!cleanedModeInput.empty() && cleanedModeInput[0] == '2'){
        mode = 2;
    }
    else{
        mode = 1;
    }

    bool switchedToDP = false;
    if(mode == 2 && text.size() > 120){
        mode = 1;
        switchedToDP = true;
    }

    const auto start = chrono::high_resolution_clock::now();

    CompressionPlan plan{};
    if(mode == 2){
        plan = compressWithBacktracking(text, 6, 18);
    }
    else{
        plan = compressWithDP(text, 6, 30);
    }

    const auto end = chrono::high_resolution_clock::now();
    const double elapsedMs = chrono::duration<double, milli>(end - start).count();

    string modeUsed;
    if(mode == 2){
        modeUsed = "Backtracking";
    }
    else{
        modeUsed = "Dynamic Programming";
    }

    if(switchedToDP){
        cout << "\nInput is long. Switching to Dynamic Programming to avoid slow backtracking.\n";
    }

    cout << "\nMode used: " << modeUsed << "\n";
    printSummary(plan, elapsedMs);
    printPlan(plan);

    return 0;
}
