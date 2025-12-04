#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // Incluida para std::transform y std::tolower
#include <vector>
#include <limits> // Incluida para std::numeric_limits (limpieza del buffer)

using namespace std;


struct Pelicula {
    string titulo;
    string fecha_estreno;
    string generos;
    string resumen;
    string eslogan;

    void mostrar() {
        cout << "\n--- INFORMACIÓN DE PELÍCULA ---" << endl;
        cout << "  Título: " << titulo << endl;
        cout << "  Fecha de Estreno: " << fecha_estreno << endl;
        cout << "  Géneros: " << generos << endl;
        cout << "  Eslogan: " << eslogan << endl;
        cout << "  Resumen: " << resumen << endl;
        cout << "-------------------------------" << endl;
    }
};

class Nodo {
public:
    Pelicula datos;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(Pelicula& pelicula) : datos(pelicula), siguiente(nullptr), anterior(nullptr) {}
};

class Lista_D_ligada {
private:
    Nodo* cabeza;
    Nodo* cola;

    string normalizar(string texto) {
        // Uso de <algorithm>: Convierte todo el texto a minúsculas
        transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
        return texto;
    }

    vector<string> parsear_linea_csv(string& linea) {
        vector<string> campos;
        stringstream ss(linea);
        string campo;
        bool en_comillas = false;
        char c;

        while (ss.get(c)) {
            if (c == '"') {
                en_comillas = !en_comillas;
            } else if (c == ',' && !en_comillas) {
                campos.push_back(campo);
                campo.clear();
            } else {
                campo += c;
            }
        }
        campos.push_back(campo);
        return campos;
    }


public:
    Lista_D_ligada() : cabeza(nullptr), cola(nullptr) {}

    ~Lista_D_ligada() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* sig = actual->siguiente;
            delete actual;
            actual = sig;
        }
        cabeza = nullptr;
        cola = nullptr;
    }

    void insertar_pelicula(Pelicula& pelicula, string posicion = "final", bool silencioso = false) {
        Nodo* nuevoNodo = new Nodo(pelicula);

        if (cabeza == nullptr) {
            cabeza = cola = nuevoNodo;
        } else if (posicion == "inicio") {
            nuevoNodo->siguiente = cabeza;
            cabeza->anterior = nuevoNodo;
            cabeza = nuevoNodo;
        } else {
            nuevoNodo->anterior = cola;
            cola->siguiente = nuevoNodo;
            cola = nuevoNodo;
        }
        
        if (!silencioso) { 
            cout << "Película '" << pelicula.titulo << "' insertada con éxito." << endl;
        }
    }

    void importar_datos(string nombre_archivo) {
        ifstream archivo(nombre_archivo);
        if (!archivo.is_open()) {
            cerr << "ERROR: No se pudo abrir el archivo " << nombre_archivo << endl;
            return;
        }

        cout << "\nIniciando carga de datos desde " << nombre_archivo << ". Esto puede tomar un momento..." << endl;
        string linea;
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            vector<string> campos = parsear_linea_csv(linea);

            if (campos.size() > 22) {
                Pelicula peli;
                peli.titulo = campos[1];
                peli.fecha_estreno = campos[5];
                peli.resumen = campos[15];
                peli.eslogan = campos[18];
                peli.generos = campos[19];
                
                insertar_pelicula(peli, "final", true); 
            }
        }
        cout << "\nCarga de datos desde '" << nombre_archivo << "' finalizada con éxito." << endl;
    }

    void buscar_por_titulo(string titulo_busqueda) {
        string busqueda_normalizada = normalizar(titulo_busqueda);
        bool encontrado = false;
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (normalizar(actual->datos.titulo).find(busqueda_normalizada) != string::npos) {
                actual->datos.mostrar();
                encontrado = true;
            }
            actual = actual->siguiente;
        }

        if (!encontrado) {
            cout << "\nNo se encontró ninguna película que contenga el título: '" << titulo_busqueda << "'" << endl;
        }
    }

    void buscar_por_palabra_clave(string palabra_clave) {
        string busqueda_normalizada = normalizar(palabra_clave);
        bool encontrado = false;
        Nodo* actual = cabeza;

        cout << "\n--- BÚSQUEDA POR PALABRA CLAVE: " << palabra_clave << " ---" << endl;
        while (actual != nullptr) {
            if (normalizar(actual->datos.generos).find(busqueda_normalizada) != string::npos ||
                normalizar(actual->datos.resumen).find(busqueda_normalizada) != string::npos) {
                
                cout << "  - Encontrado en: **" << actual->datos.titulo << "**" << endl;
                cout << "    Fecha: " << actual->datos.fecha_estreno << endl;
                cout << "    Géneros: " << actual->datos.generos << "\n" << endl;
                encontrado = true;
            }
            actual = actual->siguiente;
        }

        if (!encontrado) {
            cout << "No se encontró la palabra clave '" << palabra_clave << "' en ninguna película." << endl;
        }
        cout << "------------------------------------------" << endl;
    }

    void eliminar_por_titulo(string titulo_eliminar) {
        string busqueda_normalizada = normalizar(titulo_eliminar);
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            if (normalizar(actual->datos.titulo).find(busqueda_normalizada) != string::npos) {
                actual->datos.mostrar(); 

                char confirmar;
                cout << "¿Desea eliminar esta película? (s/n): ";
                cin >> confirmar;
                // Uso de <limits>: Limpia el buffer de entrada para evitar problemas en el siguiente getline().
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                if (tolower(confirmar) == 's') {
                    Nodo* a_borrar = actual;

                    if (a_borrar == cabeza) {
                        cabeza = cabeza->siguiente;
                        if (cabeza != nullptr) cabeza->anterior = nullptr;
                    } else {
                        a_borrar->anterior->siguiente = a_borrar->siguiente;
                    }

                    if (a_borrar == cola) {
                        cola = cola->anterior;
                        if (cola != nullptr) cola->siguiente = nullptr;
                    } else if (a_borrar->siguiente != nullptr) {
                        a_borrar->siguiente->anterior = a_borrar->anterior;
                    }

                    if (cabeza == nullptr) cola = nullptr;

                    delete a_borrar;
                    cout << "Película eliminada con éxito." << endl;
                    return; 
                } else {
                    cout << "Eliminación cancelada." << endl;
                    return;
                }
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró ninguna película que contenga el título: '" << titulo_eliminar << "'" << endl;
    }

    void actualizar_pelicula(string titulo_actualizar) {
        string busqueda_normalizada = normalizar(titulo_actualizar);
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            if (normalizar(actual->datos.titulo).find(busqueda_normalizada) != string::npos) {
                actual->datos.mostrar();

                int opcion;
                cout << "\n¿Qué dato desea modificar?" << endl;
                cout << "1. Título" << endl;
                cout << "2. Fecha de Estreno" << endl;
                cout << "3. Géneros" << endl;
                cout << "4. Eslogan" << endl;
                cout << "5. Resumen" << endl;
                cout << "Ingrese opción (1-5): ";
                cin >> opcion;
                // Uso de <limits>: Limpia el buffer de entrada antes de leer una cadena con espacios.
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                string nuevo_valor;
                switch (opcion) {
                    case 1: cout << "Nuevo título: "; getline(cin, nuevo_valor); actual->datos.titulo = nuevo_valor; break;
                    case 2: cout << "Nueva fecha: "; getline(cin, nuevo_valor); actual->datos.fecha_estreno = nuevo_valor; break;
                    case 3: cout << "Nuevos géneros: "; getline(cin, nuevo_valor); actual->datos.generos = nuevo_valor; break;
                    case 4: cout << "Nuevo eslogan: "; getline(cin, nuevo_valor); actual->datos.eslogan = nuevo_valor; break;
                    case 5: cout << "Nuevo resumen: "; getline(cin, nuevo_valor); actual->datos.resumen = nuevo_valor; break;
                    default: cout << "Opción inválida." << endl; return;
                }
                cout << "Dato actualizado con éxito." << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró ninguna película que contenga el título: '" << titulo_actualizar << "'" << endl;
    }

    void mostrar_todo() {
        if (cabeza == nullptr) {
            cout << "\nLa lista está vacía." << endl;
            return;
        }
        Nodo* actual = cabeza;
        cout << "\n--- LISTA COMPLETA DE PELÍCULAS ---" << endl;
        int contador = 1;
        while (actual != nullptr) {
            cout << contador++ << ". " << actual->datos.titulo << " (" << actual->datos.fecha_estreno << ")" << endl;
            actual = actual->siguiente;
        }
        cout << "-----------------------------------" << endl;
    }
};

void mostrar_menu() {
    cout << "\n======================================" << endl;
    cout << "  SISTEMA DE GESTIÓN DE PELÍCULAS" << endl;
    cout << "======================================" << endl;
    cout << "1. Cargar datos desde CSV" << endl;
    cout << "2. Insertar nueva película (Al final)" << endl;
    cout << "3. Buscar película por título" << endl;
    cout << "4. Buscar por palabra clave (en géneros/resumen)" << endl;
    cout << "5. Eliminar película por título" << endl;
    cout << "6. Actualizar datos de película" << endl;
    cout << "7. Mostrar todas las películas (solo títulos)" << endl;
    cout << "8. Salir" << endl;
    cout << "Ingrese su opción: ";
}

int main() {
    Lista_D_ligada catalogo_peliculas; 
    string entrada;
    int opcion = 0;

    do {
        mostrar_menu();
        getline(cin, entrada);
        try {
            opcion = stoi(entrada);
        } catch (...) {
            opcion = 0; 
        }

        string titulo_temp, palabra_clave_temp;
        Pelicula nueva_pelicula;

        switch (opcion) {
            case 1:
                cout << "Ingrese el nombre del archivo (ej: Movie.csv): ";
                getline(cin, entrada);
                catalogo_peliculas.importar_datos(entrada);
                break;
            case 2: {
                cout << "Ingrese Título: "; getline(cin, nueva_pelicula.titulo);
                cout << "Ingrese Fecha de Estreno: "; getline(cin, nueva_pelicula.fecha_estreno);
                cout << "Ingrese Géneros: "; getline(cin, nueva_pelicula.generos);
                cout << "Ingrese Eslogan: "; getline(cin, nueva_pelicula.eslogan);
                cout << "Ingrese Resumen: "; getline(cin, nueva_pelicula.resumen);
                catalogo_peliculas.insertar_pelicula(nueva_pelicula, "final", false); 
                break;
            }
            case 3:
                cout << "Ingrese parte del título a buscar: ";
                getline(cin, titulo_temp);
                catalogo_peliculas.buscar_por_titulo(titulo_temp);
                break;
            case 4: 
                cout << "Ingrese palabra clave a buscar: ";
                getline(cin, palabra_clave_temp);
                catalogo_peliculas.buscar_por_palabra_clave(palabra_clave_temp);
                break;
            case 5:
                cout << "Ingrese parte del título a eliminar: ";
                getline(cin, titulo_temp);
                catalogo_peliculas.eliminar_por_titulo(titulo_temp);
                break;
            case 6:
                cout << "Ingrese parte del título de la película a actualizar: ";
                getline(cin, titulo_temp);
                catalogo_peliculas.actualizar_pelicula(titulo_temp);
                break;
            case 7:
                catalogo_peliculas.mostrar_todo();
                break;
            case 8:
                cout << "Saliendo del programa. ¡Hasta luego!" << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                break;
        }

    } while (opcion != 8);

    return 0;
}