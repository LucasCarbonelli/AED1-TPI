// La funcion buscarDrone(d, this) no está definida!!!!! Esta función hay que revisarla, la comento para que compile
void Sistema::despegar(const Drone & d)
{
	if (d == this->enjambreDrones()[buscarDrone(d)])
	{
		if (posicionLibre(DondeEstaElGranero(this->_campo)))
		{
			//this->enjambreDrones()[buscarDrone(d)]._enVuelo && this->enjambreDrones()[buscarDrone(d)]._bateria = 100 && posicionActual;
			; // mañana vemos........ --->>> lo use devuelta che
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
		//hacer parcelaValida (esta en rango y no hay construccion y que no hay drone)
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

//Auxiliares

bool Sistema::buscarPosicion(const Secuencia<Posicion> ps, const Posicion p) const{
	Secuencia<Posicion>::size_type i = 0;
	while(i < ps.size() && !(p.x == ps[i].x && p.y == ps[i].y)){
		i++;
	}
	return i != ps.size();
}


int Sistema::buscarDrone(Drone d) {
	int i = 0;
	while ( i < this->_enjambre.size()) {
		if (this->_enjambre[i] == d))
		{
			break;
		}
		i++;
	}
	return i;
}

SACA EL HayDrone SACALOOO

mismoDrone SACALOOO
SACALO JUAAAAN SACALO QUE ME CAGO

/* lo dejamos aca para compararlo con el de Ernesto

bool mismoDrone(Drone d, e) {
	bool m = false;
	//acá comparo una serie de cosas, tipo lista, que nose si puedo comparar así no mas... nose si pasara en otras parte del TP, habria que ver...
	if (d.id == e.id && d.bateria() == e.bateria() && d.enVuelo == e.enVuelo && d.vueloRealizado() == e.vueloRealizado() && d.posicionActual() == e.posicionActual && d.productosDisponibles() == e.productosDisponibles())
	{
		m = true;
	}
	return m;
}
*/

void Sistema::aplicarProductos(Drone d, Posicion p) {
	switch(p)
	case tieneProducto(d, Fertilizante) && (this->estadoDelCultivo(p) == RecienSembrado: this->estadoDelCultivo(p) == ListoParaCosechar && listaProductosSin(Fertilizante, d);
		break;
	case tieneProducto(d, Fertilizante) && this->estadoDelCultivo(p) == EnCrecimiento):	this->estadoDelCultivo(p) == ListoParaCosechar && listaProductosSin(Fertilizante, d);
		break;
	case this->estadoDelCultivo(p) == ConMaleza && tieneProducto(d, Herbicida) && d.bateria() => 5: this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(Herbicida, d)
		break;
	case this->estadoDelCultivo(p) == ConMaleza && tieneProducto(d, HerbicidaLargoAlcance) && d.bateria() => 5:	this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(HerbicidaLargoAlcance, d);
		break;
	case this->estadoDelCultivo(p) == ConPlaga && tieneProducto(d, Plaguicida) && d.bateria() => 10: this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(Plaguicida, d);
		break; 
	case this->estadoDelCultivo(p) == ConPlaga && tieneProducto(d, PlaguicidaBajoConsumo) && d.bateria() => 5:	this->estadoDelCultivo(p) == RecienSembrado && listaProductosSin(PlaguicidaLargoAlcan, d);
		break;
}


void listaProductosSin(Producto p, Drone d) {
	Secuencia<Producto> ps;
	if (tieneProducto(d, p))
	{
		int j = primerLugarCon(d.productosDisponibles(), p);
		int i = 0;
		while (i < d.productosDisponibles().size()) {
			if (i != j)
			{
				ps[i] = d.productosDisponibles()[i]
			}
			i++;
		}
	d._productos = ps;
}