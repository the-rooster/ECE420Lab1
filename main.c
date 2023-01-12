#include "lab1_IO.h"
#include "timer.h"
#include <pthread.h>
#include <math.h>

struct arg_struct {
    int ***A;
    int ***B;
    int ***C;
    int i_l;
    int i_u;
    int j_l;
    int j_u;
    int *n;
    int thread_id;
};

int dot_prod(int ***A, int ***B,int i,int j,int* size){
    //takes the dot product of row i from mat A and column j of mat B
    int sum = 0;
    for(int z=0;z<*size;z++){
        sum = sum + ((*A)[i][z]) * ((*B)[z][j]);
    }

    return sum;
}

void *calc_partial_matrix(void * args_void){
    struct arg_struct* args = (struct arg_struct*) args_void;

    for(int i=args->i_l;i<=args->i_u;i++){
        for(int j=args->j_l;j<=args->j_u;j++){
            (*args->C)[i][j] = dot_prod(args->A,args->B,i,j,args->n);
        }
    }
    
    free(args_void);
    pthread_exit(NULL);
    
}

int main (int argc, char* argv[])
{
    double start, end;

    GET_TIME(start);

    int ***A = malloc(sizeof(int***));
    int ***B = malloc(sizeof(int***));
    int ***C = malloc(sizeof(int***));

    int *n = malloc(sizeof(int*));

    Lab1_loadinput(A,B,n);
    
    int p = atoi(*(argv + 1));

    pthread_t* threads = malloc(p * sizeof(pthread_t));

    *C = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++)
    {
      (*C)[i] = malloc(*n * sizeof(int));
    }


    Lab1_loadinput(A,B,n);


    for(int k=0; k<p; k++){
        int sqrt_p = (int) sqrt(p);
        int x = k / sqrt_p;
        int y = k % sqrt_p;
        int i_l = (*n) * x / sqrt_p;
        int i_u = (*n) * (x + 1) / sqrt_p - 1;
        int j_l = (*n) * y / sqrt_p;
        int j_u = (*n) * (y + 1) / sqrt_p - 1;

        struct arg_struct* args = malloc(sizeof(struct arg_struct));
        args->A = A; args->B = B; args->C = C; args->i_l = i_l; args->i_u = i_u; args->j_l = j_l;args->j_u = j_u;args->n = n; args->thread_id = k;

        if(pthread_create(&threads[k],NULL,&calc_partial_matrix,(void *) args) != 0){
            return -1;
        }
    }

    for(int k=0;k<p;k++){
        pthread_join(threads[k],NULL);
    }

    GET_TIME(end);

    Lab1_saveoutput(*C,n,end - start);
    

}