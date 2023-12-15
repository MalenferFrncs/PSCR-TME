#include "Banque.h"

#include <iostream>
#include <mutex>

using namespace std;

namespace pr {

void lock(bool comp, recursive_mutex *mutd, recursive_mutex *mutc){
	if(comp){	/* on fait en sorte de toujours bloquer les verous dans l'ordre croissant de leurs indices pour eviter les interblocage*/
		mutd->lock();
		mutc->lock();
	}
	else{
		mutc->lock();
		mutd->lock();
	}
}

void unlock(recursive_mutex *mutd, recursive_mutex *mutc){
	mutc->unlock();
	mutd->unlock();
}

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	recursive_mutex *mutd = debiteur.getMutex();
	recursive_mutex *mutc = crediteur.getMutex();

	lock((deb<cred),mutd,mutc);
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	unlock(mutd,mutc);
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		unique_lock<recursive_mutex> g(*compte.getMutex());
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
