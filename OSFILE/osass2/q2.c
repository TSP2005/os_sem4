#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    char ch;
    int nonAlphabeticCount = 0;

    while ((ch = getchar()) != EOF) {
        putchar(ch);

        if (!isalpha(ch)) {
            nonAlphabeticCount++;
        }
    }

    fprintf(stderr, "Number of non-alphabetic characters (including newlines): %d\n", nonAlphabeticCount);
    exit(0);
}
