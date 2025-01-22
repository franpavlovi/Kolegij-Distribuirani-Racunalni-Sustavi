/*
Napisati program u kojem �e svi procesi generirati jedan nasumi�an broj,
nakon �ega �e jedan od procesa na�initi aritmeti�u sredinu brojeva, koriste�i
operaciju redukcije, te �e dobivenu aritmeti�ku srednju vrijednost poslati
svim ostalim procesima. Procesi �e nakon toga izra�unati odstupanje u postocima
u odnosu na dobivenu srednju vrijednost.
D suma svih brojeva iz svih procesa, prikupljena pomo�u MPI_Reduce.*
Koristiti isklju�ivo kolektivnu komunikaciju. Program napisati isklju�ivo
kori�tenjem C++ funkcija.
*/

#include <iostream>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

int main (int argc, char* argv[]) {

    MPI::Init(argc, argv);
    int rank;
    int size;
    double c, d; 
    float prosjek = 0;
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    srand((unsigned)time(NULL)+rank*size+rank); 
    c = rand()%100;                             

    MPI::COMM_WORLD.Reduce(&c,&d,1,MPI::DOUBLE,MPI::SUM,0); 
                                                            
                                                           
    if(rank == 0){
		prosjek=(float)d/size;
	}

    MPI::COMM_WORLD.Bcast(&prosjek, 1, MPI::FLOAT, 0);
    cout<<"Proces rang: " << rank << " generirani random broj je: " << c << " prosjek je: " << prosjek << " odstupanje je: " << abs(100-(c/prosjek*100)) << "% " << endl;
    MPI::Finalize();
    return 0;
}
