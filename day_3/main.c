// 'day_3/main.c' (C)
#include <stdio.h>

long finBiggerTwoSetOfNumbersFromDigits(int* digits, int len) {
    if (len < 2) return 0;

    int suffixMax[100];
    suffixMax[len - 1] = -1;

    for (int i = len - 2; i >= 0; --i) {
        int next = digits[i + 1];
        int prevMax = suffixMax[i + 1];
        suffixMax[i] = (next > prevMax) ? next : prevMax;
    }

    long best = 0;
    for (int i = 0; i < len - 1; ++i) {
        if (suffixMax[i] >= 0) {
            long candidate = 10L * digits[i] + suffixMax[i];
            if (candidate > best) best = candidate;
        }
    }
    return best;
}

int main(void) {
    FILE *file = fopen("/Users/dariusgeorgestan/Desktop/adventOfCode/day_3/input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char line[101];
    long sum = 0;
    int digits[100];

    while (fgets(line, sizeof(line), file)) {
        int len = 0;
        for (int i = 0; line[i] && line[i] != '\n'; ++i) {
            if (line[i] >= '0' && line[i] <= '9') {
                digits[len++] = line[i] - '0';
            }
        }
        if (len >= 2) {
            sum += finBiggerTwoSetOfNumbersFromDigits(digits, len);
        }
    }

    printf("%ld\n", sum);
    fclose(file);
    return 0;
}
