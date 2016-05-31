#include "drone.h"
#include "campo.h"
#include <sstream>

Drone::Drone()
{
	this->_id = 1;
	this->_bateria = 100 ; 
	Secuencia<Posicion> pos ; 
	this->_trayectoria = pos  ;
	Secuencia <Producto> productos ;
	this->_productos = productos;
	this->_enVuelo = false;
	Posicion p;
	p.x = 0 ;
	p.y = 0 ;
	this->_posicionActual = p;
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
	this->_id = i;
	this->_bateria = 100 ; 
	Secuencia<Posicion> pos ; 
	this->_trayectoria = pos  ;
	this->_productos = ps ;
	this->_enVuelo = false;
	Posicion p;
	p.x = 0 ;
	p.y = 0 ;
	this->_posicionActual = p;
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

	return this->_posicionActual;
}

const Secuencia<Producto>& Drone::productosDisponibles() const
{
	return this->_productos;
}

bool Drone::vueloEscalerado() const
{
	return (this->enVuelo() && this->escalerado());

}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
	Secuencia<InfoVueloCruzado> ListaInfoVC;
	Secuencia<Drone>::size_type i = 0;
	Secuencia<Posicion>::size_type j = 0;
	while (i < ds.size() && j < ds[i].vueloRealizado().size()) {
		if (cantidadDronesCruzados(ds[i].vueloRealizado()[j], ds)  > 1 && buscarInfoVuelosCruzados(ListaInfoVC, ds[i].vueloRealizado()[j]))
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
	os << "Bateria: " << std::to_string(this->_bateria) << "\n";
	// Listado de posiciones.
	os << "Vuelo Realizado: ";
	if (this->_trayectoria.size() == 0) {
		os << "[]";	
	} else {
		os << "[";
		os << "(" << std::to_string(this->_trayectoria[0].x) << "," << std::to_string(this->_trayectoria[0].y) << ")";
		for(Secuencia<Posicion>::size_type i = 1; i < this->_trayectoria.size(); i++){
			os << ", (";
			os << std::to_string(this->_trayectoria[i].x) << "," << std::to_string(this->_trayectoria[i].y);
			os << ")";
			}
		os << "]";
	}
	os << "\n";

	// Listado de productos.
	os << "Productos Disponibles: ";
	if (this->_productos.size() == 0) {
		os << "Ninguno.";	
	} else {
		os << this->_productos[0];
		for(Secuencia<Posicion>::size_type i = 1; i < this->_productos.size(); i++){
			os << "," << this->_productos[i];
		}
	}
}

void Drone::guardar(std::ostream & os) const
{
	//Cabecera: Drone, Id y Bateria.
	os << "{ D "  << std::to_string(this->_id) << " " << std::to_string(this->_bateria) << " " ;

	// Listado de posiciones.
	if (this->_trayectoria.size() == 0) {
		os << "[]";	
	} else {
		os << "[";
		os << "[" << std::to_string(this->_trayectoria[0].x) << "," << std::to_string(this->_trayectoria[0].y) << "]";
		for(Secuencia<Posicion>::size_type i = 1; i < this->_trayectoria.size(); i++) {
			os << ",[";
			os << std::to_string(this->_trayectoria[i].x) << "," << std::to_string(this->_trayectoria[i].y);
			os << "]";
		}
		os << "] ";
	}
	// Listado de productos.
	if (this->_productos.size() == 0) {
		os << "[]";	
	} else {
		os << "[";
		os << this->_productos[0];
		for(Secuencia<Producto>::size_type i = 1; i < this->_productos.size(); i++){
			os << ", " << this->_productos[i];
		}
		os << "]";
	}

	os << " ";
	if(this->_enVuelo) os << "true";
	else os << "false";
	os << " ";

	os << "[" << this->_posicionActual.x << "," << this->_posicionActual.y << "]";

	os << "}";
}

void Drone::cargar(std::istream & is)
{
	const char caracterFinal = '}';
	const char caracterDrone = 'D';
	char c;

	is >> c;

	// Busco la letra D de drone para que solo queden espacios entre la entrada is y el id

	while(c != caracterDrone){
		is >> c;
	}

	is >> this->_id;
	is >> this->_bateria;

	guardarTrayectoria(is);
	guardarProductos(is);
	guardarVuelo(is);
	guardarPosicionActual(is);

	// Busco sacar de la entrada is la llave con la que termina el drone para poder usar esta función en Sistema::cargar

	is >> c;
	while(c != caracterFinal){
		is >> c;
	}
}



void Drone::moverA(const Posicion pos)
{
	this->_enVuelo = true;
	this->_trayectoria.push_back(pos);
	this->_posicionActual = pos;

}

void Drone::setBateria(const Carga c)
{
	this->_bateria = c;
}    		

void Drone::borrarVueloRealizado()
{
	this->_enVuelo = false ;
	Secuencia<Posicion> nuevaTrayectoria; 
	this->_trayectoria = nuevaTrayectoria;
}
    		
void Drone::cambiarPosicionActual(const Posicion p)
{
	this->_posicionActual = p ;

}
    		
void Drone::sacarProducto(const Producto p)
{
	Secuencia<Producto> productosSinP;
	Secuencia<Producto> ::size_type i = 0 ; 
	bool seSaco= false; 
	while ( i < this->_productos.size() ){

		if (seSaco == false &&  this->productosDisponibles()[i] == p){
			seSaco = true;
		}

		if (seSaco == true || this->productosDisponibles()[i] != p ){
			Producto p = this->productosDisponibles()[i]; 
			productosSinP.push_back(p);
		}
		i = i + 1 ;
	}

	this->_productos = productosSinP;

}

bool Drone::operator==(const Drone& otroDrone) const
{
	return this->id() == otroDrone.id() && mismaTrayectoria(this->vueloRealizado(), otroDrone.vueloRealizado()) && this->bateria() == otroDrone.bateria() && this->enVuelo() == otroDrone.enVuelo() && mismos(this->productosDisponibles() , otroDrone.productosDisponibles()) && this->posicionActual().x == otroDrone.posicionActual().x && this->posicionActual().y == otroDrone.posicionActual().y;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	d.mostrar(os);
	return os;
}



//Auxiliares


 bool Drone::mismaTrayectoria(const Secuencia<Posicion> l1,const  Secuencia<Posicion> l2)  {
	Secuencia<Posicion>::size_type i =0 ;
	bool esLaMisma= true ;
	if (l1.size() == l2.size()) {
		while (i < l1.size()){
			if (l1[i].x == l2[i].x && l1[i].y == l2[i].y){
				i = i+1 ;
			}
			else {
				i=i+1 ;
				esLaMisma = false ;
			}
			
		}
	}
	else{
		esLaMisma = false;
	}
	return esLaMisma;
}


bool Drone::escalerado() const {
	Secuencia<Posicion>::size_type i = 0 ;
	bool val = true ; 
	while ( val == true && i < this->vueloRealizado().size() -2 ){
		if ((vueloRealizado()[i].x - vueloRealizado()[i+2].x ==1 || vueloRealizado()[i].x - vueloRealizado()[i+2].x == -1 ) && (vueloRealizado()[i].y - vueloRealizado()[i+2].y ==1 || vueloRealizado()[i].y - vueloRealizado()[i+2].y == -1 )) {
			i = i+1;
		}
		else 
			val = false;
	}
	return val;
}

bool Drone::seCruzoConOtro(Secuencia<Drone> ds, int i) const {
	Secuencia<Drone>::size_type j=0; 
	bool seCruzo = false ; // inicializo una variable que nos dice si hasta el momento un drone se cruzo con el drone del parametro
	while (j < ds.size()){
		if  (( this->id() == ds[j].id() ) == false && this->vueloRealizado()[i].x == ds[j].vueloRealizado()[i].x && this->vueloRealizado()[i].y == ds[i].vueloRealizado()[j].y){
			seCruzo = true; // si se cruzo con otro drone distinto en la posicion i-esima entonces cambiamos la variable a true
			j=j+1;
		}
		else j=j+1;
	}
	return seCruzo; 
}


 int Drone::cantidadDronesCruzados(const Posicion p , const Secuencia<Drone> ds) {
	Secuencia<Drone> dronesCruzados ; // inicializo una secuencia vacia que se llama dronesCruzados y aca vamos a guardar los drones que se cruzaron. 
	
	Secuencia<Drone>::size_type i = 0; // Con la variable i recorro la lista de drones ds.

	while (i < ds.size()){
		Secuencia<Posicion>::size_type j = 0; // Con la variable j recorro el vuelo realizado de los drones.

		while ( j < ( ds[i].vueloRealizado()).size() ){
			
			if (ds[i].vueloRealizado()[j].x == p.x && ds[i].vueloRealizado()[j].y == p.y && ds[i].seCruzoConOtro(ds, j) ){
				dronesCruzados.push_back(ds[i]); // si el drone que estoy mirando se cruzo con otro en la posicion p entonces lo agrego a la secuencia
				j = j+1 ;


			}
			else j = j+1;

		}

		i= i+1 ;
	}

	return dronesCruzados.size();
} 


 Secuencia<Posicion> Drone::posConCruces( const Secuencia<Drone> ds) {
	Secuencia<Drone>::size_type j = 0;
	Secuencia<Posicion> posicionesC;
	while (j < ds.size()) {
		Secuencia<Posicion>::size_type i = 0;

		while ( i < ds[j].vueloRealizado().size()) {
			if (ds[j].seCruzoConOtro(ds, i))
			 {
			 	posicionesC.push_back(ds[j].vueloRealizado()[i]);
			 }
			i++;
		}
		j++;
	}
	return posicionesC;
}


bool Drone::buscarInfoVuelosCruzados(const Secuencia<InfoVueloCruzado> ls, const Posicion p){

	Secuencia<InfoVueloCruzado>::size_type i = 0 ;
	bool esta = false ; // no esta hasta que se demuestre lo contrario 
	while (i<ls.size()){
		if (ls[i].posicion.x == p.x && ls[i].posicion.y == p.y ) {
			esta = true;
			i=i+1;
		}
		else 
			i = i+1;

	}
	return esta;
}



template <class T> bool Drone::mismos (const Secuencia<T> l1 , const Secuencia<T> l2) const{
	bool estan = true;	
	if (l1.size() == l2.size()){
//		bool estan= true;
		int i=0;
		while (estan == true && i< l1.size() ){

			if(cuenta(l1 , l1[i]) == cuenta (l2 , l1[i])){
				i=i+1;
			}
			else estan = false ;

//		return estan;
		}
	}
	else{
		estan = false;
	}
	return estan;
}



template <class T> int Drone::cuenta (const Secuencia <T> l1 , const T e) const {
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


Producto Drone::stringAProducto(const std::string s) const {
	Producto prod;

	if(s == "Fertilizante") prod = Fertilizante;
	else if(s == "Plaguicida") prod = Plaguicida;
	else if(s == "PlaguicidaBajoConsumo") prod = PlaguicidaBajoConsumo;
	else if(s == "Herbicida") prod = Herbicida;
	else if(s == "HerbicidaLargoAlcance") prod = HerbicidaLargoAlcance;

	return prod;
}

void Drone::guardarTrayectoria(std::istream & is){
	std::string pos;
	Secuencia<Posicion> trayecto;
	const char cierraPosicion = ']';

	std::getline(is, pos, cierraPosicion);
	while(esPosicion(pos)){
		trayecto.push_back(extraerPosicion(pos));
		std::getline(is, pos, cierraPosicion);
	}
	this->_trayectoria = trayecto;
}

void Drone::guardarProductos(std::istream & is){
	Secuencia<Producto> listaProductos;
	const std::string productoLetraInicial = "PFH";
	const std::string caracterPosteriorProducto = ",]";
	const char cierraProductos = ']';
	std::string prod;
	std::getline(is, prod, cierraProductos);
	std::string::size_type i, j;
	i = 0;
	i = prod.find_first_of(productoLetraInicial, i);

	while (i < prod.size()) {
		i = prod.find_first_of(productoLetraInicial, i);
		j = prod.find_first_of(caracterPosteriorProducto, i);
		listaProductos.push_back(stringAProducto(prod.substr(i, j-i)));
		i = j;
	}
	this->_productos = listaProductos;
}

void Drone::guardarVuelo(std::istream & is){
	const char cFalse = 'f';
	const char cTrue = 't';

	char c;
	is >> c;
	while(c != cTrue && c != cFalse){
		is >> c;
	}

	this->_enVuelo = (c == cTrue);
}

void Drone::guardarPosicionActual(std::istream & is){
	std::string pos;
	const char cierraPosicion = ']';
	std::getline(is, pos, cierraPosicion);
	this->_posicionActual = extraerPosicion(pos);
}

bool Drone::esPosicion(const std::string s) const {
	const std::string numeros = "0123456789";
	return s.find_first_of(numeros) < s.size();
}

Posicion Drone::extraerPosicion(const std::string s) const {
	const std::string numeros = "0123456789";
	const char caracterPosteriorX = ',';
	std::string::size_type i, j;

	Posicion pos;
	i = s.find_first_of(numeros);
	j = s.find_first_of(caracterPosteriorX, i);
	pos.x = std::stoi(s.substr(i, j-i));
	i = j;
	i = s.find_first_of(numeros, i);
	pos.y = std::stoi(s.substr(i, s.size()));
	return pos;
}