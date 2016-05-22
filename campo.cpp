#include "campo.h"

Campo::Campo()
{
	this->_dimension.ancho = 2;
	this->_dimension.largo = 2;
	Grilla<Parcela> g(this->_dimension);
	g.parcelas[0][0] = Casa;
	g.parcelas[1][1] = Granero;
	this->_grilla.parcelas = g.parcelas;
}

Campo::Campo(const Posicion &posG, const Posicion &posC)
{
	this->_dimension.ancho = std::max(posG.x, posC.x) + 1;
	this->_dimension.largo = std::max(posG.y, posC.y) + 1;
	Grilla<Parcela> g(this->_dimension);
	g.parcelas[posC.x][posC.y] = Casa;
	g.parcelas[posG.x][posG.y] = Granero;
	this->_grilla.parcelas = g.parcelas;
}

Dimension Campo::dimensiones() const
{
	return this->_dimension;
//	return Dimension();
}

Parcela Campo::contenido(const Posicion & p) const
{
	return this->_grilla.parcelas[p.x][p.y];
//	return Parcela();
}

void Campo::mostrar(std::ostream & os) const
{
	os << std::string(4, ' ');

	for(int j = 0; j < this->_dimension.largo; j++){
		std::cout.setf (std::ios::left, std::ios::adjustfield);
		std::cout.width(9);
		os << j;
	}

	os << std::endl;

	for(int i = 0; i < this->_dimension.ancho; i++){
		std::cout.width(4);
		os << i;
		for(int j = 0; j < this->_dimension.largo; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(9);
			os << this->_grilla.parcelas[i][j];
		}
		os << std::endl;
	}
}

void Campo::guardar(std::ostream & os) const
{
	os << "{ C [" << std::to_string(this->_dimension.ancho) << "," << std::to_string(this->_dimension.largo) << "] ";
	os << "[";

	for(int i = 0; i < this->_dimension.ancho; i++){
		os << "[";
		for(int j = 0; j < this->_dimension.largo; j++){
			os << this->_grilla.parcelas[i][j];
			if(j != this->_dimension.largo) os << ",";
		}
		os << "]";
	}

	os << "]}";
}

void Campo::cargar(std::istream & is)
{
	const std::string numeros = "0123456789";
	const std::string parcelaLetraInicial = "CG";
	const std::string caracterPosteriorPrimerNumeros = ",";
	const std::string caracterPosteriorSegundoNumeros = "]";
	const std::string caracterPosteriorParcela = ",]";
	std::string campoAlmacenado;
	std::string::size_type i, j;

	std::getline(is, campoAlmacenado);
	
	i = 0;
	i = campoAlmacenado.find_first_of(numeros, i);
	j = campoAlmacenado.find(caracterPosteriorPrimerNumeros, i);
	this->_dimension.ancho = std::stoi(campoAlmacenado.substr(i, j-i));
	i = j;
	i = campoAlmacenado.find_first_of(numeros, i);
	j = campoAlmacenado.find(caracterPosteriorSegundoNumeros, i);
	this->_dimension.largo = std::stoi(campoAlmacenado.substr(i, j-i));
	i = j;

	Grilla<Parcela> g(this->_dimension);

	for(int posAncho = 0; posAncho < this->_dimension.ancho; posAncho++){
		for(int posLargo = 0; posLargo < this->_dimension.largo; posLargo++){
			i = campoAlmacenado.find_first_of(parcelaLetraInicial, i);
			j = campoAlmacenado.find_first_of(caracterPosteriorParcela, i);
			g.parcelas[posAncho][posLargo] = Campo::stringAParcela(campoAlmacenado.substr(i, j-i));
			i = j;
		}
	}

	this->_grilla.parcelas = g.parcelas;
}

bool Campo::operator==(const Campo & otroCampo) const
{
	return this->_dimension.ancho == otroCampo._dimension.ancho && this->_dimension.largo == otroCampo._dimension.largo && this->_grilla.parcelas == otroCampo._grilla.parcelas;
}

std::ostream & operator<<(std::ostream & os, const Campo & c)
{
	c.mostrar(os);
	return os;
}

std::ostream & operator<<(std::ostream & os, const Parcela & p)
{
	switch(p){
		case Casa:		os << "Casa";
			break;
		case Cultivo: 	os << "Cultivo";
			break;
		case Granero: 	os << "Granero";
			break;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, const Producto & p)
{
	switch(p){
		case Casa:		os << "Fertilizante";
			break;
		case Cultivo: 	os << "Plaguicida";
			break;
		case Granero: 	os << "PlaguicidaBajoConsumo";
			break;
		case Granero: 	os << "Herbicida";
			break;
		case Granero: 	os << "HerbicidaLargoAlcance";
			break;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e)
{
	switch(p){
		case Casa:		os << "RecienSembrado";
			break;
		case Cultivo: 	os << "EnCrecimiento";
			break;
		case Granero: 	os << "ListoParaCosechar";
			break;
		case Granero: 	os << "ConMaleza";
			break;
		case Granero: 	os << "ConPlaga";
			break;
		case Granero: 	os << "NoSensado";
			break;
	}
	return os;
}

// Auxiliares

Parcela Campo::stringAParcela(std::string s){
	Parcela p;

	if(s == "Casa") p = Casa;
	else if (s == "Cultivo") p = Cultivo;
	else p = Granero;

	return p;
}