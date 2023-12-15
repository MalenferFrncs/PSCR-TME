#pragma once


#include<cstdlib>
#include <mutex>
#include <condition_variable>


namespace pr{

    class Barrier {
        mutable std::recursive_mutex m; 
        int cpt; 
        int attendu;
        std::condition_variable_any cv;

    public :
        Barrier(int N):cpt(0),attendu(N){}

        void done(){        //Appelée par les workers
            std::unique_lock<std::recursive_mutex> lg(m);
            ++cpt;
            if(cpt == attendu){
                cv.notify_one();
            }
        }

        void waitFor(){     //Appelée par le thread qui attend que tout le monde ait fini pour process les données
            cv.wait(m);
        }

    };

}