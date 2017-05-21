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
  std::lock_guard<std::mutex> lock(zrodlo1Guard);
  zrodlo1.push (rand() % N);
  {
    std::lock_guard<std::mutex> lock(zrodlo2Guard);
    zrodlo2.push (rand() % M);
  }

}

void wstaw () {
  int a, b;
  std::lock_guard<std::mutex> lock(macierzGuard);
  for (int i=N-2; i>=0; i--)
    for (int j=M-1; j>=0; j--) {
      macierz[i][j] = macierz[i+1][j];
    }
  for (int i=0; i<M; i++) {
    macierz[0][i] = 0;
  }
  {
    std::lock_guard<std::mutex> lock(zrodlo1Guard);
    a = zrodlo1.front();
    zrodlo1.pop();
  }
  {
    std::lock_guard<std::mutex> lock(zrodlo2Guard);
    b = zrodlo2.front();
    zrodlo2.pop();
  }

  macierz[a][b] = 1;
}

void wyswietl () {
  std::lock_guard<std::mutex> lock(macierzGuard);
  std::string s;
  initscr();
  for (int i=0; i<N; i++)
    for (int j=0; j<M; j++) {
      s = std::to_string(macierz[i][j]);
      mvprintw(i, j, s.c_str());
      getch();                              //test
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
