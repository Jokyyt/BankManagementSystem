#ifndef BANKFUNCTIONS_H
#define BANKFUNCTIONS_H

typedef struct User {
    char *username;
    char *password;
    double solde;
    int id;
} User;

// Déclarez vos fonctions
int creerCompte(User *user);
int seConnecter(User *user);
int consultSolde(User *user, const char *username, const char *password);
int ajouterSolde(User *user, float montant);
int retirerSolde(User *user, float montant);
int closeAccount(User *user, const char *username, const char *password);
int editAccount(User *user);

#endif
