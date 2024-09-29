/*
ZADATAK:
Zadan je težinski graf G sa skupom vrhova 𝑉 = {1,...,𝑛} i funkcijom 𝑓:𝑉 × 𝑉 → 𝐍, 
𝑓(𝑖,𝑗) = 𝑎|𝑖−𝑗|%𝑐 + 𝑏(𝑖 − 𝑗)^2 − 1 koja svakom paru vrhova pridjeljuje težinu brida između njih (ovdje % predstavlja modulo operaciju).

Potrebno je za proizvoljan par vrhova 𝑘,𝑙 ∊ 𝑉 pomoću Dijkstrinog algoritma pronaći udaljenost između ta dva vrha.

NAPOMENA:
Očekuje se da će program za ulaze koji zadovoljavaju 𝑛 ≤ 3500 izvršiti zadatak za najviše 10 sekundi.
Parametri 𝑎, 𝑏, 𝑐 su prirodni brojevi ne veći od 1 000 000 takvi da funkcija 𝑓 poprima vrijednosti do najviše 1 000 000 000.

ULAZ:
U izvršnoj datoteci programa trebaju biti ispisani upiti za unošenje svakog od parametara navedenih u zadatku. Svaki se upit javlja u novoj liniji nakon unošenja prethodnog parametra.

Primjer ulaza programa:
Unesite prirodan broj n: 7
Unesite prirodan broj a: 8
Unesite prirodan broj b: 1
Unesite prirodan broj c: 2
Unesite vrh k: 1
Unesite vrh l: 6

IZLAZ:
Program treba ispisati traženo rješenje s odgovarajućom porukom.

Primjer izlaza programa:
Udaljenost vrhova k i l je 16

PROGRAM:
Cjelokupan izvorni kod programa mora biti u jednoj datoteci. Implementacija Dijkstrinog algoritma mora biti samostalna.
*/


#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <chrono> 

using namespace std;

// Prototypes
void printMatrix(vector<vector<int>> & Matrix);
template <typename T> void printMatrix2(vector<vector<T>> & Matrix);
int edgeWeight(int i, int j, int a, int b, int c);
template <typename T> void fillMatrix(vector<vector<T>> & Matrix, int a, int b, int c);
template <typename T> int DijkstrasAlgorithm2(vector<vector<T>> & Matrix, int k, int v);

// Klasa StopWatch korištena za mjerenje vremena
class StopWatch { 
   private: 
      chrono::time_point<chrono::steady_clock> start; 
   public: 
      void reset() { 
         start = chrono::steady_clock::now(); 
      } 
      StopWatch() { 
         reset(); 
      } 
      // Vraća proteklo vrijeme u sekundama
      double elapsedSeconds() { 
         chrono::duration<double> d = chrono::steady_clock::now() - start;
         return chrono::duration_cast<chrono::microseconds>(d).count() / 1000000.; 
      } 
};

int main() {

   // Deklaracija potrebnih varijabli
   int n, a, b, c, k, l;

   // Unos podataka
   {
      do {
         cout << "Unesite prirodan broj n: ";
         cin >> n;
      } while (n > 3500 || n <= 0);

      do {
         cout << "Unesite prirodan broj a: ";
         cin >> a;
      } while (a > 1000000 || a <= 0);
      
      do {
         cout << "Unesite prirodan broj b: ";
         cin >> b;
      } while (b > 1000000 || b <= 0);
      
      do {
         cout << "Unesite prirodan broj c: ";
         cin >> c;
      } while (c > 1000000 || c <= 0);

      do {
         cout << "Unesite prirodan broj k: ";
         cin >> k;
      } while (k >  n || k <= 0);
      
      do {
         cout << "Unesite prirodan broj l: ";
         cin >> l;
      } while (l > n || l <= 0);
   }

   // Matrica susjedstva modificirana tako da na poziciji (i,j) sadrži težinu brida
   vector<vector<int>> AdjacencyMatrix(n, vector<int> ());

   // Popunjavanje matrice sa težinama bridova
   fillMatrix(AdjacencyMatrix, a, b, c);

   StopWatch StopWatch;
   cout << "Najmanja udaljenost vrhova k i l je " << DijkstrasAlgorithm2(AdjacencyMatrix, k, l) << endl;
   cout << "Program se izvršio u: " << StopWatch.elapsedSeconds() << " sekundi" << endl;

   return 0;
}

// Funkcija za ispis matrice
void printMatrix(vector<vector<int>> & Matrix) {
   for (int i = 0; i < Matrix.size(); i++) {
      for (int j = 0; j < Matrix[i].size(); j++) {
         cout << Matrix[i][j] << " ";
      }
      cout << endl;
   }
}

// Funkcija za ispis matrice sa formatiranjem
template <typename T> void printMatrix2(vector<vector<T>> & Matrix) {
   typename vector<vector<T>>::iterator t1;
   typename vector<T>::iterator t2;
   cout << endl;

   for(t1 = Matrix.begin(); t1 != Matrix.end(); t1++){
      for (t2 = t1->begin(); t2 != t1->end(); t2++){
         printf(" %10d", *t2);
      }
      cout << endl;
   }
}

// Funkcija za izračun težine brida između vrhova i, j
int edgeWeight(int i, int j, int a, int b, int c) {
   return pow(a, abs(i - j) % c) + b * pow(i - j, 2) - 1;
}

// Funkcija za popunjavanje matrice težinama
template <typename T> void fillMatrix(vector<vector<T>> & Matrix, int a, int b, int c) {
   for (int i = 0; i < Matrix.size(); i++) {
      for (int j = 0; j < Matrix.size() ; j++) {
         Matrix[i].push_back(edgeWeight(i, j, a, b, c));
      }
   }
}

// Dijkstra algoritam za izračun najkraće udaljenosti
template <typename T> int DijkstrasAlgorithm2(vector<vector<T>> & Matrix, int k, int v) {
   map<int, int> S;   // Skup S (ključ je index vrha)
   map<int, int> SK;  // Skup SK (ključ su vrhovi, vrijednost je udaljenost od k)
   vector<int> l;     // Vektor s najmanjim udaljenostima

   // Inicijalizacija skupa SK
   for(int vrh = 1; vrh <= Matrix.size(); vrh++) {
      SK[vrh] = INT_MAX; 
   }

   // Postavljanje početnih vrijednosti za S
   S[0] = k;
   l.push_back(0);
   SK.erase(k);

   int minAll;
   int minTop;

   // Glavna petlja Dijkstrinog algoritma
   while(S.size() != Matrix.size()) {
      minAll = INT_MAX;

      for(auto it1 = SK.begin(); it1 != SK.end(); it1++) {
         int d_component = Matrix[it1->first - 1][(S.find(S.size() - 1))->second - 1];
         int distance = d_component + l[(S.find(S.size() - 1))->first];
         it1->second = min(distance, it1->second);
         
         if(it1->second < minAll) {
            minAll = it1->second;
            minTop = it1->first;
         }
      }
      S[S.size()] = minTop;
      l.push_back(minAll);
      SK.erase(minTop);
   }

   // Pronalaženje rezultata
   for (auto i = S.begin(); i != S.end(); i++) {
      if(i->second == v) {
         return l[i->first];
      }
   }
   return -1;
}
