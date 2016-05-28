Sistema::Sistema()
{
	// Creo un campo con el constructor por defecto.
	Campo c = Campo();
	this->_campo = c;
	// Creo un listado de drones vacio.
	Secuencia<Drone> ds; 
	this->_enjambre = ds;

	// Creo la grilla correspondiente al campo y le asigno el estado noSensado a todos los cultivos.
	Grilla<EstadoCultivo> EC(c.dimensiones());
	
	int i = 0;
	while(i < c.dimensiones().ancho){
		int j = 0;
		while(j < c.dimensiones().largo){
			EC.parcelas[i][j] = NoSensado;
			j++;
		}
		i++;
	}
	
	this->_estado.parcelas = EC.parcelas;
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
	this->_campo = c;
	this->_enjambre = ds;

	// idem anterior
	Grilla<EstadoCultivo> EC(c.dimensiones());
	
	int i = 0;
	while(i < c.dimensiones().ancho){
		int j = 0;
		while(j < c.dimensiones().largo){
			EC.parcelas[i][j] = NoSensado;
			j++;
		}
		i++;
	}
	
	this->_estado.parcelas = EC.parcelas;
}

void Sistema::mostrar(std::ostream & os) const
{
	// Muestra el campo del sistema con el estado debajo 

	os << std::string(4, ' ');

	for(int j = 0; j < this->_campo.dimensiones().largo; j++){
		std::cout.setf (std::ios::left, std::ios::adjustfield);
		std::cout.width(20);
		os << j;
	}

	os << std::endl;

	for(int i = 0; i < this->_campo.dimensiones().ancho; i++){
		std::cout.width(4);
		os << i;
		for(int j = 0; j < this->_campo.dimensiones().largo; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(20);
			Posicion p;
			p.x = i;
			p.y = j;
			os << this->_campo.contenido(p);
		}
		os << std::endl;

		os << std::string(4, ' ');
		for(int j = 0; j < this->_campo.dimensiones().largo; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(20);
			os << this->_estado.parcelas[i][j];
		}
		os << std::endl;
	}

	//mostramos los drones del sistema

	for (Secuencia<Drone>::size_type i = 0; i < this->_enjambre.size(); i++){
		os << std::endl;
		this->_enjambre[i].mostrar(os);
		os << std::endl;
	}
}

void Sistema::guardar(std::ostream & os) const
{
	os << "{ S ";
	this->_campo.guardar(os);
	os << " [";
	for(Secuencia<Drone>::size_type i = 0; i < this->_enjambre.size(); i++){
		_enjambre[i].guardar(os);
		if(i < this->_enjambre.size() - 1) os << ", ";
	}
	os << "] ";

	os << "[";
	for(int i = 0; i < this->_campo.dimensiones().ancho; i++){
		os << "[";
		for(int j = 0; j < this->_campo.dimensiones().largo; j++){
			os << this->_estado.parcelas[i][j];
			if(j < this->_campo.dimensiones().largo - 1) os << ",";
		}
		os << "]";
		if(i < this->_campo.dimensiones().ancho - 1) os << ", ";
	}
	os << "]}";
}

void Sistema::cargar(std::istream & is)
{
	const std::string parcelaLetraInicial = "RELCN";
	const std::string caracterAnteriorAestados = "[";
	const std::string caracterPosteriorAestados = ",]";
	const char caracterFinListaDrones = ']';
	std::string sistemaAlmacenado;
	std::string::size_type i, j;
	Secuencia<Drone> ds;

	// Cargar Campo

	this->_campo.cargar(is);

	// Cargar Drones

	char c;
	is >> c;

	while(c != caracterFinListaDrones){
		Drone d;
		d.cargar(is);
		ds.push_back(d);
		is >> c;
	}

	this->_enjambre = ds;

	// Cargar EstadoCultivo

	Grilla<EstadoCultivo> ec(_campo.dimensiones());
	std::getline(is, sistemaAlmacenado);

	i = 0;

	for(int posAncho = 0; posAncho < this->_campo.dimensiones().ancho; posAncho++){
		for(int posLargo = 0; posLargo < this->_campo.dimensiones().largo; posLargo++){
			i = sistemaAlmacenado.find_first_of(parcelaLetraInicial, i);
			j = sistemaAlmacenado.find_first_of(caracterPosteriorAestados, i);
			ec.parcelas[posAncho][posLargo] = stringAEstadoCultivo(sistemaAlmacenado.substr(i, j-i));
			i = j;
		}
	}
	this->_estado.parcelas = ec.parcelas;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	// TODO: insert return statement here
	s.mostrar(os);
	return os;
}

// Auxiliares

EstadoCultivo Sistema::stringAEstadoCultivo(const std::string s) const {
	EstadoCultivo ec;

	if(s == "RecienSembrado") ec = RecienSembrado;
	else if(s == "EnCrecimiento") ec = EnCrecimiento;
	else if(s == "ListoParaCosechar") ec = ListoParaCosechar;
	else if(s == "ConMaleza") ec = ConMaleza;
	else if(s == "ConPlaga") ec = ConPlaga;
	else if(s == "NoSensado") ec = NoSensado;

	return ec;
}