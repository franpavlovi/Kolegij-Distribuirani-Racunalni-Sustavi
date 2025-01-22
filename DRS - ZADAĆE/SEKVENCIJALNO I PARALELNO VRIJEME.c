/*
Napravite program koji izraèunava zbroj prilagoðenih prefiksa dan
u nastavku za 4 razlièita niza i nakon toga ih sve zbrojiti.
Upotrijebite OpenMP directive za paralelno izvršavanje.
? A(1) = A(1)
? A(2) = A(2) + A(1)
? A(i) = A(i-2) + A(i-1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define M 1000

int A[N][M];
int B[N][M];

int main() {

    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(M * sizeof(int)); 
        B[i] = (int *)malloc(M * sizeof(int));
    } 

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }

    
    double seq_start = omp_get_wtime(); 
    for (int n = 0; n < N; ++n) {
        for (int i = 2; i < M; ++i) {
            if (i == 2) {
                B[n][i] = B[n][i] + B[n][i - 1];
            } else if (i == 1) {
                B[n][i] = B[n][i];
            } else {
                B[n][i] = B[n][i - 2] + B[n][i - 1];
            }
        }
    }

    int seq_total_sum = 0;
    for (int n = 0; n < N; ++n) {
        for (int i = 0; i < M; ++i) {
            seq_total_sum += B[n][i];
        }
    }
    double seq_end = omp_get_wtime(); 

    
    omp_set_num_threads(10);  
    double par_start = omp_get_wtime();

    #pragma omp parallel for
    for (int n = 0; n < N; ++n) {
        for (int i = 2; i < M; ++i) {
            if (i == 2) {
                A[n][i] = A[n][i] + A[n][i - 1];
            } else if (i == 1) {
                A[n][i] = A[n][i];
            } else {
                A[n][i] = A[n][i - 2] + A[n][i - 1];
            }
        }
    }

    int par_total_sum = 0;
    
    #pragma omp parallel for reduction(+:par_total_sum)
    for (int n = 0; n < N; ++n) {
        for (int i = 0; i < M; ++i) {
            par_total_sum += A[n][i];
        }
    }
    double par_end = omp_get_wtime();

    
    printf("Sekvencijalno vrijeme: %f sekundi\n", seq_end - seq_start);
    printf("Paralelno vrijeme: %f sekundi\n", par_end - par_start);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    return 0;
}
