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
vector<vector<int>> grafo, aristasCiclos;

void printVector(vector<pair<int, int>> vector){
    for(int i = 0; i < vector.size(); ++i)
        cout << vector[i].first << " " << vector[i].second << endl;
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

void printPath(int s, int v) {
    if (v == s) {
        cout << s+1 << endl;
    } else {
        cout << v+1 << endl;
        
        verticesCiclos[v] = true;
        aristasCiclos[s][v] = 1;
        aristasCiclos[v][s] = 1;
        aristasCiclos[parent[v]][v] = 1;
        aristasCiclos[v][parent[v]] = 1;

        printGrafo(aristasCiclos);
        
        cout << "la arista" << parent[v]+1 << " " << v +1<< "esta en un ciclo" << endl;
        if (!verticesCiclos[parent[v]]) {
            printPath(s, parent[v]);
        }
    
    }
}

void print(vector<int> vector){
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
                cout << father +1 << " " << children[i] +1 << " es un backedge" << endl;
                printPath(children[i], father);
            }
        }
    }
    k += 1;
    finish[father] = k;
    color[father] == NEGRO;
}

void DFS(){
    k = 0;
    start = vector<int>(r, UNDEFINED);
    finish = vector<int>(r, UNDEFINED);
    parent = vector<int>(r, UNDEFINED);
    backedges = vector<int>(r, NONE);
    color = vector<int>(r, BLANCO);
    aristasCiclos = vector<vector<int>>(r,vector<int>(r));
    verticesCiclos = vector<int>(r,false);

    for(int node = 0; node < r; node++){
        if(color[node] == BLANCO){
            DFS_visit(node);
        }
    }
}

bool recorridoBueno(pair<int, int> query){
    return 0;
}

int main(int argc, char const *argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ifstream entrada;
    entrada.open(argv[1]);
    //time_t init_time;
    //time(&init_time);

    vector<pair<int, int>> querys;

    entrada >> r >> c >> q;
    grafo = vector<vector<int>>(r,vector<int>(0));
    
    //creo el grafo
    for(int i = 0; i < c; ++i){
        int f, s;
        entrada >> f >> s; 
        grafo[f-1].push_back(s-1); 
        grafo[s-1].push_back(f-1); 
    }
    
    //creo las querys
    for (int i = 0; i < q; i++){
        int f, s;
        entrada >> f >> s;
        querys.push_back(make_pair(f-1, s-1));
    }

    DFS();
    //cout << recorridoBueno(make_pair<int, int>(1,5)) << endl;
    //print(start);
    cout << endl;
    //print(finish);
    cout << endl;
    //print(parent);
    cout << endl;


    //printGrafo(grafo);
    cout << endl;
    //printVector(querys);
    return 0;
}
    //time_t end_time;
    //time(&end_time);
    //cout << "tiempo: " << difftime(end_time, init_time) << endl;
