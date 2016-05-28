#include <iostream>
#include <fstream>

#include "tipos.h"
#include "campo.h"
#include "drone.h"
#include "sistema.h"

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

	cout << endl << "Mostrar por pantalla el tipo Producto:" << endl;

	Producto prod1 = Fertilizante;
	Producto prod2 = PlaguicidaBajoConsumo;

	cout << prod1 << endl << prod2 << endl;

	cout << endl << "Campo guardado en \"salidaCampo.txt\"" << endl;

	ofstream campoOut("salidaCampo.txt");
	if(campoOut.is_open()){
		p2.guardar(campoOut);
		campoOut.close();
	}

	cout << endl << endl << "\"Cargar\" un Campo y mostrarlo:" << endl;

	// escribir (o copiar en la consola) por ejemplo
	// { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]}

	Campo p3;
	p3.cargar(cin);
	
	cout << endl << p3;
	

	cout << endl << endl << "\"Cargar\" un Drone y mostrarlo:" << endl;

	Drone d;
	d.cargar(cin);
	cout << endl << d;

	cout << endl << endl << "Drone guardado en \"salidaDrone.txt\"" << endl;

	ofstream droneOut("salidaDrone.txt");
	if(droneOut.is_open()){
		d.guardar(droneOut);
		droneOut.close();
	}

	cout << endl << endl << "\"Cargar\" un Sistema y mostrarlo:" << endl;

	Sistema s;
	s.cargar(cin);
	cout << endl << s;

	cout << endl << "Sistema guardado en \"salidaSistema.txt\"" << endl;

	ofstream sistemaOut("salidaSistema.txt");
	if(sistemaOut.is_open()){
		s.guardar(sistemaOut);
		sistemaOut.close();
	}
	
	cout << endl;

    return 0;
}