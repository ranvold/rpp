#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <algorithm>
#include <utility>

using namespace std;
using namespace std::chrono;

vector<pair<int, int>> findAllPairsWithSumOMP(const vector<int>& arr, int targetSum, int threadsNum) {
    vector<pair<int, int>> pairs;

    #pragma omp parallel num_threads(threadsNum)
    {
        vector<pair<int, int>> local_pairs;

        #pragma omp for schedule(dynamic) nowait
        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = i + 1; j < arr.size(); ++j) {
                if (arr[i] + arr[j] == targetSum) {
                    local_pairs.emplace_back(arr[i], arr[j]);
                }
            }
        }

        #pragma omp critical
        pairs.insert(pairs.end(), local_pairs.begin(), local_pairs.end());
    }

    return pairs;
}

vector<int> createLargeArray(size_t size) {
    vector<int> arr;
    for (int i = 1; i <= static_cast<int>(size) * 2; i += 2) {
        arr.push_back(i);
    }
    return arr;
}

void printPairs(const vector<pair<int, int>>& pairs, int targetSum) {

    if (pairs.size() > 0) {
        cout << "The number of matching pairs for the required sum " << targetSum << ": " << pairs.size() << endl;
        cout << "The pairs: " << endl;

        for (const auto& p : pairs) {
            cout << "(" << p.first << ", " << p.second << ")" << endl;
        }
    } else {
        cout << "No pair found with sum " << targetSum << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Please provide the array size and targetSum sum as command-line arguments." << endl;
        return 1;
    }

    size_t arraySize = static_cast<size_t>(atoi(argv[1]));

    int targetSum = atoi(argv[2]);

    int threadsNum = atoi(argv[3]);

    vector<int> arr = createLargeArray(arraySize);

    auto start = high_resolution_clock::now();
    vector<pair<int, int>> result = findAllPairsWithSumOMP(arr, targetSum, threadsNum);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    sort(result.begin(), result.end());
    cout << "The number of threads: " << threadsNum << endl;
    printPairs(result, targetSum);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl << endl;

    return 0;
}
