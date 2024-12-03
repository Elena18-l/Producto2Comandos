#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void obtener_ruta_documentos(char* ruta_documentos, size_t tamano) {
    char* userprofile = getenv("USERPROFILE");
    if (userprofile) {
        snprintf(ruta_documentos, tamano, "%s\\Documents\\temp_ipconfig.txt", userprofile);
    }
    else {
        snprintf(ruta_documentos, tamano, "temp_ipconfig.txt");  // Ruta por defecto si no se encuentra USERPROFILE
    }
}

// Limpiar la cadena (eliminar saltos de línea y espacios extras)
void limpiar_string(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

// Guardar los datos en un archivo
void guardar_datos(const char* ip, const char* mascara, const char* puerta) {
    char ruta_archivo[MAX_PATH];
    obtener_ruta_documentos(ruta_archivo, MAX_PATH);
    snprintf(ruta_archivo, sizeof(ruta_archivo), "%s\\adaptador.txt", getenv("USERPROFILE"));

    FILE* f = fopen(ruta_archivo, "w");
    if (f == NULL) {
        printf("Error al guardar los datos en el archivo.\n");
        return;
    }

    if (strlen(ip) > 0) {
        fprintf(f, "Dirección IP: %s\n", ip);
    }
    if (strlen(mascara) > 0) {
        fprintf(f, "Máscara de red: %s\n", mascara);
    }
    if (strlen(puerta) > 0) {
        fprintf(f, "Puerta de enlace: %s\n", puerta);
    }

    fclose(f);
    printf("Datos guardados en: %s\n", ruta_archivo);
}

// Función principal
void adaptadoresRed() {
    char ruta_temp[MAX_PATH];
    obtener_ruta_documentos(ruta_temp, MAX_PATH);

    printf("Ruta temporal utilizada: %s\n", ruta_temp);

    // Ejecutar ipconfig y redirigir salida al archivo temporal
    char comando[MAX_PATH + 20];
    snprintf(comando, sizeof(comando), "ipconfig > \"%s\"", ruta_temp);
    int resultado = system(comando);

    if (resultado != 0) {
        printf("Error al ejecutar ipconfig.\n");
        return;
    }

    FILE* fp = fopen(ruta_temp, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo temporal");
        return;
    }

    char line[256];
    char adaptadores[10][256];
    int adaptador_count = 0;

    // Leer líneas del archivo y buscar adaptadores
    while (fgets(line, sizeof(line), fp)) {
        limpiar_string(line);
        if (strstr(line, "Adaptador") || strstr(line, "adapter")) {
            if (adaptador_count < 10) {
                strcpy(adaptadores[adaptador_count], line);
                printf("[%d] %s\n", adaptador_count + 1, line);
                adaptador_count++;
            }
        }
    }

    if (adaptador_count == 0) {
        printf("No se detectaron adaptadores de red.\n");
        fclose(fp);
        return;
    }

    int opcion;
    printf("\nSeleccione un adaptador (1-%d): ", adaptador_count);
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > adaptador_count) {
        printf("Opción no válida.\n");
        fclose(fp);
        return;
    }

    rewind(fp);
    char* adaptador_seleccionado = adaptadores[opcion - 1];
    char ip[20] = { 0 }, mascara[20] = { 0 }, puerta[20] = { 0 };
    int estado_ip = 0, estado_mascara = 0, estado_puerta = 0;

    while (fgets(line, sizeof(line), fp)) {
        limpiar_string(line);
        if (strstr(line, adaptador_seleccionado)) {
            while (fgets(line, sizeof(line), fp)) {
                limpiar_string(line);
                if (strstr(line, "IPv4") && !estado_ip) {
                    sscanf(line, "%*[^:]: %19s", ip);
                    estado_ip = 1;
                }
                else if (strstr(line, "subred") && !estado_mascara) {
                    sscanf(line, "%*[^:]: %19s", mascara);
                    estado_mascara = 1;
                }
                else if (strstr(line, "Puerta de enlace predeterminada") && !estado_puerta) {
                    sscanf(line, "%*[^:]: %19s", puerta);
                    estado_puerta = 1;
                }

                if (estado_ip && estado_mascara && estado_puerta) {
                    break;
                }
            }
            break;
        }
    }

    fclose(fp);
    remove(ruta_temp);

    if (strlen(ip) > 0 || strlen(mascara) > 0 || strlen(puerta) > 0) {
        guardar_datos(ip, mascara, puerta);
    }
    else {
        printf("No se encontraron detalles para el adaptador seleccionado.\n");
    }
}