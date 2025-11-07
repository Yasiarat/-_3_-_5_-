#include <stdio.h>
#include <stdlib.h>

char encript(char *str, int k)
{
    unsigned char symb = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        symb = (unsigned char) str[i];

        symb = (symb + k - 33) % 223 + 33;

        str[i] = (char) symb;
    }

}

char decript(char *str, int k)
{
    unsigned char symb = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        symb = ((unsigned char) str[i] - k - 33 + 223) % 223 + 33;

        str[i] = (char) symb;
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
