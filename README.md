# Projet : Gestion des prêts de livres (C)
## Description
Application console en C pour gérer les livres, les emprunts et les statistiques d'une bibliothèque.
Implémentation conforme à l'énoncé fourni. (Référence : énoncé projet ASD1 2025-2026). :contentReference[oaicite:1]{index=1}

## Fonctionnalités
- Partie 1 — Gestion de la bibliothèque :
  - Afficher tous les livres.
  - Ajouter un livre.
  - Supprimer (si non emprunté).
  - Modifier nombre d'exemplaires.
  - Rechercher par titre ou numéro.
  - Afficher livres disponibles.

- Partie 2 — Gestion des emprunts :
  - Ajouter un emprunt (max 7 livres par adhérent, durée max 31 jours).
  - Consulter emprunts d'un adhérent.
  - Enregistrer retour d'un livre.
  - Afficher emprunts en cours.
  - Afficher livres en retard à une date donnée.
  - Afficher emprunts pour une date de retour prévue.
  - Supprimer emprunts appartenant à une période.
  
- Partie 3 — Statistiques :
  - Nombre total de livres, adhérents, emprunts.
  - Livre(s) le(s) plus emprunté(s).
  - Livre(s) jamais emprunté(s).
  - Livre(s) les plus empruntés pour une période.
  - Emprunteur(s) les plus fréquents.

## Format des fichiers de données
- `data/books.csv` : `id;title;copies;total_emprunts`
- `data/loans.csv` : `cin;book_id;bj;bm;by;rj;rm;ry;returned`  
  (dates : jour/mois/année, `returned`: 0 ou 1)

Ces fichiers sont lus au démarrage et sauvegardés à chaque modification.

## Compilation & exécution
Assure-toi d'avoir un compilateur C (gcc).


