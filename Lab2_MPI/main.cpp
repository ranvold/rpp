#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

vector<pair<int, int>> findLocalPairsWithSum(const vector<int>& arr, int targetSum, int startIdx, int endIdx) {
    vector<pair<int, int>> local_pairs;

    for (int i = startIdx; i < endIdx; ++i) {
        for (int j = i + 1; j < arr.size(); ++j) {
            if (arr[i] + arr[j] == targetSum) {
                local_pairs.emplace_back(arr[i], arr[j]);
            }
        }
    }
    return local_pairs;
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
    MPI_Init(&argc, &argv);

    int world_size, world_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc < 3) {
        if (world_rank == 0) {
            cerr << "Please provide the array size, target sum, and number of threads as command-line arguments." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    size_t arraySize = static_cast<size_t>(atoi(argv[1]));
    int targetSum = atoi(argv[2]);

    vector<int> arr;
    if (world_rank == 0) {
        arr = createLargeArray(arraySize);
    }

    MPI_Bcast(&arraySize, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    if (world_rank != 0) {
        arr.resize(arraySize);
    }

    MPI_Bcast(arr.data(), arraySize, MPI_INT, 0, MPI_COMM_WORLD);

    int elements_per_process = arraySize / world_size;
    int startIdx = world_rank * elements_per_process;
    int endIdx = (world_rank == world_size - 1) ? arraySize : startIdx + elements_per_process;

    auto start = high_resolution_clock::now();
    vector<pair<int, int>> local_pairs = findLocalPairsWithSum(arr, targetSum, startIdx, endIdx);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    vector<int> recv_counts(world_size);
    vector<int> displs(world_size);

    int local_size = local_pairs.size() * 2;
    MPI_Gather(&local_size, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        displs[0] = 0;
        for (int i = 1; i < world_size; i++) {
            displs[i] = displs[i - 1] + recv_counts[i - 1];
        }
    }

    vector<int> global_pairs;
    if (world_rank == 0) {
        global_pairs.resize(displs[world_size - 1] + recv_counts[world_size - 1]);
    }

    MPI_Gatherv(local_pairs.data(), local_size, MPI_INT, global_pairs.data(), recv_counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        vector<pair<int, int>> final_pairs;
        for (size_t i = 0; i < global_pairs.size(); i += 2) {
            final_pairs.emplace_back(global_pairs[i], global_pairs[i + 1]);
        }

        cout << "The number of processes: " << world_size << endl;
        printPairs(final_pairs, targetSum);
        cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
