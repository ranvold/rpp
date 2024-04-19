#include <mpi.h>
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

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize the sorted array (for demonstration purposes)
    std::vector<int> arr(10000); // Array with 1 billion elements
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 9999; // Target value to search for

    // Divide the array into chunks
    int chunk_size = arr.size() / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? arr.size() - 1 : start + chunk_size - 1;

    // Measure execution time using std::chrono
    auto start_time = std::chrono::high_resolution_clock::now(); // Start time

    // Perform binary search in each process's chunk
    int local_result = binarySearch(arr, start, end, target);

    // Communicate the local result to the root process
    int global_result = -1;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Output the final result and execution time
        if (global_result != -1) {
            std::cout << "Target value " << target << " found at index " << global_result << "\n";
        } else {
            std::cout << "Target value not found\n";
        }
    
        auto end_time = std::chrono::high_resolution_clock::now(); // End time

        // Calculate execution time
        std::chrono::duration<double> execution_time = end_time - start_time;
        
        std::cout << "Parallel execution time: " << execution_time.count() * 1000 << " milliseconds\n";
    }

    MPI_Finalize();
    return 0;
}
