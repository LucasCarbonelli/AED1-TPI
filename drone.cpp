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
	return this->_enVuelo == true && escalerado(this->_trayectoria); 
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
	return Secuencia<InfoVueloCruzado>();
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
	return true  ;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	return os;
}


//auxiliares


bool escalerado (const Secuencia<Posicion> ps ) {
	int i = 0 ;
	bool val = true ; 
	while ( val == true && i < ps.size() -1 ){
		if (ps[i].x - ps[i+2].x ==1 || ps[i].x - ps[i+2].x == -1 ) {
			if (ps[i].y - ps[i+2].y ==1 || ps[i].y - ps[i+2].y == -1 ){
				val = true; 
				i = i+1;
			}
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
