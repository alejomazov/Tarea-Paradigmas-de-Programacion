#include <iostream>
#include <string>
#include <limits>
#include "Examen.h"
#include "Pregunta.h"
#include "PreguntaVF.h"
#include "PreguntaSM.h"
#include "PreguntaRC.h"

#include <fstream>
#include "libs/json/json.hpp"
/* #include "Examen.cpp"
#include "Pregunta.cpp"
#include "PreguntaVF.cpp"
#include "PreguntaSM.cpp"
#include "PreguntaRC.cpp" */

using json = nlohmann::json;
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

// funciones extra para tomar y crear un archivo json
json serializePregunta(const Pregunta* pregunta) {
    json j;
    j["id"] = pregunta->getId();
    j["tipo"] = pregunta->getTipo();
    j["nivelBloom"] = pregunta->getNivelBloom();
    j["tiempoEstimado"] = pregunta->getTiempoEstimado();
    j["enunciado"] = pregunta->getEnunciado();
    j["solucion"] = pregunta->getSolucion();
    j["puntaje"] = pregunta->getPuntaje();
    return j;
}


json serializeExamen(const Examen &examen) {
    json j;
    j["nombre"] = examen.getNombre();
    j["asignatura"] = examen.getAsignatura();
    j["cantidadPreguntas"] = examen.getCantidadPreguntas();
    j["numPreguntasActual"] = examen.getNumPreguntasActual();

    // Creamos un arreglo JSON para las preguntas
    json jPreguntas = json::array();
    for (int i = 0; i < examen.getNumPreguntasActual(); i++) {
        // Obtenemos cada pregunta usando el getter que agregamos
        Pregunta* p = examen.getPregunta(i);
        jPreguntas.push_back( serializePregunta(p) );
    }
    j["preguntas"] = jPreguntas;
    
    return j;
}


void guardarExamenEnArchivo(const Examen &examen, const std::string &nombreArchivo = "examenes.json") {
    json jExamenes; 
    std::ifstream inFile(nombreArchivo);
    
    // Si el archivo existe, leemos su contenido.
    if (inFile.is_open()) {
        try {
            inFile >> jExamenes;  // Se espera que jExamenes sea un arreglo
            if (!jExamenes.is_array()) {
                // Si no lo es, lo reiniciamos como un arreglo vacío
                jExamenes = json::array();
            }
        } catch (const json::parse_error &e) {
            // Si ocurre un error al parsear (archivo corrupto, por ejemplo), inicializamos un arreglo vacío.
            jExamenes = json::array();
        }
        inFile.close();
    } else {
        // Si el archivo no existe, lo iniciamos como arreglo vacío.
        jExamenes = json::array();
    }
    
    // Serializamos el examen nuevo.
    json nuevoExamen = serializeExamen(examen);
    // Agregamos el examen nuevo al arreglo que ya se leyó (o que se creó vacío).
    jExamenes.push_back(nuevoExamen);
    
    // Abrimos el archivo en modo escritura (esto sobrescribe lo anterior)
    std::ofstream outFile(nombreArchivo);
    if (outFile.is_open()) {
        // Guardamos el JSON con indentación para facilitar la lectura.
        outFile << jExamenes.dump(4);
        outFile.close();
        cout << "Examen guardado correctamente en " << nombreArchivo << endl;
    } else {
        cerr << "Error al abrir el archivo para escribir." << endl;
    }
}


json leerExamenesDesdeArchivo(const std::string &nombreArchivo = "examenes.json") {
    json jExamenes;
    std::ifstream inFile(nombreArchivo);
    if (inFile.is_open()) {
        try {
            inFile >> jExamenes;
            if (!jExamenes.is_array()) {
                jExamenes = json::array();
            }
        } catch (const json::parse_error &e) {
            jExamenes = json::array();
        }
        inFile.close();
    } else {
        // Si no existe el archivo, retornamos un arreglo vacío.
        jExamenes = json::array();
    }
    return jExamenes;
}

void mostrarExamenesGuardados(const std::string &nombreArchivo = "examenes.json") {
    json jExamenes = leerExamenesDesdeArchivo(nombreArchivo);
    cout << "Exámenes guardados en " << nombreArchivo << ":\n" << jExamenes.dump(4) << endl;
}


int main() {
    Examen* examen = nullptr;
    int opcion;
    string temp;

    do {
        mostrarMenu();
        // cin >> opcion;
        while (!(std::cin >> opcion) || (opcion < 1 || opcion > 9)) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ingreso no válido. Ingrese un número entre 1 y 9: ";
        }

        /* if (cin.fail()) {
            cin.clear(); // Limpia el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer
            cout << "Entrada inválida. Por favor, ingrese un número." << endl;
            continue; // Vuelve al menú
        } */
        //cin.ignore();

        switch(opcion) {
            case 1: { // crear examen
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
                if (examen != nullptr) {
                    guardarExamenEnArchivo(*examen);
                }
                break;
            }
            case 2: { // añadir pregunta
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
            case 3: { // actualizar pregunta
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                int id;
                cout << "ID de la pregunta a actualizar: ";
                cin >> id;
                cin.ignore();
                examen->actualizarPregunta(id);
                if (examen != nullptr) {
                    guardarExamenEnArchivo(*examen);
                }
                break;
            }
            case 4: { // borrar pregunta
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                char confirmacion;
                std::cout << "¿Está seguro de que desea borrar el ítem? (S/N): ";
                std::cin >> confirmacion;
                if (toupper(confirmacion) != 'S') {
                    std::cout << "Operación cancelada.\n";
                    break;
                }

                int id;
                cout << "ID de la pregunta a borrar: ";
                cin >> id;
                cin.ignore();
                examen->borrarPregunta(id);
                break;
            }
            case 5: { // consultar informacion pregunta
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
            case 6: { // filtrar evaluacion
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
            case 7: { // mostrar evaluacion
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                examen->mostrarExamen();
                break;
            }
            case 8: { // mostrar pregunta
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
