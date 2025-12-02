//
// Created by Darius-George Stan on 05.12.2025.
//

// C
#include <stdio.h>
#include <string.h>

#define MAX_ROWS 2000
#define MAX_COLS 2000

int main(void) {
    FILE *file = fopen("/Users/dariusgeorgestan/Desktop/adventOfCode/day_4/input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char grid[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    char line[MAX_COLS + 5];

    while (fgets(line, sizeof(line), file)) {
        int len = (int)strcspn(line, "\r\n");
        if (len == 0) continue;
        if (cols == 0) cols = len;
        // If variable-width lines appear, truncate/align to first line width
        if (len > cols) len = cols;
        for (int j = 0; j < len; ++j) grid[rows][j] = line[j];
        rows++;
        if (rows >= MAX_ROWS) break;
    }
    fclose(file);

    static const int dr[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
    static const int dc[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

    long long accessible = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != '@') continue;
            int adj = 0;
            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    if (grid[nr][nc] == '@') adj++;
                }
            }
            if (adj < 4) accessible++;
        }
    }

    printf("%lld\n", accessible);
    return 0;
}
