
#ifndef EXAMEN_H
#define EXAMEN_H
#include "Pregunta.h"
#include <string>

class Examen {
private:
    string nombre;
    string asignatura;
    int cantidadPreguntas;
    Pregunta* preguntas[100];
    int numPreguntasActual;

public:
    Examen(string _nombre = "", string _asignatura = "", int _cantidadPreguntas = 0);
    ~Examen();

    // Getters y Setters
    string getNombre() const { return nombre; }
    string getAsignatura() const { return asignatura; }
    int getCantidadPreguntas() const { return cantidadPreguntas; }

    void setNombre(string _nombre) { nombre = _nombre; }
    void setAsignatura(string _asignatura) { asignatura = _asignatura; }
    void setCantidadPreguntas(int _cantidad) { cantidadPreguntas = _cantidad; }

    // Getter para numPreguntasActual
    int getNumPreguntasActual() const {
        return numPreguntasActual;
    }

    // Getter para acceder a la i-ésima pregunta
    Pregunta *getPregunta(int i) const {
        return preguntas[i];  
    }

    // Métodos
    void agregarPregunta(Pregunta* pregunta);
    void actualizarPregunta(int id);
    void borrarPregunta(int id);
    void consultarPregunta(int id) const;
    void filtrarPreguntas(int nivelBloom) const;
    void mostrarExamen() const;
    void mostrarPreguntas() const;
};

#endif
