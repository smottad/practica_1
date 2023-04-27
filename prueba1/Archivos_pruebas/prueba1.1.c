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

    // Crear tuber�a de b�squeda
    if (mkfifo(pipe_busqueda, 0666) == -1) {
        perror("Error al crear la tuber�a de b�squeda");
        exit(1);
    }

    // Crear tuber�a de respuesta
    if (mkfifo(pipe_respuesta, 0666) == -1) {
        perror("Error al crear la tuber�a de respuesta");
        exit(1);
    }

    // Abrir tuber�a de b�squeda
    if ((fd = open(pipe_busqueda, O_RDONLY)) == -1) {
        perror("Error al abrir la tuber�a de b�squeda");
        exit(1);
    }

    // Leer datos desde la tuber�a de b�squeda
    if (read(fd, &datos, sizeof(datos_t)) == -1) {
        perror("Error al leer los datos desde la tuber�a de b�squeda");
        exit(1);
    }

    // Cerrar la tuber�a de b�squeda
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

    // Abrir tuber�a de respuesta
    if ((fd = open(pipe_respuesta, O_WRONLY)) == -1) {
        perror("Error al abrir la tuber�a de respuesta");
        exit(1);
    }

    // Enviar los datos de respuesta a la tuber�a
    if (encontrado) {
        if (write(fd, &datos, sizeof(datos_t)) == -1) {
            perror("Error al escribir los datos de respuesta en la tuber�a");
            exit(1);
        }
    } else {
        char respuesta[TAM_PIPE] = "NA";
        if (write(fd, respuesta, strlen(respuesta) + 1) == -1) {
            perror("Error al escribir los datos de respuesta en la tuber�a");
            exit(1);
        }
    }

    // Cerrar la tuber�a de respuesta
    close(fd);

    // Eliminar las tuber�as
    unlink(pipe_busqueda);
    unlink(pipe_respuesta);



    int fd;
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Abrir la tuber�a nombrada para lectura
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Bienvenido a la b�squeda de Tiempo de Viaje Medio.\n");
    printf("Ingrese el ID de Origen: ");
    fgets(message, BUFFER_SIZE, stdin);
    write(STDOUT_FILENO, "Ingrese el ID de Destino: ", strlen("Ingrese el ID de Destino: "));
    fgets(message, BUFFER_SIZE, stdin);
    write(STDOUT_FILENO, "Ingrese la Hora del D�a (0-23): ", strlen("Ingrese la Hora del D�a (0-23): "));
    fgets(message, BUFFER_SIZE, stdin);

    // Enviar la solicitud de b�squeda al proceso de b�squeda
    write(fd, message, strlen(message));

    // Leer la respuesta del proceso de b�squeda
    if (read(fd, response, BUFFER_SIZE) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Mostrar la respuesta al usuario
    if (strcmp(response, "NA\n") == 0) {
        printf("No se encontr� el Tiempo de Viaje Medio solicitado.\n");
    } else {
        printf("El Tiempo de Viaje Medio es: %s", response);
    }

    // Cerrar la tuber�a nombrada y salir
    close(fd);
    return 0;


    return 0;
}
