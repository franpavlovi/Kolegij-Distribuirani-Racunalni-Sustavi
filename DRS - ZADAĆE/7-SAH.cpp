/*
Treba napraviti kvazi šahovsku igru. igra se odvija na kvadratnoj ploci dimenzija m x m. U igri sudjeluju tri vrste figura: top, lovac i pijun.
Top se krece kao i u standardnoj sahovskoj igri, tj. vertikalno ili horizontalno za proizvoljan broj mjesta. LOvcu je dozvoljena kretnja samo po jedno
mjesto dijagonalno od mjesta na kome se trenutno nalazi, a pijunu samo jedno mjesto horizontalno ili vertikalno. Top je jaci od lovca, lovac je jaci od pijuna,
a pijun je jaci od topa. Kada se dvije razlicite figure nadju na istom polju, ostaje ona koja je jaca.

Ukoliko neka figura odigra na polje gdje se veæ nalazi figura istog tipa, ta figura jede dolazeæu figuru, tj. uzima se da je jaèa jer brani
svoju teritoriju. Figure se proizvoljno kreæu po ploèi u ovisnosti od svog tipa, i ne mogu odigrati potez izvan ploèe. Ukoliko figura
ipak izabere da se kreæe izvan ploèe, ostaje u mjestu. Igra se završava kada na ploèi ostane jedna figura, i ta figura je oznaèena
kao pobjednik. Program napisati korištenjem C++ i MPI funkcija.

U MPI programu prvo kreirati strukture:
    Figura:
        int id – id figure,
        int x – globalna x koordinata figure,
        int y – globalna y koordinata figure,
        int tip – tip figure, 0-top, 1-lovac, 2-pijun,
        int brojPojedenih – koliko je figura pojela drugih figura,
        int brojPoteza – broj odigranih poteza figure, i
    Polje:
        int id – id figure koja se nalazi na polju (-1 oznaèava da je polje prazno),
        int tip – tip figure koja se nalazi na danom polju.
*/

#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

using namespace std;
const int BOARD_SIZE = 4;
const int BROJ_FIGURA = 4;

struct Figura {
int id;
int x;
int y;
int tip; // 0-pijun, 1-lovac, 2-top,
int brojPojedenih;
int brojPoteza;
bool ziv;
};
struct polje {
int id;
int tip;
};
polje ploca[BOARD_SIZE][BOARD_SIZE];

Figura figura[BROJ_FIGURA];

void initializeBoard(int rank) {
        for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            ploca[i][j].tip = -1; 
        }
    }
     if (rank == 0){
    	figura[0].id = 0;
    	figura[0].x = 1;
    	figura[0].y = 1;
    	figura[0].tip = 0;
    	figura[0].brojPojedenih = 0;
    	figura[0].brojPoteza = 0;
    	figura[0].ziv = true;
    	ploca[figura[0].x][figura[0].y].id = figura[0].id;
        ploca[figura[0].x][figura[0].y].tip = figura[0].tip;

    	for(int i = 0; i<BOARD_SIZE;i++){
            if(rank!=i) {
                MPI::COMM_WORLD.Send(&figura[0], sizeof(figura), MPI::BYTE, i, 17); 
                MPI::COMM_WORLD.Recv(&figura[i], sizeof(figura), MPI::BYTE, i, 17); 
            }
    	}
    }
    if (rank == 1){
    	figura[1].id = 1;
    	figura[1].x = 2;
    	figura[1].y = 2;
    	figura[1].tip = 1;
    	figura[1].brojPojedenih = 0;
    	figura[1].brojPoteza = 0;
    	figura[1].ziv = true;
    	ploca[figura[1].x][figura[1].y].id = figura[1].id;
        ploca[figura[1].x][figura[1].y].tip = figura[1].tip;

    	for(int i = 0; i<BOARD_SIZE;i++){
            if(rank!=i) {
                MPI::COMM_WORLD.Send(&figura[1], sizeof(figura), MPI::BYTE, i, 17);
                MPI::COMM_WORLD.Recv(&figura[i], sizeof(figura), MPI::BYTE, i, 17);
            }
    	}

    }
    if (rank == 2){
    	figura[2].id = 2;
    	figura[2].x = 3;
    	figura[2].y = 0;
    	figura[2].tip = 2;
    	figura[2].brojPojedenih = 0;
    	figura[2].brojPoteza = 0;
    	figura[2].ziv = true;
    	ploca[figura[2].x][figura[2].y].id = figura[2].id;
        ploca[figura[2].x][figura[2].y].tip = figura[2].tip;

    	for(int i = 0; i<BOARD_SIZE;i++){
            if(rank!=i) {
                MPI::COMM_WORLD.Send(&figura[2], sizeof(figura), MPI::BYTE, i, 17);
                MPI::COMM_WORLD.Recv(&figura[i], sizeof(figura), MPI::BYTE, i, 17);
            }
    	}

    }
    if (rank == 3){
    	figura[3].id = 3;
    	figura[3].x = 3;
    	figura[3].y = 3;
    	figura[3].tip = 2;
    	figura[3].brojPojedenih = 0;
    	figura[3].brojPoteza = 0;
    	figura[3].ziv = true;
    	ploca[figura[3].x][figura[3].y].id = figura[3].id;
        ploca[figura[3].x][figura[3].y].tip = figura[3].tip;

    	for(int i = 0; i<BOARD_SIZE;i++){
            if(rank!=i) {
                MPI::COMM_WORLD.Send(&figura[3], sizeof(figura), MPI::BYTE, i, 17);
                MPI::COMM_WORLD.Recv(&figura[i], sizeof(figura), MPI::BYTE, i, 17);
            }
    	}

    }

MPI::COMM_WORLD.Barrier();


}

bool pomicanje(int i, int x, int y){

	 if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) { 
        return false;
    }
    if (ploca[x][y].tip == -1) {

        ploca[figura[i].x][figura[i].y].tip = -1;
        figura[i].x = x;
        figura[i].y = y;
        figura[i].brojPoteza++;
        ploca[x][y].id = figura[i].id;
        ploca[x][y].tip = figura[i].tip;
        return true;
    } else {
if(figura[i].tip == 2 && ploca[x][y].tip == 0){
            cout << "Figurica " <<  ploca[x][y].tip <<" jede "<< figura[i].tip << endl;
            figura[i].ziv = false;
            figura[i].brojPoteza++;
            figura[ploca[x][y].id].brojPojedenih ++;
            return true;
} else if(figura[i].tip == 0 && ploca[x][y].tip == 2){
            cout << "Figurica " << figura[i].tip <<" jede "<< ploca[x][y].tip << endl;
            ploca[figura[i].x][figura[i].y].tip = -1;
            ploca[figura[i].x][figura[i].y].id = -1;
    	    figura[ploca[x][y].id].ziv = false;
            figura[i].x = x;
            figura[i].y = y;
            figura[i].brojPojedenih++;
    	    figura[i].brojPoteza++;
            ploca[x][y].id = figura[i].id;
            ploca[x][y].tip = figura[i].tip;
            return true;
}else if(figura[i].tip > ploca[x][y].tip) {
            cout << "Figurica " << figura[i].tip <<" jede "<< ploca[x][y].tip << endl;
            ploca[figura[i].x][figura[i].y].tip = -1;
            ploca[figura[i].x][figura[i].y].id = -1;
    	    figura[ploca[x][y].id].ziv = false;
            figura[i].x = x;
            figura[i].y = y;
            figura[i].brojPojedenih++;
    	    figura[i].brojPoteza++;
            ploca[x][y].id = figura[i].id;
            ploca[x][y].tip = figura[i].tip;
            return true;
        } else if(figura[i].tip < ploca[x][y].tip) {
            cout << "Figurica " <<  ploca[x][y].tip <<" jede "<< figura[i].tip << endl;
            figura[i].ziv = false;
            figura[i].brojPoteza++;
            figura[ploca[x][y].id].brojPojedenih ++;
            return true;
        } else if (figura[i].tip == ploca[x][y].tip){
            cout << "Figurice tipa "<<figura[i].tip <<" jednake jedna ispada" << endl;
            ploca[figura[i].x][figura[i].y].tip = -1;
            ploca[figura[i].x][figura[i].y].id = -1;
            figura[ploca[x][y].id].ziv = false;
            figura[i].x = x;
            figura[i].y = y;
            figura[i].brojPojedenih++;
    	    figura[i].brojPoteza++;
            ploca[x][y].id = figura[i].id;
            ploca[x][y].tip = figura[i].tip;
            return false;
        }
    }

return false;
}

int kretanje(int i,int rnk){
	int nova_kord, nova_y;
	srand(time(0));
    int nRandonNumber = 0 + rand() % (( 3 + 1 ) - 0);
	if (figura[i].tip == 0){
		if(nRandonNumber == 0)  {
			nova_kord = figura[i].x + 1;
			pomicanje(i,nova_kord,figura[i].y);
			}
		if(nRandonNumber == 1){
			nova_kord = figura[i].y - 1;
			pomicanje(i,figura[i].x,nova_kord);
			}
		if(nRandonNumber == 2){
			nova_kord = figura[i].y + 1;
			pomicanje(i,figura[i].x,nova_kord);
			}
		if(nRandonNumber == 3){
			nova_kord = figura[i].x - 1;
			pomicanje(i,nova_kord,figura[i].y);
			}
		return 0;
	}
	if (figura[i].tip == 1){
		if(nRandonNumber == 0)  {
			nova_kord = figura[i].x + 1;
			nova_y = figura[i].y + 1;
			pomicanje(i,nova_kord,nova_y);
			}
		if(nRandonNumber == 1){
			nova_kord = figura[i].x - 1;
			nova_y = figura[i].y - 1;
			pomicanje(i,nova_kord,nova_y);
			}
		if(nRandonNumber == 2){
			nova_kord = figura[i].x - 1;
			nova_y = figura[i].y + 1;
			pomicanje(i,nova_kord,nova_y);

			}
		if(nRandonNumber == 3){
			nova_kord = figura[i].x + 1;
			nova_y = figura[i].y - 1;
			pomicanje(i,nova_kord,nova_y);
			}
	return 1;
	}

	if (figura[i].tip == 2){
	if(nRandonNumber == 0)  {
			nova_kord = figura[i].x + 2;
			pomicanje(i,nova_kord,figura[i].y);
			}
		if(nRandonNumber == 1){
			nova_kord = figura[i].y - 3;
			pomicanje(i,figura[i].x,nova_kord);
			}
		if(nRandonNumber == 2){
			nova_kord = figura[i].y + 1;
			pomicanje(i,figura[i].x,nova_kord);
			}
		if(nRandonNumber == 3){
			nova_kord = figura[i].x - 1;
			pomicanje(i,nova_kord,figura[i].y);
			}
	return 2;
	}
return -1;
}

int main (int argc, char* argv[]) {

  int rank,i;
  MPI::Status status;
  MPI::Init(argc, argv);
  rank = MPI::COMM_WORLD.Get_rank();
  initializeBoard(rank);

}
  while(true){
     for(int i = 0; i < BROJ_FIGURA ; i++){
    	if (figura[i].ziv == false) continue; 
    	

        if(rank == 0){
            kretanje(i,rank);
            int brojac = 0;
             for(int i = 0; i < BROJ_FIGURA ; i++){
            	if (figura[i].ziv == true)brojac++;
             }
        if (brojac == 1) {
            cout << "Šah mat pobjednik figura id  " << figura[i].id << endl;
            cout << "Broj pojedenih: " << figura[i].brojPojedenih << endl;
            cout << "Broj poteza: " << figura[i].brojPoteza << endl;
            for(int j = 0; i<BOARD_SIZE;i++){
                if(rank!=i) {
                    MPI::COMM_WORLD.Send(&figura[i], sizeof(figura), MPI::BYTE, j, 19);
                    MPI::Finalize();
                return 0;
                }
            }
        }
    }
  }	for(int i = 0; i<BOARD_SIZE;i++){
    	if(rank!=0) {
    	    MPI::COMM_WORLD.Recv(&figura[i], sizeof(figura), MPI::BYTE, i, 19);
        	MPI::Finalize();
    	return 0;
    	}
    }
  }
}
