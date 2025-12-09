#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int *data;
    int size;
    int cap;
} IntSet;

static void set_init(IntSet *s, int cap) {
    s->data = (int *)malloc(sizeof(int) * cap);
    s->size = 0;
    s->cap = cap;
}

static void set_free(IntSet *s) {
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->cap = 0;
}

static void set_clear(IntSet *s) {
    s->size = 0;
}

static void set_ensure(IntSet *s, int need) {
    if (need <= s->cap) return;
    int ncap = s->cap ? s->cap * 2 : 16;
    while (ncap < need) ncap *= 2;
    s->data = (int *)realloc(s->data, sizeof(int) * ncap);
    s->cap = ncap;
}

static bool set_contains(const IntSet *s, int v) {
    for (int i = 0; i < s->size; i++) {
        if (s->data[i] == v) return true;
    }
    return false;
}

static void set_add(IntSet *s, int v) {
    if (set_contains(s, v)) return;
    set_ensure(s, s->size + 1);
    s->data[s->size++] = v;
}

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
        while (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
            buf[--len] = '\0';
        }
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
                len = (size_t)cols;
            }
        }
        char *line = (char *)malloc(len + 1);
        memcpy(line, buf, len + 1);
        if (rows == cap) {
            cap = cap ? cap * 2 : 16;
            grid = (char **)realloc(grid, sizeof(char *) * cap);
        }
        grid[rows++] = line;
    }
    fclose(fp);

    if (rows == 0 || cols <= 0) {
        fprintf(stderr, "Empty input\n");
        // Cleanup
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

    IntSet current, next;
    set_init(&current, 16);
    set_init(&next, 16);
    set_add(&current, s_col);

    long long splits = 0;

    for (int r = 1; r < rows; r++) {
        set_clear(&next);
        for (int i = 0; i < current.size; i++) {
            int col = current.data[i];
            if (col < 0 || col >= cols) continue;
            char cell = grid[r][col];
            if (cell == '^') {
                splits++;
                if (col - 1 >= 0) set_add(&next, col - 1);
                if (col + 1 < cols) set_add(&next, col + 1);
            } else {
                set_add(&next, col);
            }
        }
        IntSet tmp = current;
        current = next;
        next = tmp;
    }

    printf("%lld\n", splits);

    set_free(&current);
    set_free(&next);
    for (int i = 0; i < rows; i++) free(grid[i]);
    free(grid);
    return 0;
}
