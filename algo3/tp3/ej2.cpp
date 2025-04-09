#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#define INF INT_MAX

using namespace std;

/*
 * Idea: el ejercicio es similar al 13 de la guía 4. En un primer momento, vamos a tener que crear el grafo con
 * las mínimas aristas posibles que cumplan las latencias dadas, si es posible. Luego, vamos a generar la matriz de
 * distancias (siendo la distancia la cantidad de nodos intermedios) entre cada par de nodos.
 *
 * La parte dificil es generar el grafo. Para ello, vamos a crear un primer grafo con todas las aristas posibles para
 * después iterar una por una chequeando si es necesaria. Una arista i-j no es necesaria cuando existe un nodo k tal que
 * L (i-j) = L (i-k) + L (k-j) , siendo la funcion L(x-y) la latencia entre el nodo x y el nodo y. En el caso que nos
 * encontremos nodos i,j, k que cumplan L (i-j) > L (i-k) + L (k-j) , entonces una de las latencias dadas es erronea, o
 * mejor dicho, no se puede crear un grafo que cumpla dichas latencias.
 */


void quitar_aristas(int &n, vector<vector<int>> &l, vector<vector<int>> &a, bool &p){
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {

                if (i != j && j != k && i !=k ) { // No nos interesa y nos rompe el código chequear aristas de un nodo
                                                  // a sí mismo.

                    if (l[i][j] == l[i][k] + l[k][j]) {
                        /*
                         * Este caso indica que encontramos un camino de i a j con un nodo intermedio que tiene la misma
                         * latencia que la del camino sin nodo intermedio, entonces sacamos la arista entre i y j, puesto
                         * que no es necesaria. Si sacasemos el camino con el nodo intermedio, podriamos estar quitando
                         * aristas de otro tipo de latencias.
                         */
                        a[i][j] = 0;
                        a[j][i] = 0;
                    }
                    if (l[i][j] > l[i][k] + l[k][j]) {
                        /*
                         * Este caso indica que encontramos una latencia menor a una dada de entrada, es decir, no es una
                         * latencia válida.
                         */
                        p = false;
                        j = n;
                        i = n;
                        k = n;

                    }
                }

            }

        }

    }

}

void floydWarshall(int &n,const vector<vector<int>> &a, vector<vector<int>> &distancias) {
    /*
     * Calculamos las distancias (siendo la distancia la cantidad de nodos intermedios)
     */

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if ( distancias[i][k] != INF && distancias[k][j] != INF) {
                    
                    distancias[i][j] = min(distancias[i][j], distancias[i][k] + distancias[k][j]);
                    
                }
            }
        }
    }
}




int main() {
    int cantidad_tests;
    cin >> cantidad_tests;

    for (int t = 0; t < cantidad_tests; t++) {

        int nodos;
        cin >> nodos;

        /*
         * Vamos a crear un primer grafo con todas las aristas posibles. Todas las aristas posibles son las latencias.
         */

        vector<vector<int>> latencias(nodos, vector<int>(nodos, 0));
        vector<vector<int>> adyacencias(nodos, vector<int>(nodos, 0));

        for (int j = 1; j < nodos ; ++j) {
            for (int k = 0; k < j; ++k) {
                int latencia;
                cin >> latencia;

                latencias[j][k] = latencia;
                latencias[k][j] = latencia;

                adyacencias[j][k] = 1;
                adyacencias[k][j] = 1;

            }
        }

        bool posible = true;

        quitar_aristas(nodos , latencias , adyacencias, posible);

        if (posible){

            cout << "POSIBLE" << endl;

            // Creamos una matriz de distancias.

            vector<vector<int>> distancias(nodos , vector<int>( nodos));

            for (int i = 0; i < nodos; i++) {
                for (int j = 0; j < nodos; j++) {
                    if (i == j) {
                        distancias[i][j] = 0; // La distancia de un nodo a sí mismo es 0.
                    } else if (adyacencias[i][j]) {
                        distancias[i][j] = 1; // Si hay una arista, es 1.
                    } else {
                        distancias[i][j] = INF; // Si no hay arista, es infinito.
                    }
                }
            }

            floydWarshall(nodos,adyacencias, distancias);

            for (int i = 0; i < nodos; i++) {
                for (int j = 0; j < nodos; j++) {
                    if(j + 1 == nodos){
                        cout << distancias[i][j]<< endl;
                    }else{
                        cout << distancias[i][j] << " " ;
                    }
                }
            }

        }else{
            cout << "IMPOSIBLE" << endl;
        }

    }

    return 0;
}
