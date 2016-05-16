#ifndef TIPOS_H
#define TIPOS_H

#include <vector>
#include <stdlib.h>

#include <string>
#include <iostream>


typedef int ID;
typedef int Carga;
typedef int Ancho;
typedef int Largo;
template<typename T>
typedef std::vector<T> Secuencia;

enum Parcela{Cultivo, Granero, Casa};
enum Producto{Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance};
enum EstadoCultivo {RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado};

struct Posicion
{
	int x;
	int y;
};

struct Dimension
{
	Ancho ancho;
	Largo largo;
};

templace<typename T>
struct Grilla
{
	Grilla(Dimension d)
	{
		grilla.resize(d.ancho, std::vector<T>(d.largo, Parcela::Cultivo));
	}
	std::vector<std::vector<T>> parcelas;
};

struct InfoVueloCruzado
{
	Posicion posicion;
	int cantidadCruces;
};

#endif /*TIPOS_H*/
