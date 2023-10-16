// bankfunctions.h

#ifndef BANKFUNCTIONS_H
#define BANKFUNCTIONS_H

typedef struct User {
    char username[32];
    char password[40];
    float solde;
    int id;
} User;

// Déclarez vos fonctions
int verifyIDGeneration(User *user);
int FoundLine(User *user, const char *username, const char *password);
int checkID(User *user, int *id);
int generateID(User *user, int *id);
int checkInfos(User *user, const char *username, const char *password);
int creerCompte(User *user);
int seConnecter(User *user);
int consultSolde(User *user, const char *username, const char *password, int *id);
int ajouterSolde(User *user, float montant);
int retirerSolde(User *user, float montant);
int closeAccount(User *user, const char *username, const char *password);
int editAccount(User *user);


#endif
