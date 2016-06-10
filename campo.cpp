#include "campo.h"

// Decidimos arbitrariamente que las dimensiones del campo sean 2x2 y que la Casa esté ubicada en (0,0) y el Granero en (1,1)
Campo::Campo()
{
	this->_dimension.ancho = 2;
	this->_dimension.largo = 2;
	Grilla<Parcela> g(this->_dimension);
	g.parcelas[0][0] = Casa;
	g.parcelas[1][1] = Granero;
	this->_grilla.parcelas = g.parcelas;
}

Campo::Campo(const Posicion &posG, const Posicion &posC, Dimension dimension) {
    _dimension = dimension;
    _grilla = Grilla<Parcela>(_dimension);
    _grilla.parcelas[posG.x][posG.y] = Granero;
    _grilla.parcelas[posC.x][posC.y] = Casa;
}

// Decidimos arbitrariamente que las dimensiones del campo sean las menores capaces de contener a la Casa y el Granero en las posiciones que se pasan como parámetro
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
}

Parcela Campo::contenido(const Posicion & p) const
{
	return this->_grilla.parcelas[p.x][p.y];
}

void Campo::mostrar(std::ostream & os) const
{
	// muestra el campo como una tabla, el eje horizontal es 'ancho' y el vertical es 'largo', en cada casillero se muestra el contenido de la parcela

	// muestra la coordenada en el eje 'ancho'
	os << std::string(4, ' ');

	for(int j = 0; j < this->_dimension.ancho; j++){
		std::cout.setf (std::ios::left, std::ios::adjustfield);
		std::cout.width(9);
		os << j;
	}

	os << std::endl;

	// muestra la coordenada en el eje 'largo' y el contenido de la parcela

	for(int i = 0; i < this->_dimension.largo; i++){
		std::cout.width(4);
		os << i;
		for(int j = 0; j < this->_dimension.ancho; j++){
			std::cout.setf (std::ios::left, std::ios::adjustfield);
			std::cout.width(9);
			os << this->_grilla.parcelas[j][i];
		}
		os << std::endl;
	}
}

void Campo::guardar(std::ostream & os) const
{
	// guarda las dimensiones
	os << "{ C [" << std::to_string(this->_dimension.ancho) << "," << std::to_string(this->_dimension.largo) << "] ";
	os << "[";

	// recorre la grilla de parcelas y guarda el contenido de las mismas
	for(int i = 0; i < this->_dimension.ancho; i++){
		os << "[";
		for(int j = 0; j < this->_dimension.largo; j++){
			os << this->_grilla.parcelas[i][j];
			if(j < this->_dimension.largo - 1) os << ",";
		}
		os << "]";
		if(i < this->_dimension.ancho - 1) os << ", ";
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
	const char caracterFinal = '}';
	std::string campoAlmacenado;
	std::string::size_type i, j;

	// genera un string con toda la información del campo
	std::getline(is, campoAlmacenado, caracterFinal);

	// busca números en el string que representan las dimensiones del campo
	i = 0;
	i = campoAlmacenado.find_first_of(numeros, i);
	j = campoAlmacenado.find(caracterPosteriorPrimerNumeros, i);
	this->_dimension.ancho = std::stoi(campoAlmacenado.substr(i, j-i));
	i = j;
	i = campoAlmacenado.find_first_of(numeros, i);
	j = campoAlmacenado.find(caracterPosteriorSegundoNumeros, i);
	this->_dimension.largo = std::stoi(campoAlmacenado.substr(i, j-i));
	i = j;

	// busca letras que coincidan con la primera y la última letra de los nombres del tipo parcela
	// toma la palabra en formato string, la transforma a tipo parcela y se la asigna a la posición correspondiente en la grilla de parcelas
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


// Auxiliares

Parcela Campo::stringAParcela(const std::string s) const {
	Parcela p;

	if(s == "Casa") p = Casa;
	else if (s == "Cultivo") p = Cultivo;
	else if (s == "Granero") p = Granero;

	return p;
}