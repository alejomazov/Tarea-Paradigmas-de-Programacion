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

using json = nlohmann::json;
using namespace std;

void mostrarMenu() {
    cout << "\n=== MENÚ ===" << endl;
    cout << "1. Crear Examen" << endl;
    cout << "2. Cargar Examen desde archivo" << endl; 
    cout << "3. Añadir Pregunta" << endl;
    cout << "4. Actualizar Pregunta" << endl;
    cout << "5. Borrar Pregunta" << endl;
    cout << "6. Consultar Info Pregunta" << endl;
    cout << "7. Filtrar Pregunta" << endl;
    cout << "8. Mostrar Evaluación" << endl;
    cout << "9. Mostrar Preguntas" << endl;
    cout << "10. Salir" << endl;
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
                jExamenes = json::array();
            }
        } catch (const json::parse_error &e) {
            jExamenes = json::array();
        }
        inFile.close();
    } else {
        // Si el archivo no existe, lo iniciamos como arreglo vacío.
        jExamenes = json::array();
    }
    
    // Serializamos el examen nuevo (actualizado).
    json nuevoExamen = serializeExamen(examen);
    
    // Buscamos si ya existe un examen con el mismo nombre (y asignatura, por ejemplo).
    bool encontrado = false;
    for (size_t i = 0; i < jExamenes.size(); i++) {
        if (jExamenes[i]["nombre"] == examen.getNombre() &&
            jExamenes[i]["asignatura"] == examen.getAsignatura()) {
            // Reemplazamos la entrada existente por la nueva versión.
            jExamenes[i] = nuevoExamen;
            encontrado = true;
            break;
        }
    }
    
    // Si no se encontró, se agrega al final.
    if (!encontrado) {
        jExamenes.push_back(nuevoExamen);
    }
    
    // Abrimos el archivo en modo de escritura para sobrescribir.
    std::ofstream outFile(nombreArchivo);
    if (outFile.is_open()) {
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

Examen* cargarExamenDesdeJSON(const std::string &nombreArchivo = "examenes.json") {
    json jExamenes = leerExamenesDesdeArchivo(nombreArchivo);

    if (jExamenes.empty()) {
        cout << "No hay exámenes guardados." << endl;
        return nullptr;
    }

    cout << "Exámenes disponibles:" << endl;
    for (size_t i = 0; i < jExamenes.size(); ++i) {
        cout << i + 1 << ". " << jExamenes[i]["nombre"] << " - " << jExamenes[i]["asignatura"] << endl;
    }

    int opcion;
    cout << "Seleccione el número del examen que desea cargar: ";
    cin >> opcion;
    cin.ignore();

    if (opcion < 1 || opcion > static_cast<int>(jExamenes.size())) {
        cout << "Opción inválida." << endl;
        return nullptr;
    }

    json examenJSON = jExamenes[opcion - 1];
    string nombre = examenJSON["nombre"];
    string asignatura = examenJSON["asignatura"];
    int cantidadPreguntas = examenJSON["cantidadPreguntas"];

    // Creamos el objeto Examen con los datos básicos.
    Examen* examen = new Examen(nombre, asignatura, cantidadPreguntas);

    // Verificamos si existe la propiedad "preguntas" y es un arreglo.
    if (examenJSON.contains("preguntas") && examenJSON["preguntas"].is_array()) {
        for (const auto &p : examenJSON["preguntas"]) {
            string tipo = p["tipo"];
            int nivel = p["nivelBloom"];
            int tiempo = p["tiempoEstimado"];
            string enunciado = p["enunciado"];
            string solucion = p["solucion"];
            int puntaje = p["puntaje"];

            Pregunta* pregunta = nullptr;
            if (tipo == "V") {
                pregunta = new PreguntaVF(nivel, tiempo, enunciado, solucion, puntaje);
            } else if (tipo == "M") {
                pregunta = new PreguntaSM(nivel, tiempo, enunciado, solucion, puntaje);
            } else if (tipo == "R") {
                pregunta = new PreguntaRC(nivel, tiempo, enunciado, solucion, puntaje);
            }

            if (pregunta) {
                examen->agregarPregunta(pregunta);
            }
        }
    } else {
        cout << "El examen seleccionado no contiene preguntas guardadas." << endl;
    }

    return examen;
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
        while (!(std::cin >> opcion) || (opcion < 1 || opcion > 10)) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ingreso no válido. Ingrese un número entre 1 y 10: ";
        }

        /* if (cin.fail()) {
            cin.clear(); // Limpia el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer
            cout << "Entrada inválida. Por favor, ingrese un número." << endl;
            continue; // Vuelve al menú
        } */
        cin.ignore();

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

            case 2: { // cargar examen
                if (examen) {
                    delete examen;
                    examen = nullptr;
                }
                examen = cargarExamenDesdeJSON();
                break;
            }
            
            case 3: { // añadir pregunta
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
                
                Pregunta *nuevaPregunta = nullptr;
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
                // Actualizamos el archivo JSON con el examen ya modificado (con la nueva pregunta)
                guardarExamenEnArchivo(*examen);
                break;
            }
            case 4: { // actualizar pregunta
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
            case 5: { // borrar pregunta
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
                // Actualizamos el archivo JSON para reflejar la eliminación
                guardarExamenEnArchivo(*examen);
                break;
            }
            case 6: { // consultar informacion pregunta
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
            case 7: { // filtrar evaluacion
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
            case 8: { // mostrar evaluacion
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                examen->mostrarExamen();
                break;
            }
            case 9: { // mostrar pregunta
                if (!examen) {
                    cout << "No hay examen creado." << endl;
                    break;
                }
                examen->mostrarPreguntas();
                break;
            }
            case 10: {
                break;
            }
            default: {
                cout << "Opción inválida." << endl;
                break;
            }
        }
    } while (opcion != 10);
    
    delete examen;
    return 0;
}
