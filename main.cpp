/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Joseph
 * Maintainer: 
 * Created: Wed Oct 11 09:28:12 2023 (+0100)
 * Last-Updated: Thu Nov 09 14:03:00 2023 (+0100)
 *           By: Alex Paquette
 *     Update #: 14
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */

#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include<unistd.h>

const int COUNT = 5;
const int THINKTIME=3;
const int EATTIME=5;
std::vector<Semaphore> forks(COUNT);//improvement: need to figure out how to initialize these to 1
Semaphore table(COUNT - 1);


void think(int myID){
  int seconds=rand() % THINKTIME + 1;
  std::printf("%d is thinking!\n", myID);
  sleep(seconds);
}

void get_forks(int philID){
  forks[philID].Wait();
  forks[(philID+1)%COUNT].Wait();
}

void put_forks(int philID){
  forks[philID].Signal();
  forks[(philID+1)%COUNT].Signal();  
}

void eat(int myID){
  int seconds=rand() % EATTIME + 1;
  std::printf("%d is chomping!\n", myID);
  sleep(seconds);  
}

void philosopher(int id/* other params here*/){
  while(true){
    think(id);
    table.Wait();
    get_forks(id);
    eat(id);
    put_forks(id);
    table.Signal();
  }//while  
}//philosopher



int main(void){
  srand (time(NULL)); // initialize random seed: 
  std::vector<std::thread> vt(COUNT);
  int id=0;

  //signal each fork (can't figure out how to initialize them to 1 in the vector)
  for(auto& sem : forks){
    sem.Signal();
  }

  for(std::thread& t: vt){
    t=std::thread(philosopher,id++/*,params*/);
  }
  /**< Join the philosopher threads with the main thread */
  for (auto& v :vt){
      v.join();
  }
  return 0;
}

/* main.c ends here */
