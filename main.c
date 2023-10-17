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
#include <errno.h>
#include "Librairies\cJSON\cJSON.h"

int main() {
    User currentUser;
    int connected = 0; // 0 signifie que l'utilisateur n'est pas connecté
    if (Login(&currentUser) == 0) {
        connected = 1;
        printf("Connected!!\n");
    } else {
        printf("Not Connected...\n");
        return 0;
    }
}