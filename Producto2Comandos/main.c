//Elena López Estremera
//Producto 2. Adquiriendo las destrezas básicas-Programa comandos personalizados para el sistema operativo
//Bibliotecas necesarias
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//Declaramos las funciones de los demás módulos
void InsertarFyH();
void verificarIPs();
void adaptadoresRed();
void MenuPrincipal();
//Función del main
int main() {
	int opcion;
	do {
		MenuPrincipal();//mostramos menú principal
		scanf("%d", &opcion);
		switch (opcion)
		{
		case 1:
			InsertarFyH();//insertamos fecha y hora en un txt
			break;

		case 2:
			verificarIPs();//comparamos IPs a través de pings y lo guardamos en el mismo txt que la funcion anterior
			break;
		case 3:
			adaptadoresRed();//elegimos un adaptador y printamos y guardamos en un txt algunas de sus caracterísicas concretos como la IP
			break;
		}
	} while (opcion != 4);
	return 0;
}

