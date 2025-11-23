#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "books.h"
#include "loans.h"
#include "stats.h"
#include "utils.h"

#define BOOKS_FILE "./data/books.csv"
#define LOANS_FILE "./data/loans.csv"

void ensure_data_folder(void){
    struct stat st = {0};
    if (stat("./data", &st) == -1) {
        if (mkdir("./data", 0700) != 0) {
            perror("Impossible de créer le dossier ./data");
            exit(EXIT_FAILURE);
        }
    }
}

void part1_menu(BookArray *books, LoanArray *loans){
    while(1){
        printf("\n--- PARTIE 1 : GESTION BIBLIOTHEQUE ---\n");
        printf("1. Afficher tous les livres\n2. Ajouter un livre\n3. Supprimer un livre (s'il n'est pas emprunté)\n4. Modifier nombre d'exemplaires\n5. Rechercher livre par titre ou numero\n6. Afficher livres disponibles\n0. Retour menu principal\nChoix: ");
        int choice;
        if (scanf("%d",&choice)!=1){ flush_stdin(); continue; }
        flush_stdin();
        if (choice==0) break;
        if (choice==1){
            books_list(books);
        } else if (choice==2){
            int id; char title[256]; int copies;
            printf("ID du livre: "); if (scanf("%d",&id)!=1){ flush_stdin(); continue; } flush_stdin();
            printf("Titre: "); fgets(title,sizeof(title),stdin); title[strcspn(title,"\n")]=0;
            printf("Nombre d'exemplaires: "); if (scanf("%d",&copies)!=1){ flush_stdin(); continue;} flush_stdin();
            if (books_add(books,id,title,copies)==0){
                books_save(books, BOOKS_FILE);
                printf("Livre ajouté.\n");
            } else printf("Livre avec cet ID existe déjà.\n");
        } else if (choice==3){
            int id; printf("ID à supprimer: "); if (scanf("%d",&id)!=1){ flush_stdin(); continue;} flush_stdin();
            if (is_book_currently_borrowed(loans,id)){
                printf("Impossible: le livre est actuellement emprunté.\n");
            } else if (books_remove(books,id)==0){
                books_save(books, BOOKS_FILE);
                printf("Supprimé.\n");
            } else printf("ID introuvable.\n");
        } else if (choice==4){
            int id,newc; printf("ID: "); if (scanf("%d",&id)!=1){ flush_stdin(); continue;} flush_stdin();
            printf("Nouveau nombre d'exemplaires: "); if (scanf("%d",&newc)!=1){ flush_stdin(); continue;} flush_stdin();
            if (books_modify_copies(books,id,newc)==0){
                books_save(books, BOOKS_FILE);
                printf("Modifié.\n");
            } else printf("ID introuvable.\n");
        } else if (choice==5){
            printf("Rechercher par (1) ID (2) Titre: ");
            int t; if (scanf("%d",&t)!=1){ flush_stdin(); continue;} flush_stdin();
            if (t==1){
                int id; printf("ID: "); if (scanf("%d",&id)!=1){ flush_stdin(); continue;} flush_stdin();
                Book *bk = books_find_by_id(books,id);
                if (bk) printf("%d %s %d copies\n", bk->id, bk->title, bk->copies); else printf("Introuvable\n");
            } else {
                char title[256]; printf("Titre exact (case insensitive): "); fgets(title,sizeof(title),stdin); title[strcspn(title,"\n")]=0;
                Book *bk = books_find_by_title(books,title);
                if (bk) printf("%d %s %d copies\n", bk->id, bk->title, bk->copies); else printf("Introuvable\n");
            }
        } else if (choice==6){
            printf("Livres disponibles pour emprunt:\n");
            printf("ID\tCopies\tTitre\n");
            for(size_t i=0;i<books->size;i++){
                if (books->arr[i].copies > 0 && !is_book_currently_borrowed(loans, books->arr[i].id)){
                    printf("%d\t%d\t%s\n", books->arr[i].id, books->arr[i].copies, books->arr[i].title);
                } else if (books->arr[i].copies > 0){
                    printf("%d\t%d\t%s\n", books->arr[i].id, books->arr[i].copies, books->arr[i].title);
                }
            }
        } else printf("Choix invalide.\n");
    }
}

void part2_menu(BookArray *books, LoanArray *loans){
    while(1){
        printf("\n--- PARTIE 2 : GESTION EMPRUNTS ---\n");
        printf("1. Ajouter un emprunt\n2. Consulter emprunts d'un adhérent\n3. Enregistrer retour d'un livre\n4. Afficher emprunts en cours\n5. Afficher livres en retard (date saisie)\n6. Afficher emprunts pour une date de retour prévue\n7. Supprimer emprunts dans une période\n0. Retour\nChoix: ");
        int ch; if (scanf("%d",&ch)!=1){ flush_stdin(); continue;} flush_stdin();
        if (ch==0) break;
        if (ch==1){
            long long cin; int book_id;
            printf("CIN adhérent: "); if (scanf("%lld",&cin)!=1){ flush_stdin(); continue;} flush_stdin();
            if (loans_count_active_by_cin(loans,cin) >= 7){ printf("Cet adhérent a déjà 7 livres empruntés.\n"); continue; }
            printf("ID du livre à emprunter: "); if (scanf("%d",&book_id)!=1){ flush_stdin(); continue;} flush_stdin();
            Book *bk = books_find_by_id(books, book_id);
            if (!bk){ printf("Livre introuvable.\n"); continue; }
            if (bk->copies <= 0 && is_book_currently_borrowed(loans, book_id)){
                Date nearest = {9999,12,31};
                int found = 0;
                for(size_t i=0;i<loans->size;i++){
                    if (loans->arr[i].book_id==book_id && loans->arr[i].returned==0){
                        if (compare_date(loans->arr[i].expected_return, nearest) < 0){
                            nearest = loans->arr[i].expected_return; found=1;
                        }
                    }
                }
                if (found){
                    printf("Livre non disponible. Prochain retour prévu le %02d/%02d/%04d\n",
                        nearest.day, nearest.month, nearest.year);
                } else printf("Livre non disponible pour l'instant.\n");
                continue;
            }
            Date borrow = read_date_from_user("Date d'emprunt");
            Date expected = read_date_from_user("Date de retour prévue (<=31 jours)");
            long int days1 = borrow.year*365 + borrow.month*31 + borrow.day;
            long int days2 = expected.year*365 + expected.month*31 + expected.day;
            if (days2 < days1 || (days2 - days1) > 31){
                printf("Période invalide (doit être >=0 et <=31 jours).\n");
                continue;
            }
            loans_add(loans, cin, book_id, borrow, expected);
            bk->total_emprunts++;
            if (bk->copies > 0) bk->copies--;
            loans_save(loans, LOANS_FILE);
            books_save(books, BOOKS_FILE);
            printf("Emprunt ajouté.\n");
        } else if (ch==2){
            long long cin; printf("CIN: "); if (scanf("%lld",&cin)!=1){ flush_stdin(); continue;} flush_stdin();
            loans_list_by_cin(loans, cin);
        } else if (ch==3){
            long long cin; int book_id;
            printf("CIN: "); if (scanf("%lld",&cin)!=1){ flush_stdin(); continue;} flush_stdin();
            printf("ID livre: "); if (scanf("%d",&book_id)!=1){ flush_stdin(); continue;} flush_stdin();
            Date actual = read_date_from_user("Date de retour réelle");
            if (loans_mark_returned(loans, cin, book_id, actual) == 0){
                Book *bk = books_find_by_id(books,book_id);
                if (bk) bk->copies++;
                loans_save(loans, LOANS_FILE);
                books_save(books, BOOKS_FILE);
                printf("Retour enregistré.\n");
            } else printf("Emprunt introuvable.\n");
        } else if (ch==4){
            loans_list_active(loans);
        } else if (ch==5){
            Date d = read_date_from_user("Date courante pour vérifier retards");
            loans_list_due_on(loans, d);
        } else if (ch==6){
            Date d = read_date_from_user("Date de retour prévue");
            loans_list_expected_on(loans, d);
        } else if (ch==7){
            printf("Saisir date de début de période:\n");
            Date s = read_date_from_user("Date début");
            Date e = read_date_from_user("Date fin");
            printf("Emprunts avant suppression:\n");
            loans_list_active(loans);
            loans_remove_by_period(loans, s, e);
            loans_save(loans, LOANS_FILE);
            printf("Emprunts après suppression:\n");
            loans_list_active(loans);
        } else printf("Choix invalide.\n");
    }
}

void part3_menu(BookArray *books, LoanArray *loans){
    while(1){
        printf("\n--- PARTIE 3 : STATISTIQUES ---\n");
        printf("1. Compter total livres, adhérents, emprunts\n2. Livre(s) le(s) plus emprunté(s)\n3. Livre(s) jamais emprunté(s)\n4. Livre(s) le(s) plus emprunté(s) pour une période\n5. Emprunteur(s) les plus fréquents\n0. Retour\nChoix: ");
        int ch; if (scanf("%d",&ch)!=1){ flush_stdin(); continue;} flush_stdin();
        if (ch==0) break;
        if (ch==1) stats_counts(books, loans);
        else if (ch==2) stats_most_borrowed(books);
        else if (ch==3) stats_never_borrowed(books);
        else if (ch==4){
            Date s = read_date_from_user("Date début");
            Date e = read_date_from_user("Date fin");
            stats_most_borrowed_in_period(books, loans, s, e);
        } else if (ch==5) stats_most_frequent_borrowers(loans);
        else printf("Choix invalide.\n");
    }
}

int main(void){
    ensure_data_folder();

    BookArray books;
    LoanArray loans;
    books_init(&books);
    loans_init(&loans);

    if (books_load(&books, BOOKS_FILE) != 0) {
        printf("Aucun fichier livres trouvé (data/books.csv). Démarrage avec bibliothèque vide.\n");
    }
    if (loans_load(&loans, LOANS_FILE) != 0) {
        printf("Aucun fichier emprunts trouvé (data/loans.csv). Démarrage vide.\n");
    }

    while(1){
        printf("\n==== MENU PRINCIPAL ====\n1. Gestion bibliothèque\n2. Gestion emprunts\n3. Statistiques\n0. Quitter\nChoix: ");
        int choice;
        if (scanf("%d",&choice)!=1){ flush_stdin(); continue; }
        flush_stdin();
        if (choice==0) break;
        else if (choice==1) part1_menu(&books, &loans);
        else if (choice==2) part2_menu(&books, &loans);
        else if (choice==3) part3_menu(&books, &loans);
        else printf("Choix invalide.\n");
    }

    books_save(&books, BOOKS_FILE);
    loans_save(&loans, LOANS_FILE);

    books_free(&books);
    loans_free(&loans);
    printf("Au revoir !\n");
    return 0;
}

