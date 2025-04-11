#ifndef PREGUNTA_VF_H
#define PREGUNTA_VF_H
#include "Pregunta.h"

class PreguntaVF : public Pregunta {
public:
    PreguntaVF(int _nivelBloom, int _tiempoEstimado, string _enunciado, string _solucion, int _puntaje)
        : Pregunta(0, "V", _nivelBloom, _tiempoEstimado, _enunciado, _solucion, _puntaje) {}

    void mostrarInfo() const override;
};

#endif