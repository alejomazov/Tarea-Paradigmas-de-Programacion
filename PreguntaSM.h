#ifndef PREGUNTA_SM_H
#define PREGUNTA_SM_H
#include "Pregunta.h"

class PreguntaSM : public Pregunta {
public:
    PreguntaSM(int _nivelBloom, int _tiempoEstimado, string _enunciado, string _solucion, int _puntaje)
        : Pregunta(0, "M", _nivelBloom, _tiempoEstimado, _enunciado, _solucion, _puntaje) {}

    void mostrarInfo() const override;
};

#endif
