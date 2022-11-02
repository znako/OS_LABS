#ifndef OS_LABS_LAB3_H
#define OS_LABS_LAB3_H

#include <vector>

std::vector<int> MergeSortArr(std::vector<int> arr, int length, int threadCount);
//int generate_random_number(unsigned int lower_limit, unsigned int upper_limit);
void merge_sort(std::vector<int>* arr, int left, int right);
void merge(std::vector<int>* arr, int left, int middle, int right);
void* thread_merge_sort(void* arg);
// void merge_sections_of_array(std::vector<int>* arr, int number, int aggregation, int length, int NUMBERS_PER_THREAD);
//void test_array_is_in_order(int arr[]);

#endif //OS_LABS_LAB3_H