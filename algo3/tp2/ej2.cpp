#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int INF = INT_MAX;




int Ambulancia(const vector<vector<int>> &mani,vector<vector<bool>> visitado,vector<vector<int>> tiempo_en_llegar, const pair<int, int> &inicio, const pair<int, int> &fin, int t ){

    // Idea: correr un bfs alterado el cual no avanza esquinas en donde se va a quedar atascada la ambulancia.

    visitado[inicio.first][inicio.second] = true;
    tiempo_en_llegar[inicio.first][inicio.second] = t;

    queue<pair<int, int>> q;
    q.push(inicio);
    while (!q.empty()) {

        pair<int, int> v = q.front();
        q.pop();

        // ¿Cuando avanzo y cuando no? Se tienen que cumplir estos requisitos:
        // - Estar en rango
        // - No haber pasado antes
        // - No quedarme atascado

        // CASO ABAJO

        pair<int, int> aba = make_pair(v.first + 1 , v.second);

        if ( (aba.first  < mani.size()) && (aba.second  < mani[0].size()) && !visitado[aba.first][aba.second] && tiempo_en_llegar[v.first][v.second] < mani[aba.first][aba.second] - 1){

            visitado[aba.first][aba.second] = true;
            tiempo_en_llegar[aba.first][aba.second] = tiempo_en_llegar[v.first][v.second] + 1;
            q.push(aba);

        }


        // CASO ARRIBA

        pair<int, int> arr = make_pair(v.first - 1 , v.second);

        if ( (arr.first  < mani.size()) && (arr.second  < mani[0].size()) && !visitado[arr.first][arr.second] && tiempo_en_llegar[v.first][v.second] < mani[arr.first][arr.second] - 1){

            visitado[arr.first][arr.second] = true;
            tiempo_en_llegar[arr.first][arr.second] = tiempo_en_llegar[v.first][v.second] + 1;
            q.push(arr);

        }


        // CASO DERECHA

        pair<int, int> der = make_pair(v.first  , v.second + 1);

        if ( (der.first  < mani.size()) && (der.second  < mani[0].size()) && !visitado[der.first][der.second] && tiempo_en_llegar[v.first][v.second] < mani[der.first][der.second] - 1){

            visitado[der.first][der.second] = true;
            tiempo_en_llegar[der.first][der.second] = tiempo_en_llegar[v.first][v.second] + 1;
            q.push(der);

        }


        //CASO IZQUIERDA

        pair<int, int> izq = make_pair(v.first  , v.second - 1);

        if ( (izq.first  < mani.size()) && (izq.second  < mani[0].size()) && !visitado[izq.first][izq.second] && tiempo_en_llegar[v.first][v.second] < mani[izq.first][izq.second] - 1){

            visitado[izq.first][izq.second] = true;
            tiempo_en_llegar[izq.first][izq.second] = tiempo_en_llegar[v.first][v.second] + 1;
            q.push(izq);

        }
    }


    return tiempo_en_llegar[fin.first][fin.second];

}

int main() {
    int cantidad_tests;
    cin >> cantidad_tests;

    for (int i = 0; i < cantidad_tests; i++) {
        int filas, columnas;

        cin >> filas;
        cin >> columnas;

        int valor;

        vector<vector<int>> manifestaciones(filas,vector<int>(columnas));
        vector<vector<bool>> visitado(filas,vector<bool>(columnas,false));
        vector<vector<int>> tiempo(filas,vector<int>(columnas,-1));


        for (int j = 0; j < filas; ++j) {
            for (int k = 0; k < columnas; ++k) {

                cin >> valor;

                if (valor == 0){
                    valor = INF;
                }

                manifestaciones[j][k]= valor;
            }
        }

        pair<int, int> hospital;
        cin >> hospital.first;
        cin >> hospital.second;

        pair<int, int> paciente;
        cin >> paciente.first;
        cin >> paciente.second;

        int ida = Ambulancia(manifestaciones, visitado, tiempo, hospital, paciente, 0);

        if (ida == -1 ){
            cout << "IMPOSIBLE"<< endl;
        }else{
            int vuelta = Ambulancia(manifestaciones, visitado, tiempo, paciente, hospital , ida);
            if (vuelta == -1){
                cout << "IMPOSIBLE"<< endl;
            }else{
                cout << ida << " " << vuelta << endl;
            }
        }

    }

    return 0;
}

/*

3
2 3
0 3 0
0 0 0
0 0
0 2
1 3
0 1 0
0 2
0 0
1 3
0 1 0
0 2
0 2






*/
