#include <iostream>
#include <string>
#include <vector>

using namespace std;


struct Estudiante {
    int matricula;
    string nombre;
    float calificaciones[3]; 
    float promedio;

    
    void calcularPromedio() {
        float sumaCalificaciones = 0; 
        for (int i = 0; i < 3; i++) {
            sumaCalificaciones += calificaciones[i];
        }
        promedio = sumaCalificaciones / 3.0;
    }
};


struct Nodo {
    
    Estudiante estudiante; 
    Nodo* izquierdo;
    Nodo* derecho;
};


class ArbolBST {
private:
    

    
    Nodo* crearNodo(Estudiante datosNuevoEstudiante) { 
        Nodo* nuevoNodo = new Nodo(); 
        nuevoNodo->estudiante = datosNuevoEstudiante;
        nuevoNodo->izquierdo = NULL;
        nuevoNodo->derecho = NULL;
        return nuevoNodo;
    }

    
    Nodo* insertarRecursivo(Nodo* nodoActual, Estudiante estudianteAInsertar) {
        if (nodoActual == NULL) {
            return crearNodo(estudianteAInsertar);
        }

        
        if (estudianteAInsertar.matricula < nodoActual->estudiante.matricula) {
            nodoActual->izquierdo = insertarRecursivo(nodoActual->izquierdo, estudianteAInsertar);
        } else if (estudianteAInsertar.matricula > nodoActual->estudiante.matricula) {
            nodoActual->derecho = insertarRecursivo(nodoActual->derecho, estudianteAInsertar);
        }
        

        return nodoActual;
    }

    
    void imprimirEstudiante(Estudiante estudianteAImprimir) {
        cout << "  Matricula: " << estudianteAImprimir.matricula << endl;
        cout << "  Nombre:    " << estudianteAImprimir.nombre << endl;
        cout << "  Califs:    [" << estudianteAImprimir.calificaciones[0] << ", "
             << estudianteAImprimir.calificaciones[1] << ", " << estudianteAImprimir.calificaciones[2] << "]" << endl;
        cout << "  Promedio:  " << estudianteAImprimir.promedio << endl;
        cout << "-------------------------" << endl;
    }

    
    void inOrden(Nodo* nodoActual) {
        if (nodoActual != NULL) {
            inOrden(nodoActual->izquierdo);
            imprimirEstudiante(nodoActual->estudiante);
            inOrden(nodoActual->derecho);
        }
    }

    
    void inOrdenCondicional(Nodo* nodoActual, float limitePromedio, bool esMenorQueLimite) {
        if (nodoActual != NULL) {
            inOrdenCondicional(nodoActual->izquierdo, limitePromedio, esMenorQueLimite);

            if (esMenorQueLimite && nodoActual->estudiante.promedio < limitePromedio) {
                imprimirEstudiante(nodoActual->estudiante);
            } else if (!esMenorQueLimite && nodoActual->estudiante.promedio > limitePromedio) {
                imprimirEstudiante(nodoActual->estudiante);
            }

            inOrdenCondicional(nodoActual->derecho, limitePromedio, esMenorQueLimite);
        }
    }

    
    Nodo* buscarRecursivo(Nodo* nodoActual, int matriculaBuscada) {
        if (nodoActual == NULL || nodoActual->estudiante.matricula == matriculaBuscada) {
            return nodoActual;
        }

        if (matriculaBuscada < nodoActual->estudiante.matricula) {
            return buscarRecursivo(nodoActual->izquierdo, matriculaBuscada);
        } else {
            return buscarRecursivo(nodoActual->derecho, matriculaBuscada);
        }
    }

    
    Nodo* encontrarMinimo(Nodo* nodoInicial) {
        Nodo* nodoActual = nodoInicial;
        while (nodoActual && nodoActual->izquierdo != NULL) {
            nodoActual = nodoActual->izquierdo;
        }
        return nodoActual;
    }

    
    Nodo* eliminarRecursivo(Nodo* nodoActual, int matriculaAEliminar) {
        if (nodoActual == NULL) {
            return nodoActual; 
        }

        
        if (matriculaAEliminar < nodoActual->estudiante.matricula) {
            nodoActual->izquierdo = eliminarRecursivo(nodoActual->izquierdo, matriculaAEliminar);
        } else if (matriculaAEliminar > nodoActual->estudiante.matricula) {
            nodoActual->derecho = eliminarRecursivo(nodoActual->derecho, matriculaAEliminar);
        } else {
            
            
            if (nodoActual->izquierdo == NULL) {
                Nodo* nodoTemporal = nodoActual->derecho;
                delete nodoActual;
                return nodoTemporal;
            } else if (nodoActual->derecho == NULL) {
                Nodo* nodoTemporal = nodoActual->izquierdo;
                delete nodoActual;
                return nodoTemporal;
            }

            
            
            Nodo* nodoTemporalSucesor = encontrarMinimo(nodoActual->derecho);

            
            nodoActual->estudiante = nodoTemporalSucesor->estudiante;

            
            nodoActual->derecho = eliminarRecursivo(nodoActual->derecho, nodoTemporalSucesor->estudiante.matricula);
        }
        return nodoActual;
    }

    
    void contarAlumnosConCalificacionMenor(Nodo* nodoActual, int valorLimite, int* punteroContador) {
        if (nodoActual == NULL) {
            return;
        }
        
        contarAlumnosConCalificacionMenor(nodoActual->izquierdo, valorLimite, punteroContador);

        bool tieneCalificacionMenor = false;
        for (int i = 0; i < 3; i++) {
            if (nodoActual->estudiante.calificaciones[i] < valorLimite) {
                tieneCalificacionMenor = true;
                break; 
            }
        }

        if (tieneCalificacionMenor) {
            (*punteroContador)++; 
        }

        contarAlumnosConCalificacionMenor(nodoActual->derecho, valorLimite, punteroContador);
    }

    
    void imprimirNivel(Nodo* nodoActual, int nivelBuscado, int nivelActual) {
        if (nodoActual == NULL) {
            return; 
        }

        if (nivelActual == nivelBuscado) {
            imprimirEstudiante(nodoActual->estudiante);
        } else {
            imprimirNivel(nodoActual->izquierdo, nivelBuscado, nivelActual + 1);
            imprimirNivel(nodoActual->derecho, nivelBuscado, nivelActual + 1);
        }
    }

    
    void llenarVector(Nodo* nodoActual, vector<Estudiante>* vectorEstudiantes) {
        if (nodoActual != NULL) {
            llenarVector(nodoActual->izquierdo, vectorEstudiantes);
            vectorEstudiantes->push_back(nodoActual->estudiante);
            llenarVector(nodoActual->derecho, vectorEstudiantes);
        }
    }

    
    
    void mostrarArbolGraficoAux(Nodo* nodoActual, int espacio) {
        
        if (nodoActual == NULL) {
            return;
        }

        
        
        int ESPACIO_POR_NIVEL = 10;
        espacio += ESPACIO_POR_NIVEL;

        
        mostrarArbolGraficoAux(nodoActual->derecho, espacio);

        
        cout << endl; 
        for (int i = ESPACIO_POR_NIVEL; i < espacio; i++) {
            cout << " "; 
        }
        
        cout << "[" << nodoActual->estudiante.matricula << "]" << endl;

        
        mostrarArbolGraficoAux(nodoActual->izquierdo, espacio);
    }


public:
    Nodo* raiz; 

    
    ArbolBST() {
        raiz = NULL; 
    }

    

    
    void reportePorMatricula() {
        cout << "\n--- Reporte Ordenado por Matricula ---" << endl;
        if(raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
        } else {
            inOrden(raiz);
        }
        cout << "--------------------------------------" << endl;
    }

    
    void reportePorPromedio() {
        cout << "\n--- Reporte Ordenado por Promedio (Mayor a Menor) ---" << endl;
        if(raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
            cout << "----------------------------------------------------" << endl;
            return;
        }

        vector<Estudiante> todosLosEstudiantes;
        llenarVector(raiz, &todosLosEstudiantes);

        
        int totalEstudiantes = todosLosEstudiantes.size();
        bool intercambiado;
        for (int i = 0; i < totalEstudiantes - 1; i++) {
            intercambiado = false;
            for (int j = 0; j < totalEstudiantes - i - 1; j++) {
                if (todosLosEstudiantes[j].promedio < todosLosEstudiantes[j + 1].promedio) {
                    
                    
                    Estudiante estudianteTemporal = todosLosEstudiantes[j]; 
                    todosLosEstudiantes[j] = todosLosEstudiantes[j + 1];
                    todosLosEstudiantes[j + 1] = estudianteTemporal;
                    
                    intercambiado = true;
                }
            }
            if (intercambiado == false) {
                break;
            }
        }
        

        for (int i = 0; i < todosLosEstudiantes.size(); i++) {
            imprimirEstudiante(todosLosEstudiantes[i]);
        }
        cout << "----------------------------------------------------" << endl;
    }

    
    void reporteMenorA70() {
        cout << "\n--- Reporte Promedio < 70 ---" << endl;
         if(raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
        } else {
            inOrdenCondicional(raiz, 70.0, true); 
        }
        cout << "-----------------------------" << endl;
    }

    
    void reporteMayorA70() {
        cout << "\n--- Reporte Promedio > 70 ---" << endl;
         if(raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
        } else {
            inOrdenCondicional(raiz, 70.0, false); 
        }
        cout << "-----------------------------" << endl;
    }

    
    void insertar(Estudiante estudianteAInsertar) {
        estudianteAInsertar.calcularPromedio(); 
        raiz = insertarRecursivo(raiz, estudianteAInsertar);
        cout << "Estudiante " << estudianteAInsertar.nombre << " insertado." << endl;
    }

    
    void eliminar(int matriculaAEliminar) {
        Nodo* nodoAEliminar = buscarRecursivo(raiz, matriculaAEliminar);
        
        if (nodoAEliminar == NULL) {
            cout << "No se encontro estudiante con matricula " << matriculaAEliminar << endl;
        } else {
            raiz = eliminarRecursivo(raiz, matriculaAEliminar);
            cout << "Estudiante con matricula " << matriculaAEliminar << " eliminado." << endl;
        }
    }

    
    void actualizarCalificacion(int matriculaBuscada) {
        Nodo* nodoEncontrado = buscarRecursivo(raiz, matriculaBuscada);

        if (nodoEncontrado != NULL) {
            int indiceCalificacion; 
            float nuevaCalificacion;
            
            cout << "Calificaciones actuales: [" << nodoEncontrado->estudiante.calificaciones[0] << ", "
                 << nodoEncontrado->estudiante.calificaciones[1] << ", " << nodoEncontrado->estudiante.calificaciones[2] << "]" << endl;
            
            cout << "Introduce el numero de calificacion a cambiar (0, 1 o 2): ";
            cin >> indiceCalificacion;
            
            if (indiceCalificacion >= 0 && indiceCalificacion < 3) {
                cout << "Introduce la nueva calificacion: ";
                cin >> nuevaCalificacion;
                
                nodoEncontrado->estudiante.calificaciones[indiceCalificacion] = nuevaCalificacion;
                nodoEncontrado->estudiante.calcularPromedio(); 
                
                cout << "Calificacion actualizada para " << nodoEncontrado->estudiante.nombre << endl;
                cout << "Nuevo promedio: " << nodoEncontrado->estudiante.promedio << endl;
            } else {
                cout << "Numero de calificacion no valido." << endl;
            }
        } else {
            cout << "Estudiante con matricula " << matriculaBuscada << " no encontrado." << endl;
        }
    }

    
    void contarMenorA(int matriculaBase) { 
        int contadorAlumnos = 0; 
        
        Nodo* nodoEstudianteBase = buscarRecursivo(raiz, matriculaBase);
        if (nodoEstudianteBase == NULL) {
            cout << "La matricula " << matriculaBase << " no existe en el arbol." << endl;
            cout << "No se puede realizar la comparacion." << endl;
            return;
        }

        
        contarAlumnosConCalificacionMenor(raiz, matriculaBase, &contadorAlumnos);
        
        cout << "Numero de alumnos con AL MENOS UNA calificacion < " << matriculaBase << ": " << contadorAlumnos << endl;
    }

    
    void mostrarNivel(int nivelAMostrar) {
        cout << "\n--- Nodos en el Nivel " << nivelAMostrar << " ---" << endl;
        if(raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
        } else {
            imprimirNivel(raiz, nivelAMostrar, 0); 
        }
        cout << "---------------------------" << endl;
    }

    
    void mostrarArbolGrafico() {
        cout << "\n--- Arbol Grafico (Matriculas) ---" << endl;
        if (raiz == NULL) {
            cout << "El arbol esta vacio." << endl;
        } else {
            
            
            mostrarArbolGraficoAux(raiz, 0);
        }
        cout << "\n----------------------------------" << endl;
    }
};




void mostrarMenu() {
    cout << "\n===== MENU DE GESTION DE ESTUDIANTES =====" << endl;
    cout << "1. Reporte por Matricula (In-Orden)" << endl;
    cout << "2. Reporte por Promedio (Mayor a Menor)" << endl;
    cout << "3. Reporte de Promedios < 70" << endl;
    cout << "4. Reporte de Promedios > 70" << endl;
    cout << "5. Insertar Estudiante" << endl;
    cout << "6. Eliminar Estudiante (por Matricula)" << endl;
    cout << "7. Actualizar Calificacion (por Matricula)" << endl;
    cout << "8. Contar (Calificacion < Matricula dada)" << endl;
    cout << "9. Mostrar Nivel del Arbol" << endl;
    cout << "10. Mostrar Arbol Grafico (ASCII)" << endl; 
    cout << "0. Salir" << endl;
    cout << "==========================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    ArbolBST arbolEstudiantes; 
    int opcionMenu; 
    
    
    Estudiante est1 = {100, "Ana Lopez", {80, 90, 70}};
    Estudiante est2 = {50, "Luis Garcia", {60, 65, 68}};
    Estudiante est3 = {150, "Maria Sol", {95, 100, 98}};
    Estudiante est4 = {75, "Juan Perez", {70, 75, 80}};
    Estudiante est5 = {25, "Eva Ruiz", {50, 60, 55}};
    Estudiante est6 = {120, "Carlos Diaz", {85, 88, 92}};

    arbolEstudiantes.insertar(est1);
    arbolEstudiantes.insertar(est2);
    arbolEstudiantes.insertar(est3);
    arbolEstudiantes.insertar(est4);
    arbolEstudiantes.insertar(est5);
    arbolEstudiantes.insertar(est6);
    

    do {
        mostrarMenu();
        cin >> opcionMenu;
        cin.ignore(); 

        switch (opcionMenu) {
            case 1:
                arbolEstudiantes.reportePorMatricula();
                break;
            case 2:
                arbolEstudiantes.reportePorPromedio();
                break;
            case 3:
                arbolEstudiantes.reporteMenorA70();
                break;
            case 4:
                arbolEstudiantes.reporteMayorA70();
                break;
            case 5: {
                Estudiante nuevoEstudiante; 
                cout << "Ingrese Matricula: ";
                cin >> nuevoEstudiante.matricula;
                cin.ignore(); 
                cout << "Ingrese Nombre: ";
                getline(cin, nuevoEstudiante.nombre);
                cout << "Ingrese Calificacion 1: ";
                cin >> nuevoEstudiante.calificaciones[0];
                cout << "Ingrese Calificacion 2: ";
                cin >> nuevoEstudiante.calificaciones[1];
                cout << "Ingrese Calificacion 3: ";
                cin >> nuevoEstudiante.calificaciones[2];
                arbolEstudiantes.insertar(nuevoEstudiante);
                break;
            }
            case 6: {
                int matriculaAEliminar; 
                cout << "Ingrese Matricula a eliminar: ";
                cin >> matriculaAEliminar;
                arbolEstudiantes.eliminar(matriculaAEliminar);
                break;
            }
            case 7: {
                int matriculaAActualizar; 
                cout << "Ingrese Matricula a actualizar: ";
                cin >> matriculaAActualizar;
                arbolEstudiantes.actualizarCalificacion(matriculaAActualizar);
                break;
            }
            case 8: {
                int matriculaParaComparar; 
                cout << "Ingrese Matricula base para contar: ";
                cin >> matriculaParaComparar;
                arbolEstudiantes.contarMenorA(matriculaParaComparar);
                break;
            }
            case 9: {
                int nivelAMostrar; 
                cout << "Ingrese el Nivel a mostrar (0 es la raiz): ";
                cin >> nivelAMostrar;
                arbolEstudiantes.mostrarNivel(nivelAMostrar);
                break;
            }
            case 10: 
                arbolEstudiantes.mostrarArbolGrafico();
                break;
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }

    } while (opcionMenu != 0);

    return 0;
}
