#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

const int ARRAY_SIZE = 200000000; // Размер массива
const int THREAD_COUNT = 8; // Количество потоков

std::vector<int> arr(ARRAY_SIZE); // Объявление массива для хранения случайных чисел

void fill_array() {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(1, 25);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = distr(eng); // Заполнение массива случайными числами
    }
}

void sum(int start, int end, std::atomic<long long int>& result) {
    int res = 0;
    for (int i = start; i < end; i++) {
        res += arr[i];
    }
    result += res;
    std::cout << "Thread " << std::this_thread::get_id() << " sum: " << res << std::endl;
}

int main() {
    fill_array(); // Заполнение массива случайными числами

    int results[THREAD_COUNT]; // Массив для хранения результатов работы потоков
    std::vector<std::thread> threads(THREAD_COUNT); // Вектор потоков
    std::atomic<long long int> result(0);

    auto start_time = std::chrono::high_resolution_clock::now(); // Начало подсчета времени выполнения

    // Создание и запуск потоков
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(sum, i * (ARRAY_SIZE / THREAD_COUNT), (i + 1) * (ARRAY_SIZE / THREAD_COUNT), std::ref(result));
    }

    // Дожидаемся завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now(); // Конец подсчета времени выполнения
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time); // Вычисление затраченного времени
    std::cout << " Total sum : " << result.load() << std::endl;
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl; // Вывод времени выполнения

    return 0;
}

