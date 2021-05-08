#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>

const int N = 4;
const int M = 5;
int ARR[N][M];
const int THREAD_COUNT = 4;

struct Information {
    int* arr;
    int start;
    int end;
    int maxValue;
    int maxValueCount;
    std::chrono::nanoseconds elapsedTime;
};

void fillStaticArray(int arr[N][M]);

void printStaticArray(int arr[N][M]);

void countMaxLocal(Information& info) {
    // // todo: elapsed time
    auto startPoint = std::chrono::system_clock::now();
    for (int i = info.start; i < info.end; i++) {
        if (info.arr[i] > info.maxValue || info.maxValueCount == 0) {
            info.maxValue = info.arr[i];
            info.maxValueCount = 1;
        }
        else if (info.arr[i] == info.maxValue) {
            info.maxValueCount++;
        }
    }
    auto endPoint = std::chrono::system_clock::now();
    //std::cout << "Thread " << _threadid << " elapsed time is " << std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count() << "ns" << std::endl;
    info.elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint);
}

Information countMax(int* arr) {
    // ! to be done
    std::thread threads[THREAD_COUNT];
    Information info[THREAD_COUNT];
    int elementsPerThread = N * M / THREAD_COUNT;
    auto startPoint = std::chrono::system_clock::now();
    for (int i = 0; i < 4; i++) {
        info[i].arr = arr;
        info[i].start = elementsPerThread * i;

        info[i].end = (elementsPerThread * (i + 1) > N * M) ? N * M : elementsPerThread * (i + 1);

        info[i].maxValue = 0;
        info[i].maxValueCount = 0;
        threads[i] = std::thread(&countMaxLocal, std::ref(info[i]));
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i].join();
    }
    auto endPoint = std::chrono::system_clock::now();
    Information restMax;
    restMax.elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint);
    for (int i = THREAD_COUNT * elementsPerThread; i < N * M; i++) {
        if (arr[i] > restMax.maxValue || restMax.maxValueCount == 0) {
            restMax.maxValue = arr[i];
            restMax.maxValueCount = 1;
        }
        else if (arr[i] == restMax.maxValue) {
            restMax.maxValueCount++;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        std::cout << "Thread " << i << " elapsed time is " << info[i].elapsedTime.count() << "ns\n";
        if (info[i].maxValue > restMax.maxValue || restMax.maxValueCount == 0) {
            restMax.maxValue = info[i].maxValue;
            restMax.maxValueCount = info[i].maxValueCount;
        }
        else if (info[i].maxValue == restMax.maxValue) {
            restMax.maxValueCount += info[i].maxValueCount;
        }
    }
    return restMax;
}

int main()
{
    // * 16*. Дана прямоугольная целочисленная матрица. Распараллеливание по элементам. Найти количество максимальных элементов.
    // * P. S. Для задач с четными номерами использовать Interlocked-функции, для задач с нечетными номерами - атомарные типы.\
    
    fillStaticArray(ARR);

    printStaticArray(ARR);

    std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;

    auto result = countMax(*ARR);

    std::cout << "Maximum number: " << result.maxValue << ". Times met: " << result.maxValueCount << std::endl;
    std::cout << "Overall elapsed time is: " << result.elapsedTime.count() << "ns" << std::endl;

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