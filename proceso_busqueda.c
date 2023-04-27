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

struct Record {
    int key;
    float value;
};

struct HashTable {
    int size;
};

int hash(int key, int size) {
    return key % size;
}

float search(struct HashTable* ht, FILE* file, int sourceID, int dstID, int hod) {
    int key = sourceID + dstID + hod;
    int index = hash(key, ht->size);
    fseek(file, sizeof(int), SEEK_SET);
    struct Record current;
    for (int i = 0; i < index; i++) {
        fseek(file, sizeof(struct Record) * ht->size, SEEK_CUR);
    }
    while (fread(&current, sizeof(struct Record), 1, file)) {
        if (current.key == key) {
            return current.value;
        }
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

 // Lee el tamaño de la tabla hash del archivo binario
    int size;
    fread(&size, sizeof(int), 1, file);

    // Crea la tabla hash
    struct HashTable ht = {
        .size = size
    };


    // Busca el valor correspondiente en la tabla hash
    float result = search(&ht, file, sourceID, dstID, hod);
    if (result == -1) {
        printf("No se encontró un valor para esa clave\n");
    } else {
        printf("El valor correspondiente es: %f\n", result);
    }

    // Cierra el archivo binario
    fclose(file);


    // Remove the named pipe
    unlink(myfifo);

    return 0;
}
