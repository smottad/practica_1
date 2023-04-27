#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "datos.h.txt"

#define TAM_PIPE 100

int main() {
    int fd;
    char *pipe_busqueda = "/tmp/pipe_busqueda";
    char *pipe_respuesta = "/tmp/pipe_respuesta";
    struct stat sb;
    datos_t datos;
    int encontrado = 0;

    // Crear tubería de búsqueda
    if (mkfifo(pipe_busqueda, 0666) == -1) {
        perror("Error al crear la tubería de búsqueda");
        exit(1);
    }

    // Crear tubería de respuesta
    if (mkfifo(pipe_respuesta, 0666) == -1) {
        perror("Error al crear la tubería de respuesta");
        exit(1);
    }

    // Abrir tubería de búsqueda
    if ((fd = open(pipe_busqueda, O_RDONLY)) == -1) {
        perror("Error al abrir la tubería de búsqueda");
        exit(1);
    }

    // Leer datos desde la tubería de búsqueda
    if (read(fd, &datos, sizeof(datos_t)) == -1) {
        perror("Error al leer los datos desde la tubería de búsqueda");
        exit(1);
    }

    // Cerrar la tubería de búsqueda
    close(fd);

    // Abrir archivo binario
    FILE *archivo = fopen("datos.bin", "rb");
    if (!archivo) {
        perror("Error al abrir el archivo binario");
        exit(1);
    }

    // Buscar los datos
    while (fread(&datos, sizeof(datos_t), 1, archivo) == 1) {
        if (datos.sourceid == datos.sourceid_buscado &&
            datos.dstid == datos.dstid_buscado &&
            datos.hod == datos.hod_buscado) {
            encontrado = 1;
            break;
        }
    }

    // Cerrar el archivo binario
    fclose(archivo);

    // Abrir tubería de respuesta
    if ((fd = open(pipe_respuesta, O_WRONLY)) == -1) {
        perror("Error al abrir la tubería de respuesta");
        exit(1);
    }

    // Enviar los datos de respuesta a la tubería
    if (encontrado) {
        if (write(fd, &datos, sizeof(datos_t)) == -1) {
            perror("Error al escribir los datos de respuesta en la tubería");
            exit(1);
        }
    } else {
        char respuesta[TAM_PIPE] = "NA";
        if (write(fd, respuesta, strlen(respuesta) + 1) == -1) {
            perror("Error al escribir los datos de respuesta en la tubería");
            exit(1);
        }
    }

    // Cerrar la tubería de respuesta
    close(fd);

    // Eliminar las tuberías
    unlink(pipe_busqueda);
    unlink(pipe_respuesta);



    int fd;
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Abrir la tubería nombrada para lectura
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Bienvenido a la búsqueda de Tiempo de Viaje Medio.\n");
    printf("Ingrese el ID de Origen: ");
    fgets(message, BUFFER_SIZE, stdin);
    write(STDOUT_FILENO, "Ingrese el ID de Destino: ", strlen("Ingrese el ID de Destino: "));
    fgets(message, BUFFER_SIZE, stdin);
    write(STDOUT_FILENO, "Ingrese la Hora del Día (0-23): ", strlen("Ingrese la Hora del Día (0-23): "));
    fgets(message, BUFFER_SIZE, stdin);

    // Enviar la solicitud de búsqueda al proceso de búsqueda
    write(fd, message, strlen(message));

    // Leer la respuesta del proceso de búsqueda
    if (read(fd, response, BUFFER_SIZE) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Mostrar la respuesta al usuario
    if (strcmp(response, "NA\n") == 0) {
        printf("No se encontró el Tiempo de Viaje Medio solicitado.\n");
    } else {
        printf("El Tiempo de Viaje Medio es: %s", response);
    }

    // Cerrar la tubería nombrada y salir
    close(fd);
    return 0;


    return 0;
}
