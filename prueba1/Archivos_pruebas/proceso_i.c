#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];

    // Crea la tuberia con permisos de escritura y lectura para todos los usuarios
    mkfifo(myfifo, 0666);

    printf("Ingresa el source ID: ");
    fgets(buf, MAX_BUF, stdin);
    int sourceID = atoi(buf);

    printf("Ingresa el destination ID: ");
    fgets(buf, MAX_BUF, stdin);
    int dstID = atoi(buf);

    printf("Ingresa la hora del dia: ");
    fgets(buf, MAX_BUF, stdin);
    int hod = atoi(buf);

    // Abre la tuberia para escritura
    fd = open(myfifo, O_WRONLY);

    // Escribe los datos recibidos hacia la tuberia
    write(fd, &sourceID, sizeof(int));
    write(fd, &dstID, sizeof(int));
    write(fd, &hod, sizeof(int));

    // Cierra la tuberia
    close(fd);

    return 0;
}
