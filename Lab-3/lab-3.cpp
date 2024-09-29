/*
ZADATAK:
Matricom susjedstva zadan je jednostavan graf 𝐺 s 𝑛 vrhova, 𝑛 ≤ 15. 
Potrebno je provjeriti je li dani graf planaran i, ako nije planaran, ispisati sadrži li podgraf homeomorfan s 𝐾_5 ili 𝐾_3,3.

ULAZ:
U izvršnoj datoteci programa treba biti ispisan upit za unošenje imena datoteke koja sadrži informacije o grafu. 
Datoteka se treba nalaziti u mapi zajedno s datotekom programa.

Primjer ulaza programa:
Unesite ime datoteke: graf.txt
(graf.txt datoteka treba sadržavati matricu susjedstva grafa)

IZLAZ:
Program ispisuje rezultat provjere. Ako graf nije planaran, navodi koji podgraf (K_5 ili K_3,3) je prisutan.
Primjer izlaza programa:
Graf nije planaran, ima podgraf homeomorfan s K_3,3.

Program mora biti napisan u jednom izvornom kodu (.cpp) bez dodatnih datoteka.
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Zastavica za isomorfizam
bool isomorphic = false;

// Funkcija za provjeru jesu li dva grafa izomorfna
bool checkIsomorphism(const vector<vector<int>>& graph1, const vector<vector<int>>& graph2, const vector<int>& permutation) {
    int n = graph1.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph1[i][j] != graph2[permutation[i]][permutation[j]]) {
                return false;
            }
        }
    }
    return true;
}

// Pomoćna rekurzivna funkcija za generiranje permutacija
bool isIsomorphicUtil(const vector<vector<int>>& graph1, const vector<vector<int>>& graph2, vector<int>& permutation, vector<bool>& used, int pos) {
    int n = graph1.size();
    if (pos == n) {
        return checkIsomorphism(graph1, graph2, permutation);
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            used[i] = true;
            permutation[pos] = i;
            if (isIsomorphicUtil(graph1, graph2, permutation, used, pos + 1)) {
                return true;
            }
            used[i] = false;
        }
    }
    return false;
}

// Glavna funkcija za provjeru izomorfizma
bool areIsomorphic(const vector<vector<int>>& graph1, const vector<vector<int>>& graph2) {
    if (graph1.size() != graph2.size()) return false;

    int n = graph1.size();
    vector<int> permutation(n);
    vector<bool> used(n, false);

    return isIsomorphicUtil(graph1, graph2, permutation, used, 0);
}

// Funkcija za stvaranje prilagođene matrice grafa
vector<vector<int>> createAdjustedMatrix(const vector<vector<int>>& originalMatrix) {
    int n = originalMatrix.size();
    vector<bool> isConnected(n, false);

    // Provjeravanje jesu li vrhovi povezani
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (originalMatrix[i][j] == 1) {
                isConnected[i] = true;
                isConnected[j] = true;
            }
        }
    }

    // Brojanje povezanih vrhova
    int connectedCount = 0;
    for (int i = 0; i < n; ++i) {
        if (isConnected[i]) {
            connectedCount++;
        }
    }

    vector<vector<int>> adjustedMatrix(connectedCount, vector<int>(connectedCount, 0));
    int row = 0;

    // Popunjavanje adjusted matrice
    for (int i = 0; i < n; ++i) {
        if (isConnected[i]) {
            int col = 0;
            for (int j = 0; j < n; ++j) {
                if (isConnected[j]) {
                    adjustedMatrix[row][col] = originalMatrix[i][j];
                    col++;
                }
            }
            row++;
        }
    }
    return adjustedMatrix;
}

// Funkcija za ispis grafa
void printGraph(const vector<vector<int>>& graph) {
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

// Funkcija za "uljepšavanje" grafa
void beautifyGraph(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<bool> removed(n, false);

    bool updated;
    do {
        updated = false;
        for (int i = 0; i < n; ++i) {
            if (!removed[i]) {
                int degree = 0;
                int neighbor1 = -1, neighbor2 = -1;
                for (int j = 0; j < n; ++j) {
                    if (graph[i][j] == 1 && !removed[j]) {
                        degree++;
                        if (neighbor1 == -1) neighbor1 = j;
                        else neighbor2 = j;
                    }
                }

                if (degree == 2) {
                    graph[i][neighbor1] = 0;
                    graph[i][neighbor2] = 0;
                    graph[neighbor1][i] = 0;
                    graph[neighbor2][i] = 0;
                    graph[neighbor1][neighbor2] = 1;
                    graph[neighbor2][neighbor1] = 1;
                    removed[i] = true;
                    updated = true;
                }
            }
        }
    } while (updated);
}

// Funkcija za generiranje i uljepšavanje podgrafova
void generateAndBeautifySubgraphs(const vector<vector<int>>& originalGraph, const vector<vector<int>>& K_5, const vector<vector<int>>& K_33) {
    int n = originalGraph.size();
    vector<pair<int, int>> edges;
    
    // Brojanje bridova u originalnom grafu
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (originalGraph[i][j]) {
                edges.emplace_back(i, j);
            }
        }
    }

    int totalSubsets = 1 << edges.size(); // 2^broj_bridova mogućih podgrafova

    for (int subset = 0; subset < totalSubsets; ++subset) {
        vector<vector<int>> subgraph(n, vector<int>(n, 0));

        // Kreiranje podgrafa na temelju podskupa bridova
        for (int bit = 0; bit < edges.size(); ++bit) {
            if (subset & (1 << bit)) {
                int u = edges[bit].first;
                int v = edges[bit].second;
                subgraph[u][v] = 1;
                subgraph[v][u] = 1;
            }
        }

        // Uljepšavanje podgrafa
        beautifyGraph(subgraph);

        vector<vector<int>> adjustedSubgraph = createAdjustedMatrix(subgraph);

        if (adjustedSubgraph.size() == 5 && areIsomorphic(adjustedSubgraph, K_5)) {
            isomorphic = true;
            cout << "Graf nije planaran, ima podgraf homeomorfan s K_5" << endl;
            break;
        }
        if (adjustedSubgraph.size() == 6 && areIsomorphic(adjustedSubgraph, K_33)) {
            isomorphic = true;
            cout << "Graf nije planaran, ima podgraf homeomorfan s K_3,3" << endl;
            break;
        }
    }
}

int main() {
    string filename;
    cout << "Unesite ime datoteke: ";
    cin >> filename;

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Ne mogu otvoriti datoteku: " << filename << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    vector<vector<int>> Graph(n, vector<int>(n, 0));
    vector<vector<int>> K_5(5, vector<int>(5, 0));
    vector<vector<int>> K_33(6, vector<int>(6, 0));

    // Učitavanje grafa iz datoteke
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inputFile >> Graph[i][j];
        }
    }
    inputFile.close();

    // Učitavanje grafa K_5
    ifstream inputFile2("K_5.txt");
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            inputFile2 >> K_5[i][j];
        }
    }
    inputFile2.close();

    // Učitavanje grafa K_33
    ifstream inputFile3("K_33.txt");
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            inputFile3 >> K_33[i][j];
        }
    }
    inputFile3.close();

    // Ispis matrice susjedstva
    cout << "Matrica susjedstva:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << Graph[i][j] << " ";
        }
        cout << endl;
    }

    generateAndBeautifySubgraphs(Graph, K_5, K_33);

    if (!isomorphic) cout << "Graf je planaran!" << endl;

    return 0;
}
