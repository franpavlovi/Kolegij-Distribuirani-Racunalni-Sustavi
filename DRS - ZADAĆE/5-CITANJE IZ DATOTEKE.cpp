/*
Napi�ite program kori�tenjem C++ funkcija u kojem �e svi procesi u�itati podatke
iz proizvoljne datoteke, sinkronizirati se na barijeri, te zatim ispisati u�itane elemente,
i to na na�in da proces 0 ispi�e prvi, proces 1 drugi itd. 

Program mora biti mogu�e izvr�iti na proizvoljnom broju procesa bez gubitka funkcionalnosti.
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
