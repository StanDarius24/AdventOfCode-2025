#include <stdio.h>
#include <string.h>

unsigned long long maxNumberFromKDigits(const int* digits, int len, int k) {
    if (len < k) return 0;

    int stack[100];
    int top = 0;
    int toDrop = len - k;

    for (int i = 0; i < len; ++i) {
        int d = digits[i];
        while (top > 0 && stack[top - 1] < d && toDrop > 0) {
            --top;
            --toDrop;
        }
        if (top < k) {
            stack[top++] = d;
        } else {

            if (toDrop > 0) --toDrop;
        }
    }

    // Build the 12-digit number
    unsigned long long value = 0ULL;
    for (int i = 0; i < k; ++i) {
        value = value * 10ULL + (unsigned long long)stack[i];
    }
    return value;
}

static void print_u128(unsigned __int128 x) {
    // Convert to decimal string
    char buf[64];
    int i = 63;
    buf[i] = '\0';
    if (x == 0) {
        puts("0");
        return;
    }
    while (x > 0) {
        unsigned int digit = (unsigned int)(x % 10);
        buf[--i] = (char)('0' + digit);
        x /= 10;
    }
    puts(&buf[i]);
}

int main(void) {
    FILE *file = fopen("/Users/dariusgeorgestan/Desktop/adventOfCode/day_3/input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char line[1024];
    int digits[1000];
    const int K = 12;
    unsigned __int128 sum = 0;

    while (fgets(line, sizeof(line), file)) {
        int len = 0;
        for (int i = 0; line[i] && line[i] != '\n'; ++i) {
            if (line[i] >= '0' && line[i] <= '9') {
                digits[len++] = line[i] - '0';
            }
        }
        if (len >= K) {
            unsigned long long v = maxNumberFromKDigits(digits, len, K);
            sum += (unsigned __int128)v;
        }
    }

    fclose(file);
    print_u128(sum);
    return 0;
}