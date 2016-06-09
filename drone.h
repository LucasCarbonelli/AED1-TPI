#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include "tipos.h"
#include <vector>

class Drone{
    public:
        Drone();
        Drone(ID i, const std::vector<Producto> &ps);

        ID id() const;
        Carga bateria() const;
        bool enVuelo() const;
        const Secuencia<Posicion>& vueloRealizado() const;
        Posicion posicionActual() const;
        const Secuencia<Producto>& productosDisponibles() const;

        bool vueloEscalerado() const;
        static Secuencia<InfoVueloCruzado>  vuelosCruzados(const Secuencia<Drone>& ds);

        void mostrar(std::ostream& os) const;
        void guardar(std::ostream& os) const;
        void cargar(std::istream& is);

        bool operator==(const Drone& otroDrone) const;

        void moverA(const Posicion pos);
        void setBateria(const Carga c);
        void borrarVueloRealizado();
        void cambiarPosicionActual(const Posicion p);
        void sacarProducto(const Producto p);

    private:
        ID _id;
        Carga _bateria;
        Secuencia<Posicion> _trayectoria;
        Secuencia<Producto> _productos;
        bool _enVuelo;
        Posicion _posicionActual;

        // Auxiliares

        bool escalerado() const;
        bool esEscalerado(int i) const;
        static int cantidadDronesCruzados(const Posicion p , const Secuencia<Drone> ds);
        bool seCruzoConOtro(Secuencia<Drone> ds, int i) const;
        static bool buscarInfoVuelosCruzados(const Secuencia<InfoVueloCruzado> ls, const Posicion p);
        void cargarTrayectoria(std::istream & is);
        void cargarProductos(std::istream & is);
        void cargarVuelo(std::istream & is);
        void cargarPosicionActual(std::istream & is);
        bool esPosicion(const std::string s) const;
        Posicion extraerPosicion(const std::string s) const;
        Producto stringAProducto(const std::string s) const;
        static bool mismaTrayectoria(const Secuencia<Posicion> l1, const  Secuencia<Posicion> l2);
        template <class T> bool mismos(const Secuencia<T> l1, const Secuencia<T> l2) const;
        template <class T> int cuenta(const Secuencia <T> l1, const T e) const;
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Drone & d);

#endif // DRONE_H_INCLUDED
