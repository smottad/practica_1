#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 1024

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    float value;
    struct Node* next;
};

struct HashTable {
    int size;
    struct Node** table;
};

int hash(int key, int size) {
    return key % size;
}

float search(struct HashTable* ht, int sourceID, int dstID, int hod) {
    int key = sourceID + dstID + hod;
    int index = hash(key, ht->size);
    struct Node* current = ht->table[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1;
}
int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    int sourceID, dstID, hod;

    // Crear la tuberia
    mkfifo(myfifo, 0666);

    // Abre la tuberia para lectura
    fd = open(myfifo, O_RDONLY);

    // Leer los datos de la tuberia
    read(fd, &sourceID, sizeof(int));
    read(fd, &dstID, sizeof(int));
    read(fd, &hod, sizeof(int));

    // Cerrar la tuberia
    close(fd);

// Abre el archivo binario para leer
    FILE* file = fopen("tabla_hash_final.bin", "rb");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo\n");
        exit(1);
    }

    // Lee el tamaño de la tabla hash desde el archivo
    int size;
    fread(&size, sizeof(int), 1, file);

    // Crea la tabla hash con el tamaño leído desde el archivo
    struct HashTable ht = {
        .size = size,
        .table = calloc(size, sizeof(struct Node*))
    };    FILE *fp = fopen("tabla_hash_final.bin", "rb");

    // Lee los pares clave-valor desde el archivo y los agrega a la tabla hash
    int key;
    float value;
    while (fread(&key, sizeof(int), 1, file) == 1) {
        fread(&value, sizeof(float), 1, file);
        struct Node* newNode = malloc(sizeof(struct Node));
        newNode->key = key;
        newNode->value = value;
        newNode->next = ht.table[hash(key, size)];
        ht.table[hash(key, size)] = newNode;
    }

    // Cierra el archivo binario
    fclose(file);


    // Realiza la búsqueda en la tabla hash y muestra el resultado
    float result = search(&ht, sourceID, dstID, hod);
    if (result == -1) {
        printf("No se encontró el valor buscado.\n");
    } else {
        printf("mean_travel_time = %.2f\n", result);
    }

    // Libera la memoria utilizada por la tabla hash
    for (int i = 0; i < ht.size; i++) {
        struct Node* current = ht.table[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht.table);


    // Remove the named pipe
    unlink(myfifo);

    return 0;
}
