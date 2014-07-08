#include <stdio.h>

int main(void) {
    unsigned char c = 0;
    while (fread(&c, 1, 1, stdin) > 0) {
        c = (c + 64) % 128;
        fwrite(&c, 1, 1, stdout);
    }
    return 0;
}
