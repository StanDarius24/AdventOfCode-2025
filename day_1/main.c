//
// Created by Darius-George Stan on 02.12.2025.
//

#include <stdio.h>

typedef enum { L, R } Direction;
typedef struct {
    Direction dir;
    int num;
} Element;

int START = 50;

int rotateDial(Element entry, int password) {
    if (entry.dir == R) {
        START = (START + entry.num) % 100;
    } else {
        START = (START - entry.num) % 100;
    }
    if (START == 0) {
        password++;
    }
    return password;
}

int main(void) {
    FILE *file = fopen("/Users/dariusgeorgestan/Desktop/adventOfCode/day_1/input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    Element entry;
    int count = 0;
    int password = 0;
    char line[32];

    while (fgets(line, sizeof(line), file)) {
        char d;
        int n;
        if (sscanf(line, "%c%d", &d, &n) == 2) {
            entry.dir = (d == 'L') ? L : R;
            entry.num = n;
            password = rotateDial(entry, password);
        }
    }
    printf("%d\n", password);
    fclose(file);
}
