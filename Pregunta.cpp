
#include "Pregunta.h"
#include <iostream>

int Pregunta::nextId = 1;

Pregunta::Pregunta(int _id, string _tipo, int _nivelBloom, int _tiempoEstimado,
                   string _enunciado, string _solucion, int _puntaje)
    : id(nextId++), tipo(_tipo), nivelBloom(_nivelBloom), tiempoEstimado(_tiempoEstimado),
      enunciado(_enunciado), solucion(_solucion), puntaje(_puntaje) {}

void Pregunta::mostrarInfo() const {
    cout << "ID: " << id << endl;
    cout << "Tipo: " << (tipo == "V" ? "Verdadero o Falso" : "Selección Múltiple") << endl;
    
    string nivelBloomNombre;
    switch(nivelBloom) {
        case 0: nivelBloomNombre = "Recordar"; break;
        case 1: nivelBloomNombre = "Comprender"; break;
        case 2: nivelBloomNombre = "Aplicar"; break;
        case 3: nivelBloomNombre = "Analizar"; break;
        case 4: nivelBloomNombre = "Evaluar"; break;
        case 5: nivelBloomNombre = "Crear"; break;
        default: nivelBloomNombre = "No definido";
    }
    cout << "Nivel Bloom: " << nivelBloom << ". " << nivelBloomNombre << endl;
    
    cout << "Tiempo estimado: " << tiempoEstimado << " minutos" << endl;
    cout << "Enunciado: " << enunciado << endl;
    cout << "Solución: " << solucion << endl;
    cout << "Puntaje: " << puntaje << " puntos" << endl;
}
