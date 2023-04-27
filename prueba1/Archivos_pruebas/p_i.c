#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUF 1024

typedef struct {
    int sourceID;
    int dstID;
    int hod;
    double mean_travel_time;
} Trip;

int main()
{
    // Abrir la tuber�a nombrada para escribir la entrada del usuario
    int fd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);

    // Pedir la entrada del usuario
    int sourceID, dstID, hod;
    printf("Enter the source ID: ");
    scanf("%d", &sourceID);
    printf("Enter the destination ID: ");
    scanf("%d", &dstID);
    printf("Enter the hour of day: ");
    scanf("%d", &hod);

    // Escribir la entrada del usuario en la tuber�a nombrada
    Trip trip;
    trip.sourceID = sourceID;
    trip.dstID = dstID;
    trip.hod = hod;
    write(fd, &trip, sizeof(Trip));
    close(fd);

    // Leer el resultado de la b�squeda de la tuber�a nombrada
    char * myfifo2 = "/tmp/myfifo2";
    mkfifo(myfifo2, 0666);
    fd = open(myfifo2, O_RDONLY);
    double mean_travel_time;
    read(fd, &mean_travel_time, sizeof(double));
    close(fd);

    // Mostrar el resultado de la b�squeda al usuario
    if (mean_travel_time == -1) {
        printf("No data found for the specified criteria.\n");
    } else {
        printf("The mean travel time for the trip from source ID %d to destination ID %d at hour of day %d is %lf\n",
            sourceID, dstID, hod, mean_travel_time);
    }

    // Eliminar las tuber�as nombradas
    unlink(myfifo);
    unlink(myfifo2);

    return 0;
}
