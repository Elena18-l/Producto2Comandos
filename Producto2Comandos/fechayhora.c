//Elena López Estremera
//Producto 2. Adquiriendo las destrezas básicas-Programa comandos personalizados para el sistema operativo
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Función para obtener la ruta a la carpeta Documentos del usuario
void obtener_ruta_documentosFyH(char* ruta_documentos, size_t tamano) {
    char* userprofile = getenv("USERPROFILE");
    if (userprofile) {
        snprintf(ruta_documentos, tamano, "%s\\Documents\\producto2.txt", userprofile);
    }
    else {
        // Si USERPROFILE no está disponible, usar una ruta relativa
        snprintf(ruta_documentos, tamano, "producto2.txt");
    }
}

// Función para insertar la fecha y la hora en un archivo
void InsertarFyH() {
    char rutaArchivo[260];
    obtener_ruta_documentosFyH(rutaArchivo, sizeof(rutaArchivo));

    // Generamos el archivo
    FILE* fp = fopen(rutaArchivo, "w");
    if (fp != NULL) {
        // Obtenemos la fecha y hora locales
        time_t tiempoAhora = time(NULL);
        struct tm* hora_local = localtime(&tiempoAhora);

        // Insertamos la fecha y hora locales
        fprintf(fp, "Hora actual: %02d:%02d:%02d\n", hora_local->tm_hour, hora_local->tm_min, hora_local->tm_sec);

        // Cerramos el archivo
        fclose(fp);

        // Mensaje de confirmación
        printf("Se ha insertado correctamente la fecha y la hora en: %s\n", rutaArchivo);
    }
    else {
        // Mostramos mensaje de error en caso que sea nulo
        perror("El archivo no se puede crear");
    }
}
