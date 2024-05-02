#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

int binarySearch(const std::vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return mid; // Повернути індекс цільового значення
        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1; // Повернути -1, якщо цільове значення не знайдено
}

int main() {
    // Ініціалізація впорядкованого масиву (для демонстраційних цілей)
    std::vector<int> arr(100000000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    int target = 99999999; // Цільове значення для пошуку

    // Вимірювання часу виконання
    auto start_time = std::chrono::high_resolution_clock::now(); // Початок часу

    // Виконання бінарного пошуку
    int result = binarySearch(arr, 0, arr.size() - 1, target);

    auto end_time = std::chrono::high_resolution_clock::now(); // Кінець часу

    // Обчислення часу виконання
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Виведення результату та часу виконання
    if (result != -1) {
        std::cout << "Target value " << target << " found at index " << result << "\n";
    } else {
        std::cout << "Target value not found\n";
    }

    std::cout << "Sequential execution time: " << execution_time.count() * 1000 << " milliseconds\n";

    return 0;
}
