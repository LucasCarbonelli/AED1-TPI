#include "sistema.h"
#include <algorithm>

Sistema::Sistema()
{
	// Creo un campo con el constructor por defecto.
	Campo c = Campo();
	this->_campo = c;
	// Creo un listado de drones vacio.
	Secuencia<drone> ds; 
	this->_enjambre = ds;
	// Creo la grilla correspondiente al campo y le asigno el estado noSensado a todos los cultivos.
	Grilla<EstadoCultivo> EC(c.dimensiones());
	int i = 0;
	int j = 0;
	while(i < c.dimensiones.ancho){
		while(j < c.dimenciones.largo){
			Posicion p;
			p.x = i;
			p.y = j;
			if (c.contenido(p) == Cultivo) {this->_estado[i][j] = NoSensado}
			j++;
		}
		i++;

	}
	this->_estado = EC;
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
	this->_campo = c;
	this->_enjambre = ds;
	Grilla<EstadoCultivo> EC(c.dimensiones());
	int i = 0;
	int j = 0;
	while(i < c.dimensiones.ancho){
		while(j < c.dimensiones.largo){
			Posicion p;
			p.x = i;
			p.y = j;
			if (c.contenido(p) == Cultivo) {this->_estado[i][j] = NoSensado};
			j++;
		}
		i++;

	}
	this->_estado = EC;
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

	//quiza ese lo paso a auxiliar
	while (i < this->.campo().dimensiones().ancho) {

		while (j < this.campo().dimensiones().largo) {
			Posicion p;
			p.x = i;
			p.y = j;
			//se puede preguntar y o asignar estado de cultivo a posiciones así? funciona así esto? no hay que hacer pasaje por stringtoalgo ni nada?
			if (this.estadoDelCultivo(p) == ConPlaga)
			{
				PosConPlaga.push_back(p);
			}
			j++
		}
		i++
	}



	//quiza esto lo paso a auxiliar
	int k = 0;
	while (k < PosConPlaga.size()) {
		Posicion p;
		p.x = PosConPlaga[k].x
		p.y = PosConPlaga[k].y
		//al final x es el largo o el ancho??
		if (PosConPlaga[k].x + 1 <= this.campo().dimensiones().largo)
		{
			p.x = PosConPlaga[k].x + 1;
			this.estadoDelCultivo(p) = ConPlaga;
		}
		//nose si es necesario aclarar que en este caso esa pos debe ser menor que el largo, ya que si la pos original era menor al largo, -1 lo va a ser seguro.
		if (PosConPlaga[k].x - 1 >= 0 && PosConPlaga[k].x - 1 <= this.campo().dimensiones().largo)
		{
			p.x = PosConPlaga[k].x - 1;
			this.estadoDelCultivo(p) = ConPlaga;
		}
		if (PosConPlaga[k].y + 1 <= this.campo().dimensiones().ancho)
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y + 1;
			this.estadoDelCultivo(p) = ConPlaga;
		}
		//idem arriba.
		if (PosConPlaga[k].y - 1 >= 0 && PosConPlaga[k].y - 1 <= this.campo().dimensiones().ancho)
		{
			p.x = PosConPlaga[k].x;
			p.y = PosConPlaga[k].y - 1;
			this.estadoDelCultivo(p) = ConConPlaga;
		}
	}
}

void Sistema::despegar(const Drone & d)
{
	//mismo problema que en seVinoLaMaleza con el tema de parametro con const
	if (buscarDrone(d, this))
	{
		D = d; //como asignador, se puede? sino hago un aux, o pongo todo lo que signfica acá.
		Posicion p;
		p.x = 1;
		p.y = 1;
		if(posicionLibre(p, this)) {
			D.enVuelo() && D.bateria() = 100;
		}
		if (posicionLibre(posG, this) && (posG.x != 1 || posG.y !=1))
		{
			D.enVuelo() && D.bateria() = 100;
		}
	}


	//lo de posG lo puse porque lo pasan por parametro en crear campo, en el cpp... medio turbio igual, sino como se donde esta el granero?
	//puse dos if por el caso del campo creado sin parametros
	
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





