/*
Napisati program kori�tenjem C++ funkcija u kojem �e jedan proces poslati vektor
od 100 elemenata svim ostalim procesima, nakon �ega �e svaki proces vratiti taj vektor
po�iljatelju, ali uve�an za N+1, gdje je N broj procesa.
*/

#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

void send_vector(vector<int>& v, int root)
{
    int rank, size;

    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    if (rank == root)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != root)
            {
                cout << "Poslani vector: ";
                for (int j = 0; j < v.size(); j++)
                {
                    cout << v[j] << " ";
                }
                cout << endl;
                MPI::COMM_WORLD.Send(v.data(), v.size(), MPI::INT, i, 0);
            }
        }
    }

    else
    {
        MPI::COMM_WORLD.Recv(v.data(), v.size(), MPI::INT, root, 0);

        for (int i = 0; i < v.size(); i++)
        {
            v[i] += size + 1;
        }

        MPI::COMM_WORLD.Send(v.data(), v.size(), MPI::INT, root, 1);
    }

    if (rank == root)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != root)
            {
                MPI::COMM_WORLD.Recv(v.data(), v.size(), MPI::INT, i, 1);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    MPI::Init(argc, argv);

    vector<int> v(100);
    int root = 0;
    int rank;

    rank = MPI::COMM_WORLD.Get_rank();

    for (int i = 0; i < v.size(); i++)
    {
        v[i] = i;
    }

    send_vector(v, root);
    
    if (rank == root)
    {
        cout << "Proces " << rank << " je primio poruku: ";
        for (int i = 0; i < v.size(); i++)
        {
            cout << v[i] << " ";
        }
        cout << endl;
    }

    MPI::Finalize();
    return 0;
}
