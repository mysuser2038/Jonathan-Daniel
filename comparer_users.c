#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Pour utiliser la fonction toupper

#define MAX_LINE_LENGTH 1000

// Structure pour stocker le contenu des personnes
typedef struct {
    char contenu[MAX_LINE_LENGTH];
} Personne;

// Fonction pour remplacer tous les caractères 'find' par 'replace' dans la chaîne 'str'
void remplacer(char *str, char find, char replace) {
    while (*str) {  // Parcourt chaque caractère de la chaîne jusqu'à la fin (caractère nul)
        if (*str == find) {  // Si le caractère courant est celui à remplacer
            *str = replace;  // Remplacer par le nouveau caractère
        }
        str++;
    }
}

// Fonction pour extraire le deuxième champ et le mettre en majuscules, en remplaçant les tirets par des espaces
void extraire_deuxieme_champ_majuscule(const char *str, char delimiter, char *result) {
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, str);

    char *deuxieme_champ = strtok(temp, &delimiter); // Extraire le premier champ
    if (deuxieme_champ != NULL) {
        deuxieme_champ = strtok(NULL, &delimiter); // Extraire le deuxième champ
        if (deuxieme_champ != NULL) {
            // Remplacer les tirets par des espaces dans le deuxième champ
            remplacer(deuxieme_champ, '-', ' ');
            // Convertir le deuxième champ en majuscules
            for (int i = 0; deuxieme_champ[i] != '\0'; i++) {
                deuxieme_champ[i] = toupper((unsigned char) deuxieme_champ[i]);
            }
            strcpy(result, deuxieme_champ);
        }
    }
}

// Fonction pour ouvrir et lire un fichier, et traiter chaque ligne avec une fonction donnée
void traiter_fichier(const char *filename, char original_delimiter, void (*traitement)(const char *, char)) {
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // Ouvrir le fichier en mode lecture
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Lire et traiter chaque ligne du fichier
    while (fgets(line, sizeof(line), file)) {
        // Remplacer les délimiteurs originaux par des espaces
        remplacer(line, original_delimiter, ' ');  // Appeler la fonction de remplacement des caractères
        traitement(line, ' ');  // Appeler la fonction de traitement sur chaque ligne
    }

    // Fermer le fichier
    fclose(file);
}

// Fonction de traitement pour les salariés et les prestataires (imprimer le contenu)
void traitement_imprimer(const char *line, char delimiter) {
    printf("%s", line);
}

// Fonction pour traiter les combinaisons et n'afficher que le deuxième champ en majuscules
void traitement_comb(const char *line, char delimiter) {
    char result[MAX_LINE_LENGTH];
    extraire_deuxieme_champ_majuscule(line, delimiter, result);
    printf("%s\n", result);
}

// Fonction pour vérifier les noms d'email dans la liste des salariés et prestataires
void verifier_email(const char *filename, const char *sal_filename, const char *prest_filename) {
    FILE *email_file;
    char email_line[MAX_LINE_LENGTH];
    char sal_line[MAX_LINE_LENGTH];
    char prest_line[MAX_LINE_LENGTH];
    char email_nom[MAX_LINE_LENGTH];
    int trouve;

    // Ouvrir le fichier d'emails en mode lecture
    email_file = fopen(filename, "r");
    if (email_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'emails");
        return;
    }

    // Ouvrir les fichiers de salariés et de prestataires
    FILE *sal_file = fopen(sal_filename, "r");
    if (sal_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de salariés");
        fclose(email_file);
        return;
    }

    FILE *prest_file = fopen(prest_filename, "r");
    if (prest_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de prestataires");
        fclose(email_file);
        fclose(sal_file);
        return;
    }

    // Lire et vérifier chaque ligne du fichier d'emails
    while (fgets(email_line, sizeof(email_line), email_file)) {
        extraire_deuxieme_champ_majuscule(email_line, ',', email_nom);

        // Réinitialiser les indicateurs de recherche
        rewind(sal_file);
        rewind(prest_file);
        trouve = 0;

        // Vérifier dans les salariés
        while (fgets(sal_line, sizeof(sal_line), sal_file)) {
            if (strstr(sal_line, email_nom) != NULL) {
                trouve = 1;
                break;
            }
        }

        // Vérifier dans les prestataires
        if (!trouve) {
            while (fgets(prest_line, sizeof(prest_line), prest_file)) {
                if (strstr(prest_line, email_nom) != NULL) {
                    trouve = 1;
                    break;
                }
            }
        }

        // Si non trouvé, imprimer le nom
        if (!trouve) {
            printf("Non trouvé : %s\n", email_nom);
        }
    }

    // Fermer les fichiers
    fclose(email_file);
    fclose(sal_file);
    fclose(prest_file);
}

int main() {
    // Traiter le fichier des salariés
    printf("\nTraitement des salariés:\n\n");
    traiter_fichier("Salaries.csv", ',', traitement_imprimer);

    // Traiter le fichier des prestataires
    printf("\n\nTraitement des prestataires:\n\n");
    traiter_fichier("Prestataires.csv", ';', traitement_imprimer);

    // Traiter le fichier des combines
    printf("\n\nTraitement des combines:\n\n");
    traiter_fichier("combines.csv", ',', traitement_comb);


    // Vérifier les emails par rapport aux salariés et prestataires
    printf("\n\nVérification des emails:\n\n");
    verifier_email("combines.csv", "Salaries.csv", "Prestataires.csv");

    return 0;
}
