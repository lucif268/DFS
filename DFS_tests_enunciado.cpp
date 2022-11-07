#include<iostream>
#include<vector>
#include<math.h>
#include<fstream>

using namespace std;

#define NONE 0
#define UNDEFINED -1
#define BLANCO  1
#define GRIS    2
#define NEGRO   3

int r, c, q, k;
vector<int> start, finish, parent, color, backedges, verticesCiclos;
vector<vector<int>> grafo, grafo_desconexo, aristasCiclos;
vector<bool> puenteConParent;

void printVector(vector<int> vector){
    for(int i = 0; i < vector.size(); ++i)
        cout << vector[i] << endl;
}

void printGrafo(vector<vector<int>> grafo){
    for(int i = 0; i < grafo.size(); ++i){
        cout << i+1 << ": {";
        for(int j = 0; j < grafo[i].size(); ++j){
            cout << grafo[i][j] +1 << " ";
        }
        cout << "}" << endl;
    }
}

int deteccionDePuentes(int padre){
    int cantidad = 0;
    for(int u = 0; u < grafo[padre].size(); u++){
        if(parent[grafo[padre][u]] == padre){
            cantidad += deteccionDePuentes(grafo[padre][u]);
        } else{
            if(start[padre] > start[grafo[padre][u]] & parent[padre] != grafo[padre][u])
                cantidad++;
            if(start[padre] < start[grafo[padre][u]])
                cantidad--;
        }
    }
    if(parent[padre] != UNDEFINED & cantidad == 0){
        puenteConParent[padre] = true;
    }
    return cantidad;
}

vector<vector<int>> modificar_grafo(){
    //dadas las aristas puentes,  se agrupen las componentes conexas.
    //buscamos componentes conexas O(v)
    grafo_desconexo = vector<vector<int>>(r,vector<int>(0));

    for(int i = 0; i < parent.size(); ++i){
        if(puenteConParent[i] == false & parent[i] != UNDEFINED){
            //grafo_desconexo[i].push_back(i);
        } else if (puenteConParent[i] == true){
            //hay un puente entre i y parent[i]
            grafo_desconexo[parent[i]].push_back(i);
        }
    }
    return grafo_desconexo;
}

void printPath(int s, int v) {
    if (v == s) {
        //cout << s+1 << endl;
    } else {
        //cout << v+1 << endl;

        verticesCiclos[v] = true;
        aristasCiclos[s][v] = 1;
        aristasCiclos[v][s] = 1;
        aristasCiclos[parent[v]][v] = 1;
        aristasCiclos[v][parent[v]] = 1;

        //printGrafo(aristasCiclos);

        //cout << "la arista" << parent[v]+1 << " " << v +1<< "esta en un ciclo" << endl;
        if (!verticesCiclos[parent[v]]) {
            printPath(s, parent[v]);
        }

    }
}

void print(vector<bool> vector){
    for(int i = 0; i < vector.size(); ++i)
        cout << vector[i] << endl;
}


void DFS_visit(int father){
    k += 1;
    start[father] = k;
    vector<int> children = grafo[father];

    color[father] = GRIS;
    for(int i = 0; i < children.size(); ++i){
        if(color[children[i]] == BLANCO){
            parent[children[i]] = father;
            DFS_visit(children[i]);
        } else {
            if (!(parent[father] == children[i]) && start[father] > start[children[i]]) {
                //cout << father +1 << " " << children[i] +1 << " es un backedge" << endl;
                printPath(children[i], father);
            }
        }
    }
    k += 1;
    finish[father] = k;
    color[father] == NEGRO;
}

void DFS(vector<vector<int>> grafo){
    k = 0;
    start = vector<int>(r, UNDEFINED);
    finish = vector<int>(r, UNDEFINED);
    parent = vector<int>(r, UNDEFINED);
    backedges = vector<int>(r, NONE);
    color = vector<int>(r, BLANCO);
    aristasCiclos = vector<vector<int>>(r,vector<int>(r));
    verticesCiclos = vector<int>(r,false);
    puenteConParent = vector<bool>(r,false);

    for(int node = 0; node < r; node++){
        if(color[node] == BLANCO){
            DFS_visit(node);
        }
    }
}

bool recorridoBueno(pair<int, int> query){
    return 0;
}

void responderQuery(int s, int t) {
    if ((finish[s] < start[t]) || (finish[t] < start[s])) {
        cout << "N" << endl;
    } else {
        cout << "Y" << endl;
    }
}

int main(int argc, char const *argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);

    ifstream entrada;
    entrada.open(argv[1]);

    //time_t init_time;
    //time(&init_time);

    while(true) {
        entrada >> r >> c >> q;

        if (r == 0 && c == 0 && q == 0) {
            break;
        }

        grafo = vector<vector<int>>(r,vector<int>(0));

        //creo el grafo
        for(int i = 0; i < c; ++i){
            int f, s;
            entrada >> f >> s;
            grafo[f-1].push_back(s-1);
            grafo[s-1].push_back(f-1);
        }

        //creo las querys

        vector<pair<int, int>> querys;

        for (int i = 0; i < q; i++){
            int f, s;
            entrada >> f >> s;
            querys.push_back(make_pair(f-1, s-1));
        }



        DFS(grafo);
        deteccionDePuentes(0);
        //print(puenteConParent);
        vector<vector<int>> nuevo_grafo = modificar_grafo();
        grafo = nuevo_grafo;
        //printGrafo(grafo_desconexo);
        DFS(nuevo_grafo);

        //printVector(parent);

        // Respondo las queries
        for (int i=0; i<querys.size(); i++) {
            responderQuery(querys[i].first, querys[i].second);
        }

        cout << "-" << endl;
    }



}
//time_t end_time;
//time(&end_time);
//cout << "tiempo: " << difftime(end_time, init_time) << endl;
