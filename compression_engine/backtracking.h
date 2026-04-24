#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "dp_compression.h"

#include <string>
using namespace std;

CompressionPlan compressWithBacktracking(const string& text, int minSegmentLength = 6, int maxSegmentLength = 20);

#endif
