#include <iostream>
#include <vector>
#include <string>

using namespace std;

string respuesta;


bool saldos_sospechosos(const int& cant,const int& final, const vector<int>& saldos, const vector<int>& suma_total_hasta_pos,
                        int& pos, int& acumulador_saldo , string& signos,vector<vector<string>>& memo) {

    //Caso base: llegamos al final de los saldos. Se actualizan la variable global "resultado"
    
    if (pos >= cant) {

        if (acumulador_saldo == final) {

            if (respuesta.empty()) {
                respuesta = signos;
                return true;

            } else {

                for (int i = 0; i < cant; ++i) {
                    if (signos[i] != respuesta[i]) {
                        respuesta[i] = '?';
                    }
                }
                return true;
            }

        } else {
            return false;
        }
    }

    // Si en la matriz me encuentro con una f, significa que ese estado recursivo ya lo calcule, pero no existe combinación
    // posterior posible que me haga llegar a una solución válida.

    // Si en la matriz me encuentro con una combinación(i.e. no está vacía y no tiene una f), significa que ese estado 
    // recursivo ya lo calcule y existe una combinación
    // posterior posible que me hace llegar a una solución válida, por lo que se guarda en la matriz. Esa combinación es
    // una "union" entre todas las combinaciones que me hacen llegar al saldo final.

    // Si en la matriz me encuentro con un string vacio, significa que nunca calcule ese estado recursivo.

    if (memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100] == "f"){
        return false;
    }else {
        if (!memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100].empty()) {
            int a = pos;
            int b = acumulador_saldo;
            string c = signos;

            signos = signos + memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100];
            pos = cant;
            acumulador_saldo = final;

            bool cuco = saldos_sospechosos(cant, final, saldos, suma_total_hasta_pos, pos, acumulador_saldo, signos,
                                           memo);

            pos = a;
            acumulador_saldo = b;
            signos = c;

            return cuco;
        }
    }

    //Poda saldos restantes: si sumo todo lo que puedo sumar y no llego al final o resto todo lo que puedo restar y no 
    // llego al final, entonces nunca llegare a una solución valida.
    
    if (acumulador_saldo + suma_total_hasta_pos[pos] < final || acumulador_saldo - suma_total_hasta_pos[pos] > final) {
        return false;
    }

    // Se procede en la recursión calculando el caso donde el saldo en la respectiva pos lo sumo y lo resto. Tengo que modificar
    //las variables, ya que se pasan por referencia.
    
    signos.push_back('+');
    acumulador_saldo = acumulador_saldo + saldos[pos];
    pos++;
    bool suma = saldos_sospechosos(cant, final, saldos, suma_total_hasta_pos, pos, acumulador_saldo, signos, memo);

    signos[pos - 1] = '-';
    acumulador_saldo = acumulador_saldo - (2 * saldos[pos - 1]);
    bool resta = saldos_sospechosos(cant, final, saldos, suma_total_hasta_pos, pos, acumulador_saldo, signos, memo);

    // Se devuelven las variables a su estado original.
    signos.pop_back();
    acumulador_saldo = acumulador_saldo + saldos[pos - 1];
    pos--;
    
    // Una vez calculadas ambas posibilidades, queremos saber si en alguna de ellas se llegó a una solución válida. Si 
    // es el caso, guardamos la respectiva combinación en la matriz con su respectivo prefijo (este será un '+', un '-' o un '?',
    // dependiendo de que caminos llegaron a una solución).
    
    if (suma && resta) {
        
        //  Si poniendo un + y un - llegamos a una solución, queremos guardar en la matriz la combinación de ambos caminos. 
        string esa = memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] + saldos[pos]) / 100];
        for (int i = 0; i < memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] + saldos[pos]) / 100].size(); ++i) {
            if (memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] + saldos[pos]) / 100][i] != memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] - saldos[pos]) / 100][i]) {
                esa[i] = '?';
            }
        }
        memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100] =  '?' + esa;
    }else{
        if(suma || resta) {
            if (suma) {
                memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100] =
                        '+' + memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] + saldos[pos]) / 100];
            }
            if (resta) {
                memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100] =
                        '-' + memo[pos + 1][(acumulador_saldo + suma_total_hasta_pos[0] - saldos[pos]) / 100];
            }
        }else{
            memo[pos][(acumulador_saldo + suma_total_hasta_pos[0]) / 100] = "f";
        }
    }
    
    //Devolvemos si alguno de los caminos llego a una solución.

    return suma || resta;
}

int main() {

    int cantidad_entradas;
    cin >> cantidad_entradas;

    for (int i = 0; i < cantidad_entradas; ++i) {
        int cant;
        int final;
        cin >> cant;
        cin >> final;

        int suma_saldos = 0;
        vector<int> saldos(cant);
        vector<int> suma_total_hasta_pos(cant);

        for (int a = 0; a < cant; ++a) {
            cin >> saldos[a];
            suma_saldos = suma_saldos + saldos[a];
        }

        // Vamos a inicializar una matriz de tamaño cant x posibles_acum_memoria_dina. Esto nos va a servir por si en
        // algún caso de la recursion nos topamos con una misma pos y un mismo valor del acumulador de saldo. Se inicializa
        // con strings vacíos, lo que indica que ningún caso recursivo ha sido guardado. Siempre que se llegue a una
        // solución válida (ya sea poniendo + o -), se guardara en la matriz la combinación que nos hizo llegar al resultado.
        // En el caso que tanto poniendo un + o un - se llega a una solución válida, se guarda la combinación de ambas combinaciones.

        // posibles_acum_memoria_dina (tamaño de la matriz) tiene este valor, puesto que sabiendo el máximo valor que la suma de todos los saldos
        // ya sabemos todas los posibles valores del acumulador. Como máximo es la suma de todos los saldos, y como mínimo
        // la resta de todos ellos. Tener en cuenta que los saldos son múltiplos de 100, lo cual acorta el tamaño.



        int posibles_acum_memoria_dina= (suma_saldos/100) * 2;

        vector<vector<string>> memo_dinamica(cant + 1, vector<string>(posibles_acum_memoria_dina + 1));


        //Creo el vector con la suma de los saldos de atrás para adelante.
        for (int b = 0; b < cant; ++b) {
            suma_total_hasta_pos[b] = suma_saldos;
            suma_saldos= suma_saldos - saldos[b];
        }
        string signos = "";
        int pos = 0;
        int acumulador = 0;
        
        // La función saldos_sospechosos devuelve un bool, indicando que se encontró una solución válida. Esto nos servirá 
        // para la programación dinámica.
        
        saldos_sospechosos(cant, final, saldos , suma_total_hasta_pos, pos,  acumulador, signos, memo_dinamica);
        cout << respuesta << endl;
        respuesta= "";
    }

    return 0;
}

/*4
6 300
100 500 500 200 100 100
2 -200
500 700
4 400
500 700 700 100
18 100
0 0 0 0 200 0 0 0 0 0 0 0 100 0 0 0 0 0

1
2 0
0 0

1
4 400
500 700 700 100

1
8 300
1200 1200 500 500 400  1100 1100 100



*/
