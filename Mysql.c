#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#define MAX_LINE_LENGTH 1000
#define MAX_FIELDS 10

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *field;
    int field_count = 0;

    // Se connecter à la base de données
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Erreur lors de l'initialisation de la connexion MySQL\n");
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "user", "password", "database", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Erreur lors de la connexion à la base de données: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // Ouvrir le fichier en mode lecture
    file = fopen("worker.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        mysql_close(conn);
        return 1;
    }

    // Lire et insérer chaque ligne du fichier dans la base de données
    while (fgets(line, sizeof(line), file)) {
        field_count = 0;
        field = strtok(line, ",");
        char query[MAX_LINE_LENGTH] = "INSERT INTO your_table_name VALUES (";
        while (field != NULL && field_count < MAX_FIELDS) {
            // Concaténer les valeurs des champs dans la requête SQL
            strcat(query, "'");
            strcat(query, field);
            strcat(query, "', ");
            field = strtok(NULL, ",");
            field_count++;
        }

        // Supprimer la virgule et l'espace en trop et ajouter la parenthèse fermante
        if (field_count > 0) {
            query[strlen(query) - 2] = ')';
            query[strlen(query) - 1] = ';';
        } else {
            strcat(query, ");");
        }

        // Exécuter la requête SQL
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Erreur lors de l'exécution de la requête: %s\n", mysql_error(conn));
            fclose(file);
            mysql_close(conn);
            return 1;
        }
    }

    // Fermer le fichier
    fclose(file);

    // Fermer la connexion à la base de données
    mysql_close(conn);
    return 0;
}
