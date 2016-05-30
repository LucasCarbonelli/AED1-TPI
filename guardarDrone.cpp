busca el primer corchete que abre la primera Lista
le pasa la entrada a la funcion guardar trayectoria


void Drone::cargar(std::istream & is)
{
	const char caracterFinal = '}';

	//Busco el primer numero y el espacio que le sigue, para identificar el Id.
	guardarId(is);

	//Busco el numero siguiente y el proximo espacio, para identificar la carga de la bateria.
	guardarBateria(is);

	guardarTrayectoria(is);
	
	guardarProductos(is);

	guardarVuelo(is);

	guardarPosicionActual(is);

	char c;
	is >> c;
	while(c != caracterFinal){
		is >> c;
	}
}


// Auxiliares

void Drone::guardarId(std::istream & is){
	std::string idAlmacenado;
	const std::string caracterEspacio = " ";
	std::string::size_type i, j;
	std::getline(is, idAlmacenado, caracterEspacio);
	i = idAlmacenado.find_first_of(numeros);
	j = idAlmacenado.find(caracterEspacio, i);
	this->_id = std::stoi(idAlmacenado.substr(i, j-i));
}

void Drone::guardarBateria(std::istream & is){
	std::string bateriaAlmacenado;
	const std::string caracterEspacio = " ";
	std::string::size_type i, j;
	std::getline(is, bateriaAlmacenado, caracterEspacio);
	i = bateriaAlmacenado.find_first_of(numeros, i);
	j = bateriaAlmacenado.find(caracterEspacio, i);
	this->_bateria = std::stoi(bateriaAlmacenado.substr(i, j-i));
}

// cuando guardarTrayectoria termina el input está parado después de los corchetes que cierran la lista

// armar lista de posiciones en string
void Drone::guardarTrayectoria(std::istream & is){
	std::string pos;
	std::string cierraPosicion = "]";

	std::getline(is, pos, cierraPosicion);
	while(esPosicion(pos)){
		this->_trayectoria.push_back(extraerPosicion(pos));
		std::getline(is, pos, cierraPosicion);
	}
}

bool Drone::esPosicion(const std:string s) const {
	const std::string numeros = "0123456789";

	return s.find_first_of(numeros) < s.size();
}

Posicion Drone::extraerPosicion(const std:string s) {
	const std::string numeros = "0123456789";
	const std::string caracterPosteriorX = ",";
	std::string::size_type i, j;

	Posicion pos;
	i = s.find_first_of(numeros, i);
	j = s.find_first_of(caracterPosteriorX, i);
	pos.x = std::stoi(s.substr(i, j-i));
	i = j;
	i = s.find_first_of(numeros, i);
	pos.y = std::stoi(s.substr(i, s.size()));
	return pos;
}

void Drone::guardarProductos(std::istream & is){
	std::string cierraProductos = "]";
	std::string prod;
	std::getline(is, prod, cierraProductos);
	std::string::size_type i, j;
	i = 0;

	while (i < prod.size()) {
		i = prod.find_first_of(productoLetraInicial, i);
		j = prod.find_first_of(caracterPosteriorProducto, i);
		this->_trayectoria.push_back(Drone::stringAProducto(prod.substr(i, j-i)));
		i = j;
	}
}

void Drone::guardarVuelo(std::istream & is){
	const std::string caracterEnVuelo = "tf";
	const std::string enVueloFalse = "false";
	const std::string enVueloTrue = "true";
	std::string enVueloAlmacenado;
	std::getline(is, enVueloAlmacenado, caracterEnVuelo);

	if(enVueloAlmacenado.find(enVueloFalse) < enVueloAlmacenado.size()){
		this->_enVuelo = false;
	}
	else if(enVueloAlmacenado.find(enVueloTrue) < enVueloAlmacenado.size()){
		this->_enVuelo = true;
	}
}

void Drone::guardarPosicionActual(std::istream & is){
	std::string pos;
	std::string cierraPosicion = "]";
	std::getline(is, pos, cierraPosicion);
	this->_posicionActual = extraerPosicion(pos);
}
