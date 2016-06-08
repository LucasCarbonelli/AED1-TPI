#include <iostream>
#include <fstream>

#include "tipos.h"
#include "campo.h"
#include "drone.h"
#include "sistema.h"

using namespace std;

int main()
{

/*	//COSAS DE CARGAR CAMPO, NI IDEA QUE ES ESTO, CREO QUE SON PRUEBAS O QUE SE YO

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


	//CARGAR CAMPO

	Campo p3;
	p3.cargar(cin);
	
	cout << endl << p3;
	

	cout << endl << endl << "\"Cargar\" un Drone y mostrarlo:" << endl;

	Drone d;
	d.cargar(cin);
	cout << endl << d;

	if(!(d.enVuelo() && d.vueloRealizado().size() == 0)){
		cout << endl << "El vuelo es escalerado?" << d.vueloEscalerado() << endl;	
	}

	cout << endl << endl << "Drone guardado en \"salidaDrone.txt\"" << endl;

	ofstream droneOut("salidaDrone.txt");
	if(droneOut.is_open()){
		d.guardar(droneOut);
		droneOut.close();
	}

	//CREAR SISTEMA CON CAMPO Y DRONE PASADOS RECIEN

	cout << "Ahora vamos a cargar el campo y el drone que pasaste en un nuevo sistema! que divertido!" << endl;
	
	Secuencia<Drone> ds;
	ds.push_back(d);
	Sistema sistemaDivertido(p3, ds);

	sistemaDivertido.mostrar(cout);

	

	Secuencia<Drone>::size_type i = 0;
	while (i < sistemaDivertido.enjambreDrones().size()) {
		sistemaDivertido.enjambreDrones()[i].mostrar(cout);
		i++;
	}

	//CARGAR SISTEMA (esto ya no esta relacionado con lo hecho anteriormente)

	cout << endl << endl << "\"Cargar\" un Sistema y mostrarlo:" << endl;

	Sistema s;
//	cout << "falla aca??";
	s.cargar(cin);
	cout << endl << s;

	cout << endl << "Sistema guardado en \"salidaSistema.txt\"" << endl;

	ofstream sistemaOut("salidaSistema.txt");
	if(sistemaOut.is_open()){
		s.guardar(sistemaOut);
		sistemaOut.close();
	}
	
	cout << endl;


	//PRUEBAS CON METODOS SOBRE EL SISTEMA CARGADO ANTERIORMENTE

	int l = 0;
	cout << endl << "elegi lo que queres hacer con el sistema que pasaste recien, papurri" << endl;
	cin >> l;

	if (l == 1)
	{
		//da Violación de segmento
		s.crecer();
		s.mostrar(cout);
	}

	if (l == 2)
	{
		cout << "ingresa lista de parcelas, para eso ingresa la cantidad de parcelas: " << endl;
		Secuencia<Posicion> ps;
		Secuencia<Posicion>::size_type n;
		cin >> n;
		Secuencia<Posicion>::size_type i = 0;
		while ( i < n) {
			Posicion p;
			int x;
			int y;
			cin >> x >> y;
			p.x = x;
			p.y = y;
			ps.push_back(p);
			i++;
		}
		s.seVinoLaMaleza(ps);
		s.mostrar(cout);
	}

	if (l == 3)
	{
		//da Violación de segmento
		s.seExpandePlaga();
		s.mostrar(cout);
	}

	if (l == 4)
	{
		//no hace lo que tendría que hacer: modificar algún drone de enjambre, el que yo le diga. De hecho, tendrías que pasarle un sistema
		//donde el drone que le hagas despegar esté en el granero, sino tampoco tiene mucho sentido. Igual, esto emula el dron que vos le
		//decis, y de ahi le hace despegar... cómo se traga que es s. de un drone que no esta en enjambre? ni idea
		//--> osea claro, es un requiere que esté, si vos pasas fruta, por ahi el programa te devuelve algo, que es fruta en todo caso...
		//igual sigue el problema de que no modifica un drone de enjambre...
		int n = -1;
		cout << "ingresa el drone que te gusta: " << endl;
		cin >> n;
		Drone D(s.enjambreDrones()[n].id(), s.enjambreDrones()[n].productosDisponibles());
		s.despegar(D);
		D.mostrar(cout);
		s.mostrar(cout);
		cout << endl;		
	}

	if (l == 5)
	{
		// code //
	}
*/
            ///////////////////////////////////////////////////pruebas de Drone////////////////////////////////////////
	/*Secuencia<Drone> ds;

	for (Secuencia<Drone>::size_type i = 0; i < 4; ++i)
	{
		cout << endl << endl << "\"Cargar\" un Drone y mostrarlo:" << endl;

		Drone d;
		d.cargar(cin);
		cout << endl << d;
		ds.push_back(d);

		cout << endl << "El vuelo es escalerado?" << d.vueloEscalerado() << endl;

	}

	Secuencia<InfoVueloCruzado> vuelos;
	vuelos = Drone::vuelosCruzados(ds);

	cout << endl << "vuelosCruzados: " << "[";

	for (Secuencia<InfoVueloCruzado>::size_type i = 0; i < vuelos.size(); ++i)
	{
		cout<< "[" << "("<< vuelos[i].posicion.x << ","<<  vuelos[i].posicion.y << ")" << ","<< vuelos[i].cantidadCruces << "]";
	}

	cout << endl;

	cout << "cuantos hay" <<vuelos.size() <<endl ;



    return 0;

*/   
     //////////////////////////////////////////////////////Pruebas de Sistema/////////////////////////////////////////////////////////////////////

    cout << "Pruebas de Sistema" << endl;
    cout << "0 igualdad entre sistemas" << endl;
    cout << "1 void crecer()" << endl;
    cout << "2 void fertilizarPorFilas()" << endl;
    cout << "3 void aterrizarYCargarBaterias(Carga b)" << endl;
    cout << endl;
    cout << "Pruebas de Campo" << endl;
    cout << "4 cargar/mostrar/guardar" << endl;

    int nroFuncion;
    cout << endl << "¿Qué función quiere probar? ";
    cin >> nroFuncion;

    switch(nroFuncion){
    	case 0:	{
			// Igualdad de Sistemas
			cout << "Igualdad de Sistemas" << endl;
			cout << "\"Cargar\" un Sistema y mostrarlo:" << endl;

			Sistema s1;
			s1.cargar(cin);
			cout << endl << s1 << endl;

			cout << "\"Cargar\" otro Sistema y mostrarlo:" << endl;
			Sistema s2;
			s2.cargar(cin);
			cout << endl << s2 << endl;

			if(s1 == s2){
				cout << endl << "Los dos sistemas cargados son iguales" << endl;
			} else {
				cout << endl << "Los dos sistemas cargados son distintos" << endl;
			}
			break;
		}

		case 1:	{
			// void crecer()
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

			s.crecer() ;
			cout <<endl << " CRECER "<< endl ;
			s.mostrar(cout);

			cout << endl;
			break;
		}

		case 2:	{
			// void fertilizarPorFilas()
			cout << endl << endl << "\"Cargar\" un Sistema y mostrarlo:" << endl;

			Sistema s;
			s.cargar(cin);
			cout << endl << s;

			s.fertilizarPorFilas();
			cout << endl << s;
			break;
		}

		case 3: {
			// void aterrizarYCargarBaterias(Carga b)
			cout << endl << endl << "\"Cargar\" un Sistema y mostrarlo:" << endl;

			Sistema s;
			s.cargar(cin);
			cout << endl << s;

			Carga b;
			cout << endl << "Ingrese b: ";
			cin >> b;
			cout << endl;

			s.aterrizarYCargarBaterias(b);
			cout << endl << s;
			break;
		}

		case 4: {
			cout << endl << endl << "\"Cargar\" un Campo y mostrarlo:" << endl;

			Campo c;
			c.cargar(cin);
			cout << endl << c;

			cout << endl << "Campo guardado en \"salidaCampo.txt\"" << endl;

			ofstream campoOut("salidaCampo.txt");
			if(campoOut.is_open()){
				c.guardar(campoOut);
				campoOut.close();
			}
			break;
		}
    }
}