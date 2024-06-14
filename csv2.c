#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>   

#define MAX_LINE_LENGTH 1000   


typedef struct {
    char contenu[MAX_LINE_LENGTH];
} Salaries;


typedef struct {
    char contenu[MAX_LINE_LENGTH];
} Prestataires;

// Fonction pour remplacer tous les caractères 'find' par 'replace' dans la chaîne 'str'
void remplacer(char *str, char find, char replace) {
    while (*str) {  // Parcourt chaque caractère de la chaîne jusqu'à la fin (caractère nul)
        if (*str == find) {  // Si le caractère courant est celui à remplacer
            *str = replace;  // Remplacer par le nouveau caractère
        }
        str++; 
    }
}

// Fonction pour traiter un fichier de salaries
void trait_sala(const char *filename, char original_delimiter) {
    FILE *file; 
    char line[MAX_LINE_LENGTH];  

    // Ouvrir le fichier en mode lecture
    file = fopen(filename, "r");
    if (file == NULL) {  
        perror("Erreur lors de l'ouverture du fichier");  
        return; 
    }

    // Lire et imprimer chaque ligne du fichier
    while (fgets(line, sizeof(line), file)) {  
        // Remplacer les delimiteurs originaux par des espaces
        remplacer(line, original_delimiter, ' ');  // Appeler la fonction de remplacement des caractères

        Salaries salarie;  
        // Copier la ligne modifiée dans le contenu de salarie
        strncpy(salarie.contenu, line, sizeof(salarie.contenu));
        salarie.contenu[sizeof(salarie.contenu) - 1] = '\0';  // S'assurer que la chaîne est terminée par un caractère nul

        printf("%s", salarie.contenu);  // Imprimer le contenu de salarie
    }

    // Fermer le fichier
    fclose(file);
}

// Fonction pour traiter un fichier de prestataires
void trait_prest(const char *filename, char original_delimiter) {
    FILE *file;  
    char line[MAX_LINE_LENGTH];  
    // Ouvrir le fichier en mode lecture
    file = fopen(filename, "r");
    if (file == NULL) {  
        perror("Erreur lors de l'ouverture du fichier");  
        return;  // Retourner sans continuer
    }

    // Lire et imprimer chaque ligne du fichier
    while (fgets(line, sizeof(line), file)) {  
        // Remplacer les delimiteurs originaux par des espaces
        remplacer(line, original_delimiter, ' ');  // Appeler la fonction de remplacement 

        Prestataires prestataire;  
        // Copier la ligne modifiée dans le contenu de prestataire
        strncpy(prestataire.contenu, line, sizeof(prestataire.contenu));
        prestataire.contenu[sizeof(prestataire.contenu) - 1] = '\0';  

        printf("%s", prestataire.contenu); 
    }
    
    // Fermer le fichier
    fclose(file);
}


int main() {
    // Traiter le fichier des salaries
    printf("\nTraitement des salariés:\n\n");  
    trait_sala("Salaries.csv", ',');  // Appeler la fonction de traitement des salaries avec le fichier "Salaries.csv" et le délimiteur ','

    // Traiter le fichier des prestataires
    printf("\n\nTraitement des prestataires:\n\n");  
    trait_prest("Prestataires.csv", ';');   

    return 0;  
}

