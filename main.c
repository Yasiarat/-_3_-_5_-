#include <stdio.h>
#include <stdlib.h>

// функция шифрования
char encript(char *str, int k)
{
    unsigned char symb = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        symb = (unsigned char) str[i];

        symb = (symb + k - 33) % 223 + 33;

        str[i] = (char) symb;
    }

}

//функция расшифрования
char decript(char *str, int k)
{
    unsigned char symb = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        symb = ((unsigned char) str[i] - k - 33 + 223) % 223 + 33;

        str[i] = (char) symb;
    }

}

// Функция чтения файла
char* read_file(const char *fn, long *f_sz)
{
    FILE *f = fopen(fn, "rb");
    if (f == NULL) {
        printf("Не удалось открыть файл %s для чтения\n", fn);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *f_sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *str = (char*)malloc(*f_sz + 1);

    if (str == NULL) {
        printf("Не удалось выделить память\n");
        fclose(f);
        return NULL;
    }

    size_t len = fread(str, 1, *f_sz, f);
    str[len] = '\0';

    fclose(f);
    return str;
}

// Функция для записи в файл
int write_file(const char *fn, const char *str, long str_sz)
{
    FILE *f = fopen(fn, "wb");
    if (f == NULL) {
        printf("Не удалось открыть файл %s для записи\n", fn);
        return 0;
    }

    size_t len = fwrite(str, 1, str_sz, f);
    fclose(f);

    if (len != str_sz) {
        printf("Не удалось записать все данные в файл\n");
        return 0;
    }
}

int main()
{
    char s[] = "Hello, world";
    s[0] =  (char) 253;
    encript(s, 3);
    decript(s, 3);

    printf("%s\n", s);
    printf("%c\n", 255);
    return 0;
}
