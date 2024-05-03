#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>

int binarySearch(const std::vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        if (target < left || target > right) {
            return -1;
        }
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> arr(10000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 9999;
    int chunk_size = arr.size() / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? arr.size() - 1 : start + chunk_size - 1;

    auto start_time = std::chrono::high_resolution_clock::now();

    int local_result = binarySearch(arr, start, end, target);

    int global_result = -1;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> execution_time = end_time - start_time;
        
        if (global_result != -1) {
            std::cout << "Target value " << target << " found at index " << global_result << "\n";
        } else {
            std::cout << "Target value not found\n";
        }
        
        std::cout << "Parallel execution time: " << execution_time.count() * 1000 << " milliseconds\n";
    }

    MPI_Finalize();
    return 0;
}
