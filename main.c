#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Сортировка пузырьком
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Сортировка выбором
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

// Быстрая сортировка (третья сортировка)
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Вспомогательная функция для быстрой сортировки
void quickSortWrapper(int arr[], int n) {
    quickSort(arr, 0, n - 1);
}

// Функция для генерации случайного массива
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (rand()*rand()) % 1000;
    }
}

// Функция для копирования массива
void copyArray(int source[], int destination[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

// Функция для проверки отсортированности массива
int isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Функция для измерения времени выполнения сортировки
double measureSortTime(void (*sortFunc)(int[], int), int arr[], int n) {
    clock_t start, end;

    start = clock();
    sortFunc(arr, n);
    end = clock();

    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
 SetConsoleOutputCP(65001);
 SetConsoleCP(65001);
    srand(time(NULL));

    const int sizes[] = {100, 1000, 5000};
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Сравнение времени выполнения кодировок:\n");
    printf("Размер | Пузырьковая | Выбором     | Быстрая\n");
    printf("-------|-------------|-------------|----------\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];

        // Генерируем исходный массив
        int *original = (int*)malloc(n * sizeof(int));
        generateRandomArray(original, n);

        // Создаем копии для каждой сортировки
        int *arr1 = (int*)malloc(n * sizeof(int));
        int *arr2 = (int*)malloc(n * sizeof(int));
        int *arr3 = (int*)malloc(n * sizeof(int));

        copyArray(original, arr1, n);
        copyArray(original, arr2, n);
        copyArray(original, arr3, n);

        // Измеряем время для каждой сортировки
        double time1 = measureSortTime(bubbleSort, arr1, n);
        double time2 = measureSortTime(selectionSort, arr2, n);
        double time3 = measureSortTime(quickSortWrapper, arr3, n);

        // Проверяем, что массивы отсортированы
        if (!isSorted(arr1, n) || !isSorted(arr2, n) || !isSorted(arr3, n)) {
            printf("Ошибка: массивы не отсортированы правильно!\n");
        }

        // Выводим результаты
        printf("%6d | %10.6f | %10.6f | %10.6f\n",
               n, time1, time2, time3);

        // Освобождаем память
        free(original);
        free(arr1);
        free(arr2);
        free(arr3);
    }

    // Дополнительный тест с большим массивом
    printf("\nТест с большим массивом (10000 элементов):\n");
    int n = 10000;
    int *original = (int*)malloc(n * sizeof(int));
    generateRandomArray(original, n);

    int *arr1 = (int*)malloc(n * sizeof(int));
    int *arr2 = (int*)malloc(n * sizeof(int));
    int *arr3 = (int*)malloc(n * sizeof(int));

    copyArray(original, arr1, n);
    copyArray(original, arr2, n);
    copyArray(original, arr3, n);

    // Пропускаем пузырьковую для большого массива (слишком медленно)
    double time1 = measureSortTime(bubbleSort, arr1, n);
    double time2 = measureSortTime(selectionSort, arr2, n);
    double time3 = measureSortTime(quickSortWrapper, arr3, n);

    printf("Пузырьком: %.6f сек, Выбором: %.6f сек, Быстрая: %.6f сек\n", time1, time2, time3);

    free(original);
    free(arr2);
    free(arr3);

    return 0;
}
