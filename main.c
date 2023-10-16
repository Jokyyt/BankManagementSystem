/* 

***SYSTEME DE GESTION BANCAIRE***

Dans ce projet de programmation C, vous simulerez un système bancaire simple avec crédit et débit sur des comptes bancaires.
Ici, vous apprendrez à créer un compte d'épargne et à exposer les opérations de base telles que le crédit, le débit, la vérification du solde, la clôture du compte, etc., en utilisant un simple code en langage C composé de fonctions pour chaque opération. La gestion des fichiers peut stocker des données et des informations utilisateur, et plus tard, vous pouvez utiliser une simple base de données pour le stockage.
Ce projet vous aidera à apprendre à structurer votre code puis à coder chaque module séparément.


LISTES DES FONCTIONS :

- Créer un compte ✓ 
- Se connecter à son compte ✓ 
- Retirer de l'argent ✓
- Déposer de l'argent ✓
- Consulter le compte ✓
- Clôturer le compte ✓
- Changer de username/password ✓
- Virement entre comptes / - Gérérateur de ID pour chaque compte 
- Verifier si username/password existent deja utilisés ✓




*/
#include "bankfunctions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>



int main() {
    User currentUser;
    int choix;
    int connecte = 0; // 0 signifie que l'utilisateur n'est pas connecté

    while (1) {
        if (!connecte) {
            Sleep(1000);
            printf("1. Log in\n2. Create an account\n3. Exit\nChoose an option: ");
            scanf("%d", &choix);

            switch (choix) {
                case 1:
                    if (seConnecter(&currentUser) == 0) {;
                        connecte = 1;
                        Sleep(1000);
                        break;
                    }
                    else {
                        Sleep(1000);
                        printf("Connection error\n");
                        break;
                    }
                case 2:
                    if (creerCompte(&currentUser) == 0) {;
                        Sleep(1000);
                        break;
                    } else {
                        Sleep(1000);
                        printf("Account creation error\n");
                        return 0;
                    }
                case 3:
                    printf("Thank you for using our service. Goodbye!\n");
                    return 0; // Quitter de manière propre
                default:
                    printf("Invalid option. Try again.\n");
                    break;
            }
        } else {
            Sleep(1000);
            printf("\n1. Mon Compte\n2. Withdraw money\n3. Deposit money\n4. Check balance\n5. Close the account\n6. Log out\n7. Exit\nChoose an option: ");
            scanf("%d", &choix);

            switch (choix) {
                case 1:
                    Sleep(1000);
                    editAccount(&currentUser);
                    break;
                case 2:
                    Sleep(2000);
                    retirerSolde(&currentUser, 0);
                    break;
                case 3:
                    Sleep(1000);
                    ajouterSolde(&currentUser, 0);
                    break;
                case 4:
                    Sleep(1000);
                    consultSolde(&currentUser, currentUser.username, currentUser.password);
                    break;
                case 5:
                    Sleep(2000);
                    if (closeAccount(&currentUser, currentUser.username, currentUser.password) == 0) {;
                        connecte = 0;
                        break;
                    } else {
                        Sleep(1000);
                        break;
                    }
                case 6:
                    printf("Disconnection in progress...\n");
                    Sleep(1500);
                    connecte = 0;
                    break;
                case 7:
                    printf("Thank you for using our service. Goodbye!\n");
                    return 0; // Quitter de manière propre
                default:
                    printf("Invalid option. Try again.\n");
                    break;
            }
        }
    }

    return 0;
}


