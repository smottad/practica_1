#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Estructura para representar una fila de la tabla hash
typedef struct {
    int32_t valores[7];
} FilaTabla;

int main() {
    // Abrir los dos archivos binarios
    FILE *file1 = fopen("tabla_hash_corta.bin", "rb");
    FILE *file2 = fopen("tabla_hash.bin", "rb");

    // Leer los datos del primer archivo
    fseek(file1, 0, SEEK_END);
    long num_bytes1 = ftell(file1);
    long num_rows1 = num_bytes1 / sizeof(FilaTabla);
    fseek(file1, 0, SEEK_SET);
    FilaTabla *tabla1 = malloc(num_rows1 * sizeof(FilaTabla));
    fread(tabla1, sizeof(FilaTabla), num_rows1, file1);

    // Leer los datos del segundo archivo
    fseek(file2, 0, SEEK_END);
    long num_bytes2 = ftell(file2);
    long num_rows2 = num_bytes2 / sizeof(FilaTabla);
    fseek(file2, 0, SEEK_SET);
    FilaTabla *tabla2 = malloc(num_rows2 * sizeof(FilaTabla));
    fread(tabla2, sizeof(FilaTabla), num_rows2, file2);

    // Fusionar las dos tablas en una nueva tabla
    long num_rows_fusionada = num_rows1 + num_rows2;
    FilaTabla *tabla_fusionada = malloc(num_rows_fusionada * sizeof(FilaTabla));
    for (int i = 0; i < num_rows1; i++) {
        tabla_fusionada[i] = tabla1[i];
    }
    for (int i = 0; i < num_rows2; i++) {
        tabla_fusionada[num_rows1 + i] = tabla2[i];
    }

    // Escribir la nueva tabla en un archivo binario
    FILE *file_fusionado = fopen("tabla_fusionada.bin", "wb");
    fwrite(tabla_fusionada, sizeof(FilaTabla), num_rows_fusionada, file_fusionado);

    // Liberar la memoria y cerrar los archivos
    free(tabla1);
    free(tabla2);
    free(tabla_fusionada);
    fclose(file1);
    fclose(file2);
    fclose(file_fusionado);

    return 0;
}
