#include<queue>
#include<thread>
#include<mutex>
#include<string>
#include<ncurses.h>
#include<time.h>
#include<stdlib.h>

#define N 20
#define M 20

std::queue < int > zrodlo1;
std::mutex zrodlo1Guard;

std::queue < int > zrodlo2;
std::mutex zrodlo2Guard;

int macierz[N][M] = {0};
std::mutex macierzGuard;

void dodajDoKolejki () {
  for (int i = 0; i <= 1000; i++) {
    zrodlo1Guard.lock();
    zrodlo2Guard.lock();
    zrodlo1.push (rand() % N);
    zrodlo2.push (rand() % M);
    zrodlo1Guard.unlock();
    zrodlo2Guard.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void wstaw () {
  int a, b;
  for (int i = 0; i <= 1000; i++) {
    macierzGuard.lock();
     for (int j = 0; j<N; j++){
      for (int k = M-1;  k > 0; k-- ){
        macierz[j][k] = macierz[j][k-1];
      }
        macierz[j][0] = 0;
      }
      zrodlo1Guard.lock();
      zrodlo2Guard.lock();
      a = zrodlo1.front();
      zrodlo1.pop();
      b = zrodlo2.front();
      zrodlo2.pop();
      zrodlo1Guard.unlock();
      zrodlo2Guard.unlock();
      macierz[a][b] = 1;
      macierzGuard.unlock();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void wyswietl () {
  initscr();
  for (int i = 0; i <= 1000; i++) {
    macierzGuard.lock();
    std::string s;
    for (int i=0; i<N; i++)
      for (int j=0; j<M; j++) {
        s = std::to_string(macierz[i][j]);
        mvprintw(i*2, j*3, s.c_str());
        macierzGuard.unlock();
      }
      getch();
      std::this_thread::sleep_for(std::chrono::seconds(1));
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
