#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 1024
#define TABLE_SIZE 1000000

typedef struct {
    int sourceID;
    int dstID;
    int hod;
    float mean_travel_time;
} Trip;

void search_trip(int sourceID, int dstID, int hod) {
    // Calcula la posición en la tabla hash
    int key = ((sourceID * 31 + dstID) * 31 + hod) % TABLE_SIZE;

    // Abre el archivo binario para lectura
    FILE *fp = fopen("tabla_hash_final.bin", "rb");

    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Busca la posición de la clave
    fseek(fp, key * sizeof(Trip), SEEK_SET);

    // Lee los elementos de la lista enlazada
    int found = 0;
    Trip trip;
    while (fread(&trip, sizeof(Trip), 1, fp)) {
        if (trip.sourceID == sourceID && trip.dstID == dstID && trip.hod == hod) {
            printf("Tiempo de viaje promedio: %.2f\n", trip.mean_travel_time);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("NA\n");
    }

    // Cierra el archivo
    fclose(fp);
}

int main() {
    int fd;
    char * myfifo = "/tmp/myfifo";
    int sourceID, dstID, hod;

    // Crea la tuberia
    mkfifo(myfifo, 0666);

    // Abre la tuberia para lectura
    fd = open(myfifo, O_RDONLY);

    // Lee los datos de la tuberia
    read(fd, &sourceID, sizeof(int));
    read(fd, &dstID, sizeof(int));
    read(fd, &hod, sizeof(int));

    // Cierra la tuberia
    close(fd);

    // Busca el viaje correspondiente
    search_trip(sourceID, dstID, hod);

    // Elimina la tuberia
    unlink(myfifo);

    return 0;
}
