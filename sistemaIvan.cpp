// La funcion buscarDrone(d, this) no está definida!!!!! Esta función hay que revisarla, la comento para que compile
void Sistema::despegar(const Drone & d)
{
	if (mismoDrone(d, this->enjambreDrones()[buscarDrone(d)]))
	{
		if (posicionLibre(DondeEstaElGranero(this->_campo)))
		{
			//this->enjambreDrones()[buscarDrone(d)]._enVuelo && this->enjambreDrones()[buscarDrone(d)]._bateria = 100;
			; // mañana vemos........ --->>> lo use devuelta che
		}
	}

}


//acá no use push_back, en auxiliares y que se yo... si está bien, está bien, sino, quizá algunas cosas se arreglan con el push_back
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



//Auxiliares

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
EstadoCultivo sensarParcela(Posicion p) {
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