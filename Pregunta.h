
#ifndef PREGUNTA_H
#define PREGUNTA_H
#include <string>
using namespace std;

class Pregunta {
protected:
    static int nextId;
    int id;
    string tipo;
    int nivelBloom;
    int tiempoEstimado;
    string enunciado;
    string solucion;
    int puntaje;

public:
    Pregunta(int _id = 0, string _tipo = "", int _nivelBloom = 0, int _tiempoEstimado = 0,
            string _enunciado = "", string _solucion = "", int _puntaje = 0);
    virtual ~Pregunta() = default;

    // Getters
    int getId() const { return id; }
    string getTipo() const { return tipo; }
    int getNivelBloom() const { return nivelBloom; }
    int getTiempoEstimado() const { return tiempoEstimado; }
    string getEnunciado() const { return enunciado; }
    string getSolucion() const { return solucion; }
    int getPuntaje() const { return puntaje; }

    // Setters
    void setId(int _id) { id = _id; }
    void setTipo(string _tipo) { tipo = _tipo; }
    void setNivelBloom(int _nivel) { nivelBloom = _nivel; }
    void setTiempoEstimado(int _tiempo) { tiempoEstimado = _tiempo; }
    void setEnunciado(string _enunciado) { enunciado = _enunciado; }
    void setSolucion(string _solucion) { solucion = _solucion; }
    void setPuntaje(int _puntaje) { puntaje = _puntaje; }

    virtual void mostrarInfo() const;
};

#endif
