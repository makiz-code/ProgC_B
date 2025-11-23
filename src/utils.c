#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void flush_stdin(void){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int is_leap(int y){ return (y%4==0 && y%100!=0) || (y%400==0); }

bool valid_date(int d, int m, int y){
    if (y < 1900 || m < 1 || m > 12 || d < 1) return false;
    int mdays[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (is_leap(y)) mdays[2]=29;
    return d <= mdays[m];
}

int compare_date(Date a, Date b){
    if (a.year != b.year) return (a.year < b.year) ? -1 : 1;
    if (a.month != b.month) return (a.month < b.month) ? -1 : 1;
    if (a.day != b.day) return (a.day < b.day) ? -1 : (a.day > b.day) ? 1 : 0;
    return 0;
}

Date read_date_from_user(const char *prompt){
    Date dt = {0,0,0};
    while (1) {
        printf("%s (jj mm aaaa) : ", prompt);
        if (scanf("%d %d %d", &dt.day, &dt.month, &dt.year) != 3) {
            printf("Saisie invalide. Recommencez.\n");
            flush_stdin();
            continue;
        }
        flush_stdin();
        if (!valid_date(dt.day, dt.month, dt.year)) {
            printf("Date invalide. Recommencez.\n");
            continue;
        }
        break;
    }
    return dt;
}

char *strdup_safe(const char *s){
    if (!s) return NULL;
    size_t n = strlen(s)+1;
    char *r = malloc(n);
    if (!r) return NULL;
    memcpy(r,s,n);
    return r;
}

