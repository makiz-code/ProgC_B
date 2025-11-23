#ifndef BOOKS_H
#define BOOKS_H

#include <stddef.h>   // <-- added for size_t
#include "utils.h"

typedef struct {
    int id;
    char *title;
    int copies;
    int total_emprunts; // pour statistiques
} Book;

typedef struct {
    Book *arr;
    size_t size;
    size_t capacity;
} BookArray;

/* init / load / save */
void books_init(BookArray *b);
void books_free(BookArray *b);
int books_load(BookArray *b, const char *filepath);
int books_save(BookArray *b, const char *filepath);

/* opérations */
void books_list(BookArray *b);
Book *books_find_by_id(BookArray *b, int id);
Book *books_find_by_title(BookArray *b, const char *title);
int books_add(BookArray *b, int id, const char *title, int copies);
int books_remove(BookArray *b, int id); // retourne 0 si supprimé, -1 sinon
int books_modify_copies(BookArray *b, int id, int new_copies);

#endif

