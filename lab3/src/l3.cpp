#include "lab3.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct ARG
{
    std::vector<int>* a;
    int i;
    int NUMBERS_PER_THREAD;
    int OFFSET;
    int threadCount;
};

// Главная функция управления потоками и сортировкой
std::vector<int> MergeSortArr(std::vector<int> array, int length, int threadCount) {
    
    std::vector<int>* arr = &array;
    int NUMBERS_PER_THREAD = length / threadCount;
    int OFFSET = length % threadCount;

    if (threadCount > 1)
    {
        std::vector<pthread_t> threads(threadCount);
        ARG* arglist = (ARG*)malloc(sizeof(ARG) * threadCount);
        ARG* arg;

        for (long i = 0; i < threadCount; i ++) {

            arg = &arglist[i];
            arg->a = arr;
            arg->i = i;

            arg->NUMBERS_PER_THREAD = NUMBERS_PER_THREAD;

            arg->OFFSET = OFFSET;

            arg->threadCount = threadCount;

            int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)arg);
            if (rc){
                printf("Ошибка создания потока %d\n", rc);
                exit(-1);
            }
        }
        
        for(long i = 0; i < threadCount; i++) {
            pthread_join(threads[i], NULL);
        }
        int low = NUMBERS_PER_THREAD;
        int high;
        for (int i = 1; i < threadCount; i++)
        {
            high = low + NUMBERS_PER_THREAD - 1;
            if (i == (threadCount -1)){
                high = length - 1;
            } 
            merge(arr, 0, low - 1, high);
            low += NUMBERS_PER_THREAD;
        }
        free(arglist);
    }
    else{
        merge_sort(arr, 0, length-1);
    }
    return (*arr);
}

// Функция потока
void *thread_merge_sort(void* arg)
{
    ARG* argum = (ARG*)arg;
    int thread_id = (long)argum->i;
    int left = thread_id * (argum->NUMBERS_PER_THREAD);
    int right = (thread_id + 1) * (argum->NUMBERS_PER_THREAD) - 1;
    if (thread_id == argum->threadCount - 1) {
        right += argum->OFFSET;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {

        merge_sort(argum->a, left, right);
        merge_sort(argum->a, left + 1, right);
        merge(argum->a, left, middle, right);

    }
    return NULL;
}

// merge sort 
void merge_sort(std::vector<int>* arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}


void merge(std::vector<int>* arr, int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    std::vector<int> left_array(left_length);
    std::vector<int> right_array(right_length);
    
    // копируем значения в левый массив
    for (int i = 0; i < left_length; i ++) {
        left_array[i] = (*arr)[left + i];
    }
    
    // Копируем значения в правый массив
    for (int j = 0; j < right_length; j ++) {
        right_array[j] = (*arr)[middle + 1 + j];
    }
    
    i = 0;
    j = 0;
    // Выбираем значения из левого и правого массивов и заносим
    while (i < left_length && j < right_length) {
        if (left_array[i] <= right_array[j]) {
            (*arr)[left + k] = left_array[i];
            i ++;
        } else {
            (*arr)[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
    
    // Заносим оставшиеся значения
    while (i < left_length) {
        (*arr)[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        (*arr)[left + k] = right_array[j];
        k ++;
        j ++;
    }
}
