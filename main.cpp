#include <iostream>

#include "tipos.h"
#include "campo.h"

using namespace std;

int main()
{
	Posicion g = {7, 3};
	Posicion c = {5, 10};
//	Posicion g2 = {5, 1};
//	Posicion c2 = {2, 3};
	Campo p0(g, c);
	Campo p1(g, c);

	Campo p2; // Constructor sin parametros

	cout << "Mostrar por pantalla el campo p0: " << endl;

	p0.mostrar(cout);

	cout << endl << "Mostrar por pantalla el tipo Campo:" << endl;

	cout << p2;

	cout << endl << "comparando campos con ==" << endl;

	if(p0 == p1){
		cout << "p0 y p1 son iguales!" << endl;
	} else {
		cout << "p0 y p1 NO son iguales!" << endl;
	}

	if(p0 == p2){
		cout << "p0 y p2 son iguales!" << endl;
	} else {
		cout << "p0 y p2 NO son iguales!" << endl;
	}

	cout << endl << "Mostrar por pantalla el tipo Parcela:" << endl;

	Parcela parc = Casa;
	Parcela parg = Granero;

	cout << parc << endl << parg << endl;

	cout << endl << "\"guadar un campo\":" << endl;

	p2.guardar(cout);

	cout << endl << endl << "\"Cargar\" un Campo y mostrarlo:" << endl;

	// escribir (o copiar en la consola) por ejemplo
	// { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]}

	Campo p3;
	p3.cargar(cin);
	
	cout << endl << p3;

    return 0;
}