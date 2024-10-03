#include "Lista.h"
#include <cassert>

Lista::Lista() {
    _primero = nullptr;
    _size = 0;
    _ultimo = nullptr;
}

Lista::Lista(const Lista& l) : Lista() {
    //Inicializa una lista vacía y luego utiliza operator= para no duplicar el código de la copia de una lista.
    *this = l;
}

Lista::~Lista() {
    destruirNodos();
}

void Lista::destruirNodos() {
    int s = _size;
    for (int i = 0; i < s; ++i) {
        Lista::eliminar(0);
    }
}

Lista& Lista::operator=(const Lista& aCopiar) {
    destruirNodos();
    Nodo* actual = aCopiar._primero;

    while (actual != nullptr) {
        agregarAtras(actual->valor);
        actual = actual->sig;
    }


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
        _ultimo = nuevoNodo;
    }
    _primero = nuevoNodo;
    _size++;
}

void Lista::agregarAtras(const int& elem) {
    Nodo* nuevoNodo = new Nodo(elem);
    nuevoNodo->sig = nullptr;
    nuevoNodo->anterior = _ultimo;

    if (_ultimo != nullptr) {
        _ultimo->sig = nuevoNodo;
    }
    else{
        _primero = nuevoNodo;
    }
    _ultimo = nuevoNodo;
    _size++;
}

void Lista::eliminar(Nat i) {
    Nodo* nodo = _primero;
    for (int j = 0; j < i; ++j) {
        nodo = nodo->sig;
    }
    if (_size == 1){
        delete nodo;
        _ultimo = nullptr;
        _primero = nullptr;
    } else if(nodo == _ultimo){
        Nodo* anterior = nodo->anterior;
        anterior->sig = nullptr;
        delete nodo;
        _ultimo = anterior;
    }
    else if (nodo == _primero){
        Nodo* siguiente = nodo->sig;
        siguiente->anterior = nullptr;
        delete nodo;
        _primero = siguiente;
    }
    else {
        Nodo* anterior = nodo->anterior;
        Nodo* siguiente = nodo->sig;
        anterior->sig = siguiente;
        siguiente->anterior = anterior;
        delete nodo;
    }
    _size--;
}

int Lista::longitud() const {
    return _size;
}

const int& Lista::iesimo(Nat i) const {
    Nodo *nodoDeInteres = _primero;
    for (int j = 0; j < i; ++j) {
        nodoDeInteres = nodoDeInteres->sig;
    }
    return nodoDeInteres->valor;
}

int& Lista::iesimo(Nat i) {
    Nodo *nodoDeInteres = _primero;
    for (int j = 0; j < i; ++j) {
        nodoDeInteres = nodoDeInteres->sig;
    }
    return nodoDeInteres->valor;
}
