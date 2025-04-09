#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int INF = INT_MAX;

bool compararPares(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    if (a.first != b.first) {
        return a.first < b.first;
    } else {
        return a.second < b.second;
    }
}

int dfs(int v, const vector<vector<int>> &vecinos, vector<int> &padres,vector<int> &niveles,vector<int> &minimo_nivel_desde_cada_vertice, int n){

    for (auto i : vecinos[v]) {

        if (padres[i] == -1){
            padres[i] = v;
            niveles[i] = n + 1 ;
            int cuca = dfs(i, vecinos, padres,niveles, minimo_nivel_desde_cada_vertice, n + 1);
            if (cuca < minimo_nivel_desde_cada_vertice[v]){
                minimo_nivel_desde_cada_vertice[v] = cuca;
            }

        }else if(padres[v] != i){

            if (niveles[i] < minimo_nivel_desde_cada_vertice[v]){
                minimo_nivel_desde_cada_vertice[v] = niveles[i];
            }

        }
    }
    return minimo_nivel_desde_cada_vertice[v];
}


void malvinas(const int &n_bases, const vector<pair<int , int>> &enlaces, const vector<vector<int>> &vecinos, vector<pair<int,int>> &enlaces_importantes){

    //La idea es sencilla: probar si sacando una arista se forma un puente. Y para ver si se forma un puente, nos copiamos del ej2.

    //Como en la guía, dividimos al ejercicio en 2 partes: "el algoritmo puede hacer un uso inteligente de un único DFS. Conceptualmente, y a los
    //efectos de este ejercicio, puede convenir separar el algoritmo en dos fases. La primera fase
    //aplica DFS para calcular el mínimo nivel que se puede alcanzar desde cada vértice usando
    //back edges que estén en su subárbol. La segunda fase recorre todas las aristas (sin DFS)
    //para chequear la condición."

    for (auto it = enlaces.begin(); it != enlaces.end() ; it++) {

        bool haypuente = false;

        //Borro la arista de vecinos. Saco un vertice del vecindario del otro y viceversa.

        vector<vector<int>> vecinos_sin_una = vecinos;

        auto a = find(vecinos_sin_una[it->first].begin(), vecinos_sin_una[it->first].end(), it->second);
        vecinos_sin_una[it->first].erase(a);

        auto b = find(vecinos_sin_una[it->second].begin(), vecinos_sin_una[it->second].end(), it->first);
        vecinos_sin_una[it->second].erase(b);


        //Borro la arista de enlaces.

        vector<pair<int, int>> enlaces_sin_una = enlaces;

        auto c = find(enlaces_sin_una.begin(), enlaces_sin_una.end(), *it);
        enlaces_sin_una.erase(c);





        vector<int> padre(n_bases, -1);
        padre[0] = 0;
        vector<int> minimo_nivel_desde_cada_vertice(n_bases, INF);
        vector<int> niveles(n_bases);
        niveles[0] = 0;

        dfs(0, vecinos_sin_una, padre, niveles, minimo_nivel_desde_cada_vertice, 0);


        for (auto i: enlaces_sin_una) {
            if ((padre[i.second] == i.first && minimo_nivel_desde_cada_vertice[i.second] > niveles[i.first]) ||
                (padre[i.first] == i.second && minimo_nivel_desde_cada_vertice[i.first] > niveles[i.second])) {
                haypuente = true;
            }
        }

        if (haypuente){
            pair<int , int > par = *it;
            if (par.first > par.second) {
                std::swap(par.first, par.second);
            }
            enlaces_importantes.push_back(par);
        }
    }

    sort(enlaces_importantes.begin(), enlaces_importantes.end(), compararPares);


}

int main() {
    int cantidad_tests;
    cin >> cantidad_tests;

    for (int i = 0; i < cantidad_tests; i++) {
        int n_bases, n_enlaces;
        cin >> n_bases;
        cin >> n_enlaces;

        vector<pair<int,int>> enlaces(n_enlaces);
        vector<vector<int>> vecinos(n_bases);

        for (int j = 0; j < n_enlaces; ++j) {
            pair<int, int> pair;
            cin >> pair.first;
            cin >> pair.second;

            vecinos[pair.first].push_back(pair.second);
            vecinos[pair.second].push_back(pair.first);

            enlaces[j] = pair;
        }

        vector<pair<int,int>> enlaces_importantes;

        malvinas(n_bases, enlaces , vecinos, enlaces_importantes);

        cout << enlaces_importantes.size()<< endl;
        for (auto & enlaces_importante : enlaces_importantes) {
            cout << enlaces_importante.first << " " << enlaces_importante.second<< endl;
        }


    }


    return 0;
}

/*
3
3 3
0 2
2 1
1 0
4 6
0 1
0 2
0 3
1 2
1 3
2 3
6 8
0 2
2 1
1 0
2 3
3 4
4 2
2 5
5 3


1
3 3
0 2
2 1
1 0

1
4 6
0 1
0 2
0 3
1 2
1 3
2 3

 */
