/*
ZADATAK:
Zadani su nenegativni cijeli brojevi 𝑚, 𝑎1, 𝑎2, 𝑎3, 𝑎4. 
Potrebno je naći broj cjelobrojnih rješenja jednadžbe 𝑥1 + 𝑥2 + 𝑥3 + 𝑥4 = 𝑚 uz uvjete 0 ≤ 𝑥𝑖 ≤ 𝑎𝑖 za 𝑖 = 1,2,3,4.

NAPOMENA:
Očekuje se da će program za ulaze koji zadovoljavaju 𝑚 ≤ 100000 izvršiti zadatak za najviše 10 sekundi. Nastavnici Vas mogu zatražiti da unosite i testne primjere van ovih ograničenja.

ULAZ:
U izvršnoj datoteci programa trebaju biti ispisani upiti za unošenje svakog od parametara navedenih u zadatku. Svaki se upit javlja u novoj liniji nakon unošenja prethodnog parametra s tipkovnice.

Primjer ulaza programa:
Unesite nenegativan cijeli broj m: 3
Unesite nenegativan cijeli broj a_1: 2
Unesite nenegativan cijeli broj a_2: 3
Unesite nenegativan cijeli broj a_3: 4
Unesite nenegativan cijeli broj a_4: 5

IZLAZ:
Program ispisuje traženo rješenje s odgovarajućom porukom.
Primjer izlaza programa:
Broj rješenja tražene jednadžbe je 19

PROGRAM:
Čitav izvorni kod programa (source code) mora se nalaziti samo u jednoj datoteci, bez obzira na broj korištenih struktura, klasa, funkcija ili procedura.
*/

#include <iostream>
#include <chrono> // Potrebno za mjerenje vremena
#include <ctime> 

using namespace std;

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
      // Vraća proteklo vrijeme u mikrosekundama
      double elapsedSeconds() { 
         chrono::duration<double> d = chrono::steady_clock::now() - start;
         return chrono::duration_cast<chrono::microseconds>(d).count(); 
      } 
};

// Deklaracija za gornje granice Xi:
long long int a1, a2, a3, a4;

// Deklaracija i definicija za donje granice Xi:
long long int dg_1 = 0;
long long int dg_2 = 0;
long long int dg_3 = 0;
long long int dg_4 = 0;

// Deklaracija sume za koju se traži mogući broj rješenja jednađbe
long long int m; 

// Deklaracija ostalih potrebnih varijabli:
long long int ukupan_koeficijent = 0;
long long int kof_pribrojnika = 0;
long long int stupanj_pribrojnika = 0;
long long int potreban_stupanj_n = 0;
long long int kof_clana_reda_n = 0;

// Prototip korištene funkcije
long long int binomni_kof(long long int n, long long int k);

int main(void) {

   // Unos traženih podataka:
   do {
      cout << "Unesite nenegativan cijeli broj m: ";
      cin >> m;
      cout << endl;
   } while(m < 0);

   do {
      cout << "Unesite nenegativan cijeli broj a_1: ";
      cin >> a1;
      cout << endl;
   } while(a1 < 0);

   do {
      cout << "Unesite nenegativan cijeli broj a_2: ";
      cin >> a2;
      cout << endl;
   } while(a2 < 0);

   do {
      cout << "Unesite nenegativan cijeli broj a_3: ";
      cin >> a3;
      cout << endl;
   } while(a3 < 0);

   do {
      cout << "Unesite nenegativan cijeli broj a_4: ";
      cin >> a4;
      cout << endl;
   } while(a4 < 0);

   // Prilagođavanje parametara u slučaju donjih granica
   m = m - (dg_1 + dg_2 + dg_3 + dg_4);
   a1 = a1 - dg_1;
   a2 = a2 - dg_2;
   a3 = a3 - dg_3;
   a4 = a4 - dg_4;

   // Glavni dio programa:
   StopWatch StopWatch;

   // Prolazimo kroz svih 16 mogućih pribrojnika i tražimo odgovarajući n za sumu m
   long long int polje_stupanj_pribrojnika[16]{0, a4 + 1, a3 + 1, a3 + a4 + 2, a2 + 1, a2 + a4 + 2, a2 + a3 + 2,
                                          a2 + a3 + a4 + 3, a1 + 1, a1 + a4 + 2, a1 + a3 + 2, a1 + a3 + a4 + 3,
                                          a1 + a2 + 2, a1 + a2 + a4 + 3, a1 + a2 + a3 + 3, a1 + a2 + a3 + a4 + 4};

   long long int polje_kof_pribrojnika[16]{1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1};

   for (int i = 0; i < 16; i++) {
      stupanj_pribrojnika = polje_stupanj_pribrojnika[i];
      kof_pribrojnika = polje_kof_pribrojnika[i];
      potreban_stupanj_n = m - stupanj_pribrojnika;
      kof_clana_reda_n = binomni_kof(3 + potreban_stupanj_n, potreban_stupanj_n);
      ukupan_koeficijent += kof_pribrojnika * kof_clana_reda_n;
   }
   
   cout << "Broj rješenja tražene jednadžbe je " << ukupan_koeficijent << endl;
   cout << endl;
   cout << "Program se izvršio u: " << StopWatch.elapsedSeconds() << " mikrosekundi" << endl;
   cout << endl;

   return 0;
}

// Funkcija za izračun binomnog koeficijenta
long long int binomni_kof(long long int n, long long int k) {
    return
      (   k  <  0    ) ? 0 :          // Rubni slučaj - negativan k
      (   k  >  n    ) ? 0 :          // Izvan dometa
      (k == 0 || k == n) ? 1 :        // Rubni slučaj - k je 0 ili n
      (k == 1 || k == n-1) ? n :      // Slučaj za k=1 ili simetričan
      (k + k < n) ?                   // Optimizacija za manje k
      (binomni_kof(n-1, k-1) * n) / k : // Rekurzivno izračunavanje
      (binomni_kof(n-1, k) * n) / (n-k); // Optimizacija za veće k
}
