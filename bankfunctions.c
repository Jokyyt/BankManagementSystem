#include "bankfunctions.h"
#include <stdio.h>   // Inclure <stdio.h> pour FILE, NULL, etc.
#include <stddef.h> // Inclure <stddef.h> pour NULL
#include <string.h>
#include <windows.h>
#include "Librairies\cJSON\cJSON.h"

// FONCTIONS DE GESTION BANCAIRE

// CREER UN COMPTE
int creerCompte(User *user) {
    FILE *fichier = fopen("DATA/users.json", "r+");

    if (fichier == NULL) {
        perror("Error opening file");
        return 1;
    }

    char username_input[50];
    char password_input[50];
    int userExistsResult = 0; // Initialisation du résultat de vérification

    fseek(fichier, 0, SEEK_END);
    long fsize = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);
    
    char *json_str = (char *)malloc(fsize + 1);
    fread(json_str, 1, fsize, fichier);
    fclose(fichier);
    json_str[fsize] = 0;
    
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        cJSON_Delete(root);
        perror("Error parsing JSON");
        return -1;
    }

    char *usersKey = "users";
    cJSON *userArray = cJSON_GetObjectItem(root, usersKey);

    if (!userArray) {
        cJSON_Delete(root);
        perror("Error getting user array from JSON");
        return -1;
    }

    printf("NEW USER !!\n");
    cJSON *newUser = cJSON_CreateObject();

    do {
        printf("USERNAME : ");
        scanf("%49s", username_input);
        printf("PASSWORD : ");
        scanf("%49s", password_input);

        userExistsResult = checkInfos(user, username_input, password_input);

        if (userExistsResult == 3) {
            printf("Username and/or password already exist. Please choose different values.\n");
        } else if (userExistsResult == 1) {
            printf("Username already exists. Please choose a different username.\n");
        } else if (userExistsResult == 2) {
            printf("Password already exists. Please choose a different password.\n");
        } else {
            cJSON_AddStringToObject(newUser, "username", username_input);
            cJSON_AddStringToObject(newUser, "password", password_input);
            cJSON_AddNumberToObject(newUser, "solde", 0.0); // Solde initial à 0

            cJSON_AddItemToArray(cJSON_GetObjectItem(root, "users"), newUser);

            fichier = fopen("DATA/users.json", "w"); // Réouvrir en mode écriture
            if (fichier == NULL) {
                perror("Error opening file for writing");
                cJSON_Delete(root);
                return 2;
            }

            if (fprintf(fichier, "%s", cJSON_Print(root)) < 0) {
                perror("Error writing to file");
                fclose(fichier);
                cJSON_Delete(root);
                return 2;
            }

            fclose(fichier);
        }
    } while (userExistsResult != 0);

    cJSON_Delete(root); // Libérer la mémoire de l'objet cJSON

    printf("Account created successfully!\n");
    return 0;
}


int checkInfos(User *user, const char *username, const char *password) {
    FILE *fichier = fopen("DATA/users.json", "r");

    if (fichier == NULL) {
        perror("Error opening file");
        return -1;
    }

    char *json_str = NULL;
    fseek(fichier, 0, SEEK_END);
    long fsize = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    json_str = (char *)malloc(fsize + 1);
    fread(json_str, 1, fsize, fichier);
    fclose(fichier);
    json_str[fsize] = 0;

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        cJSON_Delete(root);
        perror("Error parsing JSON");
        return -1;
    }

    cJSON *userArray = cJSON_GetObjectItem(root, "users");
    if (!userArray) {
        cJSON_Delete(root);
        perror("Error getting user array from JSON");
        return -1;
    }

    for (int i = 0; i < cJSON_GetArraySize(userArray); i++) {
        cJSON *userObj = cJSON_GetArrayItem(userArray, i);
        const char *storedUsername = cJSON_GetObjectItem(userObj, "username")->valuestring;
        const char *storedPassword = cJSON_GetObjectItem(userObj, "password")->valuestring;

        int usernameMatch = strcmp(username, storedUsername) == 0;
        int passwordMatch = strcmp(password, storedPassword) == 0;

        if (usernameMatch && passwordMatch) {
            cJSON_Delete(root);
            return 3; // Le username et le password existent.
        } else if (usernameMatch) {
            cJSON_Delete(root);
            return 1; // Le username existe, mais le password n'existe pas.
        } else if (passwordMatch) {
            cJSON_Delete(root);
            return 2; // Le password existe, mais le username n'existe pas.
        }
    }

    cJSON_Delete(root);
    return 0; // Ni le username ni le password n'existent pas dans la base de données.
}
