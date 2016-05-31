#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED

#include "tipos.h"
#include "campo.h"
#include "drone.h"

class Sistema {

  public:
		Sistema();
		Sistema(const Campo &c, const Secuencia<Drone> &ds);

		const Campo& campo() const;
		EstadoCultivo estadoDelCultivo(const Posicion& p) const;
		const Secuencia<Drone>&	enjambreDrones() const;

		void crecer();
		void seVinoLaMaleza(const Secuencia<Posicion>& ps);
		void seExpandePlaga();
		void despegar(const Drone& d);
		bool listoParaCosechar() const;
		void aterrizarYCargarBaterias(Carga b);
		void fertilizarPorFilas();
		void volarYSensar(const Drone& d);

		void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar (std::istream& is);

		bool operator==(const Sistema& otroSistema) const;

	private:
        Campo _campo;
		Secuencia<Drone> _enjambre;
        Grilla<EstadoCultivo> _estado;

        // Auxiliares
        Secuencia<Posicion> parcelasDeCultivo() const;
        int cantCultivosCosechables() const;
        int dronesVolandoEnFila ( int f);
        int recorridoMaximo(Drone d);
        template <class T> T minimo (const T a, const T b) const;
        template <class T> int cuenta(const Secuencia <T> ls, const T e) const;
        int fertAplicable(Drone d);
        int cantFertilizables(const int i , Drone d);
        int parcelasLibres(const Drone d );
        Secuencia<Producto> mismosProductosDescontandoFertilizante(const Drone d);
        bool HayDrone(Posicion P);
        bool NoHayConstruccion(Posicion p);
        bool posicionLibre(Posicion p);
        Secuencia<Posicion> lugaresAdyacentes(Posicion p);
        Posicion damePosicionLibre(Secuencia<Posicion> ps);
        bool buscarPosicion(const Secuencia<Posicion> ps, const Posicion p) const;
        Posicion DondeEstaElGranero(Campo c);
        int buscarDrone(Drone d) ;
        Posicion parcelaValida(Secuencia<Posicion> ps);
        void sensarParcela(Posicion p);
        void aplicarProductos(Drone d, Posicion p);
        bool tieneProducto(Drone d, Producto p);
        void listaProductosSin(Producto s, Producto p, Drone d);
        int primerLugarCon(Secuencia<Producto> ps, Producto p);
        EstadoCultivo stringAEstadoCultivo(const std::string s) const;
        bool igualEstadoDelCultivo(const Sistema& otroSistema) const;
        bool igualEnjambreDrones(const Sistema& otroSistema) const;







       /* bool enRango(int x, int y) const;
        bool enRango(const Posicion p) const;
        bool enRangoConPlaga(int x, int y) const;
        Posicion posicionGranero() const;
        bool enRangoCultivable(int x, int y) const;
        bool enRangoCultivableLibre(int x, int y) const;
        bool parcelaLibre(int x, int y) const;
        static Posicion vecinoAlOeste(const Posicion &p);

        static bool tieneUnProducto(const Secuencia<Producto> &ps, const Producto &productoABuscar);
        // solución --> deberíamos agregarla como funciones globales en tipos.h
        void split(const std::string &s, char delim, std::vector<std::string> &elems);
        std::vector<std::string> split(const std::string &s, char delim);*/
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Sistema & s);

#endif // SISTEMA_H_INCLUDED
