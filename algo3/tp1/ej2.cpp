#include <iostream>
#include <vector>

using namespace std;

//Devuelve el indice del minimo elemento.
int min_indice(vector<float> v){
    float minimo = v[0];
    int indiceMinimo = 0;

    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] < minimo) {
            minimo = v[i];
            indiceMinimo = i;
        }
    }

    return indiceMinimo;
}

long correciones_pendientes( vector<float>&  tiempo, vector<float>&  descontento, vector<float>& relacion){

    // Estamos tratando con valores muy altos, por lo que decidimos usar lon long.
    // También, hay que tener cuidado al cruzar valores de tipo long long con floats. Para realizar la conversion de
    // manera segura usasmos static_cast.
    
    long long acumulador_descontento = 0;
    long long acumulador_tiempo = 0;

    while (!relacion.empty()){
        int indice = min_indice(relacion);
        acumulador_tiempo = acumulador_tiempo + static_cast<long long>(tiempo[indice]);
        acumulador_descontento = acumulador_descontento + acumulador_tiempo * static_cast<long long>(descontento[indice]);

        relacion.erase(relacion.begin() + indice);
        descontento.erase(descontento.begin() + indice);
        tiempo.erase(tiempo.begin() + indice);
    }
    // LL = long long 
    acumulador_descontento =  acumulador_descontento % (1000000000LL + 7LL);
    return acumulador_descontento;
}


int main(){

    int cantidad_tests;
    cin >> cantidad_tests;

    for (int i = 0; i < cantidad_tests; i++) {

        int tam;
        cin >> tam;

        // Vector con los tiempos que se tarda en corregir cada parcial.
        vector<float> tiempo(tam);

        // Vector con los coeficientes de descontento de cada parcial. Para obtener el descontento, hay que
        // multiplicar el coeficiente y el tiempo que se tardó hasta terminar de corregir el parcial (tiempos anteriores también)
        vector<float> descontento(tam);

        // Vector con la relacion entre el tiempo y el descontento. Para realizar la division, necesitamos que los datos
        // tanto resultados como dividendo y divisor sean de tipo float.
        vector<float> relacion(tam);

        for (int a = 0; a < tam; ++a) {
            cin >> tiempo[a];
        }
        for (int a = 0; a < tam; ++a) {
            cin >> descontento[a];
        }
        for (int a = 0; a < tam; ++a) {
            relacion[a] = tiempo[a] / descontento[a];
        }

        long long x = correciones_pendientes(tiempo,descontento,relacion);
        cout<<  x << endl ;

    }
    return 0;
}

/*
1
10
100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000
100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000 100000000
 */