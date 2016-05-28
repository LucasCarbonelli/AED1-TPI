#include "campo.h"
// Agregar #include "campo.h" en drone.h para que muestre correctamente el tipo Producto

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
	const std::string numeros = "0123456789";
	const std::string productoLetraInicial = "PFH";
	const std::string caracterEspacio = " ";
	const std::string caracterAbreCorchete = "[";
	const std::string caracterCierraCorchete = "]";
	//const std::string caracterCierraCorcheteDoble = "]]";
	//const std::string caracterPosteriorPrimerNumero = ",";
	const std::string caracterPosteriorProducto = ",]";
	const std::string caracterEnVuelo = "tf";
	const char caracterFinal = '}';
	std::string droneAlmacenado;
	std::string::size_type i, j;

	std::getline(is, droneAlmacenado, caracterFinal);

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
	while (droneAlmacenado.substr(i, 1) != caracterCierraCorchete) {
		i = droneAlmacenado.find_first_of(productoLetraInicial, i);
		j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
		productosDisp.push_back(Drone::stringAProducto(droneAlmacenado.substr(i, j-i)));
		i = j;
	}
	this->_productos = productosDisp;

	i = droneAlmacenado.find_first_of(caracterEnVuelo, i);
	if(droneAlmacenado[i] == 't'){
		this->_enVuelo = true;
	} else {
		this->_enVuelo = false;
	}

	i = droneAlmacenado.find_first_of(numeros, i);
	j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
	this->_posicionActual.x = std::stoi(droneAlmacenado.substr(i, j-i));
	i = j;
	i = droneAlmacenado.find_first_of(numeros, i);
	j = droneAlmacenado.find_first_of(caracterPosteriorProducto, i);
	this->_posicionActual.y = std::stoi(droneAlmacenado.substr(i, j-i));
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	d.mostrar(os);
	return os;
}

// Auxiliares

Producto Drone::stringAProducto(std::string s){
	Producto prod;

	if(s == "Fertilizante") prod = Fertilizante;
	else if(s == "Plaguicida") prod = Plaguicida;
	else if(s == "PlaguicidaBajoConsumo") prod = PlaguicidaBajoConsumo;
	else if(s == "Herbicida") prod = Herbicida;
	else if(s == "HerbicidaLargoAlcance") prod = HerbicidaLargoAlcance;

	return prod;
}