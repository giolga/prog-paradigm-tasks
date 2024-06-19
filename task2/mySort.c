#include "samples/prototypes.h"
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

// Compare two strings lexicographically.

int cmp_pstr(const void *p, const void *q) {
    return strcmp(*(char **)p, *(char **)q);
}

//  Compare two strings by length.

int cmp_pstr_len(const void *p, const void *q) {
    size_t ip = strlen(*(char **)p);
    size_t iq = strlen(*(char **)q);

    if (ip > iq)
        return 1;

    if (ip < iq)
        return -1;

    return cmp_pstr(p, q);
}

//  Compare two strings numerically.

int cmp_pstr_numeric(const void *p, const void *q) {
    int ip = atoi(*(char **)p);
    int iq = atoi(*(char **)q);

    if (ip > iq)
        return 1;

    if (ip < iq)
        return -1;

    return 0;
}

// Reverse an array of strings.

void reverse_lines(char **lines, size_t nlines) {
    int i = nlines - 1;
    int j = 0;

    while (i > j) {
        char *temp = lines[i];
        lines[i] = lines[j];
        lines[j] = temp;
        i--;
        j++;
    }

}

// Print an array of strings.

void print_lines(char **lines, size_t nlines) {
    for (int i = 0; i < nlines; i++) {
        printf("%s", lines[i]);
    }

}

//  Free an array of strings.

void free_lines(char **lines, size_t nlines) {
    for (int i = 0; i < nlines; i++) {
        free(lines[i]);
    }

    free(lines);
}

//  Sort and print lines from a file.

void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    char line[MAX_LINE_LEN];
    size_t maxlines = MIN_NLINES;
    char **lines = malloc(maxlines * sizeof(char *));
    size_t nlines = 0;

    // Read lines from file
    while (fgets(line, MAX_LINE_LEN, fp)!= NULL) {
        lines[nlines] = strdup(line);
        nlines++;
        
        if (nlines == maxlines) {
            maxlines *= 2;
            lines = realloc(lines, maxlines * sizeof(char *));
        }
    }

    // Sort lines
    if (uniq) {
        char **lines_uniq = malloc(nlines * sizeof(char *));
        size_t nlines_uniq = 0;

        for (int i = 0; i < nlines; i++) {
            char **added = binsert(&lines[i], lines_uniq, &nlines_uniq, sizeof(char *), cmp);
            assert(strcmp(*added, lines[i]) == 0);
        }

        if (reverse) {
            reverse_lines(lines_uniq, nlines_uniq);
        }

        print_lines(lines_uniq, nlines_uniq);
        free_lines(lines_uniq, nlines_uniq);
    } 
    else {
        qsort(lines, nlines, sizeof(char *), cmp);
        if (reverse) {
            reverse_lines(lines, nlines);
        }

        print_lines(lines, nlines);
    }

    free_lines(lines, nlines);

}

int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false, reverse = false;

    int opt;
    while ((opt = getopt(argc, argv, "lnru"))!= -1) {
        switch (opt) {
            case 'l': cmp = cmp_pstr_len;
                break;
            case 'n': cmp = cmp_pstr_numeric; 
                break;
            case 'r': reverse = true; 
                break;
            case 'u': uniq = true; 
                break;
            default: exit(1);
                break; //
        }
    }

    FILE *fp = stdin;

    if (optind < argc) {
        fp = fopen(argv[optind], "r");

        if (fp == NULL) {
            error(1, 0, "%s: no such file", argv[optind]);
        }
    }

    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);

    return 0;
}