#include <stdio.h>
#include <stdlib.h>

char encript(char *str, int k)
{
    int symb = 128;
    for (int i = 0; str[i] != '\0'; ++i) {
        symb += (char) str[i];

        if (symb > 255) {
            symb = (symb + 33) % 256;
            printf("!%d ", symb);
        }
printf("h%d ", symb);
        str[i] = (char) symb;
    }

}

char decript(char *str, int k)
{
    char symb = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        symb = ((int) str[i] - k) % 256;

        str[i] = (char) symb;
    }

}

int main()
{
    char s[] = "Hello, world";
    s[0] =  (char) 253;
    encript(s, 3);
    //decript(s, 3);

    printf("%s\n", s);
    printf("%c\n", 255);
    return 0;
}
