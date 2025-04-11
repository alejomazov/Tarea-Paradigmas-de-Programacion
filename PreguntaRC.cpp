#include "PreguntaRC.h"
#include <iostream>
using namespace std;

void PreguntaRC::mostrarInfo() const {
    cout << "Tipo: Respuesta Corta" << endl;
    // Se reutiliza la implementación base para mostrar el resto de la información
    Pregunta::mostrarInfo();
}