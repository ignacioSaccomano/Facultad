#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

int INF = 9999;



/*
 * Idea: es un ejercicio de asignación, donde vamos a asignar una torre a cada columna y a cada fila, con la
 * particularidad que hay casillas "rotas". Esto lo que va a generar es que se creen "nuevas filas y nuevas columnas"
 * donde poner las torres. También se puede pensar que las casillas rotas generan subdivisiones donde se pueden poner
 * torres. Lo difícil del ejercicio es modelar el grafo.
 */


int bfs(int s, int t, vector<int>& parent, vector<vector<int>>& capacity,const vector<vector<int>>& adj) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxflow(int s, int t,  vector<vector<int>>& capacity,const vector<vector<int>>& adj, int n) {
    int flow = 0;
    vector<int> parent(n);
    int new_flow;


    while (new_flow = bfs(s, t, parent, capacity, adj)) { //si el new_flow es igual a 0, se corta
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}












int main() {

    int cantidad_tests;
    cin >> cantidad_tests;

    for (int i = 0; i < cantidad_tests; i++) {

        int filas_columnas;
        cin >> filas_columnas;

        vector<vector<int>> tablero( filas_columnas, vector<int>(filas_columnas));
        for (int j = 0; j < filas_columnas; ++j) {
            for (int k = 0; k < filas_columnas; ++k) {
                int valor;
                cin >> valor;
                tablero[j][k] = valor;
            }
        }


        //auto start = chrono::high_resolution_clock::now();

        vector<vector<int>> capacidades(10000, vector<int>(10000,0));
        vector<vector<int>> adyacencias(10000);

        int nodo_disponible = 2;

        // Nuestro s va a ser el nodo 0 y nuestro t nuestro nodo 1.


        // Para guardar las nuevas sub_columnas que se crean me guardo un vector donde en la pos i tengo el nodo que
        // corresponde a su respectiva subdivision de la columna i. El sistema de las filas para guardar la subdivision
        // actual es otro. En particular, se van desechando los nodos filas cuando nos encontramos una casilla rota o
        // que cambiamos de fila

        // Guardamos las sub columnas.
        vector<int> nodo_columna(filas_columnas);
        for (int j = 0; j < filas_columnas; ++j) {
            nodo_columna[j]=nodo_disponible;
            //adyacencias.emplace_back();

            //Conectamos al nodo columna con t y viceversa.
            adyacencias[nodo_disponible].push_back(1);
            //adyacencias[1].push_back(nodo_disponible);
            capacidades[nodo_disponible][1] = 1;
            //capacidades[1][nodo_disponible] = 0;


            nodo_disponible++;
        }


        for (int j = 0; j < filas_columnas; ++j) {

            // Cada vez que pasemos  de fila necesitamos otro nodo fila.

            int nodo_fila=nodo_disponible;
            //adyacencias.emplace_back();

            //Conectamos al nodo fila con s y viceversa.
            //adyacencias[nodo_disponible].push_back(0);
            adyacencias[0].push_back(nodo_disponible);
            //capacidades[nodo_disponible][0] = 0;
            capacidades[0][nodo_disponible] = 1;


            nodo_disponible++;

            for (int k = 0; k < filas_columnas; ++k) {
                if (tablero[j][k] == 1){
                    // Cuando esta rota la casilla, tenemos que crear una nueva subdivision de la fila y/o de la columna.
                    // En este caso no conectamos la fila y la columna, representando que no podemos poner una torre.

                    if (k != 0 && k != filas_columnas - 1 && tablero[j][k +1] != 1){

                        //Caso en el que hay que crear una FILA nueva, porque la casilla NO se encuentra en un borde.
                        nodo_fila = nodo_disponible;
                        //adyacencias.emplace_back();

                        //Conectamos al nodo fila con s y viceversa.
                        //adyacencias[nodo_disponible].push_back(0);
                        adyacencias[0].push_back(nodo_disponible);
                        //capacidades[nodo_disponible][0] = 0;
                        capacidades[0][nodo_disponible] = 1;


                        nodo_disponible++;

                    }

                    if (j != 0 && j != filas_columnas - 1 && tablero[j +1][k] != 1){

                        //Caso en el que hay que crear una COLUMNA nueva, porque la casilla NO se encuentra en un borde.
                        nodo_columna[k]= nodo_disponible;
                        //adyacencias.emplace_back();

                        //Conectamos al nodo columna con t y viceversa.
                        adyacencias[nodo_disponible].push_back(1);
                        //adyacencias[1].push_back(nodo_disponible);
                        capacidades[nodo_disponible][1] = 1;
                        //capacidades[1][nodo_disponible] = 0;


                        nodo_disponible++;

                    }


                }else{
                    // Cuando NO está rota la casilla, hay que conectar la fila con su columna.
                    adyacencias[nodo_fila].push_back(nodo_columna[k]);
                    adyacencias[nodo_columna[k]].push_back(nodo_fila);
                    capacidades[nodo_fila][nodo_columna[k]] = 1;
                    //capacidades[nodo_columna[k]][nodo_fila] = 0;
                }
            }

        }

        // En esta altura ya creamos el digrafo (le damos condicion de digrafo al definir las capacidades de las vueltas en 0)

        int res = maxflow( 0, 1, capacidades, adyacencias, nodo_disponible);

        cout << res << endl;

        //auto stop = chrono::high_resolution_clock::now();

        //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        //cout << "Tiempo de ejecución: " << duration.count() << " milisegundos" << endl;
    }




    return 0;
}

/*
 *
3
2
0 0
0 1
3
0 0 0
0 1 0
0 0 0
3
1 0 0
1 1 0
0 0 1

 1
 4
 1 1 1 1
 1 1 1 1
 1 1 1 1
 1 1 1 1

 1
 4
 1 1 1 1
 1 0 0 1
 1 0 0 1
 1 1 1 1

 1
 3
 1 0 0
 0 1 0
 0 0 1

 1
 1
 1

 1
 1
 0

 1
 4
 1 0 0 0
 0 0 0 0
 0 1 1 0
 0 0 0 1





 */
