#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

int binarySearch(const std::vector<int>& arr, int left, int right, long int target) {
    while (left <= right) {
        if (target < left || target > right) {
            return -1;
        }
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int main() {
    std::vector<int> arr(10000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    long int target = 9999;

    auto start_time = std::chrono::high_resolution_clock::now();

    int result = -1;

    omp_set_num_threads(8);

    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        int chunk_size = arr.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? arr.size() - 1 : start + chunk_size - 1;

        int local_result = binarySearch(arr, start, end, target);

        if (local_result != -1) {
            #pragma omp critical
            {
                if (result == -1) {
                    result = local_result;
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> execution_time = end_time - start_time;

    if (result != -1) {
        std::cout << "Target value " << target << " found at index " << result << "\n";
    } else {
        std::cout << "Target value not found\n";
    }
    std::cout << "Parallel execution time: " << execution_time.count() * 1000 << " milliseconds\n";

    return 0;
}
