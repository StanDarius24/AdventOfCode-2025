#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 5000
#define MAX_LINE_LEN 10000

static char *lines[MAX_LINES];
static int line_count = 0;
static int width = 0;

static void rpad(char *s, int target) {
    int len = (int)strlen(s);
    if (len >= target) return;
    memset(s + len, ' ', target - len);
    s[target] = '\0';
}

int main(void) {
    const char *path = "/Users/dariusgeorgestan/Desktop/adventOfCode/day_6/input.txt";
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Failed to open %s\n", path); return 1; }

    char buf[MAX_LINE_LEN + 4];
    while (fgets(buf, sizeof(buf), f)) {
        size_t n = strlen(buf);
        while (n && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) buf[--n] = '\0';
        char *dup = malloc(n + 1);
        if (!dup) { fprintf(stderr, "OOM\n"); return 1; }
        memcpy(dup, buf, n + 1);
        lines[line_count++] = dup;
        if ((int)n > width) width = (int)n;
        if (line_count >= MAX_LINES) { fprintf(stderr, "Too many lines\n"); return 1; }
    }
    fclose(f);
    if (line_count == 0) { fprintf(stderr, "Empty input\n"); return 1; }

    for (int i = 0; i < line_count; i++) {
        lines[i] = realloc(lines[i], width + 1);
        if (!lines[i]) { fprintf(stderr, "OOM\n"); return 1; }
        rpad(lines[i], width);
    }

    char *sep = calloc(width, 1);
    for (int col = 0; col < width; col++) {
        int all_space = 1;
        for (int r = 0; r < line_count; r++)
            if (lines[r][col] != ' ') { all_space = 0; break; }
        sep[col] = all_space ? 1 : 0;
    }

    typedef struct { int start, end; } Segment;
    Segment *segs = malloc(sizeof(Segment) * (width + 1));
    int seg_count = 0, in = 0, s = 0;
    for (int col = 0; col < width; col++) {
        if (!in && !sep[col]) { in = 1; s = col; }
        if (in && (sep[col] || col == width - 1)) {
            int e = sep[col] ? col - 1 : col;
            if (e >= s) segs[seg_count++] = (Segment){ s, e };
            in = 0;
        }
    }

    const char *opline = lines[line_count - 1];
    __int128 grand = 0;

    for (int si = 0; si < seg_count; si++) {
        int start = segs[si].start, end = segs[si].end;

        char op = 0;
        for (int c = start; c <= end; c++)
            if (opline[c] == '+' || opline[c] == '*') { op = opline[c]; break; }
        if (op == 0) continue;

        __int128 acc = (op == '+') ? 0 : 1;
        int have_any = 0;

        for (int col = end; col >= start; col--) {
            long long num = 0;
            int has_digit = 0;
            for (int r = 0; r < line_count - 1; r++) {
                char ch = lines[r][col];
                if (isdigit((unsigned char)ch)) {
                    num = num * 10 + (ch - '0');
                    has_digit = 1;
                }
            }
            if (has_digit) {
                have_any = 1;
                if (op == '+') acc += num;
                else acc *= num;
            }
        }
        if (have_any) grand += acc;
    }

    char out[128];
    int pos = sizeof(out) - 1;
    out[pos] = '\0';
    __int128 val = grand < 0 ? -grand : grand;
    if (grand == 0) { printf("0\n"); }
    else {
        while (val > 0) { out[--pos] = '0' + (int)(val % 10); val /= 10; }
        if (grand < 0) out[--pos] = '-';
        printf("%s\n", &out[pos]);
    }

    for (int i = 0; i < line_count; i++) free(lines[i]);
    free(sep); free(segs);
    return 0;
}
