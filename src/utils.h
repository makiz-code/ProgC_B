#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

typedef struct {
    int day;
    int month;
    int year;
} Date;

bool valid_date(int d, int m, int y);
int compare_date(Date a, Date b); // -1 if a<b, 0 equal, 1 if a>b
Date read_date_from_user(const char *prompt);
void flush_stdin(void);
char *strdup_safe(const char *s);

#endif

