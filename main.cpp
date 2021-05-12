#include <iostream>
#include <thread>
#include <windows.h>
#include <fstream>
#include <chrono>

const int N = 4;
const int M = 5;
int ARR[N][M];
const int THREAD_COUNT = 4;

struct Max {
    int value;
    int count;
};

volatile Max result;

long long THREAD_TIME[THREAD_COUNT];

volatile long locked = false;

void fillStaticArray(int arr[N][M]);

void printStaticArray(int arr[N][M]);

void countMaxLocal(int* arr, int start, int end, long long time) {
    auto startPoint = std::chrono::system_clock::now();

    Max local;
    local.value = arr[start];
    local.count = 1;

    // finding local maximum value
    for (int i = start; i < end; i++) {
        if (arr[i] > local.value) {
            local.value = arr[i];
            local.count = 1;
        }
        else if (arr[i] == local.value) {
            local.count++;
        }
    }

    // comparing and assigning
    while (!_InterlockedExchange(&locked, true)) {
        if (local.value > result.value) {
            result.value = local.value;
            result.count = local.count; 
        }
        else if (local.value == result.value) {
            result.count += local.count;
        }
        std::cout << result.value << ":" << result.count << std::endl;
    }
    _InterlockedExchange(&locked, false);

    auto endPoint = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
    // std::cout << "Thread " << _threadid << " elapsed time is " << std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count() << "ns\n" << std::endl;
}

void countMax(int* arr) {
    // creating threads
    std::thread threads[THREAD_COUNT];

    if (arr != nullptr) {
        result.value = arr[0];
        result.count = 1;
    }

    // allocating elements per threads and starting the threads
    int elementsPerThread = N * M / THREAD_COUNT;
    auto startPoint = std::chrono::system_clock::now();
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(
            &countMaxLocal, 
            arr, 
            elementsPerThread * i, 
            (elementsPerThread * (i + 1) > N * M) ? N * M : elementsPerThread * (i + 1),
            std::ref(THREAD_TIME[i])
        );
    }

    // waiting for threads to complete
    for (int i = 0; i < THREAD_COUNT - 1; i++) {
        threads[i].join();
    }

    // handling the rest of the matrix
    for (int i = THREAD_COUNT * elementsPerThread; i < N * M; i++) {
        if (arr[i] > result.value) {
            result.value = arr[i];
            result.count = 1;
        }
        else if (arr[i] == result.value) {
            result.count++;
        }
    }
    auto endPoint = std::chrono::system_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endPoint - startPoint).count();
    for (int i = 0; i < THREAD_COUNT; i++) {
        std::cout << "Thread " << i << " elapsed time is " << THREAD_TIME[i] << "ns\n";
    }
    std::cout << "Overall elapsed time is " << elapsedTime << "ns\n";
}

int main()
{
    // * 16*. Дана прямоугольная целочисленная матрица. Распараллеливание по элементам. Найти количество максимальных элементов.
    // * P. S. Для задач с четными номерами использовать Interlocked-функции, для задач с нечетными номерами - атомарные типы.\
    
    fillStaticArray(ARR);

    printStaticArray(ARR);

    std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;

    countMax(*ARR);

    std::cout << "Maximum number: " << result.value << ". Times met: " << result.count << std::endl;

    system("pause");
    return 0;
    // test
}

void fillStaticArray(int arr[N][M]) {
    std::ifstream reader("input.txt");
    if (reader) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                reader >> arr[i][j];
            }
        }
        reader.close();
    }
    else {
        std::cout << "File not found";
    }
}

void printStaticArray(int arr[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            std::cout << arr[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}