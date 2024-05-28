#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_FIELDS 10

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *field;
    int field_count = 0;

    // Ouvrir le fichier en mode lecture
    file = fopen("data.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    // Lire et imprimer chaque ligne du fichier
    while (fgets(line, sizeof(line), file)) {
        field_count = 0;
        field = strtok(line, ",");

        while (field != NULL && field_count < MAX_FIELDS) {
            printf("%s ", field);
            field = strtok(NULL, ",");
            field_count++;
        }

        printf("\n");
    }

    // Fermer le fichier
    fclose(file);

    return 0;
}