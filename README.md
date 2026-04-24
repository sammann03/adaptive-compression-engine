# Adaptive Compression Engine

This repository contains an advanced compression tool that adaptively compresses data by selecting the optimal algorithm for different segments of the input. Instead of relying on a single compression method, this engine analyzes the input text, breaks it into variable-length segments, and applies the most effective compression algorithm—from a suite including RLE, Huffman, LZ77, and Shannon-Fano—to each individual segment.
The engine uses intelligent strategies to determine the best way to segment the data and assign algorithms, aiming to achieve a higher overall compression ratio than any single algorithm could on its own.

## Key Features

*   **Adaptive Compression:** Divides input into segments and applies the best algorithm for each, rather than a one-size-fits-all approach.
*   **Heuristic Analysis:** Calculates properties like entropy, repetition, and pattern scores for each segment to intelligently select a small set of candidate algorithms.
*   **Multiple Optimization Strategies:**
    *   **Dynamic Programming:** An efficient and optimal method to determine the best segmentation and algorithm plan to minimize the total compressed size.
    *   **Backtracking:** An exhaustive search method that also finds an optimal plan, serving as a baseline for comparison (note: much slower and not recommended for large inputs).
*   **Suite of Classic Algorithms:** Implements several fundamental compression techniques:
    *   Run-Length Encoding (RLE)
    *   Huffman Coding
    *   LZ77
    *   Shannon-Fano Coding

## How It Works

The engine follows a multi-step process to find the most effective compression plan for a given text:

1.  **Segmentation:** The input string is conceptually broken down into all possible contiguous segments within a specified minimum and maximum length.
2.  **Analysis and Candidate Selection:** For each potential segment, the engine performs a heuristic analysis:
    *   It calculates the Shannon entropy to measure randomness.
    *   It measures the ratio of consecutively repeated characters.
    *   It scores the presence of repeating patterns using a KMP-based pattern search.
    Based on these metrics, it selects a small subset of the most promising compression algorithms (e.g., RLE for highly repetitive data, Huffman for data with a non-uniform character distribution) as candidates for that specific segment.
3.  **Optimal Plan Generation:** The engine uses one of two modes to construct the final compression plan:
    *   **Dynamic Programming (Default):** This is the primary mode. It builds a solution from the bottom up, calculating the minimum possible compressed size for prefixes of the string. It guarantees finding the optimal combination of segments and algorithms in polynomial time.
    *   **Backtracking:** This mode recursively explores all possible ways to partition the string into valid segments, pruning branches that are already more costly than the best solution found so far. It is computationally expensive and automatically switches to the Dynamic Programming mode for inputs larger than 120 characters to ensure performance.
4.  **Execution:** Once the optimal plan is determined, the engine iterates through the segments in the plan, applies the chosen compression algorithm to each, and concatenates the results.

## Getting Started

### Compilation

You can compile the project using a C++ compiler like g++. All necessary source files are located in the `algorithms`, `calculations`, and `compression_engine` directories.

```bash
g++ -std=c++17 -o adaptive_compress main.cpp algorithms/*.cpp calculations/*.cpp compression_engine/*.cpp
```

### Usage

Run the compiled executable from your terminal:

```bash
./adaptive_compress
```

The program will prompt you to enter the text you wish to compress and then choose an optimization mode.

#### Example Session

```
$ ./adaptive_compress
Adaptive Compression Engine Demo
Enter text to compress: AAAAAABBBCCCCCCCDDDDDDEEEEEEEFFFFFFGGGGGGGGHHHHHHH
Choose mode:
1. Dynamic Programming
2. Backtracking
Choice: 1

Mode used: Dynamic Programming

Original text size: 53 bytes
Reduced text size: 30 bytes
Compression ratio: 56.60%
Time taken: 0.123 ms

Segments and chosen algorithms:
  Segment 1 [0, 5] -> RLE | original: 6, compressed: 2
  Segment 2 [6, 8] -> RLE | original: 3, compressed: 2
  Segment 3 [9, 15] -> LZ77 | original: 7, compressed: 6
  Segment 4 [16, 21] -> LZ77 | original: 6, compressed: 6
  Segment 5 [22, 28] -> LZ77 | original: 7, compressed: 6
  Segment 6 [29, 34] -> LZ77 | original: 6, compressed: 6
  Segment 7 [35, 52] -> ShannonFano | original: 18, compressed: 2
```

## Code Structure

The repository is organized into several directories based on functionality:

*   `main.cpp`: The entry point for the application, handling user input, and displaying results.
*   `/algorithms`: Contains the implementations of the individual compression algorithms (Huffman, LZ77, RLE, Shannon-Fano).
*   `/calculations`: Includes helper functions for text analysis, such as calculating entropy and pattern scores.
*   `/compression_engine`: The core logic of the adaptive engine, including the Dynamic Programming and Backtracking strategies for finding the optimal compression plan.
