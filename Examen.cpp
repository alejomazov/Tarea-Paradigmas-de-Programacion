#include "Examen.h"
#include <iostream>

Examen::Examen(string _nombre, string _asignatura, int _cantidadPreguntas)
    : nombre(_nombre), asignatura(_asignatura), cantidadPreguntas(_cantidadPreguntas), numPreguntasActual(0) {}

Examen::~Examen() {
    for(int i = 0; i < numPreguntasActual; i++) {
        delete preguntas[i];
    }
}

// Función auxiliar para verificar si ya existe una pregunta con el mismo enunciado
bool Examen::existePregunta(const string &enunciado) const {
    for (int i = 0; i < numPreguntasActual; i++) {
        // Comparamos el enunciado actual con el de cada pregunta ya agregada.
        if (preguntas[i] && preguntas[i]->getEnunciado() == enunciado) {
            return true;
        }
    }
    return false;
}

void Examen::agregarPregunta(Pregunta* pregunta) {
    if (numPreguntasActual >= cantidadPreguntas) {
        cout << "No se puede agregar la pregunta: se ha alcanzado el límite del examen." << endl;
        delete pregunta;
        return;
    }

    // Validamos que no exista una pregunta con el mismo enunciado.
    if (existePregunta(pregunta->getEnunciado())) {
        cout << "La pregunta ya existe (enunciado duplicado). No se agregó." << endl;
        delete pregunta;  // liberamos memoria si no se va a usar
        return;
    }

    // Si no existe duplicado, la agregamos
    preguntas[numPreguntasActual++] = pregunta;
}

void Examen::actualizarPregunta(int id) { // reemplaza la pregunta para que cambie el tipo -- pero aun asi tecnicamente "actualiza" la pregunta ya que se mantiene el id
    for (int i = 0; i < numPreguntasActual; i++) {
        if (preguntas[i]->getId() == id) {
            string enunciado, solucion;
            int nivelBloom, tiempo, puntaje;
            int tipoPregunta;
            
            do {
                cout << "Tipo de Pregunta (1=Verdadero/Falso, 2=Selección Múltiple, 3=Respuesta Corta): ";
                cin >> tipoPregunta;
                cin.ignore();
            } while (tipoPregunta != 1 && tipoPregunta != 2 && tipoPregunta != 3);
            
            // Se determina el nuevo tipo
            string nuevoTipo;
            if (tipoPregunta == 1)
                nuevoTipo = "V";
            else if (tipoPregunta == 2)
                nuevoTipo = "M";
            else if (tipoPregunta == 3)
                nuevoTipo = "R";
            
            cout << "Nivel de Taxonomía de Bloom (0-5): ";
            cin >> nivelBloom;
            cin.ignore();
            
            cout << "Tiempo Estimado (minutos): ";
            cin >> tiempo;
            cin.ignore();
            
            cout << "Enunciado: ";
            getline(cin, enunciado);
            
            cout << "Solución Esperada: ";
            getline(cin, solucion);
            
            cout << "Puntaje: ";
            cin >> puntaje;
            cin.ignore();

            // Elimina la instancia actual
            delete preguntas[i];
            // Crea una nueva instancia con el nuevo tipo y asigna el id original si es necesario
            if (nuevoTipo == "V")
                preguntas[i] = new PreguntaVF(nivelBloom, tiempo, enunciado, solucion, puntaje);
            else if (nuevoTipo == "M")
                preguntas[i] = new PreguntaSM(nivelBloom, tiempo, enunciado, solucion, puntaje);
            else if (nuevoTipo == "R")
                preguntas[i] = new PreguntaRC(nivelBloom, tiempo, enunciado, solucion, puntaje);
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


/* void Examen::actualizarPregunta(int id) { // actualizar pregunta anterior
    for(int i = 0; i < numPreguntasActual; i++) {
        if(preguntas[i]->getId() == id) {
            string tipo, enunciado, solucion;
            int nivelBloom, tiempo, puntaje;
            
            int tipoPregunta;
            do {
                cout << "Tipo de Pregunta (1=Verdadero/Falso, 2=Selección Multiple, 3=Respuesta Corta): ";
                cin >> tipoPregunta;
                cin.ignore();
            } while (tipoPregunta != 1 && tipoPregunta != 2 && tipoPregunta != 3);
            
            if (tipoPregunta == 1)
                tipo = "V"; // Verdadero/Falso
            else if (tipoPregunta == 2)
                tipo = "M"; // Selección Multiple
            else if (tipoPregunta == 3)
                tipo = "R"; // Respuesta Corta
            
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

            //delete preguntas[i];
            //preguntas[i] = new Pregunta(id, tipo, nivelBloom, tiempo, enunciado, solucion, puntaje);
            preguntas[i]->setTipo(tipo);
            preguntas[i]->setNivelBloom(nivelBloom);
            preguntas[i]->setTiempoEstimado(tiempo);
            preguntas[i]->setEnunciado(enunciado);
            preguntas[i]->setSolucion(solucion);
            preguntas[i]->setPuntaje(puntaje);
            return;
        }
    }
    cout << "Pregunta no encontrada." << endl;
} */