#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 1024

typedef struct {
    int sourceID;
    int dstID;
    int hod;
    double mean_travel_time;
} Trip;
double mean_travel_time;

int main()
{

    int sourceID, dstID, hod;
    int fd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    // Crea la tuberia con permisos de escritura y lectura para todos los usuarios

        int opcion = 0;
        while (opcion != 5) {
        printf("Bienvenido\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
        printf("Ingrese una opción (1-5): ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {

                printf("Ingrese ID del origen: ");
                if(scanf("%d", &sourceID) != 1 || sourceID < 1 || sourceID > 1160) {
                    printf("Entrada inválida. Ingrese un número entero entre 1 y 1160.\n");
                    while(getchar() != '\n');
                    sourceID = 0;
                break;
            }
            case 2: {

                printf("Ingrese ID del destino: ");
                if(scanf("%d", &dstID) != 1 || dstID < 1 || dstID > 1160) {
                    printf("Entrada inválida. Ingrese un número entero entre 1 y 1160.\n");
                    while(getchar() != '\n');
                    dstID = 0;

                break;
            }
            case 3: {

                printf("Ingrese hora del día: ");
                if(scanf("%d", &hod) != 1 || hod < 0 || hod > 23) {
                    printf("Entrada inválida. Ingrese un número entero entre 0 y 23.\n");
                    while(getchar() != '\n');
                    hod = 0;

                break;
            }
            case 4: {

                // Abre la tuberia para escritura
                Trip trip;
                trip.sourceID = sourceID;
                trip.dstID = dstID;
                trip.hod = hod;
                write(fd, &trip, sizeof(Trip));
                // Cierra la tuberia
                close(fd);
                }

            case 5:
                printf("¡Hasta pronto!\n");
                break;
            default:
                printf("Opción inválida\n");
                break;
            }
        }
return 0;
}
}
}
}
