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

	cout << endl << "El vuelo es escalerado?" << d.vueloEscalerado() << endl;

	cout << endl << endl << "Drone guardado en \"salidaDrone.txt\"" << endl;

	ofstream droneOut("salidaDrone.txt");
	if(droneOut.is_open()){
		d.guardar(droneOut);
		droneOut.close();
	}

	cout << endl << endl << "\"Cargar\" un Sistema y mostrarlo:" << endl;

	Sistema s;
	cout << "falla aca??";
	s.cargar(cin);
	cout << endl << s;

	cout << endl << "Sistema guardado en \"salidaSistema.txt\"" << endl;

	ofstream sistemaOut("salidaSistema.txt");
	if(sistemaOut.is_open()){
		s.guardar(sistemaOut);
		sistemaOut.close();
	}
	
	cout << endl;

	cout << "Ahora vamos a cargar el campo y el drone que pasaste en un nuevo sistema! que divertido!" << endl;
	
	Secuencia<Drone> ds;
	ds.push_back(d);
	Sistema sistemaDivertido(p3, ds);

	sistemaDivertido.mostrar(cout);

	

	Secuencia<Drone>::size_type i = 0;
	while (i < sistemaDivertido.enjambre().size()) {
		sistemaDivertido.enjambre()[i].mostrar(cout);
		i++;
	}

	int l = 0;
	cout << "elegi lo que queres hacer papurri" << endl;
	cin >> l;

	if (l == 1)
	{
		sistemaDivertido.campo().mostrar(cout);
		sistemaDivertido.campo().crecer();
		sistemaDivertido.campo().mostrar(cout);
	}

	if (l == 2)
	{
		cout << "ingresa lista de parcelas: " << endl;
		Secuencia<Posicion> ps;
		cin >> ps;
		sistemaDivertido.seVinoLaMaleza(ps);
		sistemaDivertido.campo().mostrar(cout);
	}

	if (l == 3)
	{
		sistemaDivertido.campo().seExpandePlaga();
		sistemaDivertido.campo().mostrar(cout);
	}

	if (l == 4)
	{
		int n = -1;
		cout << "ingresa el drone que te gusta: " << endl;
		cin >> n;
		Drone D = sistemaDivertido.enjambreDrones()[n];
		sistemaDivertido.despegar(D);
		
	}

	if (l == 5)
	{
		/* code */
	}




/*	int tipoDeDato = 0;
	cout << "qué queres toquetear?" << endl;
	cin >> tipoDeDato;
	if (tipoDeDato == 1)
	{
		//con Campo
		cout << "qué queres hacer?" << endl;
		string A;
		cin >> A;
		cout << campo.A();
	} */
	/*if (tipoDeDato == 2)
	{
		//con Drone
		cout << "Ingresa lo que queres hacer YA: " << endl;
		int metodo = 0;
		cin >> metodo;
		switch(metodo) {
			case metodo == 1:	cout << d.id() << endl;
			break;
			case metodo == 2:	cout << d.bateria() << endl;
			break;
			case metodo == 3:	cout << d.enVuelo() << endl;
			break;
			case metodo == 4:	cout << d.vueloRealizado() << endl;
			break;
			case metodo == 5:	cout << d.posicionActual() << endl;
			break;
			case metodo == 6:	cout << d.productosDisponibles() << endl;
			break;
			case metodo == 7:	cout << d.vueloEscalerado() << endl;
			break;
			case metodo == 8:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 9:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 10:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 11:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 12:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 13:	cout << d.vuelosCruzados() << endl;
			break;
		}
	}*/
	/*if (tipoDeDato == 3)
	{
		cout << "Ingresa lo que queres hacer YA: " << endl;
		int metodo = 0;
		cin >> metodo;
		switch(metodo) {
			case metodo == 1:	cout << d.id() << endl;
			break;
			case metodo == 2:	cout << d.bateria() << endl;
			break;
			case metodo == 3:	cout << d.enVuelo() << endl;
			break;
			case metodo == 4:	cout << d.vueloRealizado() << endl;
			break;
			case metodo == 5:	cout << d.posicionActual() << endl;
			break;
			case metodo == 6:	cout << d.productosDisponibles() << endl;
			break;
			case metodo == 7:	cout << d.vueloEscalerado() << endl;
			break;
			case metodo == 8:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 9:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 10:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 11:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 12:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 13:	cout << d.vuelosCruzados() << endl;
			break;
		}
	}*/





    return 0;
}