#ifndef PREGUNTA_RC_H
#define PREGUNTA_RC_H

#include "Pregunta.h"
#include <string>
using namespace std;

class PreguntaRC : public Pregunta {
public:
    // Constructor para Pregunta de Respuesta Corta; se asigna el tipo "RC"
    PreguntaRC(int _nivelBloom, int _tiempoEstimado, string _enunciado, string _solucion, int _puntaje)
        : Pregunta(0, "RC", _nivelBloom, _tiempoEstimado, _enunciado, _solucion, _puntaje) {}

    // Método polimórfico para mostrar información
    void mostrarInfo() const override;
};

#endif