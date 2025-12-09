#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *path = "/Users/dariusgeorgestan/Desktop/adventOfCode/day_7/input.txt";
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char **grid = NULL;
    int rows = 0, cap = 0, cols = -1;
    char buf[8192];
    while (fgets(buf, sizeof(buf), fp)) {
        size_t len = strlen(buf);
        while (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) buf[--len] = '\0';
        if (len == 0) continue;
        if (cols == -1) cols = (int)len;
        if ((int)len != cols) {
            if ((int)len < cols) {
                char *padded = (char *)malloc((size_t)cols + 1);
                memcpy(padded, buf, len);
                memset(padded + len, '.', (size_t)(cols - len));
                padded[cols] = '\0';
                if (rows == cap) {
                    cap = cap ? cap * 2 : 16;
                    grid = (char **)realloc(grid, sizeof(char *) * cap);
                }
                grid[rows++] = padded;
                continue;
            } else {
                buf[cols] = '\0';
            }
        }
        char *line = (char *)malloc((size_t)cols + 1);
        memcpy(line, buf, (size_t)cols + 1);
        if (rows == cap) {
            cap = cap ? cap * 2 : 16;
            grid = (char **)realloc(grid, sizeof(char *) * cap);
        }
        grid[rows++] = line;
    }
    fclose(fp);

    if (rows == 0 || cols <= 0) {
        fprintf(stderr, "Empty input\n");
        for (int i = 0; i < rows; i++) free(grid[i]);
        free(grid);
        return 1;
    }

    int s_col = -1;
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') { s_col = c; break; }
    }
    if (s_col == -1) {
        fprintf(stderr, "No S found in the first row\n");
        for (int i = 0; i < rows; i++) free(grid[i]);
        free(grid);
        return 1;
    }

    unsigned long long *ways_current = (unsigned long long *)calloc((size_t)cols, sizeof(unsigned long long));
    unsigned long long *ways_next = (unsigned long long *)calloc((size_t)cols, sizeof(unsigned long long));
    if (!ways_current || !ways_next) {
        fprintf(stderr, "Out of memory\n");
        free(ways_current); free(ways_next);
        for (int i = 0; i < rows; i++) free(grid[i]);
        free(grid);
        return 1;
    }

    ways_current[s_col] = 1ULL;

    for (int r = 1; r < rows; r++) {
        memset(ways_next, 0, sizeof(unsigned long long) * (size_t)cols);
        for (int col = 0; col < cols; col++) {
            unsigned long long count = ways_current[col];
            if (count == 0) continue;
            char cell = grid[r][col];
            if (cell == '^') {
                if (col - 1 >= 0) ways_next[col - 1] += count;
                if (col + 1 < cols) ways_next[col + 1] += count;
            } else {
                ways_next[col] += count;
            }
        }
        unsigned long long *tmp = ways_current;
        ways_current = ways_next;
        ways_next = tmp;
    }

    unsigned long long total = 0;
    for (int col = 0; col < cols; col++) total += ways_current[col];

    printf("%llu\n", total);

    free(ways_current);
    free(ways_next);
    for (int i = 0; i < rows; i++) free(grid[i]);
    free(grid);
    return 0;
}
