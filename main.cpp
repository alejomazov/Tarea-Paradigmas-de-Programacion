#include <iostream>
#include <string>
#include "Examen.h"
#include "Pregunta.h"
#include "PreguntaVF.h"
#include "PreguntaSM.h"
#include "PreguntaRC.h" // Nueva clase para "Respuesta Corta"

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
                cin.ignore();
                
                // Liberar el examen anterior si existe para evitar fugas de memoria
                if (examen) {
                    delete examen;
                }
                examen = new Examen(nombre, asignatura, cantidadPreguntas);
                break;
            }
            case 2: {
                if (!examen) {
                    cout << "Primero cree un examen." << endl;
                    break;
                }
                
                string enunciado, solucion;
                int nivelBloom, tiempo, puntaje;
                int tipoPregunta;
                
                do {
                    cout << "Tipo de Pregunta:" << endl;
                    cout << "1 = Verdadero/Falso" << endl;
                    cout << "2 = Selección Múltiple" << endl;
                    cout << "3 = Respuesta Corta" << endl;
                    cout << "Elija una opción: ";
                    cin >> tipoPregunta;
                    cin.ignore();
                } while (tipoPregunta < 1 || tipoPregunta > 3);
                
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
                
                Pregunta* nuevaPregunta = nullptr;
                if (tipoPregunta == 1) {
                    nuevaPregunta = new PreguntaVF(nivelBloom, tiempo, enunciado, solucion, puntaje);
                }
                else if (tipoPregunta == 2) {
                    nuevaPregunta = new PreguntaSM(nivelBloom, tiempo, enunciado, solucion, puntaje);
                }
                else if (tipoPregunta == 3) {
                    nuevaPregunta = new PreguntaRC(nivelBloom, tiempo, enunciado, solucion, puntaje);
                }
                
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
                cin.ignore();
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
                cin.ignore();
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
                cin.ignore();
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
                cin.ignore();
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
            case 9: {
                break;
            }
            default: {
                cout << "Opción inválida." << endl;
                break;
            }
        }
    } while (opcion != 9);
    
    delete examen;
    return 0;
}
