#include "Banque.h"
#include "Compte.h"

#define N=50


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


int main () {
    using namespace pr;
    ::srand(::time(nullptr));
    vector<thread> threads;
    threads.reserve(N);

    Banque banque(1000,200);

    for (int i=0; i < N ; i++) {
        threads.emplace_back(transfertJob,i,std::ref(banque));       
    }
    
    std::cout << "les "<< i <<" threads ont été créés"<< endl;
    std::cout << "on lance les "<< i <<" threads"<< endl;
    for (int i=0; i < N ; i++) {
        threads[i].join();
    }
    return 0;
}