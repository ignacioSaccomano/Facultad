#include <vector>
#include "algobot.h"

using namespace std;

// Ejercicio 1
vector<int> quitar_repetidos(vector<int> s) {
    return vector<int>();
}

// Ejercicio 2
vector<int> quitar_repetidos_v2(vector<int> s) {
    return vector<int>();
}

// Ejercicio 3
bool mismos_elementos(vector<int> a, vector<int> b) {
    return true;
}

// Ejercicio 4
bool mismos_elementos_v2(vector<int> a, vector<int> b) {
    return true;
}

// Ejercicio 5
map<int, int> contar_apariciones(vector<int> s) {
    return map<int, int>();
}

// Ejercicio 6
vector<int> filtrar_repetidos(vector<int> s) {
    return vector<int>();
}

// Ejercicio 7
set<int> interseccion(set<int> a, set<int> b) {
    return set<int>();
}

// Ejercicio 8
map<int, set<int>> agrupar_por_unidades(vector<int> s) {
    return map<int, set<int>>();
}

// Ejercicio 9
vector<char> traducir(vector<pair<char, char>> tr, vector<char> str) {
    vector<char> res;
    for (char s: str)
    {
        res.push_back(s);
        for(pair k: tr) {
            if(k.first == s) {
                res.pop_back();
                res.push_back(k.second);
            }
        }
    }

    return res;
    
}

// Ejercicio 10
bool integrantes_repetidos(vector<Mail> s) {
    return true;
}

// Ejercicio 11
map<set<LU>, Mail> entregas_finales(vector<Mail> s) {
  return map<set<LU>, Mail>();
}
