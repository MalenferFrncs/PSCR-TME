#include "Banque.h"
#include "Compte.h"
#include <iostream>

#define N 50
using namespace pr;
using namespace std;

void transfertJob (int index, Banque & banque) {
    std::cout << "debut job : "<< index << endl;
    for (int i= 0 ; i < 1000; i++) {
        int debite = rand() % banque.size();
        int credite = rand() % banque.size();
        int val = rand() % 100 + 1;
        banque.transfert(debite,credite,val);
    }
    std::cout << "fin job : "<< index << endl;
}

void comptaJob (Banque & banque) {
    std::cout << "debut compta " << endl;
    for (int i= 0 ; i < 10; i++) {
        
        banque.comptabiliser((1000*200));
    }
    std::cout << "fin compta " << endl;
}

int main () {
    
    ::srand(::time(nullptr));
    vector<thread> threads;
    threads.reserve(N+1);

    Banque banque(1000,200);

    for (int i=0; i < N ; i++) {
        threads.emplace_back(transfertJob,i,std::ref(banque));       
    }
    threads.emplace_back(comptaJob,std::ref(banque)); 
    
    std::cout << "les "<< N <<" threads ont été créés"<< endl;
    std::cout << "on lance les "<< N <<" threads"<< endl;
    for (int i=0; i < N+1 ; i++) {
        threads[i].join();
    }
    return 0;
}