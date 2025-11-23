#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stats_counts(BookArray *b, LoanArray *l){
    int total_books = (int)b->size;
    int total_adh = count_unique_adhérents(l);
    int total_emprunts = (int)l->size;
    printf("Total livres: %d\nTotal adhérents (unique): %d\nTotal emprunts (historique): %d\n",
        total_books, total_adh, total_emprunts);
}

void stats_most_borrowed(BookArray *b){
    if (b->size==0) { printf("Aucun livre.\n"); return; }
    int max = -1;
    for(size_t i=0;i<b->size;i++) if (b->arr[i].total_emprunts > max) max = b->arr[i].total_emprunts;
    printf("Livre(s) le(s) plus emprunté(s) (%d emprunts) :\n", max);
    for(size_t i=0;i<b->size;i++){
        if (b->arr[i].total_emprunts == max){
            printf("%d : %s\n", b->arr[i].id, b->arr[i].title);
        }
    }
}

void stats_never_borrowed(BookArray *b){
    printf("Livre(s) jamais emprunté(s) :\n");
    int found=0;
    for(size_t i=0;i<b->size;i++){
        if (b->arr[i].total_emprunts == 0){
            printf("%d : %s\n", b->arr[i].id, b->arr[i].title);
            found=1;
        }
    }
    if (!found) printf("Aucun.\n");
}

void stats_most_borrowed_in_period(BookArray *b, LoanArray *l, Date start, Date end){
    int *counts = calloc(b->size, sizeof(int));
    for(size_t i=0;i<l->size;i++){
        Loan *ln = &l->arr[i];
        if (compare_date(ln->borrow, start) >= 0 && compare_date(ln->borrow, end) <= 0){
            for(size_t j=0;j<b->size;j++){
                if (b->arr[j].id == ln->book_id){ counts[j]++; break; }
            }
        }
    }
    int max = 0;
    for(size_t j=0;j<b->size;j++) if (counts[j] > max) max = counts[j];
    printf("Livre(s) le(s) plus emprunté(s) entre %02d/%02d/%04d et %02d/%02d/%04d (max=%d):\n",
        start.day,start.month,start.year, end.day,end.month,end.year, max);
    for(size_t j=0;j<b->size;j++) if (counts[j] == max) printf("%d : %s\n", b->arr[j].id, b->arr[j].title);
    free(counts);
}

void stats_most_frequent_borrowers(LoanArray *l){
    if (l->size==0){ printf("Aucun emprunt.\n"); return; }

    int best_count = 0;

    // find max count
    for(size_t i=0;i<l->size;i++){
        long long cin = l->arr[i].cin;
        int cnt = 0;
        for(size_t j=0;j<l->size;j++) if (l->arr[j].cin == cin) cnt++;
        if (cnt > best_count) best_count = cnt;
    }

    printf("Emprunteur(s) le(s) plus fréquent(s) (%d emprunts) :\n", best_count);

    // print unique CINs with max count
    for(size_t i=0;i<l->size;i++){
        long long cin = l->arr[i].cin;
        int cnt=0;
        for(size_t j=0;j<l->size;j++) if (l->arr[j].cin == cin) cnt++;
        if (cnt == best_count){
            // check if this CIN was already printed
            int already_printed = 0;
            for(size_t k=0;k<i;k++) if (l->arr[k].cin == cin) { already_printed = 1; break; }
            if (!already_printed) printf("%lld\n", cin);
        }
    }
}

