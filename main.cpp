
#include <iostream>
#include <string>
#include "Examen.h"
#include "Pregunta.h"
using namespace std;

void mostrarMenu() {
    cout << "\n=== MENÚ ===" << endl;
    cout << "1. Crear Examen" << endl;
    cout << "2. Añadir Pregunta" << endl;
    cout << "3. Actualizar Pregunta" << endl;
    cout << "4. Borrar Pregunta" << endl;
    cout << "5. Consultar Info Pregunta" << endl;
    cout << "6. Filtrar Pregunta" << endl;
    cout << "7. Mostrar Evaluación" << endl;
    cout << "8. Mostrar Preguntas" << endl;
    cout << "9. Salir" << endl;
    cout << "Elija una opción: ";
    cout << "\n" << endl;
}

int main() {
    Examen* examen = nullptr;
    int opcion;
    string temp;

    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: {
                string nombre, asignatura;
                int cantidadPreguntas;
                
                cout << "Nombre del Examen: ";
                getline(cin, nombre);
                cout << "Asignatura: ";
                getline(cin, asignatura);
                cout << "Cantidad de Preguntas: ";
                cin >> cantidadPreguntas;
                
                examen = new Examen(nombre, asignatura, cantidadPreguntas);
                break;
            }
            case 2: {
                if (!examen) {
                    cout << "Primero cree un examen." << endl;
                    break;
                }
                
                string tipo, enunciado, solucion;
                int id=0, nivelBloom, tiempo, puntaje;
                
                int tipoPregunta;
                do {
                    cout << "Tipo de Pregunta\n\n1=Verdadero/Falso\n2=Selección Multiple): ";
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
                
                Pregunta* nuevaPregunta = new Pregunta(id, tipo, nivelBloom, tiempo, enunciado, solucion, puntaje);
                examen->agregarPregunta(nuevaPregunta);
                break;
            }
            case 3: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                int id;
                cout << "ID de la pregunta a actualizar: ";
                cin >> id;
                examen->actualizarPregunta(id);
                break;
            }
            case 4: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                int id;
                cout << "ID de la pregunta a borrar: ";
                cin >> id;
                examen->borrarPregunta(id);
                break;
            }
            case 5: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                int id;
                cout << "ID de la pregunta a consultar: ";
                cin >> id;
                examen->consultarPregunta(id);
                break;
            }
            case 6: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                int nivel;
                cout << "Nivel de Bloom a filtrar (0-5): ";
                cin >> nivel;
                examen->filtrarPreguntas(nivel);
                break;
            }
            case 7: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                examen->mostrarExamen();
                break;
            }
            case 8: {
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                examen->mostrarPreguntas();
                break;
            }
        }
    }
    while (opcion != 9);
    delete examen;
    return 0;
}
