#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>

int binarySearch(const std::vector<int>& arr, int left, int right, int target, bool& terminate) {
    while (left <= right) {
        // Перевірка умови припинення пошуку
        if (terminate) {
            return -1; // Вихід з пошуку, якщо знайдено цільове значення
        }
        
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid; // Знайдено цільове значення, повернути індекс
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1; // Цільове значення не знайдено
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ініціалізація впорядкованого масиву
    std::vector<int> arr(100000000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 1; // Цільове значення для пошуку

    // Поділ масиву на частини
    int chunk_size = arr.size() / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? arr.size() - 1 : start + chunk_size - 1;

    // Змінна умова для припинення
    bool terminate = false;

    // Вимірювання часу виконання
    auto start_time = std::chrono::high_resolution_clock::now();

    // Виконання бінарного пошуку у частці масиву кожного процесу
    int local_result = binarySearch(arr, start, end, target, terminate);

    // Якщо місцевий результат виявлений
    if (local_result != -1) {
        // Встановлюємо умову припинення для всіх процесів
        terminate = true;
    }

    // Розповсюдження умови припинення всім процесам
    MPI_Bcast(&terminate, 1, MPI_CXX_BOOL, rank, MPI_COMM_WORLD);

    // Об'єднання результатів у кореневому процесі
    int global_result = -1;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Виведення результату і часу виконання
    if (rank == 0) {
        // Обчислення часу виконання
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
