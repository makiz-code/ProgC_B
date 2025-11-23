#ifndef STATS_H
#define STATS_H

#include "books.h"
#include "loans.h"

/* fonctions statistiques */
void stats_counts(BookArray *b, LoanArray *l);
void stats_most_borrowed(BookArray *b);
void stats_never_borrowed(BookArray *b);
void stats_most_borrowed_in_period(BookArray *b, LoanArray *l, Date start, Date end);
void stats_most_frequent_borrowers(LoanArray *l);

#endif

