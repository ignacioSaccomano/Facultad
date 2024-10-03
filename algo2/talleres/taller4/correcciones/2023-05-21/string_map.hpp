
#include "string_map.h"

template <typename T>
string_map<T>::string_map(){
    raiz = new Nodo();
    _size = 0;
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    eliminarTodos();
    raiz = new Nodo();

    vector<pair<string, T>> datosACopiar = d.datos;

    for (pair<string, T> p: datosACopiar) {
        insert(p);
    }

}

template <typename T>
string_map<T>::~string_map(){
    eliminarTodos();
}


template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo* nodoActual = raiz;
    for (char c: clave) {
        if (nodoActual->siguientes[c] != nullptr)
        {
            nodoActual = nodoActual->siguientes[c];
        }
        else
        {
            return 0;
        }
    }
    if (nodoActual->definicion == nullptr)
    {
        return 0;
    }
    return 1;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* nodoActual = raiz;
    for (char c: clave) {
        nodoActual = nodoActual->siguientes[c];
    }
    return *nodoActual->definicion;
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* nodoActual = raiz;
    for (char c: clave) {
        nodoActual = nodoActual->siguientes[c];
    }
    return *nodoActual->definicion;
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* actual = raiz;

    for(char c: clave) {
        actual->anexos--;
        int l = c;
        Nodo *siguiente = actual->siguientes[l];
        if (nodoInutil(siguiente) || siguiente->anexos == 0)
        {
            actual->siguientes[l] = nullptr;
        }

        if (actual->anexos == 0 && actual->definicion == nullptr && actual != raiz){
            delete actual;
        }

        actual = siguiente;
    }

    delete actual->definicion;
    actual->definicion = nullptr;

    if (actual->anexos == 0)
    {
        delete actual;
    }

    datos.erase(datos.begin() + buscarDato(clave));

    _size --;
}

template <typename T>
int string_map<T>::size() const{
    return _size;
}

template <typename T>
bool string_map<T>::empty() const{
    return _size == 0;
}

template<typename T>
void string_map<T>::insert(const pair<string, T> &p) {

    Nodo* nodoActual = raiz;

    string palabra = p.first;

    T def = p.second;

    datos.push_back(p);

    for (char l: palabra) {
        nodoActual->anexos++;
        int c = l;    //  Convierte la letra a su homólogo numérico en el código ASCII.
        if (nodoActual->siguientes[c] == nullptr){
            nodoActual->siguientes[c] = new Nodo;
        }
        nodoActual = nodoActual->siguientes[c];
    }

    if (nodoActual->definicion != nullptr)
    {
        delete nodoActual->definicion;
    }
    else
    {
        _size ++;
    }
    nodoActual->definicion = new T(def);
}

//template<typename T>
//vector<int> string_map<T>::dependencias(Nodo* nodo) const {
//    vector<int> s;
//    for(int i = 0; i < 256; i++)
//    {
//        if (nodo->siguientes[i] != nullptr) {
//            s.push_back(i);
//        }
//    }
//    return s;
//}

template<typename T>
bool string_map<T>::nodoInutil(Nodo* nodo) {
    return nodo->anexos == 1 && nodo->definicion == nullptr;
}

template<typename T>
void string_map<T>::eliminarTodos() {
    while(size() > 0){
        string clave = datos[0].first;
        erase(clave);
    }
    delete raiz;
    raiz = nullptr;
}

template<typename T>
pair<string, T> string_map<T>::obtenerPar() const {
    Nodo* actual = raiz;
    unsigned int i = 0;
    string clave;

    while (actual->definicion == nullptr)
    {
        if (actual->siguientes[i] != nullptr)
        {
            actual = actual->siguientes[i];
            clave.push_back(char(i));
            i = 0;
        } else
        {
            i++;
        }
    }

    T* valor = actual->definicion;

    return pair<string, T>(clave, *valor);
}

template<typename T>
int string_map<T>::buscarDato(const string &s) {
    for (int i = 0; i < datos.size(); i++) {
        if (datos[i].first == s)
        {
            return i;
        }
    }
}


