#include "loans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ensure_capacity_loans(LoanArray *l){
    if (l->capacity == 0) {
        l->capacity = 256;
        l->arr = malloc(sizeof(Loan)*l->capacity);
    } else if (l->size >= l->capacity){
        l->capacity *= 2;
        l->arr = realloc(l->arr, sizeof(Loan)*l->capacity);
    }
}

void loans_init(LoanArray *l){
    l->arr = NULL;
    l->size = l->capacity = 0;
}

void loans_free(LoanArray *l){
    free(l->arr);
    l->arr = NULL;
    l->size = l->capacity = 0;
}

int loans_load(LoanArray *l, const char *filepath){
    FILE *f = fopen(filepath,"r");
    if (!f) return -1;
    char line[512];
    while (fgets(line,sizeof(line),f)){
        // cin;book_id;bj;bm;by;rj;rm;ry;returned
        long long cin=0; int bid=0;
        int bj,bm,by,rj,rm,ry,returned;
        if (sscanf(line,"%lld;%d;%d;%d;%d;%d;%d;%d;%d",
            &cin,&bid,&bj,&bm,&by,&rj,&rm,&ry,&returned) >= 9) {
            ensure_capacity_loans(l);
            l->arr[l->size].cin = cin;
            l->arr[l->size].book_id = bid;
            l->arr[l->size].borrow.day = bj;
            l->arr[l->size].borrow.month = bm;
            l->arr[l->size].borrow.year = by;
            l->arr[l->size].expected_return.day = rj;
            l->arr[l->size].expected_return.month = rm;
            l->arr[l->size].expected_return.year = ry;
            l->arr[l->size].returned = returned;
            l->size++;
        }
    }
    fclose(f);
    return 0;
}

int loans_save(LoanArray *l, const char *filepath){
    FILE *f = fopen(filepath,"w");
    if (!f) return -1;
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        fprintf(f, "%lld;%d;%d;%d;%d;%d;%d;%d;%d\n",
            ln->cin, ln->book_id,
            ln->borrow.day, ln->borrow.month, ln->borrow.year,
            ln->expected_return.day, ln->expected_return.month, ln->expected_return.year,
            ln->returned);
    }
    fclose(f);
    return 0;
}

int loans_count_active_by_cin(LoanArray *l, long long cin){
    int cnt=0;
    for(size_t i=0;i<l->size;i++){
        if (l->arr[i].cin==cin && l->arr[i].returned==0) cnt++;
    }
    return cnt;
}

int loans_add(LoanArray *l, long long cin, int book_id, Date borrow, Date ret){
    ensure_capacity_loans(l);
    l->arr[l->size].cin = cin;
    l->arr[l->size].book_id = book_id;
    l->arr[l->size].borrow = borrow;
    l->arr[l->size].expected_return = ret;
    l->arr[l->size].returned = 0;
    l->size++;
    return 0;
}

Loan *loans_find_active(LoanArray *l, long long cin, int book_id){
    for(size_t i=0;i<l->size;i++){
        if (l->arr[i].cin==cin && l->arr[i].book_id==book_id && l->arr[i].returned==0)
            return &l->arr[i];
    }
    return NULL;
}

int loans_mark_returned(LoanArray *l, long long cin, int book_id, Date actual_return){
    Loan *ln = loans_find_active(l,cin,book_id);
    if (!ln) return -1;
    ln->returned = 1;
    (void)actual_return; // on garde date réelle si besoin
    return 0;
}

void loans_list_active(LoanArray *l){
    printf("CIN\tBookID\tBorrow\tExpectedReturn\n");
    printf("-----------------------------------------------------\n");
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (ln->returned==0){
            printf("%lld\t%d\t%02d/%02d/%04d\t%02d/%02d/%04d\n",
                ln->cin, ln->book_id,
                ln->borrow.day, ln->borrow.month, ln->borrow.year,
                ln->expected_return.day, ln->expected_return.month, ln->expected_return.year);
        }
    }
}

void loans_list_by_cin(LoanArray *l, long long cin){
    printf("Emprunts pour CIN: %lld\n", cin);
    printf("BookID\tBorrow\tExpectedReturn\tStatut\n");
    printf("-----------------------------------------------------\n");
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (ln->cin==cin){
            printf("%d\t%02d/%02d/%04d\t%02d/%02d/%04d\t%s\n",
                ln->book_id,
                ln->borrow.day, ln->borrow.month, ln->borrow.year,
                ln->expected_return.day, ln->expected_return.month, ln->expected_return.year,
                ln->returned ? "Rendu" : "En cours");
        }
    }
}

void loans_list_due_on(LoanArray *l, Date d){
    printf("Emprunts en retard par rapport au %02d/%02d/%04d\n", d.day, d.month, d.year);
    printf("CIN\tBookID\tExpectedReturn\n");
    printf("-----------------------------------------------------\n");
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (ln->returned==0 && compare_date(ln->expected_return, d) < 0){
            printf("%lld\t%d\t%02d/%02d/%04d\n",
                ln->cin, ln->book_id,
                ln->expected_return.day, ln->expected_return.month, ln->expected_return.year);
        }
    }
}

void loans_list_expected_on(LoanArray *l, Date d){
    printf("Emprunts avec date de retour prévue le %02d/%02d/%04d\n", d.day, d.month, d.year);
    printf("CIN\tBookID\tBorrow\n");
    printf("-----------------------------------------------------\n");
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (!ln->returned && compare_date(ln->expected_return, d) == 0){
            printf("%lld\t%d\t%02d/%02d/%04d\n",
                ln->cin, ln->book_id,
                ln->borrow.day, ln->borrow.month, ln->borrow.year);
        }
    }
}

void loans_remove_by_period(LoanArray *l, Date start, Date end){
    printf("Avant suppression, emprunts totaux: %zu\n", l->size);
    size_t write = 0;
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (compare_date(ln->borrow, start) >= 0 && compare_date(ln->expected_return, end) <= 0){
            // supprimer: skip
            continue;
        }
        if (write != i) l->arr[write] = l->arr[i];
        write++;
    }
    l->size = write;
    printf("Après suppression, emprunts totaux: %zu\n", l->size);
}

int is_book_currently_borrowed(LoanArray *l, int book_id){
    for(size_t i=0;i<l->size;i++){
        if (l->arr[i].book_id == book_id && l->arr[i].returned==0) return 1;
    }
    return 0;
}

int count_unique_adhérents(LoanArray *l){
    // simple n^2: acceptable
    int count = 0;
    for(size_t i=0;i<l->size;i++){
        long long cin = l->arr[i].cin;
        int seen = 0;
        for(size_t j=0;j<i;j++) if (l->arr[j].cin == cin) { seen = 1; break; }
        if (!seen) count++;
    }
    return count;
}

