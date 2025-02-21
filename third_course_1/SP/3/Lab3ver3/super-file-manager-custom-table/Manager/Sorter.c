#include "Sorter.h"
#include "ErrorHandling.h"

typedef struct {
    int left;
    int right;
} Borders;

Borders partition(void** array, int low, int high, int (*compare)(void*, void*)) {
    int i = low;
    int j = high;
    int p = low - 1;
    int q = high;
    void* base = array[high];

    while (TRUE) {

        int res = compare(array[i], base);
        while (res < 0) {
            i++;
            if (i >= high) break;
            res = compare(array[i], base);
        } 

        do {
            j--;
            if (j <= low) break;
            res = compare(array[j], base);
        } while (res > 0);

        if (i >= j)
            break;

        void* tmp;

        tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;

        res = compare(array[i], base);
        if (res == 0) {
            p++;
            tmp = array[i];
            array[i] = array[p];
            array[p] = tmp;
        }


        res = compare(array[j], base);
        if (res == 0) {
            q--;
            tmp = array[q];
            array[q] = array[j];
            array[j] = tmp;
        }
    }

    void* tmp = array[i];
    array[i] = array[high];
    array[high] = tmp;


    j = i - 1;
    for (int k = low; k < p; k++, j--) {
        if (j < low) break;
        tmp = array[k];
        array[k] = array[j];
        array[j] = tmp;
    }

    i = i + 1;
    for (int k = high - 1; k > q; k--, i++) {
        tmp = array[k];
        array[k] = array[i];
        array[i] = tmp;
    }
    Borders ans = { i, j };
    return ans;
}

void quickSort(void** array, int low, int high, int (*compare)(void*, void*)) {
    if (low < high) {
        Borders pos = partition(array, low, high, compare);
        quickSort(array, low, pos.right, compare);
        quickSort(array, pos.left, high, compare);
    }
}

void Sort(void** array, int length, int(*comporator)(void*, void*)) {
    quickSort(array, 0, length - 1, compare);
}

void SortPart(void** array, int from,int to, int(*comporator)(void*, void*)) {
    quickSort(array, from, to, compare);
}

void** CreateSortedArray(void** array, int length, int(*comporator)(void*, void*)) {
    void** sortedArray = LoggedMalloc(_msize(array));

    if (sortedArray == NULL) {
        return NULL;
    }
    memcpy(sortedArray, array, _msize(array));
    compare = comporator;
    quickSort(sortedArray, 0, length - 1, compare);
    return sortedArray;
}

