#include "Conjunto.h"

template <class T>
Conjunto<T>::Conjunto() {
    raiz = nullptr;
    size = 0;
}

template <class T>
Conjunto<T>::~Conjunto() {
    while (size != 0){
        remover(raiz->valor);
    }
}

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const {
    Nodo* nodo = raiz;
    while(nodo != nullptr){
        if (nodo->valor == clave){
            return true;
        }
        else if (clave > nodo->valor)
        {
            nodo = nodo->der;
        }
        else {
            nodo = nodo->izq;
        }
    }

    return false;
}

template <class T>
void Conjunto<T>::insertar(const T& clave) {
    if(!pertenece(clave))
    {
        if (raiz == nullptr){
            Nodo* nuevoNodo = new Nodo(clave);
            raiz = nuevoNodo;
        }
        else{
            Nodo* nodo = raiz;
            bool fin = false;
            while(!fin){
                if (nodo->valor > clave){
                    if (nodo->izq != nullptr)
                    {
                        nodo = nodo->izq;
                    } else{
                        nodo->izq = new Nodo(clave);
                        fin = true;
                    }
                }
                else{
                    if (nodo->der != nullptr)
                    {
                        nodo = nodo->der;
                    } else{
                        nodo->der = new Nodo(clave);
                        fin = true;
                    }
                }
            }
        }
        size ++;
    }
}

template <class T>
void Conjunto<T>::remover(const T& clave) {
    if (pertenece(clave)){
        Nodo* nodo = raiz;
        Nodo* padre = nullptr;
        if (size == 1){
            delete raiz;
            raiz = nullptr;
        }
        else{
            while (nodo->valor != clave){   // Encuentro el nodo a eliminar y veo qué nodo es el padre para luego cambiar su hijo.
                padre = nodo;
                if (nodo->valor > clave){
                    nodo = nodo->izq;
                } else
                {
                    nodo = nodo->der;
                }
            }
            moverSubArbol(padre, nodo);
        }
        size--;
    }
}

template<class T>
void Conjunto<T>::moverSubArbol(Nodo* padre, Nodo* nodo) {
    Nodo *menor = nodo->izq;
    Nodo *mayor = nodo->der;
    if (menor != nullptr) {
        if (raiz != nodo) {
            if (padre->der == nodo) {
                padre->der = menor;
            } else {
                padre->izq = menor;
            }
        } else{
            raiz = menor;
        }
        while (menor->der != nullptr) {
            menor = menor->der;     // Busco el mas grande del subarbol izquierdo.
        }
        menor->der = nodo->der;     // Lo uno con el hijo mas grande del nodo a eliminar.

    } else if (mayor != nullptr) {
        if (raiz != nodo) {
            if (padre->der == nodo) {
                padre->der = mayor;
            } else {
                padre->izq = mayor;
            }
        } else {
            raiz = mayor;
        }
        while (mayor->izq != nullptr) {
            mayor = mayor->izq;     // Busco el mas chico del subarbol derecho.
        }
        mayor->izq = nodo->izq;     // Lo uno con el hijo mas chico del nodo a eliminar.

    }
    else{
        if (padre->der == nodo) {
            padre->der = nullptr;
        } else {
            padre->izq = nullptr;
        }
    }
    delete nodo;
}


template <class T>
const T& Conjunto<T>::siguiente(const T& clave) {
    Nodo* nodo = raiz;
    Nodo* padre = nullptr;

    while (nodo->valor != clave){
        padre = nodo;
        if (clave < nodo->valor){
            nodo = nodo->izq;
        } else{
            nodo = nodo->der;
        }
    }

    if (nodo->der == nullptr){
        return padre->valor;
    }
    else
    {
        Nodo* hijoMayor = nodo->der;
        while(hijoMayor->izq != nullptr){
            hijoMayor = hijoMayor->izq;
        }
        return hijoMayor->valor;
    }
}

template <class T>
const T& Conjunto<T>::minimo() const {
    Nodo* nodo = raiz;

    while(nodo->izq != nullptr){
        nodo = nodo->izq;
    }

    return nodo->valor;

}

template <class T>
const T& Conjunto<T>::maximo() const {
    Nodo* nodo = raiz;

    while(nodo->der != nullptr){
        nodo = nodo->der;
    }

    return nodo->valor;

}

template <class T>
unsigned int Conjunto<T>::cardinal() const {
    return size;
}