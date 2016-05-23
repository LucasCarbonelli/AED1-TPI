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
//	this->_estado = EC;
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
	this->_campo = c;
	this->_enjambre = ds;
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
}

const Sistema::campo() const
{
	return this->_campo;
}

const EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
	return this->_estado[p.x][p.y];
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
	while(i < c.dimensiones.ancho){
		while(j < c.dimensiones.largo){
			Posicion pos;
			pos.x = i;
			pos.y = y;
			if (c.contenido(pos) == Cultivo){
				if (this->estado[i][j] == RecienSembrado) {
					this->estado[i][j] = EnCrecimiento;
				}
				else if (this->estado[i][j] == EnCrecimiento) {
					this->estado[i][j] = ListoParaCosechar;
				}

				
			}

		}
	}
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	//aca no hablo de this? igual me pasan como const la secuencia ps, no puedo hacer esto?? otra: me armo un PS que tenga todos los p de ps, sacados de this,
	//y a eso le hago esto. Hice algo parecido a esto en el ejer despegar, mas abajo.
	int i = 0;
	while (i < ps.size()) {
		this->_estado[ps[i].x][ps[i].y] = ConMaleza;
		i++;
	}
}

void Sistema::seExpandePlaga()
{
	int i = 0;
	Secuencia<Posicion> PosConPlaga;

	while (i < this.campo().dimensiones().ancho) {
		int j = 0;
		while (j < this.campo().dimensiones().largo) {
			Posicion p;
			p.x = i;
			p.y = j;
			if (this.estadoDelCultivo(p) == ConPlaga)
			{
				PosConPlaga.push_back(p);
			}
			j++
		}
		i++
	}

	int k = 0;
	while (k < PosConPlaga.size()) {
		Posicion p;
		p.x = PosConPlaga[k].x
		p.y = PosConPlaga[k].y
		//al final x es el largo o el ancho??
		if (PosConPlaga[k].x + 1 <= this.campo().dimensiones().largo && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x + 1;
			this-> _estado[p.x][p.y] = ConPlaga;
		}
		//nose si es necesario aclarar que en este caso esa pos debe ser menor que el largo, ya que si la pos original era menor al largo, -1 lo va a ser seguro.
		if (PosConPlaga[k].x - 1 >= 0 && PosConPlaga[k].x - 1 <= this.campo().dimensiones().largo && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x - 1;
			this-> _estado[p.x][p.y] = ConPlaga;
		}
		if (PosConPlaga[k].y + 1 <= this.campo().dimensiones().ancho && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y + 1;
			this-> _estado[p.x][p.y] = ConPlaga;
		}
		//idem arriba.
		if (PosConPlaga[k].y - 1 >= 0 && PosConPlaga[k].y - 1 <= this.campo().dimensiones().ancho && NoHayConstruccion(p))
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y - 1;
			this-> _estado[p.x][p.y] = ConPlaga;
		}
	}
}

void Sistema::despegar(const Drone & d)
{
	//mismo problema que en seVinoLaMaleza con el tema de parametro con const
	if (buscarDrone(d, this))
	{
		D = d; //como asignador, se puede? sino hago un aux, o pongo todo lo que signfica acá.
		if (posicionLibre(DondeEstaGranero(this->_campo)))
		{
			D._enVuelo && D._bateria = 100;
		}
	}
}

bool Sistema::listoParaCosechar() const
{
	return cantCultivosCosechables(this) >= 0.9 * parcelasDeCultivo(this).size())  
}

void Sistema::aterrizarYCargarBaterias(Carga b){

	int i= 0;
	while (i < this->_enjambre.size()){
		if (this->_enjambre[i]_bateria < b ) {
			cargarLaBateria(this, this->_enjambre[i]);
			i=i+1;
		}
		else 
			i=i+1;
	}

}


void Sistema::fertilizarPorFilas()
{
}

void Sistema::volarYSensar(const Drone & d)
{
}

void Sistema::mostrar(std::ostream & os) const
{
}

void Sistema::guardar(std::ostream & os) const
{
}

void Sistema::cargar(std::istream & is)
{
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



Secuencia<Posicion> parcelasDeCultivo(const Sistema s){
	Secuencia<Posicion> parcelasDeCultivo ;
	int i = 0 ;
	int j = 0 ;

	while (i < s_campo_dimensiones_ancho){
		while (j < s_campo_dimensiones_largo){
			if (s_campo_grilla[i][j] == Cultivo){
				j= i+1;
				Posicion p ;
				p_x = i;
				p_y = j;
				parcelasDeCultivo.push_back(p);
			}
			j = j+1;

		}
		i=i+1;

	}
	return parcelasDeCultivo;

}

int cantCultivosCosechables(const Sistema s){

	int cuenta = 0;
	int i = 0 ;
	while (i< parcelasDeCultivo(s).size()){
		if (s_estado[parcelasDeCultivo(s)[i].x][parcelasDeCultivo(s)[i].x] == ListoParaCocechar){
			cuenta = cuenta + 1;
			i= i + 1 ;

		}
		else 
			i= i+1;
	}
	return cuenta;
}

void cargarLaBateria(Sistema s , const Drone d) {
	int i = 0;
	while (i < s._enjambre.size() ) {
		if(s_enjambre[i]._id == d_id){
			s_enjambre[i]._bateria  = 100;
			Secuencia<Posicion> nuevaTrayectoria;
			s_enjambre[i]._trayectoria = nuevaTrayectoria;
			s_enjambre[i]._enVuelo = false ;
		}
	} 
}




int dronesVolandoEnFila (Sistema s, int f) {

	int cuenta = 0 ;
	int i = 0 ;
	while (i< s_enjambre.size()) {
		if (s_enjambre[i]_enVuelo== true && s_enjambre[i].posicionActual().y == f){
			cuenta = cuenta + 1 ;
			i = i + 1;
		else 
		    i = i + 1; 
		} 

	}
	return cuenta;
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
	if (noHayDrone(P[1]))
	{
		m1 = true;
	}
	if (noHayDrone(P[2]))
	{
		m2 = true;
	}
	if (noHayDrone(P[3]))
	{
		m3 = true;
	}
	if (noHayDrone(P[4]))
	{
		m4 = true;
	}


	if (m1 || m2 || m3 || m4)
	{
		m = true;
	}
	reutrn m;
}

Secuencia<Posicion> Sistema::lugaresAdyacentes(Posicion p) {
	Secuencia<Posicion> P;
	Posicion p0, p1, p2, p3;
	
	p0.x = p.x + 1;
	p0.y = p.y;
	P[0] = p0;

	p1.x = p.x - 1;
	p1.y = p.y;
	P[1] = p1

	p2.x = p.x;
	p2.y = p.y + 1;
	P[2] = p2;

	p3.x = p.x;
	p3.y = p.y - 1;
	P[3] = p3;
}

bool Sistema::HayDrone(Secuencia<Posicion> P) {
	Secuencia<Drone> ds = this->_enjambre;
	bool m = true;
	int i = 0;
	Drone d;
	while (i < ds.size() && posicionActual(ds[i]) != P) {
		i++;
		if (i = ds.size())
		{
			m = false;
		}
	}
	return m;
}


Parcela Sistema::DondeEstaElGranero(Campo c) {
	Dimension d = dimension(c);
	int i = 0;
	Posicion p;
	while (i < d.largo && contenido(p) != Granero) {
		int j = 0;
		while (j < d.ancho && contenido(p) != Granero) {
			p.x = i;
			p.y = j;
		}
	}
	return p;
}

