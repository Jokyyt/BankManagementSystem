#include "bankfunctions.h"
#include <stdio.h>   // Inclure <stdio.h> pour FILE, NULL, etc.
#include <stddef.h> // Inclure <stddef.h> pour NULL
#include <string.h>
#include <windows.h>

// FONCTIONS DE GESTION BANCAIRE

// CREER UN COMPTE
int creerCompte(User *user) {
    FILE *fichier = fopen("DATA/users.txt", "a");

    if (fichier == NULL) {
        perror("Error opening file");
        return 1;
    }

    char username_input[50];
    char password_input[50];
    int userExistsResult = 0; // Initialisation du résultat de vérification

    printf("NEW USER !!\n");
    do {
        printf("USERNAME : ");
        scanf("%s", username_input);
        printf("PASSWORD : ");
        scanf("%s", password_input);

        userExistsResult = checkInfos(user, username_input, password_input);

        if (userExistsResult == 3) {
            printf("Username and/or password already exist. Please choose different values.\n");
        } else if (userExistsResult == 1) {
            printf("Username already exists. Please choose a different username.\n");
        } else if (userExistsResult == 2) {
            printf("Password already exists. Please choose a different password.\n");
        } else {
            strcpy(user->username, username_input);
            strcpy(user->password, password_input);
            user->solde = 0.0;
            user->id = generateID(user);
            verifyIDGeneration(user);
            if (fprintf(fichier, "%s:%s:%.2f:%d\n", user->username, user->password, user->solde, user->id) < 0) {
                perror("Error writing to file");
                fclose(fichier);
                return 2;
            }
        }
    } while (userExistsResult != 0);

    user->id = generateID(user);
    if (fprintf(fichier, "%s:%s:%.2f:%d\n", user->username, user->password, user->solde, user->id) < 0) {
    perror("Error writing to file");
    fclose(fichier);
    return 2; // Vous pouvez choisir un code d'erreur approprié, par exemple, 2 pour une erreur d'écriture dans le fichier.
    }

    if (fclose(fichier) == EOF) {
        perror("Error closing file");
        return 3; // Vous pouvez choisir un code d'erreur approprié, par exemple, 3 pour une erreur de fermeture de fichier.
    }

    printf("Account created successfully!\n");
    return 0;
}





// SE CONNECTER A UN COMPTE 
int seConnecter(User *user) {
    char username_input[32];
    char password_input[40];

    printf("Please enter your username : ");
    scanf("%s", username_input);
    printf("Please enter your password : ");
    scanf("%s", password_input);

    int userFound = FoundLine(user, username_input, password_input);

    if (userFound == 0) {
        printf("Successful connection !\n");
        return 0;
    } else {
        printf("Username and/or Password incorrect.\n");
        return 1;
    }
}


// CLOTURER UN COMPTE 
int closeAccount(User *user, const char *username, const char *password) {
    FILE *fichier = fopen("DATA/users.txt", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    FILE *tempFichier = fopen("DATA/temp_users.txt", "w");
    if (tempFichier == NULL) {
        perror("Error creating temporary file");
        fclose(fichier);
        return -1;
    }
    int id;
    printf("Are you sure you want to delete your account? (y/n) : ");
    char confirmation;
    scanf(" %c", &confirmation);  // Notez l'espace avant %c pour ignorer les espaces, les tabulations, etc.

    if (confirmation == 'y' || confirmation == 'Y') {

        int utilisateurTrouve = 0; // Indicateur pour savoir si l'utilisateur a été trouvé et supprimé.

        char ligne[75];  // Pour stocker chaque ligne complète du fichier.

        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            char username_input[32];
            char password_input[40];
            float solde;
            if (sscanf(ligne, "%[^:]:%[^:]:%f:%d\n", username_input, password_input, &solde, &id) == 4) {
                if (strcmp(username_input, user->username) == 0 && strcmp(password_input, user->password) == 0) {
                    // Ne copiez pas cette ligne dans le fichier temporaire (c'est comme la suppression).
                    utilisateurTrouve = 1;
                } else {
                    // Copiez cette ligne dans le fichier temporaire car elle ne correspond pas à l'utilisateur à supprimer.
                    fprintf(tempFichier, "%s:%s:%.2f\n", username_input, password_input, solde);
                }
            } else {
                // La ligne n'a pas le format attendu, conservez-la telle quelle dans le fichier temporaire.
                fprintf(tempFichier, "%s", ligne);
            }
        }
        remove("DATA/users.txt");
        rename("DATA/temp_users.txt", "DATA/users.txt");

        fclose(fichier);
        fclose(tempFichier);

        if (utilisateurTrouve) {
            // Supprime l'ancien fichier et renomme le fichier temporaire.
            printf("Account closed successfully ! \n");
            return 0;
        } else {
            printf("No matches found for user.\n");
            return -1;
        }

    } else if (confirmation == 'n' || confirmation == 'N') {
        printf("Account closure canceled.\n");
        return 1;
    }

    return -1;
}



// CONSULTER LE SOLDE DE SON COMPTE 
int consultSolde(User *user, const char *username, const char *password) {
    printf("Username : %s\n", user->username);
    printf("Password : %s\n", user->password);
    printf("ID : %d\n", user->id);
    printf("\n");
    printf("Your balance is %.2f $\n", user->solde);

    return 0;

}


// AJOUTER/RETIRER MONTANT SUR LE SOLDE
int ajouterSolde(User *user, float montant) {
    printf("Your current balance is %.2f $\n", user->solde);
    printf("How much do you want to add to your balance? : ");
    scanf("%f", &montant);
    user->solde += montant;
    printf("Your new balance is %.2f $\n", user->solde);
    return 0;
}

int retirerSolde(User *user, float montant) {
    printf("Your current balance is %.2f $\n", user->solde);

    int tentatives = 0;
    float soldeInitial = user->solde;
    int maxTentatives = 3;

    for (tentatives = 0; tentatives < maxTentatives; tentatives++) {
        printf("How much do you want to withdraw from your balance? : ");
        scanf("%f", &montant);

        if (montant <= user->solde) {
            user->solde -= montant;
            printf("Your new balance is %.2f $\n", user->solde);
            break; // Sort de la boucle si le montant est valide.
        } else {
            printf("You don't have that much money in your balance. You have %d attempts left.\n", maxTentatives - tentatives - 1);
        }
    }

    if (tentatives == maxTentatives) {
        printf("You have exceeded the maximum number of attempts. Returning to the previous menu.\n");
        user->solde = soldeInitial; // Rétablit le solde initial
    }

    return 0;
}


// RECHERCHE DE LA LIGNE DANS LA BASE DE DONNÉES
int FoundLine(User *user, const char *username, const char *password) {
    FILE *fichier = fopen("DATA/users.txt", "r");

    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fscanf(fichier, "%[^:]:%[^:]:%f:%d\n", user->username, user->password, &user->solde, &user->id) != EOF) {
        if (strcmp(username, user->username) == 0 && strcmp(password, user->password) == 0) {
            fclose(fichier);
            return 0; // L'utilisateur a été trouvé.
        }
    }

    fclose(fichier);
    return 1; // L'utilisateur n'a pas été trouvé.
}

// MODIFICATION DES DONNÉES D'UN COMPTE

int editAccount(User *user) {
    FILE *fichier = fopen("DATA/users.txt", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }
    printf("That's your Username : %s\n", user->username);
    printf("That's your Password : %s\n", user->password);
    printf("That's your ID : %d\n", user->id);
    Sleep(500);
    printf("Do you want to change your Username, your Password or BOTH ? (u/p/up) : ");
    char choice[3]; 
    scanf(" %2s", choice); // Lire jusqu'à 2 caractères pour prendre en compte "up" ou "UP".
    printf("Press any other key to go back to the previous menu.\n");

    if (strcmp(choice, "u") == 0 || strcmp(choice, "U") == 0) {
        printf("New Username : ");
        scanf("%s", user->username);
    } else if (strcmp(choice, "p") == 0 || strcmp(choice, "P") == 0) {
        printf("New Password : ");
        scanf("%s", user->password);
    } else if (strcmp(choice, "up") == 0 || strcmp(choice, "UP") == 0) {
        printf("New Username : ");
        scanf("%s", user->username);
        printf("New Password : ");
        scanf("%s", user->password);
    } else {
        printf("Invalid choice. Returning to the previous menu.\n");
        fclose(fichier);
        return -1;
    }
    fclose(fichier);
    return 0;
}

// VERIFICATION DES DONNÉES D'UN COMPTE SI LE USERNAME OU PASSWORD EXISTE DEJ
// Cette fonction verfie si l'utilisateur existe déjà dans la base de donnée. Cette focntion sera utilisé lors de la création d'un nouveau compte ou lors de mofications des données d'un compte existant.
int checkInfos(User *user, const char *username, const char *password) {
    FILE *file = fopen("DATA/users.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fscanf(file, "%49[^:]:%49[^:]:%f:%d\n", user->username, user->password, &user->solde, &user->id) != EOF) {
        int usernameMatch = strcmp(username, user->username) == 0;
        int passwordMatch = strcmp(password, user->password) == 0;

        if (usernameMatch && passwordMatch) {
            fclose(file);
            return 3; // Le username et le password existent.
        } else if (usernameMatch) {
            fclose(file);
            return 1; // Le username existe, mais le password n'existe pas.
        } else if (passwordMatch) {
            fclose(file);
            return 2; // Le password existe, mais le username n'existe pas.
        }
    }

    fclose(file);
    return 0; // Ni le username ni le password n'existent pas dans la base de données.
}



// Fonction pour générer un ID aléatoire de 5 à 6 chiffres
int generateID(User *user) {
    int min = 10000;
    int max = 99999;
    int id;
    do {
        id = rand() % (max - min + 1) + min;
    } while (checkID(user, id) != 0);
    return id;
}

// Fonction pour vérifier que le ID n'existe deja pas dans la base de données. Sinon, generateID() sera reappeler jusqu'à trouver un ID unique.

int checkID(User *user, int id) {
    FILE *file = fopen("DATA/users.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    while (fscanf(file, "%49[^:]:%49[^:]:%f:%d\n", user->username, user->password, &user->solde, &user->id) != EOF) {
        if (id == user->id) {
            fclose(file);
            return 1; // L'ID existe déjà.
        }
    }
    fclose(file);
    return 0; // L'ID n'existe pas.
}




// Fonction pour vérifier si l'ID est correctement généré et attribué
void verifyIDGeneration(User *user) {
    int id = generateID(user);
    printf("Generated ID: %d\n", id);

    FILE *file = fopen("DATA/users.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int idExists = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        int storedID;
        if (sscanf(line, "%*[^:]:%*[^:]:%*f:%d", &storedID) == 1) {
            if (id == storedID) {
                idExists = 1;
                break;
            }
        }
    }

    if (idExists) {
        printf("ID is already in use.\n");
    } else {
        printf("ID is unique and can be used.\n");
    }

    fclose(file);
}
