#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    int d;
    int m;
    int y;
} Date;

bool leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int days_in_m(int m, int y) {
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return leap(y) ? 29 : 28;
        default: return 0;
    }
}

bool valid(int d, int m, int y) {
    if (y < 1 || y > 9999) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > days_in_m(m, y)) return false;
    return true;
}

long long to_concat(int d, int m, int y) {
    return (long long)y * 10000 + m * 100 + d;
}

long long to_days(int d, int m, int y) {
    long long t = 0;
    for (int i = 1; i < y; i++) t += leap(i) ? 366 : 365;
    for (int i = 1; i < m; i++) t += days_in_m(i, y);
    return t + d;
}

void gen_rand_date(int *d, int *m, int *y) {
    do {
        *y = rand() % 9999 + 1;
        *m = rand() % 12 + 1;
        *d = rand() % days_in_m(*m, *y) + 1;
    } while (!valid(*d, *m, *y));
}

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
    printf("Сгенерировано %d дат в %s\n", cnt, fn);
}

Date* read_struct(const char *fn, int *cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return NULL;

    *cnt = 0;
    char buf[20];
    while (fgets(buf, sizeof(buf), f)) (*cnt)++;
    rewind(f);

    Date *ds = malloc(*cnt * sizeof(Date));
    for (int i = 0; i < *cnt; i++) {
        fscanf(f, "%d.%d.%d\n", &ds[i].d, &ds[i].m, &ds[i].y);
    }
    fclose(f);
    return ds;
}

long long* read_concat(const char *fn, int *cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return NULL;

    *cnt = 0;
    char buf[20];
    while (fgets(buf, sizeof(buf), f)) (*cnt)++;
    rewind(f);

    long long *ds = malloc(*cnt * sizeof(long long));
    for (int i = 0; i < *cnt; i++) {
        int d, m, y;
        fscanf(f, "%d.%d.%d\n", &d, &m, &y);
        ds[i] = to_concat(d, m, y);
    }
    fclose(f);
    return ds;
}

long long* read_days(const char *fn, int *cnt) {
    FILE *f = fopen(fn, "r");
    if (!f) return NULL;

    *cnt = 0;
    char buf[20];
    while (fgets(buf, sizeof(buf), f)) (*cnt)++;
    rewind(f);

    long long *ds = malloc(*cnt * sizeof(long long));
    for (int i = 0; i < *cnt; i++) {
        int d, m, y;
        fscanf(f, "%d.%d.%d\n", &d, &m, &y);
        ds[i] = to_days(d, m, y);
    }
    fclose(f);
    return ds;
}

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

void bubble_s(Date a[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (cmp_struct(&a[j], &a[j+1]) > 0) {
                Date t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
}

void bubble_l(long long a[], int n, int (*cmp)(const void*, const void*)) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (cmp(&a[j], &a[j+1]) > 0) {
                long long t = a[j];
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

double measure_time(void (*f)(void*, int), void *d, int n) {
    clock_t s = clock();
    f(d, n);
    clock_t e = clock();
    return ((double)(e - s)) / CLOCKS_PER_SEC;
}

void bubble_s_wrap(void *d, int n) { bubble_s((Date*)d, n); }
void bubble_c_wrap(void *d, int n) { bubble_l((long long*)d, n, cmp_concat); }
void bubble_d_wrap(void *d, int n) { bubble_l((long long*)d, n, cmp_days); }
void select_s_wrap(void *d, int n) { select_s((Date*)d, n); }
void select_c_wrap(void *d, int n) { select_l((long long*)d, n, cmp_concat); }
void select_d_wrap(void *d, int n) { select_l((long long*)d, n, cmp_days); }

void write_file(const char *fn, Date *ds, int n) {
    FILE *f = fopen(fn, "w");
    if (!f) return;
    for (int i = 0; i < n; i++) fprintf(f, "%d.%d.%d\n", ds[i].d, ds[i].m, ds[i].y);
    fclose(f);
}

bool sorted(Date *ds, int n) {
    for (int i = 0; i < n-1; i++)
        if (cmp_struct(&ds[i], &ds[i+1]) > 0) return false;
    return true;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const char *in = "dates.txt";
    const char *out = "sorted.txt";
    int n = 5000;

    printf("=== Генерация и сортировка дат ===\n");
    gen_file(in, n);

    int cnt;

    printf("\n=== Сравнение производительности ===\n");

    printf("\n--- Struct ---\n");
    Date *ds = read_struct(in, &cnt);
    Date *d1 = malloc(cnt * sizeof(Date));
    Date *d2 = malloc(cnt * sizeof(Date));
    memcpy(d1, ds, cnt * sizeof(Date));
    memcpy(d2, ds, cnt * sizeof(Date));

    double t1 = measure_time(bubble_s_wrap, ds, cnt);
    double t2 = measure_time(select_s_wrap, d1, cnt);
    double t3 = measure_time((void(*)(void*,int))quick_s_wrap, d2, cnt);

    printf("Bubble: %.6f\n", t1);
    printf("Select: %.6f\n", t2);
    printf("Quick:  %.6f\n", t3);
    printf("Sorted: %s\n", sorted(ds, cnt) ? "yes" : "no");

    printf("\n--- Concat ---\n");
    long long *dc = read_concat(in, &cnt);
    long long *c1 = malloc(cnt * sizeof(long long));
    long long *c2 = malloc(cnt * sizeof(long long));
    memcpy(c1, dc, cnt * sizeof(long long));
    memcpy(c2, dc, cnt * sizeof(long long));

    t1 = measure_time(bubble_c_wrap, dc, cnt);
    t2 = measure_time(select_c_wrap, c1, cnt);
    t3 = measure_time((void(*)(void*,int))quick_c_wrap, c2, cnt);

    printf("Bubble: %.6f\n", t1);
    printf("Select: %.6f\n", t2);
    printf("Quick:  %.6f\n", t3);

    printf("\n--- Days ---\n");
    long long *dd = read_days(in, &cnt);
    long long *dd1 = malloc(cnt * sizeof(long long));
    long long *dd2 = malloc(cnt * sizeof(long long));
    memcpy(dd1, dd, cnt * sizeof(long long));
    memcpy(dd2, dd, cnt * sizeof(long long));

    t1 = measure_time(bubble_d_wrap, dd, cnt);
    t2 = measure_time(select_d_wrap, dd1, cnt);
    t3 = measure_time((void(*)(void*,int))quick_d_wrap, dd2, cnt);

    printf("Bubble: %.6f\n", t1);
    printf("Select: %.6f\n", t2);
    printf("Quick:  %.6f\n", t3);

    write_file(out, ds, cnt);
    printf("\nОтсортированные данные записаны в %s\n", out);

    free(ds); free(d1); free(d2);
    free(dc); free(c1); free(c2);
    free(dd); free(dd1); free(dd2);

    return 0;
}
