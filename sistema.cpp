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
	//Grilla<EstadoCultivo> EC(c.dimensiones());

	// La estructura Grilla no es muy genérica.... si le pasas una posición al constructor te intenta poner "Cultivo" en todas las parcelas
	// para que no falle al compilar hay que crear una grilla vacía y modificarla después
	// Con esto, todas las posiciones cambian a NoSensado incluyendo la posición donde está la Casa y el Granero ¿está bien que sea así?
	Grilla<EstadoCultivo> eC;
	eC.parcelas.resize(c.dimensiones().ancho, std::vector<EstadoCultivo>(c.dimensiones().largo, NoSensado));
	this->_estado.parcelas = eC.parcelas;
	/*
	int i = 0;
	while(i < c.dimensiones().ancho){
		int j = 0;
		while(j < c.dimensiones().largo){
			Posicion p;
			p.x = i;
			p.y = j;
//			if (c.contenido(p) == Cultivo) {this->_estado.parcelas[i][j] = NoSensado;}
			if (c.contenido(p) == Cultivo) {EC.parcelas[i][j] = NoSensado;}
			j++;
		}
		i++;
	}
	this->_estado.parcelas = EC.parcelas;
	*/
//	this->_estado = EC;

}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
	this->_campo = c;
	this->_enjambre = ds;

	Grilla<EstadoCultivo> eC;
	eC.parcelas.resize(c.dimensiones().ancho, std::vector<EstadoCultivo>(c.dimensiones().largo, NoSensado));
	this->_estado.parcelas = eC.parcelas;

	/* idem anterior
	Grilla<EstadoCultivo> EC(c.dimensiones());
	int i = 0;
	while(i < c.dimensiones().ancho){
		int j = 0;
		while(j < c.dimensiones().largo){
			Posicion p;
			p.x = i;
			p.y = j;
//			if (c.contenido(p) == Cultivo) {this->_estado.parcelas[i][j] = NoSensado;};
			if (c.contenido(p) == Cultivo) {EC.parcelas[i][j] = NoSensado;};
			j++;
		}
		i++;

	}
	this->_estado.parcelas = EC.parcelas;
//	this->_estado = EC;
*/
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
	Campo c = this->_campo;
	while(i < c.dimensiones().ancho){
		while(j < c.dimensiones().largo){
			Posicion pos;
			pos.x = i;
			pos.y = j;
			if (c.contenido(pos) == Cultivo){
				if (this->_estado.parcelas[i][j] == RecienSembrado) {
					this->_estado.parcelas[i][j] = EnCrecimiento;
				}
				else if (this->_estado.parcelas[i][j] == EnCrecimiento) {
					this->_estado.parcelas[i][j] = ListoParaCosechar;
				}

				
			}

		}
	}
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	//aca no hablo de this? igual me pasan como const la secuencia ps, no puedo hacer esto?? otra: me armo un PS que tenga todos los p de ps, sacados de this,
	//y a eso le hago esto. Hice algo parecido a esto en el ejer despegar, mas abajo.
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
		p.x = PosConPlaga[k].x;
		p.y = PosConPlaga[k].y;
		//al final x es el largo o el ancho??
		if (PosConPlaga[k].x + 1 <= this->campo().dimensiones().largo && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x + 1;
			this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		//nose si es necesario aclarar que en este caso esa pos debe ser menor que el largo, ya que si la pos original era menor al largo, -1 lo va a ser seguro.
		if (PosConPlaga[k].x - 1 >= 0 && PosConPlaga[k].x - 1 <= this->campo().dimensiones().largo && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x - 1;
			this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		if (PosConPlaga[k].y + 1 <= this->campo().dimensiones().ancho && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y + 1;
			this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
		//idem arriba.
		if (PosConPlaga[k].y - 1 >= 0 && PosConPlaga[k].y - 1 <= this->campo().dimensiones().ancho && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y - 1;
			this->_estado.parcelas[p.x][p.y] = ConPlaga;
		}
	}
}

// La funcion buscarDrone(d, this) no está definida!!!!! Esta función hay que revisarla, la comento para que compile
void Sistema::despegar(const Drone & d)
{

	//mismo problema que en seVinoLaMaleza con el tema de parametro con const
	if (this->buscarDrone(d))
	{
		Drone D = d; //como asignador, se puede? sino hago un aux, o pongo todo lo que signfica acá.
		if (posicionLibre(DondeEstaElGranero(this->_campo)))
		{
			//D._enVuelo && D._bateria = 100;
			; // mañana vemos........
		}
	}

}

bool Sistema::listoParaCosechar() const
{
	return this->cantCultivosCosechables() >= 0.9 * this->parcelasDeCultivo().size();
}

void Sistema::aterrizarYCargarBaterias(Carga b){

	Secuencia<Drone>::size_type i= 0;
	while (i < this->_enjambre.size()){
		if (this->_enjambre[i].bateria() < b ) {
			//this->cargarLaBateria(this->_enjambre[i]);
			Drone d(this->_enjambre[i].id(), this->_enjambre[i].productosDisponibles());
			this->_enjambre[i] = d;
			i=i+1;
		}
		else 
			i=i+1;
	}

}


void Sistema::fertilizarPorFilas(){
	Secuencia<Posicion>::size_type i = 0 ;

	while ( i < this->enjambreDrones().size()){

		if (this->enjambreDrones()[i].enVuelo() == true){

			//Creamos un nuevo con el mismo id que el anteria y descontandole los fertilizantes que vamos a usar. 

			Drone d(this->enjambreDrones()[i].id(), mismosProductosDescontandoFertlizante(this->enjambreDrones()[i]));

			// Le asignamos la misma trayectoria  

			d_trayectoria = this->enjambreDrones()[i].vueloRealizado()  

			//Aca movemos el drone hasta donde recorrido maximo nos indica que es posible agregando las posiciones que recorremos a la trayectoria.

			int j = d.posicionActual().x;
			while ( j < d.posicionActual().x - this->recorridoMaximo(this->enjambre[i])){
				Posicion p ;
				p.x = j + 1;
				p.y = this->enjambreDrones()[i].posicionActual().y;
				d_trayectoria.push_back(p);
				j = j + 1;

				// Si es posible le aplicamos el fertilizante a la parcela que agregamos.
				if (this->campo().contenido( p ) == Cultivo && ( this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento)){
					this->_estado[p.x][p.y] = ListoParaCosechar ;
				}

			d_enVuelo = true;
			d_bateria = this->enjambreDrones()[i].bateria() - this->recorridoMaximo(this->enjambreDrones()); 

			this->_enjambre[i] = d;

			}

		i = i + 1;



		} 
	} 

}

void Sistema::volarYSensar(const Drone & d)
{
	Secuencia<Posicions> ps = lugaresAdyacentes(d.posicionActual());
	int i = 0;
	Posicion p;
	p.x = -1;
	p.y = -1;
	while (i < ps.size()) {
		//hacer parcelaValida (esta en rango y no hay construccion)
		if (parcelaValida(p[i]))
		{
			p = p[i];
			break;
		}
		i++;
	}
	int j = 0;
	Secuencia<Posicion> PS;
	int n = d.vueloRealizado().size();
	for (int j = 0; j < n; ++j)
	{
		PS[j] = d.vueloRealizado()[j];
	}
	PS[n] = p;
	d._trayectoria = PS;
	d._bateria = d.bateria() - 1;
	if (this->estadoDelCultivo(p) == NoSensado)
	{
		//pero si esta NoSensado, como puedo pasarlo a sensado? le pongo un valor que yo quiera? no aclara el programa... supongo que si... podria poner this->estadoDelCultivo(p) == algo
		sensarParcela(p);
	}
	else
	{
		aplicarProductos(d,p);
	}
}

void Sistema::mostrar(std::ostream & os) const
{
	// Muestra el campo del sistema con el estado debajo 

	{
	os << std::string(4, ' ');

	for(int j = 0; j < this->_campo_dimension.largo; j++){
		std::cout.setf (std::ios::left, std::ios::adjustfield);
		std::cout.width(20);
		os << j;
	}

	os << std::endl;

	for(int i = 0; i < this->_campo_dimension.ancho; i++){
		std::cout.width(4);
		os << i;
		for(int j = 0; j < this->_campo_dimension.largo; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(20);
			os << this->_campo_grilla.parcelas[i][j];
		}
		os << std::endl;

		for(int j = 0; j < this->_campo_dimension.largo; j++){
			Posicion p;
			p.x = i;
			p.y = j;
			if (this->_campo().contenido(p) == Cultivo){
				os << this->_estadoDelCultivo(p);
			}
		}
		os << std::endl;
	}

}

	//mostramos los drones del sistema 

	for (int i = 0; i < this->_enjambre.size(); i++){

		this->_enjambre[i].mostrar(os);
		os << std::endl;

	}



}

void Sistema::guardar(std::ostream & os) const
{
}

void Sistema::cargar(std::istream & is)
{
	//que encuentre el segundo {, y que cargue el campo. que encuentre el cuarto {, y que tome desde uno anterior (osea [) el tamaño de ese [], que después haga un while
	//(o for) donde cargue todos los drones dentro de ese []. luego, que busque el primer [[, y que ahi cargue los estados como dios manda.
	const std::string parcelaLetraInicial = "REL"/*ConMalezaConPlaga*/"N";
	const std::string caracterAnteriorAotrasCargas = " {";
	const std::string caracterPosteriorAotrasCargas = "}";
	const std::string caracterUltimoAotrasCargas = "]}]"; //creo que va a agarrar el primero de estos, no esto en si... seria find? sin first_of
	const std::string caracterAnteriorAestados = "[";
	const std::string caracterPosteriorAestados = "]";
	std::string sistemaAlmacenado;
	std::string::size_type i, j, k;
	Secuencia<Drone> ds;

	std::getline(is, sistemaAlmacenado);

	i = 1;
	i = sistemaAlmacenado.find_first_of(caracterAnteriorAotrasCargas, i);
	j = sistemaAlmacenado.find_first_of(caracterPosteriorAotrasCargas, i);
	this->_campo = (sistemaAlmacenado.substr(i, j)).cargar();

	i = sistemaAlmacenado.find_first_of(caracterAnteriorAotrasCargas, j);
	j = sistemaAlmacenado.find_first_of(caracterUltimoAotrasCargas, i);
	int l = 0;
	while (i < j) {
		k = sistemaAlmacenado.find_first_of(caracterPosteriorAotrasCargas, i);
		ds[l] = (sistemaAlmacenado.substr(i, k)).cargar();
		i = k;
		l++;
	}
	this->_enjambre = ds;

	i = sistemaAlmacenado.find_first_of(caracterAnteriorAestados, j);
	j = sistemaAlmacenado.find_last_of(caracterPosteriorAestados, i);
	while (i < j) {
		k = sistemaAlmacenado.find_first_of(caracterPosteriorAestados, i);
		//le asigna a cada posicion cada estado correspondiente... con this y eso...
		i = k;
	}
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	// TODO: insert return statement here
	return os;
}


//Auxiliares



Secuencia<Posicion> Sistema::parcelasDeCultivo() const{
	Secuencia<Posicion> parcelasDeCultivo;

	int i = 0 ;
	int j = 0 ;

	while (i < this->campo().dimensiones().ancho){
		while (j < this->campo().dimensiones().largo){
			Posicion p;
			p.x = i;
			p.y = j;
			if (this->campo().contenido(p) == Cultivo){
				parcelasDeCultivo.push_back(p);
			}
			j = j+1;
		}
		i=i+1;
	}
	return parcelasDeCultivo;
}

int Sistema::cantCultivosCosechables() const{

	int cuenta = 0;
	Secuencia<Posicion>::size_type i = 0 ;
	while (i < this->parcelasDeCultivo().size()){
		if (this->_estado.parcelas[this->parcelasDeCultivo()[i].x][this->parcelasDeCultivo()[i].y] == ListoParaCosechar){
			cuenta = cuenta + 1;
			i= i + 1 ;
		}
		else 
			i= i+1;
	}
	return cuenta;
}

/* No hay forma de cargar la batería ya que _bateria es privado y bateria() lo único que hace es devolver la batería que tiene
// La única forma es creando un nuevo drone
void cargarLaBateria(const Drone d) {
	Secuencia<Drone>::size_type i = 0;
	while (i < this->_enjambre.size() ) {
		if(this->enjambre[i].id() == d_id){
			this->enjambre[i].bateria()  = 100;
			Secuencia<Posicion> nuevaTrayectoria;
			this->enjambre[i]._trayectoria = nuevaTrayectoria;
			this->enjambre[i]._enVuelo = false ;
		}
	} 
}
*/



int dronesVolandoEnFila (Sistema s, int f) {

	int cuenta = 0 ;
	Secuencia<Drone>::size_type i = 0 ;
	while (i < s.enjambreDrones().size()) {
		if (s.enjambreDrones()[i].enVuelo() == true && s.enjambreDrones()[i].posicionActual().y == f){
			cuenta = cuenta + 1 ;
			i = i + 1;
		}else {
		    i = i + 1; 
		}
	}
	return cuenta;
}



int Sistema::recorridoMaximo(Drone d){

return minimo( minimo( this->fertAplicable(d), d.bateria()) , s.parcelasLibres());

}

template <class T> T minimo (T a , T b) {
	if (a < b ) {
		return a 
	}
	else 
		return b;
}


template <class T> int cuenta(Secuencia <T> ls , T e) {
	Secuencia<T>::size_type i = 0 ; 
	int cuenta = 0 ;
	while (i < ls.size()){
		if (ls[i] == e) {
			cuenta = cuenta +1 ;
			i = i + 1;

		}
		else 
			i = i+1 ;
	}
}


int Sistema::fertAplicable(Drone d){
	Secuencia<int> ls  ;
	Secuencia<int>::size_type i = 0;
	int i = 0;
	while (i <= d.posicionActual().X){
		if (this->cantFertilizables(i,d) <= cuenta(d.productosDisponibles(), Fertilizante) ){
			ls.push_back(i);
			i = i + 1;
		}
		else 
			i = i + 1;


	}
	if ls.size() > 0 {
		return d.posicionActual() - ls[0];
	}
	else 
		return d.posicionActual();

}

int Sistema::cantFertilizables(const int i , Drone d){
	int cantidad = 0;
	int j = i ;
	while ( j < d.posicionActual().x ){
		if (this->_estado[j][d.posicionActual().y] == RecienSembrado || this->_estado[j][d.posicionActual().y] == EnCrecimiento ){
			cantidad = cantidad + 1 ;
			j = j + 1; 
		}
		else j = j + 1;

	}
}

int Sistema::parcelasLibres(const Drone d ) {
	Secuencia<int> libres  ;
	Secuencia<int>::size_type i = 0;
	while (i <= d.posicionActual().x){
		Secuencia<Posicion>::size_type j = i ;
		bool condicion = true ;
		while ( condicion == true && j < d.posicionActual().x) {
			if (this->_campo_grilla[j][d.posicionActual().y] == Cultivo){
				j = j +1;
			}
			else {
				j = j + 1;
				condicion = false ;
			}
		}
		if (condicion == true) { libres.push_back(i) };
		i= i+1;
	}
	if (libres.size() > 0){ 
		return d.posicionActual().x - libres[0]; 
	}
	else return d.posicionActual().x;
} 




Secuencia<Producto> Sistema::mismosProductosDescontantoFertilizante(const Drone d){
	Secuencia<Producto> productos;
	Secuencia<Producto>::size_type i = 0;
	int cuentaFert = this->.recorridoMaximo(d);
	while( i < d.productosDisponibles.size()){
		Producto p = d.productosDisponibles[i];
		if (d.productosDisponibles()[i] == Fertilizante && cuentaFert > 0){
			cuentaFert = cuentaFert - 1;
		}
		else 
			productos.push_back(productos.push_back(p);


		i = i + 1;


	}

	return productos;
}

/*int recorridoMaximo(Sistema s, Drone d){

}

int minimo (int a , int b) {
	if (a < b ) {
		return a 
	}
	else 
		return b;
}

int fertAplicable(Sistema s, Drone d){
*/



bool Sistema::NoHayConstruccion(Posicion p) {
	bool m = false;
	if (this->_campo.contenido(p) == Cultivo)
	{
		m = true;
	}
	return m;
}


bool Sistema::posicionLibre(Posicion p) {
	bool m = false;
	Secuencia<Posicion> P = lugaresAdyacentes(p);
	
	bool m1, m2, m3, m4;
	if (!HayDrone(P[1]))
	{
		m1 = true;
	}
	if (!HayDrone(P[2]))
	{
		m2 = true;
	}
	if (!HayDrone(P[3]))
	{
		m3 = true;
	}
	if (!HayDrone(P[4]))
	{
		m4 = true;
	}


	if (m1 || m2 || m3 || m4)
	{
		m = true;
	}
	return m;
}

Secuencia<Posicion> Sistema::lugaresAdyacentes(Posicion p) {
	Secuencia<Posicion> P;
	Posicion p0, p1, p2, p3;
	
	p0.x = p.x + 1;
	p0.y = p.y;
	P[0] = p0;

	p1.x = p.x - 1;
	p1.y = p.y;
	P[1] = p1;

	p2.x = p.x;
	p2.y = p.y + 1;
	P[2] = p2;

	p3.x = p.x;
	p3.y = p.y - 1;
	P[3] = p3;

	return P;
}

bool Sistema::HayDrone(Secuencia<Posicion> P) {
	Secuencia<Drone> ds = this->_enjambre;
	bool m = true; 
	Secuencia<Drone>::size_type i = 0;
	Drone d;
	while (i < ds.size() && buscarPosicion(P, ds[i].posicionActual()) ) {
		i++;
		if (i == ds.size())
		{
			m = false; 
		}
	}
	return m;
}

/// DEFINIR!! no la defini porque uso !HayDrone
bool Sistema::noHayDrone(Posicion p){
	return false;
}

// no tengo ganas de es
bool Sistema::buscarPosicion(const Secuencia<Posicion> ps, const Posicion p) const{
	Secuencia<Posicion>::size_type i = 0;
	while(i < ps.size() && (p.x != ps[i].x && p.y != ps[i].y)){
		i++;
	}
	return i != ps.size();
}


Posicion Sistema::DondeEstaElGranero(Campo c) {
	Dimension d = c.dimensiones();
	int i = 0;
	Posicion p;
	while (i < d.largo && c.contenido(p) != Granero) {
		int j = 0;
		while (j < d.ancho && c.contenido(p) != Granero) {
			p.x = i;
			p.y = j;
		}
	}
	return p;
}


int Sistema::buscarDrone(Drone d) {
	int i = 0;
	while ( i < this.enjambreDrones().size()) {
		if (mismoDrone(this.enjambreDrones()[i], d))
		{
			break;
		}
		i++;
	}
	return i;
}

bool mismoDrone(Drone d, e) {
	bool m = false;
	//acá comparo una serie de cosas, tipo lista, que nose si puedo comparar así no mas... nose si pasara en otras parte del TP, habria que ver...
	if (d.id == e.id && d.bateria() == e.bateria() && d.enVuelo == e.enVuelo && d.vueloRealizado() == e.vueloRealizado() && d.posicionActual() == e.posicionActual && d.productosDisponibles() == e.productosDisponibles())
	{
		m = true;
	}
	return m;
}


Posicion parcelaValida(Secuencia<Posicion> ps) {
	Posicion p;
	// asi si no encuentra una valida, devuelve algo que no tiene sentido, como para no dejar que devuelva algo del campo pero no valido...
	p.x = -1;
	p.y = -1;
	int i = 0;
	while ( i < ps.size()) {
		if (NoHayConstruccion(ps[i]) && !HayDrone(ps[i]))
		{
			p = ps[i];
			break;
		}
		i++;
	}
	return p;
}


//la duda que esta arriba, que hago acá? le asigno yo un estado que se me cante? 
////tenes que modificar el sistema para sensar la parcela
void Sistema::sensarParcela(Posicion p) {
	switch(p) {
		case estadoDelCultivo(p) == 
	}
}


void Sistema::aplicarProductos(Drone d, Posicion p) {
	switch(p)
	case (this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento) && tieneProducto(d, Fertilizante):	this->estadoDelCultivo(p) == ListoParaCosechar && listaProductosSin(Fertilizante, Fertilizante, d);
		break;
	case this->estadoDelCultivo(p) == ConMaleza && (tieneProducto(d, Herbicida) || tieneProducto(d, HerbicidaLargoAlcance)) && d.bateria() => 5:	this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(Herbicida, HerbicidaLargoAlcance, d);
		break;
	case this->estadoDelCultivo(p) == ConPlaga && ((tieneProducto(d, Plaguicida) && d.bateria() => 10) || (tieneProducto(d, PlaguicidaBajoConsumo) && d.bateria() => 5)):	this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(Plaguicida, PlaguicidaLargoAlcan, d);
		break;
}

bool tieneProducto(Drone d, Producto p) {
	bool m = false;
	int i = 0;
	while (i < d.productosDisponibles().size()) {
		if (d.productosDisponibles[i] == p)
		{
			m = true;
			break;
		}
		i++;
	}
	return m;
}

void listaProductosSin(Producto s, p, Drone d) {
	Secuencia<Producto> ps;
	if (tieneProducto(d, s))
	{
		int j = primerLugarCon(d.productosDisponibles(), s);
		int i = 0;
		while (i < d.productosDisponibles().size()) {
			if (i != j)
			{
				ps[i] = d.productosDisponibles()[i]
			}
			
		}
	}
	else
	{
		if (tieneProducto(d, p))
		{
			int j = primerLugarCon(d.productosDisponibles(), p);
			int i = 0;
			while (i < d.productosDisponibles().size()) {
				if (i != j)
				{
					ps[i] = d.productosDisponibles()[i]
				}
			
			}
		}
	}
	d._productos = ps;
}

int primerLugarCon(Secuencia<Producto> ps, Producto p) {
	int m = ps.size() + 1;
	//si no esta, va a devolver algo mas grande que el size de ps, entonces ya se ve que no estaba...
	int i = 0;
	while (i < ps.size()) {
		if (ps[i] == p)
		{
			m = i;
		}
	}
	return m;
}