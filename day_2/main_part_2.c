//
// Created by Darius-George Stan on 02.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_invalid_id(long long n) {
    char s[32];
    sprintf(s, "%lld", n);
    int len = strlen(s);

    // Try all possible pattern lengths (divisors of the total length)
    // Pattern must repeat at least twice, so max pattern length is len/2
    for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++) {
        // Check if length is divisible by pattern length
        if (len % pattern_len != 0) {
            continue;
        }

        // Check if the pattern repeats throughout the entire number
        bool matches = true;
        for (int i = pattern_len; i < len; i++) {
            if (s[i] != s[i % pattern_len]) {
                matches = false;
                break;
            }
        }

        if (matches) {
            return true;
        }
    }

    return false;
}

long long find_invalid_ids_sum(long long start, long long end) {
    long long sum = 0;
    for (long long n = start; n <= end; n++) {
        if (is_invalid_id(n)) {
            sum += n;
        }
    }
    return sum;
}

long long solve(const char* input) {
    long long total = 0;
    char* input_copy = strdup(input);
    char* token = strtok(input_copy, ",");

    while (token != NULL) {
        long long start, end;
        if (sscanf(token, "%lld-%lld", &start, &end) == 2) {
            long long range_sum = find_invalid_ids_sum(start, end);
            total += range_sum;
        }
        token = strtok(NULL, ",");
    }

    free(input_copy);
    return total;
}

int main() {
    // Test with the example
    const char* example = "11-22,95-115,998-1012,1188511880-1188511890,"
                         "222220-222224,1698522-1698528,446443-446449,"
                         "38593856-38593862,565653-565659,824824821-824824827,"
                         "2121212118-2121212124";

    long long result = solve(example);
    printf("Example result: %lld\n", result);

    // Test individual invalid IDs
    printf("\nTesting individual IDs:\n");
    long long test_ids[] = {11, 22, 99, 111, 999, 1010, 1188511885, 222222,
                            446446, 38593859, 565656, 824824824, 2121212121,
                            12341234, 123123123, 1212121212, 1111111};
    for (int i = 0; i < 17; i++) {
        printf("  %lld: %s\n", test_ids[i],
               is_invalid_id(test_ids[i]) ? "INVALID" : "valid");
    }

    // Your actual puzzle input
    const char* puzzle_input =
        "92916254-92945956,5454498003-5454580069,28-45,4615-7998,"
        "4747396917-4747534264,272993-389376,36290651-36423050,177-310,"
        "3246326-3418616,48-93,894714-949755,952007-1003147,3-16,632-1029,"
        "420-581,585519115-585673174,1041-1698,27443-39304,71589003-71823870,"
        "97-142,2790995-2837912,579556301-579617006,653443-674678,"
        "1515120817-1515176202,13504-20701,1896-3566,8359-13220,51924-98061,"
        "505196-638209,67070129-67263432,694648-751703,8892865662-8892912125";

    result = solve(puzzle_input);
    printf("\n\nPuzzle answer: %lld\n", result);

    return 0;
}