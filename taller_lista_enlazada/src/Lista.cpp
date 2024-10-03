#include "Lista.h"
#include <cassert>

Lista::Lista() {
    _primero = nullptr;
    _ultimo = nullptr;
}

Lista::Lista(const Lista& l) : Lista() {
    //Inicializa una lista vacía y luego utiliza operator= para no duplicar el código de la copia de una lista.
    *this = l;
}

Lista::~Lista() {
    // Completar
}

Lista& Lista::operator=(const Lista& aCopiar) {
    // Completar
    return *this;
}

void Lista::agregarAdelante(const int& elem) {
    Nodo* nuevoNodo = new Nodo(elem);
    nuevoNodo->anterior = nullptr;
    nuevoNodo->sig = _primero;
    if (_primero != nullptr) {
        _primero->anterior = nuevoNodo;
    }
    else
    {
        _primero = nuevoNodo;
        _ultimo = nuevoNodo;
    }
    _size++;
}

void Lista::agregarAtras(const int& elem) {
    Nodo* nuevoNodo = new Nodo(elem);
    nuevoNodo->sig = nullptr;
    nuevoNodo->anterior = _ultimo;

    _ultimo = nuevoNodo;

    if (_ultimo == nullptr){
    }
    else{
        _primero = nuevoNodo;
    }
    _size++;
}

void Lista::eliminar(Nat i) {
    // Completar
}

int Lista::longitud() const {
    return _size;
}

const int& Lista::iesimo(Nat i) const {
    Nodo *nodoDeInteres = nullptr;
    for (int j = 0; j < i; ++j) {
        if(j == 1) {
            nodoDeInteres = _primero;
        }
        else{
            nodoDeInteres = nodoDeInteres->sig;
        }
    }
    return nodoDeInteres->valor;
}

int& Lista::iesimo(Nat i) {
    if (i <= _size) {
        Nodo *nodoDeInteres = _primero;
        for (int j = 0; j < i; ++j) {
            nodoDeInteres = nodoDeInteres->sig;
        }
        return nodoDeInteres->valor;
    }
}

void Lista::mostrar(ostream& o) {
    // Completar
}
