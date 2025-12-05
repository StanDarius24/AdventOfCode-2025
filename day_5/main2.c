
// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { long long start, end; } Interval;

static int cmp_interval(const void *a, const void *b) {
    const Interval *ia = (const Interval*)a, *ib = (const Interval*)b;
    if (ia->start < ib->start) return -1;
    if (ia->start > ib->start) return 1;
    if (ia->end < ib->end) return -1;
    if (ia->end > ib->end) return 1;
    return 0;
}

int main(void) {
    FILE *file = fopen("/Users/dariusgeorgestan/Desktop/adventOfCode/day_5/input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    Interval *ranges = NULL;
    int ranges_cap = 0, ranges_len = 0;

    char buf[256];

    while (fgets(buf, sizeof(buf), file)) {
        int len = (int)strcspn(buf, "\r\n");
        buf[len] = '\0';
        if (len == 0) break;

        char *dash = strchr(buf, '-');
        if (!dash) continue;
        *dash = '\0';
        char *a_str = buf;
        char *b_str = dash + 1;
        long long a = strtoll(a_str, NULL, 10);
        long long b = strtoll(b_str, NULL, 10);
        if (a > b) { long long t = a; a = b; b = t; }

        if (ranges_len == ranges_cap) {
            ranges_cap = ranges_cap ? ranges_cap * 2 : 64;
            ranges = (Interval*)realloc(ranges, ranges_cap * sizeof(Interval));
            if (!ranges) { fclose(file); fprintf(stderr, "OOM\n"); return 1; }
        }
        ranges[ranges_len++] = (Interval){a, b};
    }
    fclose(file);

    if (ranges_len == 0) {
        printf("0\n");
        free(ranges);
        return 0;
    }

    qsort(ranges, ranges_len, sizeof(Interval), cmp_interval);

    Interval *merged = (Interval*)malloc(ranges_len * sizeof(Interval));
    if (!merged) { fprintf(stderr, "OOM\n"); free(ranges); return 1; }

    int m = 0;
    Interval cur = ranges[0];
    for (int i = 1; i < ranges_len; ++i) {
        if (ranges[i].start <= cur.end + 1) {
            if (ranges[i].end > cur.end) cur.end = ranges[i].end;
        } else {
            merged[m++] = cur;
            cur = ranges[i];
        }
    }
    merged[m++] = cur;

    unsigned long long total_fresh = 0ULL;
    for (int i = 0; i < m; ++i) {
        total_fresh += (unsigned long long)(merged[i].end - merged[i].start + 1);
    }

    printf("%llu\n", total_fresh);

    free(ranges);
    free(merged);
    return 0;
}
