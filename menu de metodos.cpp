
	int tipoDeDato = 0;
	cout << "qué queres toquetear?" << endl;
	cin >> tipoDeDato;
/*	if (tipoDeDato == 1)
	{
		//con Campo
		cout << "qué queres hacer?" << endl;
		string A;
		cin >> A;
		cout << campo.A();
	} */
	if (tipoDeDato == 2)
	{
		//con Drone
		cout << "Ingresa lo que queres hacer YA: " << endl;
		int metodo = 0;
		cin >> metodo;
		switch(metodo) {
			case metodo == 1:	cout << d.id() << endl;
			break;
			case metodo == 2:	cout << d.bateria() << endl;
			break;
			case metodo == 3:	cout << d.enVuelo() << endl;
			break;
			case metodo == 4:	cout << d.vueloRealizado() << endl;
			break;
			case metodo == 5:	cout << d.posicionActual() << endl;
			break;
			case metodo == 6:	cout << d.productosDisponibles() << endl;
			break;
			case metodo == 7:	cout << d.vueloEscalerado() << endl;
			break;
			case metodo == 8:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 9:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 10:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 11:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 12:	cout << d.vuelosCruzados() << endl;
			break;
			case metodo == 13:	cout << d.vuelosCruzados() << endl;
			break;
		}
	}
	if (tipoDeDato == 3)
	{

	}


