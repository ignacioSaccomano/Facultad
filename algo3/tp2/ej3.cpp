#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Cotas para los arrays, asi creamos solo 1 para todos los test cases.
int MAX_ARISTAS = 1.0e5;
int MAX_EDIFICIOS = 1.0e4;

// Modelo las conexiones con structs para que quede mas ordenado. Conexion representa las aristas como vienen en el enunciado.
struct conexion {
    int distancia, repetidores;
    pair<int, int> arista;
    conexion(){};   // Constructor vacio para que no chille el compilador al crear los vectores iniciales.

    conexion(pair<int, int> a, int d, int r){
        repetidores = r;
        arista = a;
        distancia = d;  
    }
};

// ratio_edge modela las aristas para los AGMax que armamos para encontrar el optimo. Su peso queda determinado por di - C * ri (donde 0 <= C <= 1.0e6)
struct ratio_edge {
    float weight;
    pair<int,int> edge;
    ratio_edge(){}; // Constructor vacio para que no chille el compilador al crear los vectores iniciales.

    ratio_edge(pair<int,int> e, float w){
        edge = e;
        weight = w;
    }
};
 
struct DSU {    // De lo subido al campus.
    DSU(int n){
        padre = vector<int>(n);
        for(int v = 0; v < n; v++) padre[v] = v;
        tamano = vector<int>(n,1);
    }
 
    int find(int v){
        while(padre[v] != v) v = padre[v];
        return v;
    }
 
    void unite(int u, int v){
        u = find(u); v = find(v);
        if(tamano[u] < tamano[v]) swap(u,v);
        //ahora u es al menos tan grande como v
        padre[v] = u;
        tamano[u] += tamano[v];
    }
 
    vector<int> padre;
    vector<int> tamano;
 
    //tamano[v] <= n
    //INV: si padre[v] != v entonces tamano[padre[v]] >= 2*tamano[v]
};


// Calcula la ecuacion di - C * ri para cada arista y la pone en un vector que representara las aristas para calcular el AGMax, pero ademas de los pesos quiero conservar la distancia y repetidores que usa la arista original para incluirlos en la suma en caso de que formen parte del arbol.
vector<pair<ratio_edge, pair<int,int>>> procesar_aristas(vector<conexion> &aristas, float c, int cant)
{
    vector<pair<ratio_edge, pair<int,int>>> ans(cant);
    for (int i = 0; i < cant; i++)
    {
        float ratio = aristas[i].distancia - c * aristas[i].repetidores;
        ans[i] = make_pair(ratio_edge(aristas[i].arista, ratio), make_pair(aristas[i].distancia, aristas[i].repetidores));
    }
    
    return ans;

}

pair<int,int> kruskal(vector<conexion>& E, int cant_edificios, float c, int cant_conexiones){   // Cambio kruskal para que devuelva D y R y cambio el sort para que encuentre AGMax en vez de AGMin
    pair<int, int> res = make_pair(0,0);
    
    vector<pair<ratio_edge, pair<int,int>>> nE = procesar_aristas(E, c, cant_conexiones);   // Quiero AGMax pero con los pesos determinados por di - C * ri.

    sort(nE.begin(),nE.end(), [](pair<ratio_edge, pair<int,int>> a, pair<ratio_edge, pair<int,int>> b){
        return a.first.weight > b.first.weight;     // Porque necesitamos AGMax. 
    });
    DSU dsu(cant_edificios + 1);
 
    int aristas = 0;
    int u, v;
    for(auto [r, e] : nE){
        u = r.edge.first;
        v = r.edge.second;

        //u y v estan en distinta cc?
        if(dsu.find(u) != dsu.find(v)){
            dsu.unite(u,v);
            res.first += e.first;    // Sumo distancia
            res.second += e.second;  // Sumo repetidores
            aristas++;
        }
        if(aristas == cant_edificios-1) break;
    }
    return res;
}

// Al ir armando el AGMax con el criterio de la ecuación mencionada, lo que ocurre es que se genera un árbol cuyo D/R es mayor a C si C no es el óptimo e igual si es el óptimo. Por lo tanto, en una cantidad finita de iteraciones (y de hecho bastante corta, porque no probamos con C que no funcionan como es el caso de la búsqueda binaria) llegamos a calcular un árbol generador máximo que repite las aristas del anterior, mostrando que es el óptimo y ahí detenemos el programa.

// Es decir, en vez de trabajar con un intervalo de C e ir probando de la mitad que corresponda (i.e: bajar el C si la desigualdad no se cumple o subirlo si es así para ver si hay otro mas grande que siga cumpliendo) lo que hicimos es valernos de que la ecuación no solo habla de si el C cumple, sino que por debajo lo que hace es (al calcular AGMax) encontrar pares d, r (es decir, aristas) que al calcular D/R mejoren el C o si no pueden (caso C óptimo), lo igualen.
pair<int,int> D_R(int conexiones, vector<conexion> &aristas, int edificios)
{
    pair<int,int> s;
    float x = 0;    // Arranco con C = 0
    float ratio = 0;    // Este es el ratio resultante del arbol generado con el criterio di - C * ri

    while (true)
    {
        s = kruskal(aristas, edificios, x, conexiones);
        ratio = (float) s.first / (float) s.second;
        if(x == ratio) break;   // Si ambos ratios coinciden es porque encontre los d y r que maximizan D/R.
        x = ratio;
    }

    return s;   // Devuelvo D/R

}
 

int main()
{
    int tests, cant_edificios, cant_conexiones;
    int max_edificios, max_aristas; // Sirve para saber hasta donde leer del array de edificios/aristas segun cada test case. Sino deberia crear un array nuevo por cada uno.

    vector<conexion> aristas (MAX_ARISTAS); 

    cin >> tests;
    vector<pair<int,int>> soluciones(tests);

    for (int i = 0; i < tests; i++)
    {
        cin >> cant_edificios;
        cin >> cant_conexiones;
        int repetidores, distancia, v, w;

        for (int j = 0; j < cant_conexiones; j++)
        {
            cin >> v;
            cin >> w;
            cin >> distancia;
            cin >> repetidores;

            aristas[j] = conexion(make_pair(v,w), distancia, repetidores);
        }
        soluciones[i] = D_R(cant_conexiones, aristas, cant_edificios);
    }

    for (int i = 0; i < tests; i++)
    {
        cout << soluciones[i].first << " " << soluciones[i].second << endl;
    }

    return 0;
}
