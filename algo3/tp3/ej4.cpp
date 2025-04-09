#include <vector>
#include <iostream>
#include <queue>
#include <climits>

#define INF INT_MAX

using namespace std;

// Copiado del campus
int bfs(int s, int t, vector<int>& parent, vector<vector<int>> capacity, vector<vector<int>> adj) {
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

// Copiado del campus
int maxflow(int s, int t, vector<vector<int>> capacity, vector<vector<int>> adj) {
    int flow = 0;
    vector<int> parent(capacity.size());
    int new_flow;

    while (new_flow = bfs(s, t, parent, capacity, adj)) {
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

// Dada una red con capacidades, la unidad de flujo ahora representa que un amigo está pasando con c herramientas. Como todos tienen que ir con la misma cantidad, basta con
// dividir la capacidad original por el peso de las herramientas para saber cuántos amigos pueden pasar.
vector<vector<int>> calcular_capacidades(vector<vector<int>> &capacity, int &c)
{
    vector<vector<int>> n_capacity(capacity.size(), vector<int> (capacity.size()));

    for (int i = 0; i < capacity.size(); i++)
    {
        for (int j = 0; j < capacity.size(); j++)
        {
            n_capacity[i][j] = capacity[i][j] / c;
        }

    }


    return n_capacity;

}

void maximizar_herramientas(int &amigos, vector<vector<int>> &adj, vector<vector<int>> &capacity, vector<int> &results, int &max_weight){
    int top = max_weight;
    int bottom = 0;
    int c, c_prev = -1;

    c = top;    // Pruebo con lo más alto posible primero.

    while (true)
    {
        if (c == 0 || c == c_prev) break;  // Maximo pueden llevar 0 herramientas, no hay solucion en la red.
        vector<vector<int>> n_capacity = calcular_capacidades(capacity, c);


        int res = maxflow(0, n_capacity.size() - 1, n_capacity, adj);   // Res dice cúantos amigos pueden pasar con las reglas impuestas y con una cantidad c de herramientas.


        if (res >= amigos)   // Podríamos aumentar la cantidad de herramientas.
        {
            bottom = c;
        }
        else
        {
            top = c;    // Probamos con demasiadas herramientas.
        }
        c_prev = c;
        c = bottom + (top - bottom) / 2;    // Binary search

    }
    results.push_back(c * amigos);
}

int main()
{
    int tests, vertices, aristas, amigos, weight, max_weight;

    cin >> tests;

    vector<int> results;

    int u, v;


    for (int i = 0; i < tests; i++)
    {
        cin >> vertices;
        cin >> aristas;
        cin >> amigos;

        vector<vector<int>> capacity(vertices, vector<int> (vertices, 0));
        vector<vector<int>> adj(vertices);
        max_weight = 0;

        for (int j = 0; j < aristas; j++)
        {
            cin >> u;
            cin >> v;
            cin >> weight;


            capacity[u - 1][v - 1] = weight;
            adj[u - 1].push_back(v-1);
        }

        for (int k = 0; k < adj[0].size(); ++k) {
            max_weight += capacity[0][adj[0][k]];   //  Cota para binary search.
        }

        maximizar_herramientas(amigos, adj, capacity, results, max_weight);

    }

    for (int i = 0; i < tests; i++)
    {
        cout << results[i] << endl;
    }


    return 0;
}