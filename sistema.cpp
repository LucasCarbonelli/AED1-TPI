#include "sistema.h"
#include <algorithm>


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

const Campo& Sistema::campo() const
{
	return this->_campo;
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
	return this->_estado.parcelas[p.x][p.y];
}

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
	return this->_enjambre;
}

void Sistema::crecer()
{
	int i = 0;
	int j = 0;
	Campo c = this->campo();
	while(i < c.dimensiones().ancho){
		j = 0;
	//Con estos dos while, recorro cada parcela
		while(j < c.dimensiones().largo){
			Posicion pos;
			pos.x = i;
			pos.y = j;
			if (c.contenido(pos) == Cultivo){
				//acá cambio su estado como está especificado.
				if (this->estadoDelCultivo(pos) == RecienSembrado) {
					this->_estado.parcelas[i][j] = EnCrecimiento;
				}
				else if (this->estadoDelCultivo(pos) == EnCrecimiento) {
					this->_estado.parcelas[i][j] = ListoParaCosechar;
				}
			}
			j++;
		}
		i++;
	}
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	Secuencia<Posicion>::size_type i = 0;
	while (i < ps.size()) {
		this->_estado.parcelas[ps[i].x][ps[i].y] = ConMaleza;
		i++;
	}
}

void Sistema::seExpandePlaga()
{
	int i = 0;
	Secuencia<Posicion> PosConPlaga;

	while (i < this->campo().dimensiones().ancho) {
		int j = 0;
		//Con estos dos while recorro todo el campo, y guardo las parcelas que su estado es ConPlaga
		while (j < this->campo().dimensiones().largo) {
			Posicion p;
			p.x = i;
			p.y = j;
			if (this->estadoDelCultivo(p) == ConPlaga)
			{
				PosConPlaga.push_back(p);
			}
			j++;
		}
		i++;
	}

	Secuencia<Posicion>::size_type k = 0;
	while (k < PosConPlaga.size()) {
		Posicion p;
		//Estos cuatro if son para las cuatro posibles posiciones adyacentes donde se expandira la plaga.
		//Contemplan que no se vaya de rango y que no haya una construcción (casa, granero).
		if (PosConPlaga[k].x + 1 < this->campo().dimensiones().ancho)
		{
			p.x = PosConPlaga[k].x + 1;
			p.y = PosConPlaga[k].y;
			if(noHayConstruccion(p)) this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		if (PosConPlaga[k].x - 1 >= 0)
		{
			p.x = PosConPlaga[k].x - 1;
			p.y = PosConPlaga[k].y;
			if(noHayConstruccion(p)) this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		if (PosConPlaga[k].y + 1 < this->campo().dimensiones().largo)
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y + 1;
			if(noHayConstruccion(p)) this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		if (PosConPlaga[k].y - 1 >= 0)
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y - 1;
			if(noHayConstruccion(p)) this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		k++;
	}
}

void Sistema::despegar(const Drone & d)
{
	// Por requiere hay al menos una parcela de cultivo libre a distancia 1 del granero
	Posicion posicionLibre;
	Secuencia<Posicion> listaPosicionesAdyacentes = lugaresAdyacentes(dondeEstaElGranero());

	for(Secuencia<Posicion>::size_type i = 0; i < listaPosicionesAdyacentes.size(); i++){
		if( noHayDrone(listaPosicionesAdyacentes[i]) && noHayConstruccion(listaPosicionesAdyacentes[i]) ){
			posicionLibre = listaPosicionesAdyacentes[i];
		}
	}
	//Lo unico que cambia en el asegura que habla sobre el drone es su posición y su estado de vuelo de false a true.
	//La bateria viene dada al 100 por requiere, y sus productos no los modificamos, así que deben permanecer igual.
	_enjambre[buscarDrone(d)].moverA(posicionLibre);
}

bool Sistema::listoParaCosechar() const
{
	return cantCultivosCosechables() >= 0.9 * ((campo().dimensiones().ancho * campo().dimensiones().largo) - 2);
}

void Sistema::aterrizarYCargarBaterias(Carga b){

	Secuencia<Drone>::size_type i= 0;
	while (i < this->_enjambre.size()){
		if (this->_enjambre[i].bateria() < b) {
			this->_enjambre[i].setBateria(100);
			//cambiarPosicionActual es auxiliar en el tipo drone.
			this->_enjambre[i].cambiarPosicionActual(dondeEstaElGranero());
			this->_enjambre[i].borrarVueloRealizado();
		}
		i=i+1;
	}
}

void Sistema::fertilizarPorFilas(){
	Secuencia<Posicion>::size_type i = 0 ;

	while (i < this->enjambreDrones().size()){

		if (this->enjambreDrones()[i].enVuelo()){

			// Usamos d para no poner this->enjambreDrones()[i] y que el codigo quede mas "lindo"
			Drone &d = _enjambre[i];

			//Aca movemos el drone hasta donde recorrido maximo nos indica que es posible agregando las posiciones que recorremos a la trayectoria.

			Posicion posIncial = d.posicionActual();
			Posicion p = posIncial;
			int j = posIncial.x - 1;

			//cambiamos el estado y quitamos un fertilizante de la posición actual, si su estado inicial es RecienSembrado o EnCrecimiento
			if (this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento)
				{
				this->_estado.parcelas[p.x][p.y] = ListoParaCosechar;
				d.sacarProducto(Fertilizante);
				}

			p.x = p.x - 1;
			//ahora hacemos lo dicho en el anterior comentario, pero para todas las parcelas que puede recorrer, dadas su bateria, fertilizantes, y parcelas validas.
			while (tieneProducto(d,Fertilizante) && j >= 0 && d.bateria() > 0 && noHayConstruccion(p)) {
				d.setBateria(d.bateria() - 1);
				d.moverA(p);
				if (this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento)
				{
				this->_estado.parcelas[p.x][p.y] = ListoParaCosechar;
				d.sacarProducto(Fertilizante);
				}
				p.x--;
				j--;
			}
		}
	i++;
	}
}

void Sistema::volarYSensar(const Drone & d)
{
	// Por requiere hay al menos una parcela de cultivo libre a distancia 1 de la posición actual del drone
	Posicion posicionLibre;
	Carga bateriaVieja;
	Drone &droneD = _enjambre[buscarDrone(d)];
	Secuencia<Posicion> listaPosicionesAdyacentes = lugaresAdyacentes(d.posicionActual());

	//Me quedo con una posición adyacente que no tenga construccíón (y ya por como está hecha lugaresAdyacentes también está en rango).
	for(Secuencia<Posicion>::size_type i = 0; i < listaPosicionesAdyacentes.size(); i++){
		if( noHayConstruccion(listaPosicionesAdyacentes[i]) ){
			posicionLibre = listaPosicionesAdyacentes[i];
		}
	}

	droneD.moverA(posicionLibre);
	bateriaVieja = droneD.bateria();
	droneD.setBateria(bateriaVieja - 1);

	if(this->estadoDelCultivo(posicionLibre) == NoSensado) {
		sensarParcela(posicionLibre);		// Elección arbitraria del estado, la especificación no aclara qué debe ocurrir a la hora de sensar.
	}
	else{
		aplicarProductos(droneD, posicionLibre);
	}
}

void Sistema::mostrar(std::ostream & os) const
{
	// muestra el campo del sistema como una tabla, el eje horizontal es 'ancho' y el vertical es 'largo', 
	// en cada casillero se muestra el contenido de la parcela y el estado del cultivo

	// muestra la coordenada en el eje 'ancho'

	os << std::string(4, ' ');

	for(int j = 0; j < this->campo().dimensiones().ancho; j++){
		std::cout.setf (std::ios::left, std::ios::adjustfield);
		std::cout.width(20);
		os << j;
	}

	os << std::endl;

	// muestra la coordenada en el eje 'largo' y el contenido de la parcela y el estado del cultivo

	for(int i = 0; i < this->campo().dimensiones().largo; i++){
		std::cout.width(4);
		os << i;
		for(int j = 0; j < this->campo().dimensiones().ancho; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(20);
			Posicion p;
			p.x = j;
			p.y = i;
			os << this->campo().contenido(p);
		}
		os << std::endl;

		os << std::string(4, ' ');
		for(int j = 0; j < this->campo().dimensiones().ancho; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(20);
			os << this->_estado.parcelas[j][i];
		}
		os << std::endl;
	}

	//mostramos los drones del sistema

	for (Secuencia<Drone>::size_type i = 0; i < this->enjambreDrones().size(); i++){
		os << std::endl;
		this->enjambreDrones()[i].mostrar(os);
		os << std::endl;
	}
}

void Sistema::guardar(std::ostream & os) const
{
	os << "{ S ";

	// guarda el campo
	this->_campo.guardar(os);

	// guarda la lista de drones
	os << " [";
	for(Secuencia<Drone>::size_type i = 0; i < this->_enjambre.size(); i++){
		_enjambre[i].guardar(os);
		if(i < this->_enjambre.size() - 1) os << ", ";
	}
	os << "] ";

	// guarda el estado del cultivo
	os << "[";
	for(int i = 0; i < this->campo().dimensiones().ancho; i++){
		os << "[";
		for(int j = 0; j < this->campo().dimensiones().largo; j++){
			os << this->_estado.parcelas[i][j];
			if(j < this->campo().dimensiones().largo - 1) os << ",";
		}
		os << "]";
		if(i < this->campo().dimensiones().ancho - 1) os << ", ";
	}
	os << "]}";
}

void Sistema::cargar(std::istream & is)
{
	const std::string parcelaLetraInicial = "RELCN";
	const std::string caracterAnteriorAestados = "[";
	const std::string caracterPosteriorAestados = ",]";
	const char caracterInicioListaDrones = '[';
	const char caracterFinListaDrones = ']';
	std::string sistemaAlmacenado;
	std::string::size_type i, j;
	Secuencia<Drone> ds;

	// Cargar Campo

	this->_campo.cargar(is);

	// Cargar Drones

	char c;
	is >> c;

	while(c != caracterInicioListaDrones){
		is >> c;
	}

	is >> c;

	while(c != caracterFinListaDrones){
		Drone d;
		d.cargar(is);
		ds.push_back(d);
		is >> c;
	}

	this->_enjambre = ds;

	// Cargar EstadoCultivo
	// busca letras que coincidan con la primera y la última letra de los nombres del tipo EstadoCultivo
	// toma la palabra en formato string, la transforma a tipo EstadoCultivo y se la asigna a la posición correspondiente en la grilla de EstadoCultivo

	Grilla<EstadoCultivo> ec(this->campo().dimensiones());
	std::getline(is, sistemaAlmacenado);

	i = 0;

	for(int posAncho = 0; posAncho < this->campo().dimensiones().ancho; posAncho++){
		for(int posLargo = 0; posLargo < this->campo().dimensiones().largo; posLargo++){
			i = sistemaAlmacenado.find_first_of(parcelaLetraInicial, i);
			j = sistemaAlmacenado.find_first_of(caracterPosteriorAestados, i);
			ec.parcelas[posAncho][posLargo] = stringAEstadoCultivo(sistemaAlmacenado.substr(i, j-i));
			i = j;
		}
	}
	this->_estado.parcelas = ec.parcelas;
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return this->campo() == otroSistema.campo() && this->igualEstadoDelCultivo(otroSistema) && this->igualEnjambreDrones(otroSistema);
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	s.mostrar(os);
	return os;
}


//Auxiliares

bool Sistema::noHayConstruccion(Posicion p) const {
	return this->campo().contenido(p) == Cultivo;
}

bool Sistema::noHayDrone(Posicion p) const {
	Secuencia<Drone> ds = this->enjambreDrones();
	bool m = true;
	Secuencia<Drone>::size_type i = 0;

	//este while recorre la lista ds (que contiene a los drones del enjambre del sistema), fijandose que ninguno tenga como posición actual la posición p.
	while (i < ds.size() ) {
		if ((ds[i].posicionActual().x == p.x) && (ds[i].posicionActual().y == p.y))
		{
			m = false;
			break;
		}
		i++;
	}
	return m;
}

Secuencia<Drone>::size_type Sistema::buscarDrone(Drone d) const {
	Secuencia<Drone>::size_type i = 0;
	while ( i < this->enjambreDrones().size()) {
		if (this->enjambreDrones()[i] == d)
		{
			break;
		}
		i++;
	}
	return i;
}

Posicion Sistema::dondeEstaElGranero() const {
	Posicion granero;

	for(int i = 0; i < this->campo().dimensiones().ancho; i++){
		for(int j = 0; j < this->campo().dimensiones().largo; j++){
			Posicion p = {i, j};
			if(this->campo().contenido(p) == Granero){
				granero = p;
			}
		}
	}

	return granero;
}

Secuencia<Posicion> Sistema::lugaresAdyacentes(Posicion p) const {
	Secuencia<Posicion> listaPos;
	Posicion p0, p1, p2, p3;
	
	p0.x = p.x + 1;
	p0.y = p.y;
	if(enRango(p0)) listaPos.push_back(p0);

	p1.x = p.x - 1;
	p1.y = p.y;
	if(enRango(p1)) listaPos.push_back(p1);

	p2.x = p.x;
	p2.y = p.y + 1;
	if(enRango(p2)) listaPos.push_back(p2);

	p3.x = p.x;
	p3.y = p.y - 1;
	if(enRango(p3)) listaPos.push_back(p3);

	return listaPos;
}

bool Sistema::enRango(const Posicion &p) const {
	return p.x >= 0 && p.x < this->campo().dimensiones().ancho && p.y >= 0 && p.y < this->campo().dimensiones().largo;
}

int Sistema::cantCultivosCosechables() const{
	int cuenta = 0;
	int i = 0;

	//este while recorre las columnas del campo, mientras que el auxiliar contarFilasCosechables las filas, así obteniendo todas las cosechables.
	while (i < this->campo().dimensiones().ancho){
		cuenta = cuenta + contarFilasCosechables(i);

		i = i + 1;
	}
	return cuenta;
}

int Sistema::contarFilasCosechables(int i) const {
	int cuenta = 0;
	Posicion p;
	p.x = i;
	int j = 0; 
	while (j < this->campo().dimensiones().largo){
		p.y = j;
		if (this->campo().contenido(p) == Cultivo){
			if (this->estadoDelCultivo(p) == ListoParaCosechar){
			cuenta = cuenta + 1;
			}
		}
		j++;
	}
	return cuenta;
}

bool Sistema::tieneProducto(Drone d, Producto p) {
	bool m = false;
	Secuencia<Producto>::size_type i = 0;
	while (i < d.productosDisponibles().size()) {
		if (p == d.productosDisponibles()[i])
		{
			m = true;
			break;
		}
		i++;
	}
	return m;
}

void Sistema::sensarParcela(Posicion p) {
	this->_estado.parcelas[p.x][p.y] = RecienSembrado;	
}

void Sistema::aplicarProductos(Drone &d, Posicion p) {


	//hay un if por cada tipo de estado posible en el cual el drone puede aplicar un producto.
	if (this->estadoDelCultivo(p) == RecienSembrado && tieneProducto(d, Fertilizante))
	{
		this->_estado.parcelas[p.x][p.y] = ListoParaCosechar;
		d.sacarProducto(Fertilizante);
	}
	if (this->estadoDelCultivo(p) == EnCrecimiento && tieneProducto(d, Fertilizante))
	{
		this->_estado.parcelas[p.x][p.y] = ListoParaCosechar;
		d.sacarProducto(Fertilizante);
	}
	if (this->estadoDelCultivo(p) == ConMaleza && tieneProducto(d, Herbicida) && d.bateria() >= 5)
	{
		this->_estado.parcelas[p.x][p.y] = RecienSembrado;
		d.sacarProducto(Herbicida);
		Carga nuevaCarga = d.bateria() - 5;
		d.setBateria(nuevaCarga);
	}
	if (this->estadoDelCultivo(p) == ConMaleza && tieneProducto(d, HerbicidaLargoAlcance) && d.bateria() >= 5)
	{
		this->_estado.parcelas[p.x][p.y] = RecienSembrado;
		d.sacarProducto(HerbicidaLargoAlcance);
		Carga nuevaCarga = d.bateria() - 5;
		d.setBateria(nuevaCarga);
	}
	if (this->estadoDelCultivo(p) == ConPlaga && tieneProducto(d, Plaguicida) && d.bateria() >= 10)
	{
		this->_estado.parcelas[p.x][p.y] = RecienSembrado;
		d.sacarProducto(Plaguicida);
		Carga nuevaCarga = d.bateria() - 10;
		d.setBateria(nuevaCarga);
	}
	if (this->estadoDelCultivo(p) == ConPlaga && tieneProducto(d, PlaguicidaBajoConsumo) && d.bateria() >= 5)
	{
		this->_estado.parcelas[p.x][p.y] = RecienSembrado;
		d.sacarProducto(PlaguicidaBajoConsumo);	
		Carga nuevaCarga = d.bateria() - 5;
		d.setBateria(nuevaCarga);
	}
}

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


bool Sistema::igualEstadoDelCultivo(const Sistema& otroSistema) const {
	bool igualEstado = true;
	for(int i = 0; i < this->campo().dimensiones().ancho; i++){
		for(int j = 0; j < this->campo().dimensiones().largo; j++){
			Posicion p;
			p.x = i;
			p.y = j;
			if(this->estadoDelCultivo(p) != otroSistema.estadoDelCultivo(p)){
				igualEstado = false;
			}
		}
	}
	return igualEstado;
}

bool Sistema::igualEnjambreDrones(const Sistema& otroSistema) const {
	bool igualEnjambre = true;
	for(Secuencia<Drone>::size_type i = 0; i < this->enjambreDrones().size(); i++){
		if(cuenta(this->enjambreDrones(), this->enjambreDrones()[i]) != cuenta(otroSistema.enjambreDrones(), this->enjambreDrones()[i])){
			igualEnjambre = false;
		}
	}
	return igualEnjambre;
}

template <class T> int Sistema::cuenta(const Secuencia <T> ls, const T e) const {
	Secuencia<Producto>::size_type i = 0 ; 
	int cuenta = 0 ;
	while (i < ls.size()){
		if (ls[i] == e) {
			cuenta = cuenta + 1;
		}
		i = i + 1;
	}
	return cuenta;
}