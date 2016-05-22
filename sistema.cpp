#include "sistema.h"
#include <algorithm>

Sistema::Sistema()
{
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
}

const Campo & Sistema::campo() const
{
	return Campo();
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
	return EstadoCultivo();
}

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
	return Secuencia<Drone>();
}

void Sistema::crecer()
{
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	//aca no hablo de this? igual me pasan como const la secuencia ps, no puedo hacer esto?? otra: me armo un PS que tenga todos los p de ps, sacados de this,
	//y a eso le hago esto. Hice algo parecido a esto en el ejer despegar, mas abajo.
	int i = 0;
	while (i < ps.size()) {
		estadoDelCultivo(p[i]) = ConMaleza;
		i++;
	}
}

void Sistema::seExpandePlaga()
{
	int i = 0;
	Secuencia<Posicion> PosConPlaga;

	//quiza ese lo paso a auxiliar
	while (i < this.campo().dimensiones().ancho) {

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
	return false;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
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
