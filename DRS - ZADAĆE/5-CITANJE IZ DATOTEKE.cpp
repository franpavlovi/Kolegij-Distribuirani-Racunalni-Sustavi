/*
Napišite program korištenjem C++ funkcija u kojem æe svi procesi uèitati podatke
iz proizvoljne datoteke, sinkronizirati se na barijeri, te zatim ispisati uèitane elemente,
i to na naèin da proces 0 ispiše prvi, proces 1 drugi itd. 

Program mora biti moguæe izvršiti na proizvoljnom broju procesa bez gubitka funkcionalnosti.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
    int rank, size, num; 

    MPI::Init(argc, argv);

    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    vector<int> data; 
    ifstream file("data.txt"); 

    while (file >> num) 
    {
        data.push_back(num); 
    }
    file.close();

    MPI::COMM_WORLD.Barrier();  
                                      

    for (int i = rank; i < data.size(); i += size) 
    {
        cout << "Proces " << rank << " je procitao: " << data[i] << endl;
    }

    MPI::Finalize();
    return 0;
}
