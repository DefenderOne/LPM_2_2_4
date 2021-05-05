#include <iostream>
#include <Windows.h>
#include <process.h>
#include <fstream>

const int N = 4;
const int M = 5;
int ARR[N][M];
const int THREAD_COUNT = 4;

void fillStaticArray(int arr[N][M]);

void printStaticArray(int arr[N][M]);

// Structure to pass arguments into a parallel function and get a result from the function
struct Data {
    // * Pointer to a matrix represented as an array
    int* arr;
    int start;
    int end;
    int maxValue;
    int maxValueCount;
};

unsigned __stdcall countMaxLocal(void* data) {
    Data* info = (Data*)data;
    info->maxValueCount = 0;
    for (int i = info->start; i < info->end; i++) {
        if (info->arr[i] > info->maxValue || info->maxValueCount == 0) {
            info->maxValue = info->arr[i];
            info->maxValueCount = 1;
        }
        else if (info->arr[i] == info->maxValue) {
            info->maxValueCount++;
        }
    }
    _endthreadex(0);
    return 0;
}

Data countMax(int arr[N][M]) {
    Data dataArr[THREAD_COUNT];
    HANDLE handles[THREAD_COUNT];
    // There must be threads created
    int elementsPerThread = N * M / THREAD_COUNT;
    for (int i = 0; i < 4; i++) {
        dataArr[i].arr = *arr;
        dataArr->start = elementsPerThread * i;
        dataArr->end = elementsPerThread * (i + 1);
        handles[i] = (HANDLE)_beginthreadex(nullptr, 0, &countMaxLocal, &dataArr[i], 0, nullptr);
    }
    // waiting for threads to complete
    WaitForMultipleObjects(THREAD_COUNT, handles, true, INFINITE);

    // finding max number count in the rest part of the matrix
    int restMaxValue = 0;
    int restMaxValueCount = 0;
    for (int i = THREAD_COUNT * elementsPerThread; i < N * M; i++) {
        if ((*arr)[i] > restMaxValue || restMaxValueCount == 0) {
            restMaxValueCount = 1;
            restMaxValue = (*arr)[i];
        }
        else if (restMaxValue == (*arr)[i]) {
            restMaxValueCount++;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(handles[i]);
        if (dataArr[i].maxValue > restMaxValue || restMaxValueCount == 0) {
            restMaxValue = dataArr[i].maxValue;
            restMaxValueCount = dataArr[i].maxValueCount;
        }
        else if (dataArr[i].maxValue == restMaxValue) {
            restMaxValueCount += dataArr[i].maxValueCount;
        }
    }
    Data result;
    result.maxValue = restMaxValue;
    result.maxValueCount = restMaxValueCount;
    return result;
}

int main()
{
    // * 16*. Дана прямоугольная целочисленная матрица. Распараллеливание по элементам. Найти количество максимальных элементов.
    // * P. S. Для задач с четными номерами использовать Interlocked-функции, для задач с нечетными номерами - атомарные типы.
    // TODO: It requires 3 additional threads to be used. Total 4 threads with main one.
    // TODO: It requires handling of arrays with size less than number of threads
    
    fillStaticArray(ARR);

    printStaticArray(ARR);

    Data maxNumber = countMax(ARR);
    std::cout << "Maximum number times: " << maxNumber.maxValue << ". Times met: " << maxNumber.maxValueCount << std::endl;

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