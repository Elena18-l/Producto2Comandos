//Elena L�pez Estremera
//Producto 2. Adquiriendo las destrezas b�sicas-Programa comandos personalizados para el sistema operativo
//Bibliotecas necesarias
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//Declaramos las funciones de los dem�s m�dulos
void InsertarFyH();
void verificarIPs();
void adaptadoresRed();
void MenuPrincipal();
//Funci�n del main
int main() {
	int opcion;
	do {
		MenuPrincipal();//mostramos men� principal
		scanf("%d", &opcion);
		switch (opcion)
		{
		case 1:
			InsertarFyH();//insertamos fecha y hora en un txt
			break;

		case 2:
			verificarIPs();//comparamos IPs a trav�s de pings y lo guardamos en el mismo txt que la funcion anterior
			break;
		case 3:
			adaptadoresRed();//elegimos un adaptador y printamos y guardamos en un txt algunas de sus caracter�sicas concretos como la IP
			break;
		}
	} while (opcion != 4);
	return 0;
}

