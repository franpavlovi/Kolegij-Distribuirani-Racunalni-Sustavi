/*
*Uporabom MPI-a izraditi simulaciju raspodijeljenog problema n
filozofa. Pri svakoj promjeni program mora vizualno prikazati za
sve filozofe što oni rade. Npr. kada filozof 4 ide jesti, tada treba
ispis izgledati otprilike ovako: "Stanje filozofa: X o O X o" (X-jede,
o-razmišlja, O-èeka na vilice).
* Problem pet filozofa. Filozofi obavljaju samo dvije razlièite
aktivnosti: misle ili jedu. To rade na poseban naèin. Na jednom
okruglom stolu nalazi se pet tanjura te pet štapiæa (izmeðu
svaka dva tanjura po jedan). Filozof prilazi stolu, uzima lijevi
štapiæ, pa desni te jede. Zatim vraæa štapiæe na stol i odlazi
misliti.
* Program napisati korištenjem C++ funkcija
*/

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <unistd.h>

#define N 5

using namespace std;

char state[N] = { 'o', 'o', 'o', 'o', 'o' };

void test(int rank) {
    // 'O' == HUNGRY, 'X' == EATING, 'o' == THINKING
    if (state[rank] == 'O' && state[(rank + 4) % N] != 'X' && state[(rank + 1) % N] != 'X') {
        state[rank] = 'X';
         MPI::COMM_WORLD.Bcast(&state[0], N, MPI_CHAR, rank);
    }
     cout << "stanja filozofa: ";
            for (int i = 0; i < N; i++) {
                cout << state[i] << "  ";
            }
            cout << endl;
}

void take_forks(int rank) {
 if ( state[(rank + 4) % N] != 'O' && state[(rank + 1) % N] != 'O') {
         state[rank] = 'O';
           test(rank);
           MPI::COMM_WORLD.Bcast(&state[0], N, MPI_CHAR, rank);
    }
}

void return_forks(int rank) {
sleep(1);
    state[rank] = 'o';
   MPI::COMM_WORLD.Bcast(&state[0], N, MPI_CHAR, 0);
}

void philosopher(int rank) {
    while (true) { 
        sleep(2);
        take_forks(rank);
        sleep(0);
        return_forks(rank);
    }
}

int main(int argc, char **argv) {
    MPI::Init(argc, argv);
    int size, rank;
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    philosopher(rank);

    MPI::Finalize();
    return 0;
}
