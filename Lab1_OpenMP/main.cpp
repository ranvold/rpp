#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

int binarySearch(const std::vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid; // Return the index of the target value
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Return -1 if the target value is not found
}

int main() {
    // Initialize the sorted array (for demonstration purposes)
    std::vector<int> arr(1000000); // Array with 1 million elements
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 999999; // Target value to search for

    // Measure execution time using std::chrono
    auto start_time = std::chrono::high_resolution_clock::now(); // Start time

    int result = -1;
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        // Divide the array into chunks for each thread
        int chunk_size = arr.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? arr.size() - 1 : start + chunk_size - 1;

        // Perform binary search in each thread's chunk
        int local_result = binarySearch(arr, start, end, target);
        
        // If the target value is found, store the result and exit
        if (local_result != -1) {
            #pragma omp critical
            {
                if (result == -1) {
                    result = local_result;
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now(); // End time

    // Calculate execution time
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Output the result and execution time
    if (result != -1) {
        std::cout << "Target value " << target << " found at index " << result << "\n";
    } else {
        std::cout << "Target value not found\n";
    }
    std::cout << "Execution time: " << execution_time.count() << " seconds\n";

    return 0;
}
