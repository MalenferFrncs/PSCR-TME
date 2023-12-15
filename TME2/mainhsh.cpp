#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <unordered_map>
#include "hashmap.h"

using namespace std ;


void ajouteMotVect(std::vector<std::pair<string,int>> &tab, const string &s){
	for(size_t i = 0; i < tab.size() ; i++){
		if( tab[i].first==s ){
			++tab[i].second;
			return;
		}
	}
	tab.push_back(make_pair(s,1));	//On ne l'a pas trouvé dans le tableau
}

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	pr::Hashmap<string,int> hmap (30000);

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;

		int* word_in = hmap.get(word);
		if (word_in != nullptr){
			++(*word_in);
        }else{
            hmap.put(word,1);
        }

	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	cout << "Found a total of " << hmap.size() << " different words." << endl;

    std::vector<pair<string,int>> tab;

	for (auto it : hmap){
		///tab.push_back(pair(it.first,it.second));
	}

	std::sort(tab.begin(),tab.end(), [] (const pair<string,int> & a, const pair<string,int> & b) { return a.second > b.second ; });

    cout << "Found a total of " << tab.at(0).second << " words." << endl;
	cout << "Found a total of " <<tab.at(1).second << " different words." << endl;

    return 0;
}


