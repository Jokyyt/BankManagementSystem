#ifndef BANKFUNCTIONS_H
#define BANKFUNCTIONS_H

typedef struct User {
    char *username;
    char *password;
} User;

// Déclarez vos fonctions
int creerCompte(User *user);
int checkInfos(User *user, const char *username, const char *password);
int Login(User *user);

#endif
