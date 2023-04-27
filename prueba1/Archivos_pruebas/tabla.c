#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Definimos la estructura del nodo de la lista enlazada
typedef struct node {
    int dstid;
    int hod;
    float mean_travel_time;
    struct node *next;
} Node;

// Definimos la estructura de la tabla hash
typedef struct hashtable {
    int sourceid;
    Node *head;
} Hashtable;


// Definimos la función hash para obtener la posición en la tabla hash a partir del ID del origen
int hash(int sourceid, int size) {
    return sourceid % size;
}


int main(){
    // Abrimos el archivo csv
    FILE *fp = fopen("bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo noooooo");
        return EXIT_FAILURE;
    }

     // Definimos la función hash para obtener la posición en la tabla hash a partir del ID del origen

    // Inicializamos el tamaño de la tabla hash y la tabla hash como un vector dinámico
    int size = 10000000;
    Hashtable *table = (Hashtable*) malloc(size * sizeof(Hashtable));
    for (int i = 0; i < size; i++) {
        table[i].sourceid = -1;
        table[i].head = NULL;
    }

    // Leemos el archivo csv y agregamos los datos a la tabla hash
    char line[1024];
    while (fgets(line, 1024, fp)) {
        // Parseamos la línea del archivo csv
        char *token;
        token = strtok(line, ",");
        int sourceid = atoi(token);
        token = strtok(NULL, ",");
        int dstid = atoi(token);
        token = strtok(NULL, ",");
        int hod = atoi(token);
        token = strtok(NULL, ",");
        float mean_travel_time = atof(token);
        // Agregamos los datos a la tabla hash
        int index = hash(sourceid, size);
        if (table[index].sourceid == -1) {
            table[index].sourceid = sourceid;
            table[index].head = malloc(sizeof(Node));
            table[index].head->dstid = dstid;
            table[index].head->hod = hod;
            table[index].head->mean_travel_time = mean_travel_time;
            table[index].head->next = NULL;
        } else {
            Node *current = table[index].head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = malloc(sizeof(Node));
            current->next->dstid = dstid;
            current->next->hod = hod;
            current->next->mean_travel_time = mean_travel_time;
        }
    }

        // Cerramos el archivo csv
fclose(fp);



// Abrir el archivo binario en modo escritura
    FILE *fout = fopen("tabla_hash_nueva.bin", "wb");
    if (fout == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo binario.\n");
        return EXIT_FAILURE;
    }

    // Escribir la tabla hash en el archivo binario
    for (int i = 0; i < size; i++) {
        if (table[i].sourceid != -1) {
            fwrite(&(table[i].sourceid), sizeof(int), 1, fout);
            Node *current = table[i].head;
            while (current != NULL) {
                fwrite(&(current->dstid), sizeof(int), 1, fout);
                fwrite(&(current->hod), sizeof(int), 1, fout);
                fwrite(&(current->mean_travel_time), sizeof(float), 1, fout);
                current = current->next;
            }
            // Escribir un valor centinela (-1) para indicar el final de la lista enlazada
            int end = -1;
            fwrite(&end, sizeof(int), 1, fout);
        }
    }

    // Cerrar el archivo binario
    fclose(fout);
    return 0;


// Abrimos el archivo binario en modo lectura para comprobar que se escribió correctamente
//FILE *fin = fopen("tabla_hash.bin", "rb");
//if (fin == NULL) {
   //fprintf(stderr, "Error: no se pudo abrir el archivo binario.\n");
    //return EXIT_FAILURE;
}

// Cerramos el archivo binario
//fclose(fin);


