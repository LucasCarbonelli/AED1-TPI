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
	while(i < c.dimensiones().largo){
		while(j < c.dimensiones().ancho){
			Posicion pos;
			pos.x = j;
			pos.y = i;
			if (c.contenido(pos) == Cultivo){
				if (this->estadoDelCultivo(pos) == RecienSembrado) {
					this->_estado.parcelas[j][i] = EnCrecimiento;
				}
				if (this->estadoDelCultivo(pos) == EnCrecimiento) {
					this->_estado.parcelas[j][i] = ListoParaCosechar;
				}
			}
			j++;
				
		}
		i++;
		
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

void Sistema::despegar(const Drone & d)
{
	if (d == enjambreDrones()[buscarDrone(d)])
	{
		if (posicionLibre(DondeEstaElGranero(_campo)))
		{
			Posicion p = damePosicionLibre(lugaresAdyacentes(DondeEstaElGranero(_campo)));
			//No aclaro que bateria = 100 ya que es un requiere para el drone que pasa. Lo unico que asegura que realmente cambio
			// es su estado de vuelo, y su posición actual (incluida en su trayectoria, todo esto ya tenido en cuenta en moverA).
			_enjambre[buscarDrone(d)].moverA(p);
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

			// Usamos d para lo poner this->enjambreDrones()[i] y que el codigo quede mas "lindo"
			Drone d =  this->enjambreDrones()[i] ;


			//Aca movemos el drone hasta donde recorrido maximo nos indica que es posible agregando las posiciones que recorremos a la trayectoria.

			int j = d.posicionActual().x;
			int recorridoMaximo = this->recorridoMaximo(this->enjambreDrones()[i]);
			while ( j < d.posicionActual().x - recorridoMaximo ){
				Posicion p ;
				p.x = j + 1;
				p.y = this->enjambreDrones()[i].posicionActual().y;
				d.moverA(p);
				Carga nuevaCarga = d.bateria() - 1; 
				d.setBateria(nuevaCarga);
				j = j + 1;

				// Si es posible le aplicamos el fertilizante a la parcela que agregamos.
				if (this->campo().contenido( p ) == Cultivo && ( this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento)){
					this->_estado.parcelas[p.x][p.y] = ListoParaCosechar;
				}

		i = i + 1;



		} 
	}

} 

}

void Sistema::volarYSensar(const Drone & d)
{
	Secuencia<Posicion> ps = lugaresAdyacentes(d.posicionActual());
	Posicion p = parcelaValida(ps);

	/////// si ninguna de las parcelas era valida movias el drone a (-1,-1) y das un sistema invalido por que hay un drone en cualquier lugar .  
	if ( p.x != -1 && p.y != -1){

		// tenias que fijarte si tiene bateria para para aplicar los productos por que consumen bateria , sino tambien le ponias al drone bateria negativo y el sistema es invalido.

		///estabas modifiando el drone lo cual no podes hacer por el const , lo que habia que modifilar era el sistema.

		/*d.moverA(p);
		int bateriaVieja = d.bateria(); //  el tipo era carga , no int 
		d.setBateria(bateriaVieja - 1);*/

		this->_enjambre[buscarDrone(d)].moverA(p);
		Carga nuevaCarga = d.bateria() - 1 ;
		this->_enjambre[buscarDrone(d)].setBateria(nuevaCarga - 1);



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

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return this->campo() == otroSistema.campo() && this->igualEstadoDelCultivo(otroSistema) && this->igualEnjambreDrones(otroSistema);
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	// TODO: insert return statement here
	s.mostrar(os);
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



int Sistema::dronesVolandoEnFila (int f) {

	int cuenta = 0 ;
	Secuencia<Drone>::size_type i = 0 ;
	while (i < enjambreDrones().size()) {
		if (enjambreDrones()[i].enVuelo() == true && enjambreDrones()[i].posicionActual().y == f){
			cuenta = cuenta + 1 ;
			i = i + 1;
		}else {
		    i = i + 1; 
		}
	}
	return cuenta;
}



int Sistema::recorridoMaximo(Drone d){

return minimo( minimo( this->fertAplicable(d), d.bateria()) ,parcelasLibres(d));

}

template <class T> T Sistema::minimo (const T a , const T b) const {
	if (a < b ) {
		return a; 
	}
	else 
		return b;
}


template <class T> int Sistema::cuenta(const Secuencia <T> ls , const T e) const {
	Secuencia<Producto>::size_type i = 0 ; 
	int cuenta = 0 ;
	while (i < ls.size()){
		if (ls[i] == e) {
			cuenta = cuenta +1 ;
			i = i + 1;
		}
		else 
			i = i+1 ;
	}
	return cuenta;
}


int Sistema::fertAplicable(Drone d){
	Secuencia<int> ls  ;
	int i = 0;
	while (i <= d.posicionActual().x){
		if (this->cantFertilizables(i,d) <= cuenta(d.productosDisponibles(), Fertilizante) ){
			ls.push_back(i);
			i = i + 1;
		}
		else 
			i = i + 1;


	}
	if (ls.size() > 0) {
		return d.posicionActual().x - ls[0];
	}
	else {return d.posicionActual().x;}

}

int Sistema::cantFertilizables(const int i , Drone d){
	int cantidad = 0;
	int j = i ;
	while ( j < d.posicionActual().x ){
		Posicion p ;
		p.x = i;
		p.y = d.posicionActual().y;
		if (this->estadoDelCultivo(p) == RecienSembrado || this->estadoDelCultivo(p) == EnCrecimiento ){
			cantidad = cantidad + 1 ;
			j = j + 1; 
		}
		else j = j + 1;

	}
	return cantidad;
}

int Sistema::parcelasLibres(const Drone d ) {
	Secuencia<int> libres  ;
	int i = 0;
	//acá sólo quedaron dos warnings
	while (i <= d.posicionActual().x){
		int j = i ;
		bool condicion = true ;
		while ( condicion == true && j < d.posicionActual().x) {
			if ((_campo).contenido(d.posicionActual()) == Cultivo){
				j = j +1;
			}
			else {
				j = j + 1;
				condicion = false;
			}
		}
		if (condicion == true) { libres.push_back(i); }
		i= i+1;
	}
	if (libres.size() > 0){ 
		return d.posicionActual().x - libres[0]; 
	}
	else return d.posicionActual().x;
} 




Secuencia<Producto> Sistema::mismosProductosDescontandoFertilizante(const Drone d){
	Secuencia<Producto> productos;
	Secuencia<Producto>::size_type i = 0;
	int cuentaFert = this->recorridoMaximo(d);
	while( i < d.productosDisponibles().size()){
		Producto p = d.productosDisponibles()[i];
		if (d.productosDisponibles()[i] == Fertilizante && cuentaFert > 0){
			cuentaFert = cuentaFert - 1;
		}
		else 
			productos.push_back(p);

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

bool Sistema::HayDrone(Posicion P) {
	Secuencia<Drone> ds = this->_enjambre;
	bool m = true;
	Secuencia<Drone>::size_type i = 0;
	Drone d;
	//ya esta arreglado, lo puesto en /**/ se puede sacar
	while (i < ds.size() /*&& buscarPosicion(P, ds[i].posicionActual())*/ ) {
		i++;
		if ((ds[i].posicionActual().x == P.x) && (ds[i].posicionActual().y == P.y))
		{
			m = false;
			break;
		}
	}
	return m;
}

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


Posicion Sistema::damePosicionLibre(Secuencia<Posicion> ps) {
	Secuencia<Posicion>::size_type i = 0;
	Posicion p;
	p.x = -1;
	p.y = -1;
	while (i < ps.size()) {
		if (!HayDrone(ps[i]))
		{
			p.x = ps[i].x;
			p.y = ps[i].y;
			break;
		}
		i++;
	}
	return p;
}


bool Sistema::buscarPosicion(const Secuencia<Posicion> ps, const Posicion p) const{
	Secuencia<Posicion>::size_type i = 0;
	while(i < ps.size() && !(p.x == ps[i].x && p.y == ps[i].y)) {
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
	Secuencia<Drone>::size_type i = 0;
	while ( i < _enjambre.size()) {
		if (_enjambre[i] == d)
		{
			break;
		}
		i++;
	}
	return i;
}



Posicion Sistema::parcelaValida(Secuencia<Posicion> ps) {
	Posicion p;
	// asi si no encuentra una valida, devuelve algo que no tiene sentido, como para no dejar que devuelva algo del campo pero no valido...
	p.x = -1;
	p.y = -1;
	Secuencia<Posicion>::size_type i = 0;
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

	this->_estado.parcelas[p.x][p.y] = RecienSembrado;
	
}



void Sistema::aplicarProductos(Drone d, Posicion p) {
	//revisar: después del : uso &&, pero sacarProducto no devuelve bool, y a su vez digo estadoCultivo==, osea bool, no asignación, y la idea es asignarlo
	//además, parece que switch funciona con un int en (), no con un tipo cualquier, ver eso
	/////faltaba cambiarle la bateria al drone si es que uso productos



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


int Sistema::primerLugarCon(Secuencia<Producto> ps, Producto p) {
	int m = ps.size() + 1;
	//si no esta, va a devolver algo mas grande que el size de ps, entonces ya se ve que no estaba...
	Secuencia<Producto>::size_type i = 0;
	while (i < ps.size()) {
		if (ps[i] == p)
		{
			m = i;
		}
	}
	return m;
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
	for(int i = 0; i < this->_campo.dimensiones().ancho; i++){
		for(int j = 0; j < this->_campo.dimensiones().largo; j++){
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



/*Grilla<EstadoCultivo> nuevoEstado;
					for (int i = 0; i < this->campo().dimensiones().ancho; ++i)
					{
						Secuencia<EstadoCultivo> estadoLargo;

						for (int j = 0; j < this->campo().dimensiones().largo ; ++j)
						{
							if ( i == p.x || j == p.y){
								estadoLargo.push_back(ListoParaCosechar); 

							}
							else{
								Posicion po;
								po.x = i;
								po.y = j; 
								estadoLargo.push_back(this->estadoDelCultivo(po));
							}
						}
						nuevoEstado.push_back(estadoLargo);
					}*/

