#define _GNU_SOURCE
#include "books.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ensure_capacity(BookArray *b){
    if (b->capacity == 0) {
        b->capacity = 64;
        b->arr = malloc(sizeof(Book)*b->capacity);
    } else if (b->size >= b->capacity){
        b->capacity *= 2;
        b->arr = realloc(b->arr, sizeof(Book)*b->capacity);
    }
}

void books_init(BookArray *b){
    b->arr = NULL;
    b->size = 0;
    b->capacity = 0;
}

void books_free(BookArray *b){
    for(size_t i=0; i<b->size; i++){
        free(b->arr[i].title);
    }
    free(b->arr);
    b->arr = NULL;
    b->size = b->capacity = 0;
}

int books_load(BookArray *b, const char *filepath){
    FILE *f = fopen(filepath,"r");
    if (!f) return -1;
    char line[1024];
    while (fgets(line,sizeof(line),f)){
        int id=0, copies=0, totale=0;
        char *s = line;
        char *tok = strtok(s,";\n");
        if (!tok) continue;
        id = atoi(tok);
        tok = strtok(NULL,";\n");
        if (!tok) continue;
        char *title = strdup_safe(tok);
        tok = strtok(NULL,";\n");
        copies = tok ? atoi(tok) : 0;
        tok = strtok(NULL,";\n");
        totale = tok ? atoi(tok) : 0;
        ensure_capacity(b);
        b->arr[b->size].id = id;
        b->arr[b->size].title = title;
        b->arr[b->size].copies = copies;
        b->arr[b->size].total_emprunts = totale;
        b->size++;
    }
    fclose(f);
    return 0;
}

int books_save(BookArray *b, const char *filepath){
    FILE *f = fopen(filepath,"w");
    if (!f) return -1;
    for(size_t i=0; i<b->size; i++){
        fprintf(f, "%d;%s;%d;%d\n",
            b->arr[i].id,
            b->arr[i].title ? b->arr[i].title : "",
            b->arr[i].copies,
            b->arr[i].total_emprunts);
    }
    fclose(f);
    return 0;
}

void books_list(BookArray *b){
    printf("ID\tCopies\tEmprunts\tTitre\n");
    printf("----------------------------------------------------------\n");
    for(size_t i=0; i<b->size; i++){
        printf("%d\t%d\t%d\t\t%s\n",
            b->arr[i].id,
            b->arr[i].copies,
            b->arr[i].total_emprunts,
            b->arr[i].title);
    }
}

Book *books_find_by_id(BookArray *b, int id){
    for(size_t i=0; i<b->size; i++) if (b->arr[i].id == id) return &b->arr[i];
    return NULL;
}

Book *books_find_by_title(BookArray *b, const char *title){
    for(size_t i=0; i<b->size; i++){
        if (strcasecmp(b->arr[i].title, title) == 0) return &b->arr[i];
    }
    return NULL;
}

int books_add(BookArray *b, int id, const char *title, int copies){
    if (books_find_by_id(b,id)) return -1; // existe
    ensure_capacity(b);
    b->arr[b->size].id = id;
    b->arr[b->size].title = strdup_safe(title);
    b->arr[b->size].copies = copies;
    b->arr[b->size].total_emprunts = 0;
    b->size++;
    return 0;
}

int books_remove(BookArray *b, int id){
    size_t i;
    for(i=0; i<b->size; i++) if (b->arr[i].id == id) break;
    if (i==b->size) return -1;
    free(b->arr[i].title);
    for(size_t j=i; j+1<b->size; j++) b->arr[j] = b->arr[j+1];
    b->size--;
    return 0;
}

int books_modify_copies(BookArray *b, int id, int new_copies){
    Book *bk = books_find_by_id(b,id);
    if (!bk) return -1;
    bk->copies = new_copies;
    return 0;
}

