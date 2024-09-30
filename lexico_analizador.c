#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 256

const char *TOKENS[] = {
    "{", "}", "[", "]", ",", ":", 
    "true", "false", "null", NULL
};

const char *TOKEN_NAMES[] = {
    "L_LLAVE", "R_LLAVE", "L_CORCHETE", "R_CORCHETE", 
    "COMA", "DOS_PUNTOS", "PR_TRUE", "PR_FALSE", "PR_NULL", NULL
};

int i;
int isToken(const char *token) {
    for (i = 0; TOKENS[i] != NULL; i++) {
        if (strcmp(token, TOKENS[i]) == 0) {
            return i; // Retorna el índice del token
        }
    }
    return -1; // Token no reconocido
}

void extractString(const char *line, int *index, char *token) {
    int start = *index;
    (*index)++; // Saltar la comilla inicial
    while (line[*index] != '\"' && line[*index] != '\0') {
        if (line[*index] == '\\') {
            (*index)++; // Saltar carácter de escape
        }
        token[strlen(token)] = line[*index];
        (*index)++;
    }
    if (line[*index] == '\"') {
        token[strlen(token)] = '\0'; // Terminar cadena
        (*index)++;
    } else {
        strcpy(token, ""); // Error: sin cierre de comilla
    }
}

void extractNumber(const char *line, int *index, char *token) {
    int start = *index;
    while (isdigit(line[*index]) || line[*index] == '-' || line[*index] == '.' ||
           line[*index] == 'e' || line[*index] == 'E') {
        token[strlen(token)] = line[*index];
        (*index)++;
    }
    token[strlen(token)] = '\0';
}

void tokenizeLine(const char *line, FILE *outputFile) {
    char token[MAX_TOKEN_LENGTH];
    int i = 0;

    while (line[i] != '\0') {
        while (isspace(line[i])) i++; // Saltar espacios

        if (line[i] == '{' || line[i] == '}') {
            token[0] = line[i];
            token[1] = '\0';
            int tokenIndex = isToken(token);
            fprintf(outputFile, "%s ", TOKEN_NAMES[tokenIndex]);
            i++;
        } else if (line[i] == '[' || line[i] == ']') {
            token[0] = line[i];
            token[1] = '\0';
            int tokenIndex = isToken(token);
            fprintf(outputFile, "%s ", TOKEN_NAMES[tokenIndex]);
            i++;
        } else if (line[i] == ',') {
            token[0] = line[i];
            token[1] = '\0';
            int tokenIndex = isToken(token);
            fprintf(outputFile, "%s ", TOKEN_NAMES[tokenIndex]);
            i++;
        } else if (line[i] == ':') {
            token[0] = line[i];
            token[1] = '\0';
            int tokenIndex = isToken(token);
            fprintf(outputFile, "%s ", TOKEN_NAMES[tokenIndex]);
            i++;
        } else if (line[i] == '\"') {
            token[0] = '\0';
            extractString(line, &i, token);
            if (strlen(token) > 0) {
                fprintf(outputFile, "STRING ");
            } else {
                fprintf(stderr, "Error léxico en la línea: %s\n", line);
                return; // Detener procesamiento de esta línea
            }
        } else if (isdigit(line[i]) || (line[i] == '-' && isdigit(line[i + 1]))) {
            token[0] = '\0';
            extractNumber(line, &i, token);
            fprintf(outputFile, "NUMBER ");
        } else {
            // Comprobar palabras clave
            token[0] = '\0';
            while (isalpha(line[i])) {
                token[strlen(token)] = line[i];
                i++;
            }
            token[strlen(token)] = '\0';
            int tokenIndex = isToken(token);
            if (tokenIndex >= 0) {
                fprintf(outputFile, "%s ", TOKEN_NAMES[tokenIndex + 6]); // Ajuste para PR_TRUE, etc.
            } else {
                fprintf(stderr, "Error léxico en la línea: %s\n", line);
                return; // Detener procesamiento de esta línea
            }
        }
    }
    fprintf(outputFile, "\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada.json> <archivo_salida.txt>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");

    if (!inputFile) {
        fprintf(stderr, "Error al abrir el archivo de entrada.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), inputFile)) {
        tokenizeLine(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
