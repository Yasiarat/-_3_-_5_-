#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Структура для хранения даты
typedef struct {
    int d;
    int m;
    int y;
} Date;

// Проверка на високосный год
bool leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Количество дней в месяце
int days_in_m(int m, int y) {
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return leap(y) ? 29 : 28;
        default: return 0;
    }
}

// Проверка корректности даты
bool valid(int d, int m, int y) {
    if (y < 1 || y > 9999) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > days_in_m(m, y)) return false;
    return true;
}

// Преобразование в число ггггммдд
long long to_concat(int d, int m, int y) {
    return (long long)y * 10000 + m * 100 + d;
}

// Преобразование в количество дней от 1.1.1
long long to_days(int d, int m, int y) {
    long long t = 0;
    for (int i = 1; i < y; i++) {
        t += leap(i) ? 366 : 365;
    }
    for (int i = 1; i < m; i++) {
        t += days_in_m(i, y);
    }
    return t + d - 1; // -1 потому что 1.1.1 = 0 дней
}

// Генерация случайной даты
void gen_rand_date(int *d, int *m, int *y) {
    do {
        *y = rand() % 9999 + 1;
        *m = rand() % 12 + 1;
        *d = rand() % days_in_m(*m, *y) + 1;
    } while (!valid(*d, *m, *y));
}

// Генерация файла с датами
void gen_file(const char *fn, int cnt) {
    FILE *f = fopen(fn, "w");
    if (!f) return;

    srand(time(NULL));
    for (int i = 0; i < cnt; i++) {
        int d, m, y;
        gen_rand_date(&d, &m, &y);
        fprintf(f, "%d.%d.%d\n", d, m, y);
    }
    fclose(f);
}

// Чтение дат в структуры
void read_struct(const char *fn, Date *ds, int cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return;

    for (int i = 0; i < cnt; i++) {
        fscanf(f, "%d.%d.%d\n", &ds[i].d, &ds[i].m, &ds[i].y);
    }
    fclose(f);
}

// Чтение дат в числа ггггммдд
void read_concat(const char *fn, long long *ds, int cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return;

    for (int i = 0; i < cnt; i++) {
        int d, m, y;
        fscanf(f, "%d.%d.%d\n", &d, &m, &y);
        ds[i] = to_concat(d, m, y);
    }
    fclose(f);
}

// Чтение дат в количество дней
void read_days(const char *fn, long long *ds, int cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return;

    for (int i = 0; i < cnt; i++) {
        int d, m, y;
        fscanf(f, "%d.%d.%d\n", &d, &m, &y);
        ds[i] = to_days(d, m, y);
    }
    fclose(f);
}

// Функции сравнения
int cmp_struct(const void *a, const void *b) {
    const Date *d1 = (const Date*)a;
    const Date *d2 = (const Date*)b;
    if (d1->y != d2->y) return d1->y - d2->y;
    if (d1->m != d2->m) return d1->m - d2->m;
    return d1->d - d2->d;
}

int cmp_concat(const void *a, const void *b) {
    long long d1 = *(const long long*)a;
    long long d2 = *(const long long*)b;
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

int cmp_days(const void *a, const void *b) {
    long long d1 = *(const long long*)a;
    long long d2 = *(const long long*)b;
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

// Алгоритмы сортировки для структур
void bubble_s(Date a[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (cmp_struct(&a[j], &a[j+1]) > 0) {
                Date t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
}

void select_s(Date a[], int n) {
    for (int i = 0; i < n-1; i++) {
        int mi = i;
        for (int j = i+1; j < n; j++)
            if (cmp_struct(&a[j], &a[mi]) < 0) mi = j;
        Date t = a[i];
        a[i] = a[mi];
        a[mi] = t;
    }
}

void quick_s(Date a[], int l, int h) {
    if (l < h) {
        Date p = a[h];
        int i = l-1;
        for (int j = l; j <= h-1; j++) {
            if (cmp_struct(&a[j], &p) < 0) {
                i++;
                Date t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
        Date t = a[i+1];
        a[i+1] = a[h];
        a[h] = t;
        int pi = i+1;
        quick_s(a, l, pi-1);
        quick_s(a, pi+1, h);
    }
}

void quick_s_wrap(Date a[], int n) {
    quick_s(a, 0, n-1);
}

// Алгоритмы сортировки для long long с пользовательской функцией сравнения
void bubble_l(long long a[], int n, int (*cmp)(const void*, const void*)) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (cmp(&a[j], &a[j+1]) > 0) {
                long long t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
}

void select_l(long long a[], int n, int (*cmp)(const void*, const void*)) {
    for (int i = 0; i < n-1; i++) {
        int mi = i;
        for (int j = i+1; j < n; j++)
            if (cmp(&a[j], &a[mi]) < 0) mi = j;
        long long t = a[i];
        a[i] = a[mi];
        a[mi] = t;
    }
}

void quick_l(long long a[], int l, int h, int (*cmp)(const void*, const void*)) {
    if (l < h) {
        long long p = a[h];
        int i = l-1;
        for (int j = l; j <= h-1; j++) {
            if (cmp(&a[j], &p) < 0) {
                i++;
                long long t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
        long long t = a[i+1];
        a[i+1] = a[h];
        a[h] = t;
        int pi = i+1;
        quick_l(a, l, pi-1, cmp);
        quick_l(a, pi+1, h, cmp);
    }
}

void quick_c_wrap(long long a[], int n) {
    quick_l(a, 0, n-1, cmp_concat);
}

void quick_d_wrap(long long a[], int n) {
    quick_l(a, 0, n-1, cmp_days);
}

// Функции-обертки для измерения времени
void bubble_s_wrap(Date *d, int n) { bubble_s(d, n); }
void bubble_c_wrap(long long *d, int n) { bubble_l(d, n, cmp_concat); }
void bubble_d_wrap(long long *d, int n) { bubble_l(d, n, cmp_days); }
void select_s_wrap(Date *d, int n) { select_s(d, n); }
void select_c_wrap(long long *d, int n) { select_l(d, n, cmp_concat); }
void select_d_wrap(long long *d, int n) { select_l(d, n, cmp_days); }

// Измерение времени выполнения
double measure_time(void (*f)(void*, int), void *d, int n) {
    clock_t s = clock();
    f(d, n);
    clock_t e = clock();
    return ((double)(e - s)) / CLOCKS_PER_SEC;
}

// Запись отсортированных данных в файл
void write_file(const char *fn, Date *ds, int n) {
    FILE *f = fopen(fn, "w");
    if (!f) return;
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d.%d.%d\n", ds[i].d, ds[i].m, ds[i].y);
    }
    fclose(f);
}

// Проверка отсортированности
bool sorted(Date *ds, int n) {
    for (int i = 0; i < n-1; i++)
        if (cmp_struct(&ds[i], &ds[i+1]) > 0) return false;
    return true;
}

// Подсчет количества строк в файле
int count_lines(const char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) return 0;

    int cnt = 0;
    char buf[20];
    while (fgets(buf, sizeof(buf), f)) cnt++;
    fclose(f);
    return cnt;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);


    const char *in = "dates.txt";
    const char *out = "sorted.txt";
    int n = 10000; // Количество дат для генерации

    gen_file(in, n);

    int cnt = count_lines(in);

    // Выделение памяти для всех представлений
    Date *ds = malloc(cnt * sizeof(Date));
    long long *dc = malloc(cnt * sizeof(long long));
    long long *dd = malloc(cnt * sizeof(long long));

    // Копии для сортировки
    Date *ds_copy1 = malloc(cnt * sizeof(Date));
    Date *ds_copy2 = malloc(cnt * sizeof(Date));
    long long *dc_copy1 = malloc(cnt * sizeof(long long));
    long long *dc_copy2 = malloc(cnt * sizeof(long long));
    long long *dd_copy1 = malloc(cnt * sizeof(long long));
    long long *dd_copy2 = malloc(cnt * sizeof(long long));

    // Чтение данных
    read_struct(in, ds, cnt);
    read_concat(in, dc, cnt);
    read_days(in, dd, cnt);

    // Создание копий
    memcpy(ds_copy1, ds, cnt * sizeof(Date));
    memcpy(ds_copy2, ds, cnt * sizeof(Date));
    memcpy(dc_copy1, dc, cnt * sizeof(long long));
    memcpy(dc_copy2, dc, cnt * sizeof(long long));
    memcpy(dd_copy1, dd, cnt * sizeof(long long));
    memcpy(dd_copy2, dd, cnt * sizeof(long long));

    printf("\nСРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ\n");

    // Тестирование структур
    printf("\n Структуры (год, месяц, день)\n");
    double t_bubble_s = measure_time((void(*)(void*,int))bubble_s_wrap, ds, cnt);
    double t_select_s = measure_time((void(*)(void*,int))select_s_wrap, ds_copy1, cnt);
    double t_quick_s = measure_time((void(*)(void*,int))quick_s_wrap, ds_copy2, cnt);

    printf("Пузырьковая: %.6f сек\n", t_bubble_s);
    printf("Выбором:     %.6f сек\n", t_select_s);
    printf("Быстрая:     %.6f сек\n", t_quick_s);
    printf("Отсортировано: %s\n", sorted(ds, cnt) ? "да" : "нет");

    // Тестирование чисел ггггммдд
    printf("\n Числа (ггггммдд) \n");
    double t_bubble_c = measure_time((void(*)(void*,int))bubble_c_wrap, dc, cnt);
    double t_select_c = measure_time((void(*)(void*,int))select_c_wrap, dc_copy1, cnt);
    double t_quick_c = measure_time((void(*)(void*,int))quick_c_wrap, dc_copy2, cnt);

    printf("Пузырьковая: %.6f сек\n", t_bubble_c);
    printf("Выбором:     %.6f сек\n", t_select_c);
    printf("Быстрая:     %.6f сек\n", t_quick_c);

    // Тестирование количества дней
    printf("\n Количество дней от 1.1.1\n");
    double t_bubble_d = measure_time((void(*)(void*,int))bubble_d_wrap, dd, cnt);
    double t_select_d = measure_time((void(*)(void*,int))select_d_wrap, dd_copy1, cnt);
    double t_quick_d = measure_time((void(*)(void*,int))quick_d_wrap, dd_copy2, cnt);

    printf("Пузырьковая: %.6f сек\n", t_bubble_d);
    printf("Выбором:     %.6f сек\n", t_select_d);
    printf("Быстрая:     %.6f сек\n", t_quick_d);

    // Вывод таблицы эффективности
    printf("\n    Таблица эффективности n");
    printf("Представление\\Алгоритм | Пузырьковая | Выбором     | Быстрая\n");
    printf("-----------------------|-------------|-------------|-------------\n");
    printf("Структуры              | %-11.6f | %-11.6f | %-11.6f\n", t_bubble_s, t_select_s, t_quick_s);
    printf("Числа ггггммдд         | %-11.6f | %-11.6f | %-11.6f\n", t_bubble_c, t_select_c, t_quick_c);
    printf("Количество дней        | %-11.6f | %-11.6f | %-11.6f\n", t_bubble_d, t_select_d, t_quick_d);

    // Запись отсортированных данных
    write_file(out, ds, cnt);

    // Освобождение памяти
    free(ds); free(ds_copy1); free(ds_copy2);
    free(dc); free(dc_copy1); free(dc_copy2);
    free(dd); free(dd_copy1); free(dd_copy2);

    return 0;
}
