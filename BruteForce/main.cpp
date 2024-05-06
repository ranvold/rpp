#include <iostream>
#include <vector>
#include <utility>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<pair<int, int>> findLocalPairsWithSum(const vector<int>& arr, int targetSum) {
    vector<pair<int, int>> pairs;

    for (int i = 0; i < arr.size(); ++i) {
        for (int j = i + 1; j < arr.size(); ++j) {
            if (arr[i] + arr[j] == targetSum) {
                pairs.emplace_back(arr[i], arr[j]);
            }
        }
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
    if (argc < 3) {
        cerr << "Please provide the array size, target sum, and number of threads as command-line arguments." << endl;
        return 1;
    }
    size_t arraySize = static_cast<size_t>(atoi(argv[1]));
    int targetSum = atoi(argv[2]);

    vector<int> arr = createLargeArray(arraySize);
    auto start = high_resolution_clock::now();
    vector<pair<int, int>> pairs = findLocalPairsWithSum(arr, targetSum);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "===Sequential launch===" << endl;
    printPairs(pairs, targetSum);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    return 0;
}
