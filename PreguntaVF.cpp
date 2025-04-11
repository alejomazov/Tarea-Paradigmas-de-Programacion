#include "PreguntaVF.h"
#include <iostream>
using namespace std;

void PreguntaVF::mostrarInfo() const {
    cout << "Tipo: Verdadero o Falso" << endl;
    Pregunta::mostrarInfo(); // Reutiliza el código base
}
