#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

int binarySearch(const std::vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return mid; // Return the index of the target value
        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1; // Return -1 if the target value is not found
}

int main() {
    // Initialize the sorted array (for demonstration purposes)
    std::vector<int> arr(10000); // Array with 1 billion elements
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 9999; // Target value to search for

    // Measure execution time
    auto start_time = std::chrono::high_resolution_clock::now(); // Start time

    // Perform binary search
    int result = binarySearch(arr, 0, arr.size() - 1, target);

    auto end_time = std::chrono::high_resolution_clock::now(); // End time

    // Calculate execution time
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Output the result and execution time
    if (result != -1) {
        std::cout << "Target value " << target << " found at index " << result << "\n";
    } else {
        std::cout << "Target value not found\n";
    }

    std::cout << "Sequential execution time: " << execution_time.count() * 1000 << " milliseconds\n";

    return 0;
}
