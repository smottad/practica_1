#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    int idOrigen;
    int idDestino;
    int hora;
    float tiempoPromedio;
} Registro;

int main() {
    // Men� principal
    int opcion = 0;
    while (opcion != 5) {
        printf("Bienvenido\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
        printf("Ingrese una opci�n (1-5): ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                int origen;
                printf("Ingrese ID del origen: ");
                scanf("%d", &origen);
                // TODO: Validar el ID del origen
                break;
            }
            case 2: {
                int destino;
                printf("Ingrese ID del destino: ");
                scanf("%d", &destino);
                // TODO: Validar el ID del destino
                break;
            }
            case 3: {
                int hora;
                printf("Ingrese hora del d�a: ");
                scanf("%d", &hora);
                // TODO: Validar la hora del d�a
                break;
            }
            case 4: {

    int idOrigen, idDestino, hora;
    printf("Ingrese el ID de origen: ");
    scanf("%d", &idOrigen);
    printf("Ingrese el ID de destino: ");
    scanf("%d", &idDestino);
    printf("Ingrese la hora: ");
    scanf("%d", &hora);

    FILE *archivo = fopen("tabla_hash.bin", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    Registro registro;
    while (fread(&registro, sizeof(Registro), 1, archivo) == 1) {
        if (registro.idOrigen == idOrigen && registro.idDestino == idDestino && registro.hora == hora) {
            printf("Tiempo de viaje promedio: algo pues");
            fclose(archivo);
            return 0;
        }
    }

    printf("NA\n");
    fclose(archivo);
    return 0;


            case 5:
                printf("�Hasta pronto!\n");
                break;
            default:
                printf("Opci�n inv�lida\n");
                break;
        }
    }
    return 0;
}
}
