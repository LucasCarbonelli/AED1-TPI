#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED

#include "tipos.h"
#include "campo.h"
#include "drone.h"

class Sistema {

public:
    Sistema();

    Sistema(const Campo &c, const Secuencia<Drone> &ds);

    const Campo &campo() const;

    EstadoCultivo estadoDelCultivo(const Posicion &p) const;

    const Secuencia<Drone> &enjambreDrones() const;

    void crecer();

    void seVinoLaMaleza(const Secuencia<Posicion> &ps);

    void seExpandePlaga();

    void despegar(const Drone &d);

    bool listoParaCosechar() const;

    void aterrizarYCargarBaterias(Carga b);

    void fertilizarPorFilas();

    void volarYSensar(const Drone &d);

    void mostrar(std::ostream &os) const;

    void guardar(std::ostream &os) const;

    void cargar(std::istream &is);

    bool operator==(const Sistema &otroSistema) const;

private:
    Campo _campo;
    Secuencia<Drone> _enjambre;
    Grilla<EstadoCultivo> _estado;

    //Para correr los tests (NO TOCAR)
    friend class test_sistema_lo_basico_crecer_funciona_ok_Test;
    friend class test_sistema_se_vino_la_maleza_lista_vacia_no_modifica_nada_Test;
    friend class test_sistema_se_vino_la_maleza_se_pone_con_maleza_las_posiciones_de_la_lista_Test;
    friend class test_sistema_se_expande_plaga_si_no_hay_plaga_no_cambia_Test;
    friend class test_sistema_se_expande_plaga_si_hay_plaga_se_expande_Test;
    friend class test_sistema_listo_para_cosechar_si_es_menos_del_90_por_ciento_retorna_false_Test;
    friend class test_sistema_listo_para_cosechar_si_es_exactamente_el_90_por_ciento_retorna_true_Test;
    friend class test_sistema_listo_para_cosechar_si_es_mas_del_90_por_ciento_retorna_true_Test;
    friend class test_sistema_volar_y_sensar_todo_libre_Test;

    // --> Defina sus funciones auxiliares debajo...

    bool noHayConstruccion(Posicion p) const;
    Posicion dondeEstaElGranero() const;
    Secuencia<Posicion> lugaresAdyacentes(Posicion p) const;
    bool enRango(const Posicion &p) const;
    bool noHayDrone(Posicion p) const;
    Secuencia<Drone>::size_type buscarDrone(Drone d) const;

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
    //bool posicionLibre(Posicion p) const;
    //Posicion damePosicionLibre(Secuencia<Posicion> ps);
    bool buscarPosicion(const Secuencia<Posicion> ps, const Posicion p) const;
    Posicion parcelaValida(Secuencia<Posicion> ps);
    void sensarParcela(Posicion p);
    void aplicarProductos(Drone d, Posicion p);
    bool tieneProducto(Drone d, Producto p);
    void listaProductosSin(Producto s, Producto p, Drone d);
    int primerLugarCon(Secuencia<Producto> ps, Producto p);
    EstadoCultivo stringAEstadoCultivo(const std::string s) const;
    bool igualEstadoDelCultivo(const Sistema& otroSistema) const;
    bool igualEnjambreDrones(const Sistema& otroSistema) const;
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream &operator<<(std::ostream &os, const Sistema &s);

#endif // SISTEMA_H_INCLUDED
