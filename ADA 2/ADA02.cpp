#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


/*FUNCIONES GENERALES PARA GENERAR DATOS ALEATORIOS
===========================================================*/
int capacidad() {
    int numero_de_cliente = rand() % 30 + 1;
    return numero_de_cliente;
}

int transacciones() {
    int trans = rand() % 19 + 1;
    return trans;
}

/* CLASE CLIENTE
===========================================================*/
class Cliente {
    int id;
    int transacciones;
    int tiempoEspera;
    Cliente* sig;

public:
    Cliente(int id, int transacciones) {
        this->id = id;
        this->transacciones = transacciones;
        this->tiempoEspera = 0;
        this->sig = NULL;
    }

    int getId() { return id; }
    int getTransacciones() { return transacciones; }
    int getTiempoEspera() { return tiempoEspera; }
    Cliente* getSiguiente() { return sig; }        
    void setSiguiente(Cliente* siguiente) { sig = siguiente; } 

    void setTiempoEspera(int t) { tiempoEspera = t; }
};


/*CLASE COLA.
===========================================================*/
class Cola {
    Cliente* frente;
    Cliente* final;
    int totalClientes;
    int tiempoTotal;

public:
    Cola() {
        frente = NULL;
        final = NULL;
        totalClientes = 0;
        tiempoTotal = 0;
    }

    bool estaVacia() { return frente == NULL; }

    void agregar(Cliente* nuevo) {
        if (estaVacia()) {
            frente = nuevo;
            final = nuevo;
        } else {
            final->setSiguiente(nuevo); 
            final = nuevo;
        }
        totalClientes++;
        tiempoTotal += nuevo->getTransacciones();
    }

    void eliminar() {
        if (!estaVacia()) {
            Cliente* temp = frente;
            frente = frente->getSiguiente(); 
            delete temp;
            totalClientes--;
        }
    }

    Cliente* Frente() {
        if (estaVacia()) return NULL;
        return frente;
    }

    int tamaño() { return totalClientes; }

    int getTiempoTotal() { return tiempoTotal; }

    void imprimir() {
        Cliente* aux = frente;
        cout << "Clientes en la cola: ";
        while (aux != NULL) {
            cout << "[ID: " << aux->getId()
                 << " | Transacciones: " << aux->getTransacciones() << "] ";
            aux = aux->getSiguiente();
        }
        cout << endl;
    }
};


/*CLASE CAJA DE ATENCIÓN
===========================================================*/
class CajaAtencion {
    int idCaja;
    Cola* cola;           
    int clientesAtendidos;
    int tiempoTotalAtencion;
    int tiempoPromedioEspera;

public:
    CajaAtencion(int id, Cola* c) {
        idCaja = id;
        cola = c;
        clientesAtendidos = 0;
        tiempoTotalAtencion = 0;
        tiempoPromedioEspera = 0;
    }

    void atenderClientes() {
        int tiempoAcumulado = 0;
        Cliente* cliente = cola->Frente();

        while (cliente != NULL) {
            cliente->setTiempoEspera(tiempoAcumulado);
            tiempoAcumulado += cliente->getTransacciones();

            tiempoTotalAtencion += cliente->getTransacciones();
            tiempoPromedioEspera += cliente->getTiempoEspera();
            clientesAtendidos++;

            cola->eliminar();
            cliente = cola->Frente();
        }
    }

    double promedioAtencion() {
        if (clientesAtendidos == 0) return 0;
        return (double)tiempoTotalAtencion / clientesAtendidos;
    }

    double promedioEspera() {
        if (clientesAtendidos == 0) return 0;
        return (double)tiempoPromedioEspera / clientesAtendidos;
    }

    void reporte() {
        cout << "\n--- Caja " << idCaja << " ---" << endl;
        cout << "Clientes atendidos: " << clientesAtendidos << endl;
        cout << "Promedio de atención: " << promedioAtencion() << " min" << endl;
        cout << "Promedio de espera: " << promedioEspera() << " min" << endl;
    }
};


/*FUNCIÓN PRINCIPAL
===========================================================*/
int main() {
    srand(time(0));

    Cola cola1, cola2, cola3;
    int totalClientes = capacidad();

    for (int i = 1; i <= totalClientes; i++) {
        int t = transacciones();
        Cliente* nuevo = new Cliente(i, t);

        if (t <= 6) cola1.agregar(nuevo);
        else if (t <= 12) cola2.agregar(nuevo);
        else cola3.agregar(nuevo);
    }

    cout << "=== COLAS DE CLIENTES ===\n";
    cout << "\nCola 1:"; cola1.imprimir();
    cout << "Cola 2:"; cola2.imprimir();
    cout << "Cola 3:"; cola3.imprimir();

    CajaAtencion caja1(1, &cola1);
    CajaAtencion caja2(2, &cola2);
    CajaAtencion caja3(3, &cola3);

    caja1.atenderClientes();
    caja2.atenderClientes();
    caja3.atenderClientes();

    cout << "\n=== REPORTES DE CAJAS ===\n";
    caja1.reporte();
    caja2.reporte();
    caja3.reporte();

    return 0;
}