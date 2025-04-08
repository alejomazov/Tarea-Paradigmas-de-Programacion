
#include "Examen.h"
#include <iostream>

Examen::Examen(string _nombre, string _asignatura, int _cantidadPreguntas)
    : nombre(_nombre), asignatura(_asignatura), cantidadPreguntas(_cantidadPreguntas), numPreguntasActual(0) {}

Examen::~Examen() {
    for(int i = 0; i < numPreguntasActual; i++) {
        delete preguntas[i];
    }
}

void Examen::agregarPregunta(Pregunta* pregunta) {
    if (numPreguntasActual < cantidadPreguntas) {
        preguntas[numPreguntasActual++] = pregunta;
    } else {
        cout << "No se pueden agregar más preguntas. Límite alcanzado." << endl;
    }
}

void Examen::actualizarPregunta(int id) {
    for(int i = 0; i < numPreguntasActual; i++) {
        if(preguntas[i]->getId() == id) {
            string tipo, enunciado, solucion;
            int nivelBloom, tiempo, puntaje;
            
            int tipoPregunta;
            do {
                cout << "Tipo de Pregunta (1=Verdadero/Falso, 2=Selección Multiple): ";
                cin >> tipoPregunta;
                cin.ignore();
            } while (tipoPregunta != 1 && tipoPregunta != 2);
            
            tipo = (tipoPregunta == 1) ? "V" : "M";
            
            cout << "Nivel de Taxonomía de Bloom (0-5): ";
            cin >> nivelBloom;
            
            cout << "Tiempo Estimado (minutos): ";
            cin >> tiempo;
            cin.ignore();
            
            cout << "Enunciado: ";
            getline(cin, enunciado);
            
            cout << "Solución Esperada: ";
            getline(cin, solucion);
            
            cout << "Puntaje: ";
            cin >> puntaje;

            delete preguntas[i];
            preguntas[i] = new Pregunta(id, tipo, nivelBloom, tiempo, enunciado, solucion, puntaje);
            return;
        }
    }
    cout << "Pregunta no encontrada." << endl;
}

void Examen::borrarPregunta(int id) {
    for(int i = 0; i < numPreguntasActual; i++) {
        if(preguntas[i]->getId() == id) {
            delete preguntas[i];
            for(int j = i; j < numPreguntasActual - 1; j++) {
                preguntas[j] = preguntas[j + 1];
            }
            numPreguntasActual--;
            return;
        }
    }
    cout << "Pregunta no encontrada." << endl;
}

void Examen::consultarPregunta(int id) const {
    cout << "\nPreguntas disponibles:" << endl;
    for(int i = 0; i < numPreguntasActual; i++) {
        cout << "ID " << preguntas[i]->getId() << ": " << preguntas[i]->getEnunciado() << endl;
    }
    
    for(int i = 0; i < numPreguntasActual; i++) {
        if(preguntas[i]->getId() == id) {
            cout << "\nDetalles de la pregunta seleccionada:" << endl;
            preguntas[i]->mostrarInfo();
            return;
        }
    }
    cout << "Pregunta no encontrada." << endl;
}

void Examen::filtrarPreguntas(int nivelBloom) const {
    for(int i = 0; i < numPreguntasActual; i++) {
        if(preguntas[i]->getNivelBloom() == nivelBloom) {
            preguntas[i]->mostrarInfo();
            cout << "------------------------" << endl;
        }
    }
}

void Examen::mostrarPreguntas() const {
    cout << "\n=== PREGUNTAS DEL EXAMEN: " << nombre << " ===" << endl;
    for(int i = 0; i < numPreguntasActual; i++) {
        cout << "\nPregunta " << preguntas[i]->getId() << ":" << endl;
        cout << "Enunciado: " << preguntas[i]->getEnunciado() << endl;
    }
}

void Examen::mostrarExamen() const {
    cout << "\n=== INFORMACIÓN DEL EXAMEN ===" << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Asignatura: " << asignatura << endl;
    cout << "Cantidad de preguntas: " << numPreguntasActual << "/" << cantidadPreguntas << endl;
    
    int tiempoTotal = 0;
    for(int i = 0; i < numPreguntasActual; i++) {
        tiempoTotal += preguntas[i]->getTiempoEstimado();
    }
    cout << "Tiempo total: " << tiempoTotal << " minutos" << endl;
}
