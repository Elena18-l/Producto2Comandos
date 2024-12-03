//Elena López Estremera
//Producto 2. Adquiriendo las destrezas básicas-Programa comandos personalizados para el sistema operativo
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGHT 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hacerPing(const char* direccionIP, const char* archivoSalida);

// Función para obtener la ruta a la carpeta Documentos del usuario
void obtener_ruta_documentosIP(char* ruta_documentos, size_t tamano, const char* nombreArchivo) {
    char* userprofile = getenv("USERPROFILE");
    if (userprofile) {
        snprintf(ruta_documentos, tamano, "%s\\Documents\\%s", userprofile, nombreArchivo);
    }
    else {
        // Ruta relativa por defecto si no se encuentra USERPROFILE
        snprintf(ruta_documentos, tamano, "%s", nombreArchivo);
    }
}

void verificarIPs() {
    char archivoSalida[MAX_LINE_LENGHT];

    // Solicitar el archivo de salida al usuario
    printf("Ingrese el nombre del archivo para guardar los resultados (se guardará en Documentos): ");
    scanf("%s", archivoSalida);

    char rutaArchivoSalida[260];
    obtener_ruta_documentosIP(rutaArchivoSalida, sizeof(rutaArchivoSalida), archivoSalida);

    // Ruta dinámica para el archivo de entrada en Documentos
    char archivoEntrada[260];
    obtener_ruta_documentosIP(archivoEntrada, sizeof(archivoEntrada), "archivoips.txt");

    // Abrir el archivo de entrada
    FILE* fp = fopen(archivoEntrada, "r");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo de entrada");
        exit(1);
    }

    // Leer las IPs línea por línea y hacer ping
    char direccionIP[MAX_LINE_LENGHT];
    while (fgets(direccionIP, MAX_LINE_LENGHT, fp)) {
        // Elimina el salto de línea al final de cada IP
        direccionIP[strcspn(direccionIP, "\n")] = '\0';
        hacerPing(direccionIP, rutaArchivoSalida);
    }

    fclose(fp);
}

void hacerPing(const char* direccionIP, const char* archivoSalida) {
    char command[255] = { 0 };
    char buffer[1024] = { 0 };
    FILE* pipe;

    // Crear el comando de ping
    sprintf(command, "ping -n 1 %s", direccionIP);

    // Ejecutar el comando y leer la salida
    pipe = _popen(command, "r");
    if (!pipe) {
        perror("No se pudo ejecutar el comando ping");
        return;
    }

    // Leer la salida del comando
    int pingExitoso = 0;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        if (strstr(buffer, "TTL=")) {
            pingExitoso = 1;
        }
    }
    _pclose(pipe);

    // Abrir el archivo de salida para insertar los resultados
    FILE* fp = fopen(archivoSalida, "a");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo de salida");
        return;
    }

    // Escribir el resultado en el archivo
    if (pingExitoso) {
        fprintf(fp, "Ping exitoso a %s\n", direccionIP);
    }
    else {
        fprintf(fp, "Ping fallido a %s\n", direccionIP);
    }

    fclose(fp);
}