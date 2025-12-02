#include <stdio.h>

typedef enum { L, R } Direction;
typedef struct {
    Direction dir;
    int num;
} Element;

int START = 50;

int rotateDial(Element entry, int password) {
    if (entry.dir == R) {
        // Moving right from START by num steps
        // Count how many times we pass through position 0
        int steps_to_first_zero = (100 - START) % 100;
        if (steps_to_first_zero == 0) steps_to_first_zero = 100;

        if (entry.num >= steps_to_first_zero) {
            password++; // Hit 0 the first time
            password += (entry.num - steps_to_first_zero) / 100; // Each additional full circle
        }
        START = (START + entry.num) % 100;
    } else {
        // Moving left from START by num steps
        // Count how many times we pass through position 0
        if (START > 0) {
            // Not starting at 0, so first hit is after START steps
            if (entry.num >= START) {
                password++; // Hit 0 the first time
                password += (entry.num - START) / 100; // Each additional full circle
            }
        } else {
            // Starting at 0, first hit is after 100 steps
            if (entry.num >= 100) {
                password += entry.num / 100; // Each full circle
            }
        }
        START = ((START - entry.num) % 100 + 100) % 100;
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