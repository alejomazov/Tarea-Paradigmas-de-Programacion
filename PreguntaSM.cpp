#include "PreguntaSM.h"
#include <iostream>
using namespace std;

void PreguntaSM::mostrarInfo() const {
    cout << "Tipo: Selección Múltiple" << endl;
    Pregunta::mostrarInfo();
}