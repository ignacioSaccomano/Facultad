#include <vector>
#include <iostream>
#include <list>
#include <bits/stdc++.h>    // Para list
#define INF INT_MAX

using namespace std;

const unsigned int MAX_M = 1.0e7;
const unsigned int MAX_N = 1.0e6;

struct arista {
    int weight;
    unsigned int dest;

    arista(){};
    arista(unsigned int d, int w) : weight(w), dest(d){};
};

/*
    El modelo se separa en capas. A todos los vértices le creamos 4 copias representando que se usaron i pasadizos (0 <= i <= 3).
    Entonces, las copias mantienen las aristas adyacentes (i.e: las que conectan una habitación con la siguiente) con peso 1 y si existe un pasadizo
    entre v y u conectamos v_i y u_(i+1) <=> 0 <= i <= 2 con peso 2 respectivamente. Así queda representado que solo queremos usar 3 pasadizos,
    y que en caso de ya haberlos usado no queremos que se tengan en cuenta los que siguen (porque no van a ser accesibles desde u_3).
*/

void armar_modelo(unsigned int N, unsigned int M, vector<pair<unsigned int,unsigned int>> &aristas, vector<list<arista>> &lista_adyacencias)
{
    for (unsigned int i = 0; i < N - 1; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            lista_adyacencias[i + (j * N)].push_back(arista(i + 1 + (j * N) ,1));    // Uno a todos con el vértice que les sigue con peso 1.
        }
    }

    unsigned int u, v;

    for (unsigned int i = 0; i < M; i++)
    {
        u = aristas[i].first - 1;
        v = aristas[i].second - 1;

        for (int j = 0; j < 3; j++)
        {
            lista_adyacencias[u + (j * N)].push_back(arista(v + ((j + 1) * N), 2));
        }
    }

    for (int i = 0; i < 4; i++)
    {
        lista_adyacencias[(N-1) + (i * N)].push_back(arista(4 * N, 0)); // Nodo con peso 0 unido a todas las habitaciones finales para hacer camino mínimo solo 1 vez.
    }
}


/*
    Como es un DAG (solo podés ir hacia adelante) podemos implementar el algoritmo de camino mínimo en DAGs que es lineal [O(n + m)].
    Para la implementacion usamos la publicada en: https://www.geeksforgeeks.org/shortest-path-for-directed-acyclic-graphs/
*/

void topologicalSortUtil(int v, bool visited[], stack<int> &Stack, vector<list<arista>> &adj)
{
    // Mark the current node as visited
    visited[v] = true;
 
    // Recur for all the vertices adjacent to this vertex
    list<arista>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        arista node = *i;
        if (!visited[node.dest])
            topologicalSortUtil(node.dest, visited, Stack, adj);
    }
 
    // Push current vertex to stack which stores topological sort
    Stack.push(v);
}

int shortestPath(unsigned int s, unsigned int n, vector<list<arista>> &adj)
{
    n += 1;
    stack<int> Stack;
    int dist[n];
 
    // Mark all the vertices as not visited
    bool *visited = new bool[n];
    for (int i = 0; i < n; i++)
        visited[i] = false;
 
    // Call the recursive helper function to store
    // Topological Sort starting from all vertices 
    // one by one
    for (int i = 0; i < n; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack, adj);
 
    // Initialize distances to all vertices as
    // infinite and distance to source as 0
    for (int i = 0; i < n; i++)
        dist[i] = INF;
    dist[s] = 0;
 
    // Process vertices in topological order
    while (Stack.empty() == false)
    {
        // Get the next vertex from topological order
        int u = Stack.top();
        Stack.pop();
 
        // Update distances of all adjacent vertices
        list<arista>::iterator i;
        if (dist[u] != INF)
        {
          for (i = adj[u].begin(); i != adj[u].end(); ++i)
             if (dist[i->dest] > dist[u] + i->weight)
                dist[i->dest] = dist[u] + i->weight;
        }
    }

    return dist[n - 1];
}


int main(){
    unsigned int N, M, tests;

    vector<pair<unsigned int,unsigned int>> passages(MAX_M);

    unsigned int u, v;

    cin >> tests;

    vector<int> respuestas(tests);


    for (int i = 0; i < tests; i++)
    {
        cin >> N;
        cin >> M;

        for (int j = 0; j < M; j++)
        {
            cin >> u;
            cin >> v;

            passages[j] = make_pair(u,v);    // True quiere decir que hay pasadizo
        }

        vector<list<arista>> lista_adyacencias((4 * N) + 1);

        armar_modelo(N, M, passages, lista_adyacencias);
        
        // Calculamos camino mínimo desde el nodo 0
        respuestas[i] = shortestPath(0, 4*N, lista_adyacencias);
    }

    for (int i = 0; i < tests; i++)
    {
        cout << respuestas[i] + 1 << endl;
    }
    
    


    return 0;
}