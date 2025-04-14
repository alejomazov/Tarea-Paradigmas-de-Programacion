#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <cctype>
#include <dirent.h>    // Para listar archivos en el directorio
#include "Examen.h"
#include "Pregunta.h"
#include "PreguntaVF.h"
#include "PreguntaSM.h"
#include "PreguntaRC.h"

using namespace std;

// Constantes para el manejo de múltiples exámenes y archivos
const int MAX_EXAMENES = 100;
const int MAX_ARCHIVOS = 100;

// --- Funciones ya existentes ---

// Función para mostrar el menú principal.
void mostrarMenu() {
    cout << "\n=== MENÚ ===" << endl;
    cout << "1. Crear Examen" << endl;
    cout << "2. Cargar Examen desde archivo TXT" << endl;
    cout << "3. Añadir Pregunta" << endl;
    cout << "4. Actualizar Pregunta" << endl;
    cout << "5. Borrar Pregunta" << endl;
    cout << "6. Consultar Info Pregunta" << endl;
    cout << "7. Filtrar Pregunta" << endl;
    cout << "8. Mostrar Evaluación" << endl;
    cout << "9. Mostrar Preguntas" << endl;
    cout << "10. Guardar Examen a archivo TXT" << endl;
    cout << "11. Salir" << endl;
    cout << "Elija una opción: ";
}

// Función trim: elimina espacios y saltos de línea de ambos extremos de una cadena.
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Función extraerNumeros: extrae sólo los caracteres numéricos de una cadena.
string extraerNumeros(const string &str) {
    string result;
    for (char c : str) {
        if (isdigit(static_cast<unsigned char>(c)))
            result.push_back(c);
    }
    return result;
}

// Función para guardar el examen en un archivo TXT.
// Se utiliza un formato de bloques de 7 líneas.
void guardarExamenEnArchivoTXT(const Examen &examen, const string &nombreArchivo) {
    ofstream outFile(nombreArchivo);
    if (!outFile.is_open()) {
        cout << "Error al abrir el archivo TXT para escribir." << endl;
        return;
    }
    outFile << "Preguntas" << endl << endl;
    for (int i = 0; i < examen.getNumPreguntasActual(); i++) {
        Pregunta* p = examen.getPregunta(i);
        outFile << "\"enunciado\": \"" << p->getEnunciado() << "\"," << endl;
        outFile << "\"id\":" << p->getId() << "," << endl;
        outFile << "\"nivelBloom\":" << p->getNivelBloom() << "," << endl;
        outFile << "\"puntaje\":" << p->getPuntaje() << "," << endl;
        outFile << "\"solucion\": \"" << p->getSolucion() << "\"," << endl;
        outFile << "\"tiempoEstimado\":" << p->getTiempoEstimado() << "," << endl;
        outFile << "\"tipo\": \"" << p->getTipo() << "\"" << endl << endl;
    }
    outFile.close();
    cout << "Examen guardado correctamente en " << nombreArchivo << endl;
}

// Función para cargar un examen desde un archivo TXT con el formato especificado.
Examen* cargarExamenDesdeTXT(const string &nombreArchivo) {
    ifstream file(nombreArchivo);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo TXT: " << nombreArchivo << endl;
        return nullptr;
    }

    // Se crea el examen con datos por defecto.
    Examen* examen = new Examen("Examen TXT", "Asignatura TXT", 100);
    string line;

    // Se salta el encabezado hasta encontrar "Preguntas"
    while (getline(file, line)) {
        if (line.find("Preguntas") != string::npos)
            break;
    }

    // Se lee cada bloque de 7 líneas para cada pregunta.
    while (getline(file, line)) {
        if (trim(line).empty())
            continue;

        string enunciado, solucion, tipo;
        int id, nivelBloom = 0, puntaje = 0, tiempoEstimado = 0;

        // 1. Línea: enunciado
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                enunciado = trim(line.substr(pos + 1));
                if (!enunciado.empty() && enunciado.front() == '\"')
                    enunciado.erase(0, 1);
                if (!enunciado.empty() && enunciado.back() == ',')
                    enunciado.pop_back();
                enunciado = trim(enunciado);
                if (!enunciado.empty() && enunciado.back() == '\"')
                    enunciado.pop_back();
            }
        }

        // 2. Línea: id
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string valor = trim(line.substr(pos + 1));
                string digitos = extraerNumeros(valor);
                if (!digitos.empty())
                    id = stoi(digitos);
                else {
                    cout << "Error al convertir el campo 'id', valor no numérico: " << valor << endl;
                    id = 0;
                }
            }
        }

        // 3. Línea: nivelBloom
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string valor = trim(line.substr(pos + 1));
                string digitos = extraerNumeros(valor);
                if (!digitos.empty())
                    nivelBloom = stoi(digitos);
                else {
                    cout << "Error al convertir 'nivelBloom', valor no numérico: " << valor << endl;
                    nivelBloom = 0;
                }
            }
        }

        // 4. Línea: puntaje
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string valor = trim(line.substr(pos + 1));
                string digitos = extraerNumeros(valor);
                if (!digitos.empty())
                    puntaje = stoi(digitos);
                else {
                    cout << "Error al convertir 'puntaje', valor no numérico: " << valor << endl;
                    puntaje = 0;
                }
            }
        }

        // 5. Línea: solucion
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                solucion = trim(line.substr(pos + 1));
                if (!solucion.empty() && solucion.front() == '\"')
                    solucion.erase(0, 1);
                if (!solucion.empty() && solucion.back() == ',')
                    solucion.pop_back();
                solucion = trim(solucion);
                if (!solucion.empty() && solucion.back() == '\"')
                    solucion.pop_back();
            }
        }

        // 6. Línea: tiempoEstimado
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string valor = trim(line.substr(pos + 1));
                string digitos = extraerNumeros(valor);
                if (!digitos.empty())
                    tiempoEstimado = stoi(digitos);
                else {
                    cout << "Error al convertir 'tiempoEstimado', valor no numérico: " << valor << endl;
                    tiempoEstimado = 0;
                }
            }
        }

        // 7. Línea: tipo
        if (!getline(file, line)) break;
        {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                tipo = trim(line.substr(pos + 1));
                if (!tipo.empty() && tipo.front() == '\"')
                    tipo.erase(0, 1);
                if (!tipo.empty() && tipo.back() == '\"')
                    tipo.pop_back();
            }
        }

        // Crear la pregunta según su tipo.
        Pregunta* pregunta = nullptr;
        if (tipo == "V")
            pregunta = new PreguntaVF(nivelBloom, tiempoEstimado, enunciado, solucion, puntaje);
        else if (tipo == "M")
            pregunta = new PreguntaSM(nivelBloom, tiempoEstimado, enunciado, solucion, puntaje);
        else if (tipo == "R")
            pregunta = new PreguntaRC(nivelBloom, tiempoEstimado, enunciado, solucion, puntaje);

        if (pregunta) {
            pregunta->setId(id);                      
            examen->agregarPregunta(pregunta);
            cout << "\nCargada pregunta ID " << id << ": " << enunciado << endl;
        }
    } 

    file.close();
    return examen; 
}

// --- Funciones adicionales para manejar múltiples exámenes sin vector ---

// Función para "formatear" el nombre del archivo a partir del nombre del examen.
// Reemplaza espacios por guiones bajos y le antepone "Examen_" y finaliza con ".txt"
string formatearNombreArchivo(const string &nombreExamen) {
    string nombreFormateado = "Examen_";
    for (char c : nombreExamen) {
        if (c == ' ')
            nombreFormateado.push_back('_');
        else
            nombreFormateado.push_back(c);
    }
    nombreFormateado += ".txt";
    return nombreFormateado;
}

// Función para que el usuario seleccione un examen entre los que están cargados.
int seleccionarExamen(Examen* examenes[], int numExamenes) {
    if(numExamenes == 0) {
         cout << "No hay exámenes disponibles." << endl;
         return -1;
    }
    cout << "\nLista de exámenes disponibles:" << endl;
    for (int i = 0; i < numExamenes; i++) {
         // Se asume que la clase Examen tiene el método getNombre()
         cout << i << ". " << examenes[i]->getNombre() << endl;
    }
    int indice;
    cout << "Ingrese el índice del examen a seleccionar: ";
    cin >> indice;
    cin.ignore();
    if (indice < 0 || indice >= numExamenes) {
         cout << "Índice no válido." << endl;
         return -1;
    }
    return indice;
}

// Función para listar archivos de examen en el directorio actual.
// Se listarán aquellos archivos que inicien con "Examen_" y finalicen con ".txt".
int listarArchivosExamenes(string archivos[]) {
    int count = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL) {
         while ((ent = readdir(dir)) != NULL && count < MAX_ARCHIVOS) {
             string filename = ent->d_name;
             if (filename.size() >= 11 && 
                 filename.substr(0, 7) == "Examen_" && 
                 filename.substr(filename.size()-4) == ".txt") {
                 archivos[count] = filename;
                 count++;
             }
         }
         closedir (dir);
    } else {
         cout << "No se pudo abrir el directorio actual." << endl;
    }
    return count;
}

// --- Función para mostrar el contenido de un archivo (opcional, para depuración) ---
void mostrarExamenesGuardadosTXT(const string &nombreArchivo = "Preguntas.txt") {
    ifstream inFile(nombreArchivo);
    if (!inFile.is_open()) {
            cout << "No se puede abrir el archivo: " << nombreArchivo << endl;
            return;
        }
    string line;
    while(getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

// --- Función main ---
int main() {
    // Arreglo para almacenar punteros a exámenes
    Examen* examenes[MAX_EXAMENES] = { nullptr };
    int numExamenes = 0;

    int opcion;
    do {
        mostrarMenu();
        while (!(cin >> opcion) || (opcion < 1 || opcion > 11)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ingreso no válido. Ingrese un número entre 1 y 11: ";
        }
        cin.ignore();

        switch(opcion) {
            case 1: { // Crear examen
                if (numExamenes >= MAX_EXAMENES) {
                    cout << "Ha alcanzado el máximo número de exámenes." << endl;
                    break;
                }
                string nombre, asignatura;
                int cantidadPreguntas;
                cout << "Nombre del Examen: ";
                getline(cin, nombre);
                cout << "Asignatura: ";
                getline(cin, asignatura);
                cout << "Cantidad de Preguntas (máximo): ";
                cin >> cantidadPreguntas;
                cin.ignore();

                Examen* nuevoExamen = new Examen(nombre, asignatura, cantidadPreguntas);
                examenes[numExamenes++] = nuevoExamen;
                
                // Generamos el nombre del archivo a partir del nombre del examen.
                string nombreArchivo = formatearNombreArchivo(nombre);
                guardarExamenEnArchivoTXT(*nuevoExamen, nombreArchivo);

                cout << "Examen '" << nombre << "' creado y guardado en '" 
                     << nombreArchivo << "'." << endl;
                break;
            }
            case 2: { // Cargar examen desde archivo TXT
                string listaArchivos[MAX_ARCHIVOS];
                int numArchivos = listarArchivosExamenes(listaArchivos);
                if (numArchivos == 0) {
                    cout << "No se encontraron archivos de exámenes." << endl;
                    break;
                }
                cout << "\nArchivos de exámenes disponibles:" << endl;
                for (int i = 0; i < numArchivos; i++) {
                    cout << i << ". " << listaArchivos[i] << endl;
                }
                int indiceArchivo;
                cout << "Ingrese el número del archivo a cargar: ";
                cin >> indiceArchivo;
                cin.ignore();
                if (indiceArchivo < 0 || indiceArchivo >= numArchivos) {
                    cout << "Índice de archivo no válido." << endl;
                    break;
                }
                Examen* examenCargado = cargarExamenDesdeTXT(listaArchivos[indiceArchivo]);
                if (examenCargado) {
                    examenes[numExamenes++] = examenCargado;
                    cout << "\nExamen cargado desde '" << listaArchivos[indiceArchivo] << "'." << endl;
                }
                break;
            }
            case 3: { // Añadir pregunta
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                Examen* examenSeleccionado = examenes[idx];

                int tipoPregunta;
                do {
                    cout << "\nTipo de Pregunta:" << endl;
                    cout << "1 = Verdadero/Falso" << endl;
                    cout << "2 = Selección Múltiple" << endl;
                    cout << "3 = Respuesta Corta" << endl;
                    cout << "Elija una opción: ";
                    cin >> tipoPregunta;
                    cin.ignore();
                } while (tipoPregunta < 1 || tipoPregunta > 3);

                int nivelBloom, tiempo, puntaje;
                string enunciado, solucion;

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
                if (tipoPregunta == 1)
                    nuevaPregunta = new PreguntaVF(nivelBloom, tiempo, enunciado, solucion, puntaje);
                else if (tipoPregunta == 2)
                    nuevaPregunta = new PreguntaSM(nivelBloom, tiempo, enunciado, solucion, puntaje);
                else if (tipoPregunta == 3)
                    nuevaPregunta = new PreguntaRC(nivelBloom, tiempo, enunciado, solucion, puntaje);

                examenSeleccionado->agregarPregunta(nuevaPregunta);

                // Guarda el examen modificado con su nombre formateado.
                string nombreArchivo = formatearNombreArchivo(examenSeleccionado->getNombre());
                guardarExamenEnArchivoTXT(*examenSeleccionado, nombreArchivo);
                break;
            }
            case 4: { // Actualizar pregunta
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                Examen* examenSeleccionado = examenes[idx];

                cout << "\nEstas son las preguntas disponibles:" << endl;
                examenSeleccionado->mostrarPreguntas();
                int id;
                cout << "\nIngrese el ID de la pregunta a actualizar: ";
                cin >> id;
                cin.ignore();

                examenSeleccionado->actualizarPregunta(id);
                string nombreArchivo = formatearNombreArchivo(examenSeleccionado->getNombre());
                guardarExamenEnArchivoTXT(*examenSeleccionado, nombreArchivo);
                break;
            }
            case 5: { // Borrar pregunta
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                Examen* examenSeleccionado = examenes[idx];

                cout << "\nEstas son las preguntas disponibles:" << endl;
                examenSeleccionado->mostrarPreguntas();
                int id;
                char confirmacion;
                cout << "\nID de la pregunta a borrar: ";
                cin >> id;
                cout << "\n¿Está seguro de que desea borrar el ítem (" << id << ")? (S/N): ";
                cin >> confirmacion;
                if (toupper(confirmacion) != 'S') {
                    cout << "Operación cancelada." << endl;
                    break;
                }
                cin.ignore();
                examenSeleccionado->borrarPregunta(id);
                string nombreArchivo = formatearNombreArchivo(examenSeleccionado->getNombre());
                guardarExamenEnArchivoTXT(*examenSeleccionado, nombreArchivo);
                break;
            }
            case 6: { // Consultar información de una pregunta
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                Examen* examenSeleccionado = examenes[idx];
                int id;
                cout << "ID de la pregunta a consultar: ";
                cin >> id;
                cin.ignore();
                examenSeleccionado->consultarPregunta(id);
                break;
            }
            case 7: { // Filtrar preguntas por nivel
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                Examen* examenSeleccionado = examenes[idx];
                int nivel;
                cout << "Nivel de Bloom a filtrar (0-5): ";
                cin >> nivel;
                cin.ignore();
                examenSeleccionado->filtrarPreguntas(nivel);
                break;
            }
            case 8: { // Mostrar evaluación
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                examenes[idx]->mostrarExamen();
                break;
            }
            case 9: { // Mostrar preguntas
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                examenes[idx]->mostrarPreguntas();
                break;
            }
            case 10: { // Guardar examen a archivo TXT (opción explícita)
                int idx = seleccionarExamen(examenes, numExamenes);
                if (idx == -1) break;
                string nombreArchivo = formatearNombreArchivo(examenes[idx]->getNombre());
                guardarExamenEnArchivoTXT(*examenes[idx], nombreArchivo);
                break;
            }
            case 11: {
                cout << "Que tenga una buena jornada. Adiós." << endl;
                break;
            }
            default: {
                cout << "Opción inválida." << endl;
                break;
            }
        }
    } while (opcion != 11);

    // Liberamos la memoria ocupada por los exámenes
    for (int i = 0; i < numExamenes; i++) {
        delete examenes[i];
    }
    return 0;
}
