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
	//Cabecera: Drone, Bateria y Id.
	os << "Drone - Id: " << std::to_string(this->_id) << "\n";
	os << "Bateria: "std::to_string(this->_bateria) << "\n";
	// Listado de posiciones.
	os << "Vuelo Realizado: ";
	if (this->_trayectoria.size() == 0) then {
		os << "[]";	
	} else {
		os << "[";
		os << "(" << std::to_string(this->_trayectoria[0].x) << "," << std::to_string(this->_trayectoria[0].y) << ")";
		for(int i = 1; i < this->_trayectoria.size(); i++){
			os << ", (";
			os << std::to_string(this->_trayectoria[i].x) << "," << std::to_string(this->_trayectoria[i].y);
			os << ")";
			}
		os << "]";
	}
	os << "\n";

	// Listado de productos.
	os << "Productos Disponibles: ";
	if (this->_productos.size() == 0) then {
		os << "Ninguno.";	
	} else {
		os << std::to_string(this->productos[0]);
		for(int i = 1; i < this->_productos.size(); i++){
			os << "," << std::to_string(this->productos[i]);
		}
	}

}

void Drone::guardar(std::ostream & os) const
{
	//Cabecera: Drone, Id y Bateria.
	os << "{ D "  << std::to_string(this->_id) << " " << std::to_string(this->_bateria) << " " ;

	// Listado de posiciones.
	if (this->_trayectoria.size() == 0) then {
		os << "[]";	
	} else {
		os << "[";
		os << "[" << std::to_string(this->_trayectoria[0].x) << "," << std::to_string(this->_trayectoria[0].y) << "]";
		for(int i = 1; i < this->_trayectoria.size(); i++) {
			os << ",[";
			os << std::to_string(this->_trayectoria[i].x) << "," << std::to_string(this->_trayectoria[i].y);
			os << "]";
		}
		os << "]";
	}
	// Listado de productos.
	if (this->_productos.size() == 0) then {
		os << "[]";	
	} else {
		os << "[";
		os << std::to_string(this->productos[0])
		for(int i = 1; i < this->_productos.size(); i++){
			os << "," << std::to_string(this->productos[i]) << ","
		}
		os << "]";
	}

	os << "}";
}

void Drone::cargar(std::istream & is)
{
	const std::string numeros = "0123456789";
	const std::string productoLetraInicial = "PFH";
	const std::string caracterEspacio = " ";
	const std::string caracterAbreCorchete = "[";
	const std::string caracterCierraCorchete = "]";
	const std::string caracterCierraCorcheteDoble = "]]";
	const std::string caracterPosteriorPrimerNumero = ",";
	const std::string caracterPosteriorProducto = ",]";
	std::string droneAlmacenado;
	std::string::size_type i, j;

	std::getline(is, droneAlmacenado);

	//Busco el primer numero y el espacio que le sigue, para identificar el Id.
	i = 0;
	i = droneAlmacenado.find_first_of(numeros, i);
	j = droneAlmacenado.find(caracterEspacio, i);
	this->_id = std::stoi(droneAlmacenado.substr(i, j-i));

	//Busco el numero siguiente y el proximo espacio, para identificar la carga de la bateria.
	i = j;
	i = droneAlmacenado.find_first_of(numeros, i);
	j = droneAlmacenado.find(caracterEspacio, i);
	this->_bateria = std::stoi(droneAlmacenado.substr(i, j-i));
	i = j;


	Secuencia<Posicion> vueloR;
	// Busco el primer corchete de la lista de posiciones alcanzadas:
	i = droneAlmacenado.find_first_of(caracterAbreCorchete, i); 
	// Si se abre el corchete y el siguiente caracter no lo cierra => La lista de posiciones tiene elementos.
	while (droneAlmacenado.substr(i+1, 1) != caracterCierraCorchete) {
		Posicion pos;
		i = droneAlmacenado.find_first_of(numeros, i);
		j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
		pos.x = std::stoi(droneAlmacenado.substr(i, j-i));
		i = j;
		i = droneAlmacenado.find_first_of(numeros, i);
		j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
		pos.y = std::stoi(droneAlmacenado.substr(i, j-i));
		vueloR.push_back(pos);
		i = j;
	}
	this->_trayectoria = vueloR;
	// Si la lista tiene posiciones, entonces el drone esta en vuelo:
	this->_enVuelo = (vueloR.size() > 0);

	Secuencia<Producto> productosDisp;
	// Busco el siguiente corchete, que abre la lista de productos del drone:
	i = droneAlmacenado.find_first_of(caracterAbreCorchete, i);
	while (droneAlmacenado.substr(i+1, 1) != caracterCierraCorchete) {
		i = droneAlmacenado.find_first_of(productoLetraInicial, i);
		j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
		productosDisp.push_back(Drone::stringAProducto(campoAlmacenado.substr(i, j-i)));
		i = j;
	}
	this->_productos = productosDisp;
}

bool Drone::operator==(const Drone& otroDrone) const
{
	return this->_id == otroDrone._id && mismaTrayectoria(this->_trayectoria, otroDrone._trayectoria) && this-> _bateria == otroDrone._bateria && this->_enVuelo == otroDrone._enVuelo && mismos(this->_productos , otroDrone._productos)   ;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	return os;
}


//auxiliares


bool mismaTrayectoria(const Secuencia<Posicion> l1,const  Secuencia<Posicion> l2){
	int i =0 ;
	bool esLaMisma= true ;
	if (l1.size() == l2.size()) {
		while (i < l1.size()){
			if (l1[i].x == l2[i].x && l1[i].y == l2[i].y){
				i = i+1 ;
			}
			else {
				i=i+1 ;
				esLaMisma == false ;
			}
			
		}
	return esLaMisma;
	}
}


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


bool buscarInfoVuelosCruzados(const Secuencia<InfoVueloCruzado> ls, const Posicion p){

	int i = 0 ;
	bool esta = false ; // no esta hasta que se demuestre lo contrario 
	while (i<ls.size()){
		if (ls[i].posicion.x == p.x && ls[i].posicion.y == p.y ) {
			bool esta = true;
			i=i+1;
		}
		else 
			i = i+1;

	}
	return esta;
}





/*template <class T> int buscar(const Secuencia<T> l ,const T e){
	int i = 0;
	while (i<l.size()) {
		if (l[i] == T) {
			return i;
		} 
		else 
			i == i+1

	if i==l.sir
	}
}*/


template <class T> bool mismos (const Secuencia<T> l1 , const Secuencia<T> l2) {
	
	if (l1.size() == l2.size()){
		bool estan= true;
		int i=0;
		while (estan == true && i< l1.size() ){

			if(cuenta(l1 , l1[i]) == cuenta (l2 , l1[i])){
				i=i+1;
			}
			else estan ==false ;

		return estan;
		}
	}
}



template <class T> int cuenta (const Secuencia <T> l1 , const T e){
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


Producto Drone::stringAProducto(std::string s){
	Producto prod;

	switch(s){
		case "Plaguicida":
			prod = Plaguicida;
			break;
		case "PlaguicidaBajoConsumo":
			prod = PlaguicidaBajoConsumo;
			break;
		case "Herbicida":
			prod = Herbicida;
			break;
		case "HerbicidaLargoAlcance":
			prod = HerbicidaLargoAlcance;
			break
	}

	return prod;
}