//
// Created by Darius-George Stan on 05.12.2025.
//

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

static int is_fresh(long long id, const Interval *merged, int m) {
    int lo = 0, hi = m - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (id < merged[mid].start) {
            hi = mid - 1;
        } else if (id > merged[mid].end) {
            lo = mid + 1;
        } else {
            return 1;
        }
    }
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

    long long *ids = NULL;
    int ids_cap = 0, ids_len = 0;

    char buf[256];
    int reading_ranges = 1;

    while (fgets(buf, sizeof(buf), file)) {

        int len = (int)strcspn(buf, "\r\n");
        buf[len] = '\0';

        if (len == 0) {
            reading_ranges = 0;
            continue;
        }

        if (reading_ranges) {

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
        } else {
            char *endp = NULL;
            long long id = strtoll(buf, &endp, 10);
            if (endp == buf) continue;
            if (ids_len == ids_cap) {
                ids_cap = ids_cap ? ids_cap * 2 : 128;
                ids = (long long*)realloc(ids, ids_cap * sizeof(long long));
                if (!ids) { fclose(file); fprintf(stderr, "OOM\n"); free(ranges); return 1; }
            }
            ids[ids_len++] = id;
        }
    }
    fclose(file);

    if (ranges_len == 0 || ids_len == 0) {
        printf("0\n");
        free(ranges);
        free(ids);
        return 0;
    }

    qsort(ranges, ranges_len, sizeof(Interval), cmp_interval);
    Interval *merged = (Interval*)malloc(ranges_len * sizeof(Interval));
    if (!merged) { fprintf(stderr, "OOM\n"); free(ranges); free(ids); return 1; }

    int m = 0;
    Interval cur = ranges[0];
    for (int i = 1; i < ranges_len; ++i) {
        if (ranges[i].start <= cur.end + 0) { // overlapping or contiguous
            if (ranges[i].end > cur.end) cur.end = ranges[i].end;
        } else {
            merged[m++] = cur;
            cur = ranges[i];
        }
    }
    merged[m++] = cur;

    long long fresh = 0;
    for (int i = 0; i < ids_len; ++i) {
        if (is_fresh(ids[i], merged, m)) fresh++;
    }

    printf("%lld\n", fresh);

    free(ranges);
    free(ids);
    free(merged);
    return 0;
}
