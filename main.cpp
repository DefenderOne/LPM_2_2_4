#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>

const int N = 4;
const int M = 5;
int ARR[N][M];
const int THREAD_COUNT = 4;

void fillStaticArray(int arr[N][M]);

void printStaticArray(int arr[N][M]);

void countMaxCountLocal(int* arr, int start, int end, std::tuple<int, int>& result) {
    // ! to be done
}

std::tuple<int, int> countMaxCount(int* arr) {
    // ! to be done
    std::thread threads[THREAD_COUNT];
    std::tuple<int, int> results[THREAD_COUNT];
    int elementsPerThread = N * M / THREAD_COUNT;
    for (int i = 0; i < 4; i++) {
        threads[i] = std::thread(&countMaxCountLocal, arr, elementsPerThread * i, elementsPerThread * (i + 1), ref(results[i]));
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i].join();
    }


}

// unsigned __stdcall countMaxLocal(void* data) {
//     auto startPoint = std::chrono::steady_clock::now();
//     Data* info = (Data*)data;
//     info->maxValueCount = 0;
//     for (int i = info->start; i < info->end; i++) {
//         if (info->arr[i] > info->maxValue || info->maxValueCount == 0) {
//             info->maxValue = info->arr[i];
//             info->maxValueCount = 1;
//         }
//         else if (info->arr[i] == info->maxValue) {
//             info->maxValueCount++;
//         }
//     }
//     auto endPoint = std::chrono::steady_clock::now();
//     info->elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
//     _endthreadex(0);
//     return 0;
// }

// Data countMax(int arr[N][M]) {
//     Data dataArr[THREAD_COUNT];
//     HANDLE handles[THREAD_COUNT];
//     // There must be threads created
//     int elementsPerThread = N * M / THREAD_COUNT;
//     for (int i = 0; i < 4; i++) {
//         dataArr[i].arr = *arr;
//         dataArr[i].start = elementsPerThread * i;
//         dataArr[i].end = elementsPerThread * (i + 1);
//         dataArr[i].maxValue = 0;
//         dataArr[i].maxValueCount = 0;
//         handles[i] = (HANDLE)_beginthreadex(nullptr, 0, &countMaxLocal, &dataArr[i], 0, nullptr);
//     }
//     // waiting for threads to complete
//     WaitForMultipleObjects(THREAD_COUNT, handles, true, INFINITE);

//     // finding max number count in the rest part of the matrix
//     int restMaxValue = 0;
//     int restMaxValueCount = 0;
//     for (int i = THREAD_COUNT * elementsPerThread; i < N * M; i++) {
//         if ((*arr)[i] > restMaxValue || restMaxValueCount == 0) {
//             restMaxValueCount = 1;
//             restMaxValue = (*arr)[i];
//         }
//         else if (restMaxValue == (*arr)[i]) {
//             restMaxValueCount++;
//         }
//     }

//     for (int i = 0; i < THREAD_COUNT; i++) {
//         CloseHandle(handles[i]);
//         std::cout << "Thread " << i << " elapsed time is " << dataArr[i].elapsedTime << "ns\n";
//         if (dataArr[i].maxValue > restMaxValue || restMaxValueCount == 0) {
//             restMaxValue = dataArr[i].maxValue;
//             restMaxValueCount = dataArr[i].maxValueCount;
//         }
//         else if (dataArr[i].maxValue == restMaxValue) {
//             restMaxValueCount += dataArr[i].maxValueCount;
//         }
//     }
//     Data result;
//     result.maxValue = restMaxValue;
//     result.maxValueCount = restMaxValueCount;
//     return result;
// }

int main()
{
    // * 16*. Дана прямоугольная целочисленная матрица. Распараллеливание по элементам. Найти количество максимальных элементов.
    // * P. S. Для задач с четными номерами использовать Interlocked-функции, для задач с нечетными номерами - атомарные типы.\
    
    fillStaticArray(ARR);

    printStaticArray(ARR);

    std::cout << "Threads abailable: " << std::thread::hardware_concurrency() << std::endl;

    // Data maxNumber = countMax(ARR);
    // std::cout << "Maximum number: " << maxNumber.maxValue << ". Times met: " << maxNumber.maxValueCount << std::endl;

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