#include <iostream>
#include <vector>

using namespace std;

int max_camino = -1;
int min_camino = -1;
bool es_posible= false;

vector< vector<bool> > camino(120, vector<bool>(120, false));
vector<string> tablero(120);
pair<int,int> tam;

pair<int, int> shift_calc(const char &c, pair<int,int> p){
    switch (c)
    {
    case 'u':
        p.first += 1;
        break;
    case 'l':
        p.second += 1;
        break;
    case 'd':
        p.first -= 1;
        break;
    case 'r':
        p.second -= 1;
        break;
    }

    return p;
}


void sendero(const char &ant, pair<int,int> pos, int lon_cam){

    if (pos.first >= tam.first || pos.second >= tam.second || pos.first < 0 || pos.second < 0 || camino[pos.first][pos.second] || tablero[pos.first][pos.second] == '#'){
        return;
    }
    
    if (pos == make_pair(tam.first-1 , tam.second -1) ){
        es_posible= true;
        if(lon_cam > max_camino){
            max_camino = lon_cam;
        }
        if ((lon_cam >= 0 && lon_cam < min_camino) || min_camino == -1){
            min_camino = lon_cam;
        }
        return;
    }



    camino[pos.first][pos.second] = true;
    lon_cam++;
    
    //  El + actua como un I y una L juntos. Esta variable sirve para no escribir las llamadas del + (porque son lo mismo que juntar las otras 2).
    if (tablero[pos.first][pos.second] == 'I' || tablero[pos.first][pos.second] == '+')
    {
        sendero(ant, shift_calc(ant, pos), lon_cam);
    }
    if (tablero[pos.first][pos.second] == 'L' || tablero[pos.first][pos.second] == '+')
    {
        if (ant == 'u' || ant == 'd'){
            sendero('l', make_pair(pos.first , pos.second + 1 ),lon_cam);
            sendero('r', make_pair(pos.first , pos.second - 1 ),lon_cam);
        }
        else{
            sendero('d', make_pair(pos.first - 1 , pos.second),lon_cam);
            sendero('u', make_pair(pos.first + 1 , pos.second),lon_cam);
        }
    }
    camino[pos.first][pos.second] = false;
}

int main() {

    int cantidad_tests;
    cin >> cantidad_tests;

    for (int i = 0; i < cantidad_tests; i++) {

        cin >> tam.first;
        cin >> tam.second;

        for (int a = 0; a < tam.first; ++a) {
                cin >> tablero[a];
        }

        sendero('u', make_pair(0,0), 0);

        // Se llama 2 veces a la funcion para no dejar afuera casos. Ej: Si la primera posiciones una I,
        // hay que tener en cuenta el camino hacia la derecha y el camino hacia abajo.

        sendero('l', make_pair(0,0), 0);

        // El resultado final se imprime dependiendo de variables globales que son modificadas durante la recursión.

        if (es_posible){
            cout << "POSIBLE "<< min_camino << " " << max_camino<< endl;
        }else{
            cout << "IMPOSIBLE" << endl;
        }

        // Al finalizar con una entrada, las variables globales vuelven a sus valores predeterminados.

        max_camino = -1;
        min_camino = -1;
        es_posible = false;
    }

    // Iteración sobre el array de las entradas para llamar a la función sendero

    return 0;
}