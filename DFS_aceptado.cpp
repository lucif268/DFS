#include <iostream>
#include <vector>
#include <tuple>
#include<fstream>

using namespace std;
using arista = tuple<int,int>;

class Grafo {
public:
    Grafo(int nodos, vector<arista> aristas);
    int nodos;
    vector<arista> aristas;
    vector<vector<int>> ady;
};

Grafo::Grafo(int nodos, vector<arista> aristas): nodos(nodos), aristas(aristas) {
    ady = vector<vector<int>>(nodos+1);

    for (int i=0; i<aristas.size(); i++) {
        int nodo1 = get<0>(aristas[i]);
        int nodo2 = get<1>(aristas[i]);
        ady[nodo1].push_back(nodo2);
        ady[nodo2].push_back(nodo1);
    }
}

class DFS {
public:
    DFS(Grafo grafo);
    Grafo grafo;
    vector<int> parent;
    vector<int> discovery;
    vector<int> finish;
    vector<arista> puentes;
    bool hayCamino(int v1, int v2);

private:
    int time_;
    vector<int> color_;
    vector<int> raicesDeArboles_;
    vector<int> raizDelArbolAlQuePertenece_;
    int ultimaRaiz_;
    void dfsVisit(int u);
    int detectarPuentes(int source);
};

DFS::DFS(Grafo grafo): grafo(grafo) {
    time_ = 0;
    color_ = vector<int>(grafo.nodos+1, 1);
    parent = vector<int>(grafo.nodos+1, NULL);
    discovery = vector<int>(grafo.nodos+1);
    finish = vector<int>(grafo.nodos+1);
    raizDelArbolAlQuePertenece_ = vector<int>(grafo.nodos+1);

    for (int u=1; u<=grafo.nodos; u++) {
        if (color_[u] == 1) {
            ultimaRaiz_ = u;
            raicesDeArboles_.push_back(u);
            raizDelArbolAlQuePertenece_[u] = ultimaRaiz_;
            dfsVisit(u);
        }
    }

    for (int i=0; i<raicesDeArboles_.size(); i++) {
        int source = raicesDeArboles_[i];
        detectarPuentes(source);
    }
}

void DFS::dfsVisit(int u) {
    time_ = time_ + 1;
    discovery[u] = time_;
    color_[u] = 2;
    raizDelArbolAlQuePertenece_[u] = ultimaRaiz_;
    int v;
    for (int i=0; i<grafo.ady[u].size(); i++) {
        v = grafo.ady[u][i];
        if (color_[v] == 1) {
            parent[v] = u;
            dfsVisit(v);
        }
    }
    color_[u] = 3;
    time_ = time_ + 1;
    finish[u] = time_;
}

int DFS::detectarPuentes(int source) {
    int cantidad = 0;
    for (int i=0; i<grafo.ady[source].size(); i++) {
        int u = grafo.ady[source][i];
        if (parent[u] == source) {
            cantidad += detectarPuentes(u);
        } else {
            if ((discovery[source] > discovery[u]) && (parent[source] != u)) {
                cantidad++;
            }
            if (discovery[source] < discovery[u]) {
                cantidad--;
            }
        }
    }
    if ((cantidad == 0) && (parent[source] != source) && (parent[source] != NULL)) {
        arista puente = make_pair(parent[source], source);
        puentes.push_back(puente);
    }
    return cantidad;
}

bool DFS::hayCamino(int v1, int v2) {
    if (raizDelArbolAlQuePertenece_[v1] == raizDelArbolAlQuePertenece_[v2]) {
        return true;
    } else {
        return false;
    }
}

int main(int argc, char const *argv[]) {


    /*
    ifstream entrada;
    entrada.open(argv[1]);
     */

    while (true) {
        int r,c,q;

        cin >> r >> c >> q;

        if (r == 0 && c == 0 && q == 0) {
            break;
        }

        vector<arista> aristas;

        for (int i=0; i<c; i++) {
            int a, b;
            cin >> a >> b;
            aristas.push_back(make_pair(a,b));
        }

        Grafo g1 = Grafo(r, aristas);
        DFS dfs1 = DFS(g1);

        Grafo g2 = Grafo(r, dfs1.puentes);
        DFS dfs2 = DFS(g2);

        int v1, v2;

        for (int i=0; i<q; i++) {
            cin >> v1 >> v2;
            if (dfs2.hayCamino(v1,v2)) {
                cout << "Y" << endl;
            } else {
                cout << "N" << endl;
            }
        }

        cout << "-" << endl;
    }



    return 0;
}
