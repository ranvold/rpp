#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

int binarySearch(const std::vector<int>& arr, int left, int right, long int target, bool& terminate) {
    while (left <= right) {
        // Перевірка змінної-умови, щоб припинити пошук
        if (terminate) {
            return -1; // Вихід з пошуку, якщо цільове значення знайдено
        }

        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid; // Знайдено цільове значення, повернути індекс
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Цільове значення не знайдено
}

int main() {
    // Ініціалізація впорядкованого масиву
    std::vector<int> arr(100000000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    long int target = 99999999; // Цільове значення для пошуку

    // Вимірювання часу виконання
    auto start_time = std::chrono::high_resolution_clock::now();

    int result = -1;
    // Глобальна змінна-умова для припинення
    bool terminate = false;

    // Встановлення кількості потоків
    omp_set_num_threads(8);

    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        // Поділ масиву на частини для кожного потоку
        int chunk_size = arr.size() / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? arr.size() - 1 : start + chunk_size - 1;

        // Виконання бінарного пошуку у частці кожного потоку
        int local_result = binarySearch(arr, start, end, target, terminate);

        // Якщо місцевий результат знайдено, змінюємо змінну-умову
        if (local_result != -1) {
            #pragma omp critical
            {
                if (result == -1) {
                    result = local_result;
                    terminate = true; // Встановити змінну-умову для припинення потоку
                }
            }
        }
    }

    // Кінець вимірювання часу виконання
    auto end_time = std::chrono::high_resolution_clock::now();

    // Обчислення часу виконання
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Виведення результатів та часу виконання
    if (result != -1) {
        std::cout << "Target value " << target << " found at index " << result << "\n";
    } else {
        std::cout << "Target value not found\n";
    }
    std::cout << "Parallel execution time: " << execution_time.count() * 1000 << " milliseconds\n";

    return 0;
}
