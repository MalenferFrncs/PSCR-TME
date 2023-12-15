#include <cstddef>
#include <vector>
#include <forward_list>

using namespace std;
namespace pr
{

    template <typename K, typename V>

    class Hashmap
    {
    public:
        class Entry
        {
        public:
            const K key;
            V value;
            Entry(const K &key, const V &value) : key(key), value(value) {}
        };
        typedef std::vector<std::forward_list<Entry>> buckets_t;

        class Iterator
        {
            buckets_t &it_buckets;
            typename std::forward_list<Entry>::iterator lit;
            size_t index;

            /*Le problème de comptage semble se trouver dans la fonction d'avancer qui fait qu'on prend en compte plusieurs
            fois le même élément*/
            /*Voir pour une gestion + fine des différents cas ?
            Ne fonctionne pas, voir correction sans le advance.*/
            void advance()
            {
                ++index;
                if (index >= it_buckets.size())
                {
                    index = -1;
                    lit = it_buckets[0].end();
                }
                else if ((it_buckets[index].empty()))
                {
                    for (; index < it_buckets.size(); ++index)
                    {
                        if (!it_buckets[index].empty())
                        {
                            lit = it_buckets[index].begin(); // On re positionne notre itérateur de liste au début de la suivante
                            return;
                        }
                    }
                    if (index == it_buckets.size())
                    {
                        index = -1;
                        lit = it_buckets[0].end();
                    }
                }
            }

        public:
           
            /*Correction*/

            Iterator(size_t index, typename std::forward_list<Entry>::iterator lit, buckets_t &buck) : index(index), lit(lit), it_buckets(buck) {}
            Iterator(Hashmap &ht, int end) : it_buckets(ht.bucket)
            { // Constructeur d'un itérateur end
                index = -1;
                lit = it_buckets[0].end();
            }

            Iterator &operator++()
            {
                ++lit;
                if (lit == it_buckets[index].end())
                {
                    index++;
                    for (; index < it_buckets.size() && it_buckets[index].empty(); ++index)
                    {
                        /*NOP*/
                    }
                    if (index < it_buckets.size())
                        lit = it_buckets[index].begin();
                }
                return *this;
            }

            Entry &operator*()
            {
                return *lit;
            }
            bool operator!=(Iterator &other)
            {
                return (index != other.index) || (lit != other.lit);
            }
        };

    private:
        buckets_t bucket;
        size_t sz; // Nombre d'entrées

    public:
        Hashmap(size_t szb) : bucket(szb)
        {
            sz = 0;
            for (int i = 0; i < sz; ++i)
            {
                bucket.push_back(std::forward_list<Entry>());
            }
        }
        // (Les mécanismes de la classe canonique sont implémentés par défaut par le compilateur, pas besoin de les réécrire.)

        /*Fonctions pour l'itérateur*/

        /*Version qui ne marche pas
            Iterator begin(){
                return Iterator(*this);
            }
        */
        /*Correction*/

        Iterator begin()
        {
            for (int index = 0; index < bucket.size(); ++index)
            {
                if (!bucket[index].empty)
                {
                    return Iterator(index, bucket[index].begin(), bucket);
                }
            }
            return end();
        }

        Iterator end()
        {
            return Iterator(bucket.size(), bucket[0].end(), bucket); // On prend bucket[0].end() pour un nullptr
        }

        /*Fonction qui rend l'adresse de la valeur associée à la clé key si on la trouve, nullptr sinon*/
        V *get(const K &key)
        {
            size_t ind = (std::hash<K>()(key)) % bucket.size(); // Hasher standard

            /*Trouver le mot dans la hashmap*/
            for (auto &e : bucket[ind])
            {
                if (e.key == key)
                {
                    return &e.value;
                }
            }
            return nullptr;
        }
        /*Fonction qui ajoute l'association (key, value) à la table. */
       
        bool put(const K &key, const V value)
        {
            size_t ind = std::hash<K>()(key) % bucket.size();

            for (auto &e : bucket[ind])
            {
                if (e.key == key)
                {
                    e.value = value;
                    return true;
                }
            }
            /*On ne l'a pas trouvé dans le bucket, il faut créer l'association dans la table*/
            bucket[ind].emplace_front(key, value); /* emplace évite de créer un temporaire */
            ++sz;                                  /* La taille de la Hashmap augmente */
            return false;                          /* On a créé une insertion */
        }
        /*Fonction qui indique la taille de la Hashmap (nombre d'entrées effectives dedans)*/
        size_t size() const { return sz; }
    };

}