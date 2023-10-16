#include "bankfunctions.h"
#include <stdio.h>   // Inclure <stdio.h> pour FILE, NULL, etc.
#include <stddef.h> // Inclure <stddef.h> pour NULL
#include <string.h>
#include <windows.h>
#include "Librairies\cJSON\cJSON.h"

// FONCTIONS DE GESTION BANCAIRE

// CREER UN COMPTE
int creerCompte(User *user) {
    cJSON *root = cJSON_Parse("[]"); // Créez un tableau JSON vide

    cJSON *newUser = cJSON_CreateObject();
    cJSON_AddStringToObject(newUser, "username", user->username);
    cJSON_AddStringToObject(newUser, "password", user->password);
    cJSON_AddNumberToObject(newUser, "solde", user->solde);
    cJSON_AddNumberToObject(newUser, "id", user->id);

    cJSON_AddItemToArray(root, newUser);

    // Ouvrez le fichier JSON en écriture
    FILE *fichier = fopen("DATA/users.json", "w");
    if (fichier == NULL) {
        perror("Error opening file");
        cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
        return 1;
    }

    // Convertissez l'objet JSON en une chaîne JSON formatée
    char *json_str = cJSON_Print(root);

    // Écrivez la chaîne JSON dans le fichier
    if (fprintf(fichier, "%s\n", json_str) < 0) {
        perror("Error writing to file");
        fclose(fichier);
        cJSON_Delete(root);
        free(json_str);
        return 2;
    }

    fclose(fichier);
    cJSON_Delete(root);
    free(json_str);

    printf("Account created successfully!\n");
    return 0;
}






// SE CONNECTER A UN COMPTE 
int seConnecter(User *user) {
    char username_input[50];
    char password_input[50];

    printf("Please enter your username: ");
    scanf("%s", username_input);
    printf("Please enter your password: ");
    scanf("%s", password_input);

    cJSON *root = NULL;
    cJSON *userArray = NULL;

    // Ouvrez le fichier JSON en lecture
    FILE *fichier = fopen("DATA/users.json", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Lisez le contenu du fichier JSON
    char fileContents[4096];  // Taille du contenu du fichier (ajustez-la si nécessaire)
    size_t bytesRead = fread(fileContents, 1, sizeof(fileContents), fichier);
    fileContents[bytesRead] = '\0';  // Assurez-vous de terminer la chaîne.

    // Analysez le fichier JSON en un objet cJSON
    root = cJSON_Parse(fileContents);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        fclose(fichier);
        return -1;
    }

    // Obtenez le tableau d'utilisateurs à partir de l'objet racine
    userArray = cJSON_GetArrayItem(root, 0);

    // Parcourez le tableau d'utilisateurs
    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        if (strcmp(username_input, storedUsername) == 0 && strcmp(password_input, storedPassword) == 0) {
            // Utilisateur trouvé avec le username et le password fournis
            cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
            fclose(fichier);
            printf("Successful connection!\n");
            return 0;
        }
    }

    cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
    fclose(fichier);

    printf("Username and/or password incorrect.\n");
    return 1;
}



// CLOTURER UN COMPTE 
int closeAccount(User *user, const char *username, const char *password) {
    cJSON *root = NULL;
    cJSON *userArray = NULL;

    // Ouvrez le fichier JSON en lecture
    FILE *fichier = fopen("DATA/users.json", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Lisez le contenu du fichier JSON
    char fileContents[4096];  // Taille du contenu du fichier (ajustez-la si nécessaire)
    size_t bytesRead = fread(fileContents, 1, sizeof(fileContents), fichier);
    fileContents[bytesRead] = '\0';  // Assurez-vous de terminer la chaîne.

    // Analysez le fichier JSON en un objet cJSON
    root = cJSON_Parse(fileContents);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        fclose(fichier);
        return -1;
    }

    // Obtenez le tableau d'utilisateurs à partir de l'objet racine
    userArray = cJSON_GetArrayItem(root, 0);

    // Parcourez le tableau d'utilisateurs
    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            // Utilisateur trouvé avec le username et le password fournis
            cJSON_DeleteItemFromArray(userArray, i); // Supprimez l'objet utilisateur du tableau

            // Ouvrez le fichier JSON en écriture
            fclose(fichier);
            fichier = fopen("DATA/users.json", "w");
            if (fichier == NULL) {
                perror("Error opening file");
                cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
                return -1;
            }

            // Convertissez l'objet cJSON en une chaîne JSON formatée
            char *json_str = cJSON_Print(root);

            // Écrivez la chaîne JSON dans le fichier
            if (fprintf(fichier, "%s\n", json_str) < 0) {
                perror("Error writing to file");
                fclose(fichier);
                cJSON_Delete(root);
                free(json_str);
                return 2;
            }

            fclose(fichier);
            cJSON_Delete(root);
            free(json_str);

            printf("Account closed successfully!\n");
            return 0;
        }
    }

    cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
    fclose(fichier);

    printf("Username and/or password not found.\n");
    return -1;
}




// CONSULTER LE SOLDE DE SON COMPTE 
int consultSolde(User *user, const char *username, const char *password) {
    cJSON *root = NULL;
    cJSON *userArray = NULL;

    // Ouvrez le fichier JSON en lecture
    FILE *fichier = fopen("DATA/users.json", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Lisez le contenu du fichier JSON
    char fileContents[4096];  // Taille du contenu du fichier (ajustez-la si nécessaire)
    size_t bytesRead = fread(fileContents, 1, sizeof(fileContents), fichier);
    fileContents[bytesRead] = '\0';  // Assurez-vous de terminer la chaîne.

    // Analysez le fichier JSON en un objet cJSON
    root = cJSON_Parse(fileContents);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        fclose(fichier);
        return -1;
    }

    // Obtenez le tableau d'utilisateurs à partir de l'objet racine
    userArray = cJSON_GetArrayItem(root, 0);

    // Parcourez le tableau d'utilisateurs
    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            // Utilisateur trouvé avec le username et le password fournis
            double solde = cJSON_GetObjectItem(userObj, "solde")->valuedouble;
            
            cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
            fclose(fichier);

            printf("Username: %s\n", storedUsername);
            printf("Password: %s\n", storedPassword);
            printf("ID: %d\n", cJSON_GetObjectItem(userObj, "id")->valueint);
            printf("\n");
            printf("Your balance is %.2f $\n", solde);

            return 0;
        }
    }

    cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
    fclose(fichier);

    printf("Username and/or password not found.\n");
    return -1;
}



// AJOUTER/RETIRER MONTANT SUR LE SOLDE
int ajouterSolde(User *user, float montant) {
    printf("Your current balance is %.2f $\n", user->solde);
    printf("How much do you want to add to your balance? : ");
    scanf("%f", &montant);

    user->solde += montant;

    // Mise à jour du solde dans le fichier JSON
    cJSON *root = NULL;
    cJSON *userArray = NULL;

    // Ouvrez le fichier JSON en lecture
    FILE *fichier = fopen("DATA/users.json", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Lisez le contenu du fichier JSON
    char fileContents[4096];  // Taille du contenu du fichier (ajustez-la si nécessaire)
    size_t bytesRead = fread(fileContents, 1, sizeof(fileContents), fichier);
    fileContents[bytesRead] = '\0';  // Assurez-vous de terminer la chaîne.

    // Analysez le fichier JSON en un objet cJSON
    root = cJSON_Parse(fileContents);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        fclose(fichier);
        return -1;
    }

    // Obtenez le tableau d'utilisateurs à partir de l'objet racine
    userArray = cJSON_GetArrayItem(root, 0);

    // Parcourez le tableau d'utilisateurs
    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        if (strcmp(user->username, storedUsername) == 0 && strcmp(user->password, storedPassword) == 0) {
            // Utilisateur trouvé avec le username et le password fournis
            cJSON_ReplaceItemInObject(userObj, "solde", cJSON_CreateNumber(user->solde));

            // Ouvrez le fichier JSON en écriture
            fclose(fichier);
            fichier = fopen("DATA/users.json", "w");
            if (fichier == NULL) {
                perror("Error opening file");
                cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
                return -1;
            }

            // Convertissez l'objet cJSON en une chaîne JSON formatée
            char *json_str = cJSON_Print(root);

            // Écrivez la chaîne JSON dans le fichier
            if (fprintf(fichier, "%s\n", json_str) < 0) {
                perror("Error writing to file");
                fclose(fichier);
                cJSON_Delete(root);
                free(json_str);
                return -1;
            }

            fclose(fichier);
            cJSON_Delete(root);
            free(json_str);

            printf("Your new balance is %.2f $\n", user->solde);
            return 0;
        }
    }

    cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
    fclose(fichier);

    printf("Username and/or password not found.\n");
    return -1;
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

            // Mise à jour du solde dans le fichier JSON (de manière similaire à la fonction ajouterSolde)
            // ...

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



// MODIFICATION DES DONNÉES D'UN COMPTE

int editAccount(User *user) {
    cJSON *root = NULL;
    cJSON *userArray = NULL;

    // Ouvrez le fichier JSON en lecture
    FILE *fichier = fopen("DATA/users.json", "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Lisez le contenu du fichier JSON
    char fileContents[4096];  // Taille du contenu du fichier (ajustez-la si nécessaire)
    size_t bytesRead = fread(fileContents, 1, sizeof(fileContents), fichier);
    fileContents[bytesRead] = '\0';  // Assurez-vous de terminer la chaîne.

    // Analysez le fichier JSON en un objet cJSON
    root = cJSON_Parse(fileContents);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        fclose(fichier);
        return -1;
    }

    // Obtenez le tableau d'utilisateurs à partir de l'objet racine
    userArray = cJSON_GetArrayItem(root, 0);

    // Parcourez le tableau d'utilisateurs
    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        if (strcmp(user->username, storedUsername) == 0 && strcmp(user->password, storedPassword) == 0) {
            // Utilisateur trouvé avec le username et le password fournis
            char choice[3];
            printf("That's your Username: %s\n", storedUsername);
            printf("That's your Password: %s\n", storedPassword);
            printf("That's your ID: %d\n", cJSON_GetObjectItem(userObj, "id")->valueint);
            Sleep(500);
            printf("Do you want to change your Username, your Password, or BOTH? (u/p/up) : ");
            scanf(" %2s", choice); // Lire jusqu'à 2 caractères pour prendre en compte "up" ou "UP".
            printf("Press any other key to go back to the previous menu.\n");

            if (strcmp(choice, "u") == 0 || strcmp(choice, "U") == 0) {
                printf("New Username: ");
                scanf("%s", user->username);
                cJSON_ReplaceItemInObject(userObj, "username", cJSON_CreateString(user->username));
            } else if (strcmp(choice, "p") == 0 || strcmp(choice, "P") == 0) {
                printf("New Password: ");
                scanf("%s", user->password);
                cJSON_ReplaceItemInObject(userObj, "password", cJSON_CreateString(user->password));
            } else if (strcmp(choice, "up") == 0 || strcmp(choice, "UP") == 0) {
                printf("New Username: ");
                scanf("%s", user->username);
                cJSON_ReplaceItemInObject(userObj, "username", cJSON_CreateString(user->username));
                printf("New Password: ");
                scanf("%s", user->password);
                cJSON_ReplaceItemInObject(userObj, "password", cJSON_CreateString(user->password));
            } else {
                printf("Invalid choice. Returning to the previous menu.\n");
                cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
                fclose(fichier);
                return -1;
            }

            // Ouvrez le fichier JSON en écriture
            fclose(fichier);
            fichier = fopen("DATA/users.json", "w");
            if (fichier == NULL) {
                perror("Error opening file");
                cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
                return -1;
            }

            // Convertissez l'objet cJSON en une chaîne JSON formatée
            char *json_str = cJSON_Print(root);

            // Écrivez la chaîne JSON dans le fichier
            if (fprintf(fichier, "%s\n", json_str) < 0) {
                perror("Error writing to file");
                fclose(fichier);
                cJSON_Delete(root);
                free(json_str);
                return -1;
            }

            fclose(fichier);
            cJSON_Delete(root);
            free(json_str);

            printf("Account information updated successfully!\n");
            return 0;
        }
    }

    cJSON_Delete(root); // Libérez la mémoire de l'objet cJSON
    fclose(fichier);

    printf("Username and/or password not found.\n");
    return -1;
}

