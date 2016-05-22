#include "drone.h"



Drone::Drone()
{
	this->_id = 1;
	this->_bateria = 100 ; 
	Secuencia<Posicion> pos ; 
	this->_trayectoria = pos  ;
	Secuencia <Producto> productos ;
	this->_productos = productos;
	this-> _enVuelo = false;
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
  	this->_id = i;
	this->_bateria = 100 ; 
	Secuencia<Posicion> pos ; 
	this->_trayectoria = pos  ;
	this->_productos = ps ;
	this-> _enVuelo = false;
}

ID Drone::id() const
{
	return this->_id;
}

Carga Drone::bateria() const
{
	return this->_bateria;
}

bool Drone::enVuelo() const
{
	return this->_enVuelo;
}

const Secuencia<Posicion>& Drone::vueloRealizado() const
{
	return this->_trayectoria;
}

Posicion Drone::posicionActual() const
{

	return this->_trayectoria [this->_trayectoria.size() -1 ];
}

const Secuencia<Producto>& Drone::productosDisponibles() const
{
	return this->_productos;
}

bool Drone::vueloEscalerado() const
{
	return this->_enVuelo && escalerado(this->_trayectoria); 
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
	Secuencia<InfoVueloCruzado> ListaInfoVC;
	int i = 0;
	int j = 0;
	while (i < ds.size() && j < ds[i].vueloRealizado().size()) {
		if (cantidadDronesCruzados(ds[i].vueloRealizado()[j], ds) > 1 && buscarInfoVuelosCruzados(ListaInfoVC, ds[i].vueloRealizado()[j]))
		{
			InfoVueloCruzado VC;
			VC.posicion = ds[i].vueloRealizado()[j];
			VC.cantidadCruces = cantidadDronesCruzados(ds[i].vueloRealizado()[j], ds);
			ListaInfoVC.push_back(VC);
		}
		i++;
		if (i == ds.size())
		{
			j++;
			i = 0;
		}
	}

	return ListaInfoVC;
}

void Drone::mostrar(std::ostream & os) const
{
}

void Drone::guardar(std::ostream & os) const
{
}

void Drone::cargar(std::istream & is)
{
}

bool Drone::operator==(const Drone & otroDrone) const
{
	return this->_id == otroDrone_id && this->_trayectoria == otroDrone_trayectoria && this-> _bateria == otroDrone_bateria && this->_enVuelo == otroDrone_enVuelo && mismos(this->_productos , otroDrone_Productos)   ;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	return os;
}


//auxiliares


bool escalerado (const Secuencia<Posicion> ps ) {
	int i = 0 ;
	bool val = true ; 
	while ( val == true && i < ps.size() -2 ){
		if ((ps[i].x - ps[i+2].x ==1 || ps[i].x - ps[i+2].x == -1 ) && (ps[i].y - ps[i+2].y ==1 || ps[i].y - ps[i+2].y == -1 )) {
			i = i+1;
		}
		else 
			val = false;
	}
	return val;
}

bool seCruzoConOtro(Drone d, Secuencia<Drone> ds, int i) {
	int j=0; 
	bool seCruzo = false ; // inicializo una variable que nos dice si hasta el momento un drone se cruzo con el drone del parametro
	while (j < ds.size()){
		if  ((d== ds[j]) == false && d.vueloRealizado()[i].x == ds[j].vueloRealizado()[i].x && d.vueloRealizado()[i].y == ds[i].vueloRealizado()[j].y){
			seCruzo = true; // si se cruzo con otro drone distinto en la posicion i-esima entonces cambiamos la variable a true
			j=j+1;
		}
		else j=j+1;
	}
	return seCruzo; 
}


int cantidadDronesCruzados(const Posicion p , const Secuencia<Drone> ds) {
	Secuencia<Drone> dronesCruzados ; // inicializo una secuencia vacia que se llama dronesCruzados y aca vamos a guardar los drones que se cruzaron. 
	
	int i = 0; // Con la variable i recorro la lista de drones ds.

	while (i < ds.size()){
		int j = 0; // Con la variable j recorro el vuelo realizado de los drones.

		while ( j < ( ds[i].vueloRealizado()).size() ){
			
			if (ds[i].vueloRealizado()[j].x == p.x && ds[i].vueloRealizado()[j].y == p.y && seCruzoConOtro(ds[i], ds, j) ){
				dronesCruzados.push_back(ds[i]); // si el drone que estoy mirando se cruzo con otro en la posicion p entonces lo agrego a la secuencia
				j = j+1 ;


			}
			else j = j+1;

		}

		i= i+1 ;
	}

	return dronesCruzados.size();
} 


Secuencia<Posicion> posConCruces( const Secuencia<Drone> ds) {
	int j = 0;
	Secuencia<Posicion> posicionesC;
	while (j < ds.size()) {
		int i = 0;

		while ( i < ds[j].vueloRealizado().size()) {
			if (seCruzoConOtro(ds[j], ds, i))
			 {
			 	posicionesC.push_back(ds[j].vueloRealizado()[i]);
			 }
			i++;
		}
		j++;
	}
	return posicionesC;
}


bool mismos (const Secuencia<T> l1 , const Secuencia<T> l2) {
	if (l1.size() == l2.size()){
		bool estan= true;
		int i=0;
		while (estan == true && i<length (l1.size()){

			if(cuenta(l1 , l1[i]) == cuenta (l2 , l1[i])){
				i=i+1;
			}
			else estan ==false ;

		return estan;
		}
	}
}

			




int cuenta (const Secuencia <T> l1 , const T e){
	int i = 0;
	int cuenta = 0;
	while (i<l1.size()){
		if (l1[i] == e){
			i=i+1;
			cuenta = cuenta + 1 ; 
		}
		else i = i+1;
	} 
	return cuenta ;
}

//hacer eliminarRepetidos