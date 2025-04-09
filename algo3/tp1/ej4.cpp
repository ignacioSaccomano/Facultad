#include <iostream>
#include <vector>
#include <string>

using namespace std;


int INF = 999999999;



/*
 *  El ejercicio consta de ubicar proveedurías entre puestos o en los mismos puestos. Haciendo un par de pruebas, nos
 *  damos cuenta de que para minimizar la distancia de un puesto a la proveeduría más cercana tenemos que ubicar dichas
 *  proveedurías en los puestos. A efectos prácticos vamos a iterar por la lista de puestos y vamos a decidir si poner 
 *  una proveeduría o no.
 *
 *  La memoria dinamica va a consistir en una matriz de puestos x proveedurías x puestos, la cual va a representar la
 *  pos actual, cuantas proveedurías faltan ubicar y cuál fue la última actual en la que se colocó una proveeduría. Esta
 *  almacenará no solo la suma de las distancias, sino también donde se pusieron las proves.
 *
 */


// Devuelve la disyunción de 2 vectores.
vector<int> disyuncionLogica(const vector<int>& vector1, const vector<int>& vector2) {
    vector<int> resultado;
    size_t i = 0;
    size_t j = 0;

    while (i < vector1.size() && j < vector2.size()) {
        if (vector1[i] < vector2[j]) {
            resultado.push_back(vector1[i]);
            ++i;
        } else if (vector2[j] < vector1[i]) {
            resultado.push_back(vector2[j]);
            ++j;
        } else {
            // Ambos vectores tienen el mismo elemento, avanzar en ambos
            ++i;
            ++j;
        }
    }

    // Agregar los elementos restantes si hay alguno
    while (i < vector1.size()) {
        resultado.push_back(vector1[i]);
        ++i;
    }

    while (j < vector2.size()) {
        resultado.push_back(vector2[j]);
        ++j;
    }

    return resultado;
}
// Devuelve la union de 2 vectores.
vector<int> unionLogica(const vector<int>& vector1, const vector<int>& vector2) {
    vector<int> resultado;
    size_t i = 0;
    size_t j = 0;

    while (i < vector1.size() || j < vector2.size()) {
        if (i < vector1.size() && (j >= vector2.size() || vector1[i] < vector2[j])) {
            resultado.push_back(vector1[i]);
            ++i;
        } else if (j < vector2.size() && (i >= vector1.size() || vector2[j] < vector1[i])) {
            resultado.push_back(vector2[j]);
            ++j;
        } else {
            // En este caso, ambos elementos son iguales, agregamos uno de ellos
            resultado.push_back(vector1[i]);
            ++i;
            ++j;
        }
    }

    return resultado;
}

// Devuelve si el vector 1 es menor lexicográficamente al vector 2.
bool esLexicograficamenteMenor(const vector<int>& vector1, const vector<int>& vector2) {
    // Determinar la longitud mínima entre los dos vectores
    size_t longitudMinima = min(vector1.size(), vector2.size());

    // Comparar elementos lexicográficamente
    for (size_t i = 0; i < longitudMinima; ++i) {
        if (vector1[i] < vector2[i]) {
            return true;
        } else if (vector2[i] < vector1[i]) {
            return false;
        }
        // Si los elementos son iguales, continúa comparando el siguiente elemento
    }

    // Si todos los elementos comparados son iguales hasta la longitud mínima,
    // el vector más corto es lexicográficamente menor
    return (vector1.size() < vector2.size());
}

//Esta función suma las distancias de los puestos que se encuentran entre la última prove que se puso y la que se va
//a poner.
int dist_poner_prove(int ultima, int actual,const vector<int>& puestos){

    int res = 0;
    if (ultima == -1) { //Caso inicial

        for (int i = 0; i < actual; ++i) {
            res += abs(puestos[actual] - puestos[i]);
        }

    }else {

        if (actual == -1) { //Caso no tengo mas provees restantes.
            for (int i = ultima; i < puestos.size(); ++i) {
                res += abs(puestos[ultima] - puestos[i]);
            }

        } else {

            for (int i = ultima; i < actual; ++i) {
                if (abs(puestos[ultima] - puestos[i]) <= abs(puestos[actual] - puestos[i])) {

                    res += abs(puestos[ultima] - puestos[i]);
                } else {
                    res += abs(puestos[actual] - puestos[i]);
                }
            }
        }
    }

    return res;
}



pair<int , vector<int>> choripanes(int pro_restantes, const vector<int>& puestos, vector<int>& pos_proves, int actual, int ultimo, int suma, vector<vector<vector<pair<int , vector<int>>>>>& memo){

    // Caso dinámico
    if ( ultimo != -1 && actual != puestos.size() && pro_restantes != 0 && memo[actual][pro_restantes-1][ultimo].first != -1 ){
        return {suma + memo[actual][pro_restantes-1][ultimo].first, unionLogica(pos_proves, memo[actual][pro_restantes-1][ultimo].second)};
    }


    //Caso base ya no hay más provees por poner.
    if (pro_restantes == 0){
        suma += dist_poner_prove(ultimo, -1, puestos);
        return {suma, pos_proves};
    }

    // Caso base ya no hay más puestos. Este caso es inválido ya que si no hay más puestos y quedan proves por poner,
    //nunca vamos a llegar al mínimo.
    if (actual == puestos.size() || pro_restantes > puestos.size()-actual){
        return {INF,{}};
    }




    // No ponemos proveeduría
    pair<int, vector<int>> no_poner = choripanes(pro_restantes, puestos, pos_proves, actual + 1, ultimo, suma, memo);

    // Ponemos proveeduría
    pos_proves.push_back(puestos[actual]);
    pair<int, vector<int>> poner = choripanes( pro_restantes - 1, puestos, pos_proves, actual + 1, actual, suma + dist_poner_prove(ultimo, actual, puestos), memo);
    pos_proves.pop_back();




    pair<int, vector<int>> res;
    
    // Devolvemos el mínimo valor de la sumatoria y el mínimo lexicograficamente.
    if (poner.first > no_poner.first) {
        res.first = no_poner.first;
        res.second = no_poner.second;
    }else{
        if (poner.first < no_poner.first){
            res.first = poner.first;
            res.second = poner.second;
        }else{
            res.first = poner.first;
            if (esLexicograficamenteMenor( no_poner.second,poner.second)){
                res.second = no_poner.second;
            }else{
                res.second = poner.second;
            }
        }
    }

    // Modificamos la memoria dinamica.
    
    if (ultimo != -1) {
        memo[actual][pro_restantes - 1][ultimo].first = res.first - suma;
        memo[actual][pro_restantes - 1][ultimo].second = disyuncionLogica(res.second, pos_proves);
    }


    return res;
}



int main() {

    int cantidad_entradas;
    cin >> cantidad_entradas;

    for (int i = 0; i < cantidad_entradas; ++i) {
        int puestos,proveedurias; //1 ≤ K ≤ N ≤ 100
        cin >> puestos;
        cin >> proveedurias;

        vector<int> posiciones (puestos); // Lista ordenada

        for (int j = 0; j < puestos; ++j) {
            int n;
            cin >> n;
            posiciones[j] = n;
        }

        vector<int> pos_proveedurias;

        vector<vector<vector<pair<int , vector<int>>>>> memo(puestos, vector<vector<pair<int , vector<int>>>>  (proveedurias, vector<pair<int , vector<int>>> (puestos , {-1,{}})));

        pair<int, vector<int>> res_final =  choripanes( proveedurias, posiciones, pos_proveedurias, 0, -1, 0, memo);

        cout << res_final.first<< endl;
        for (int j = 0; j < res_final.second.size(); ++j) {
            cout << res_final.second[j] << " ";
        }
        cout<< "\n";
    }

    return 0;
}