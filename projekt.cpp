#include<queue>
#include<thread>
#include<mutex>
#include<string>
#include<ncurses.h>
#include<time.h>
#include<stdlib.h>

#define N 10
#define M 10

std::queue < int > zrodlo1;
std::mutex zrodlo1Guard;

std::queue < int > zrodlo2;
std::mutex zrodlo2Guard;

int macierz[N][M] = {0};
std::mutex macierzGuard;

void dodajDoKolejki () {
  while (1) {
    std::lock_guard<std::mutex> l1(zrodlo1Guard);
    std::lock_guard<std::mutex> l2(zrodlo2Guard);
    zrodlo1.push (rand() % N);
    zrodlo2.push (rand() % M);
  }
}

void wstaw () {
  int a, b;
  while (1) {
    std::lock_guard<std::mutex> lck(macierzGuard);
     for (int j = 0; j<N; j++){
      for (int k = M-1;  k > 0; k-- ){
        macierz[j][k] = macierz[j][k-1];
      }
        macierz[j][0] = 0;
      }
      std::lock_guard<std::mutex> l1(zrodlo1Guard);
      std::lock_guard<std::mutex> l2(zrodlo2Guard);
      a = zrodlo1.front();
      zrodlo1.pop();
      b = zrodlo2.front();
      zrodlo2.pop();
      macierz[a][b] = 1;
    }
}

void wyswietl () {
  while (1) {
    std::lock_guard<std::mutex> lock(macierzGuard);
    std::string s;
    initscr();
    for (int i=0; i<N; i++)
      for (int j=0; j<M; j++) {
        s = std::to_string(macierz[i][j]);
        mvprintw(i, j, s.c_str());
        getch();
      }
  }
}

int main () {
  auto producent = std::thread(dodajDoKolejki);
  auto przetwarzacz = std::thread(wstaw);
  auto ncurses = std::thread(wyswietl);

  producent.join();
  przetwarzacz.join();
  ncurses.join();

}
