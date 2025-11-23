#ifndef LOANS_H
#define LOANS_H

#include "utils.h"
#include "books.h"
#include <stddef.h>

typedef struct {
    long long cin;
    int book_id;
    Date borrow;
    Date expected_return;
    int returned; // 0 = en cours, 1 = rendu
} Loan;

typedef struct {
    Loan *arr;
    size_t size;
    size_t capacity;
} LoanArray;

/* init/load/save */
void loans_init(LoanArray *l);
void loans_free(LoanArray *l);
int loans_load(LoanArray *l, const char *filepath);
int loans_save(LoanArray *l, const char *filepath);

/* opérations */
int loans_count_active_by_cin(LoanArray *l, long long cin);
int loans_add(LoanArray *l, long long cin, int book_id, Date borrow, Date ret);
Loan *loans_find_active(LoanArray *l, long long cin, int book_id);
int loans_mark_returned(LoanArray *l, long long cin, int book_id, Date actual_return);
void loans_list_active(LoanArray *l);
void loans_list_by_cin(LoanArray *l, long long cin);
void loans_list_due_on(LoanArray *l, Date d);
void loans_list_expected_on(LoanArray *l, Date d);
void loans_remove_by_period(LoanArray *l, Date start, Date end);
int is_book_currently_borrowed(LoanArray *l, int book_id);
int count_unique_adhérents(LoanArray *l);

#endif

