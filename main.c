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
    //printf("%d\n",*size);
    for(int z=0;z<*size;z++){
        //printf("summing\n");
        //printf("VALS: %d %d %d\n",i,j,z);
        //printf("A: %d\n",(*A)[z][i]);
        //printf("B: %d\n",(*B)[j][z]);
        //printf("PROD: %d\n",((*A)[i][z]) * ((*B)[z][j]));
        sum = sum + ((*A)[i][z]) * ((*B)[z][j]);
        //printf("SUM: %d\n",sum);
    }

    return sum;
}

void *calc_partial_matrix(void * args_void){
    struct arg_struct* args = (struct arg_struct*) args_void;

    //printf("%d: %d %d %d %d\n",args->thread_id,args->i_l,args->i_u,args->j_l,args->j_u);
    for(int i=args->i_l;i<=args->i_u;i++){
        for(int j=args->j_l;j<=args->j_u;j++){
            //printf("allocating sum\n");
            (*args->C)[i][j] = dot_prod(args->A,args->B,i,j,args->n);
            //printf("allocated %d %d %d %d\n",i,j,(*args->C)[i][j],dot_prod(args->A,args->B,i,j,args->n));
        }
    }
    
    free(args_void);
    pthread_exit(NULL);
    
}

int main (int argc, char* argv[])
{
    //printf("we multiplyin\n");
    double start, end;

    GET_TIME(start);

    int ***A = malloc(sizeof(int***));
    int ***B = malloc(sizeof(int***));
    int ***C = malloc(sizeof(int***));

    int *n = malloc(sizeof(int*));

    Lab1_loadinput(A,B,n);
    
    int p = atoi(*(argv + 1));

    //array of threads so we can join them after computation
    pthread_t* threads = malloc(p * sizeof(pthread_t));

    *C = malloc(*n * sizeof(int*));
    //assign memory to mat C
    for (int i = 0; i < *n; i++)
    {
      (*C)[i] = malloc(*n * sizeof(int));
    }

    //printf("test: %d\n",(*A)[0][0]);
    //printf("%d\n",(*A)[1][0]);

    //printf("okay we go now\n");

    Lab1_loadinput(A,B,n);


    for(int k=0; k<p; k++){
        int sqrt_p = (int) sqrt(p);
        //printf("SQUARE ROOT: %d\n",sqrt_p);
        int x = k / sqrt_p;
        int y = k % sqrt_p;
        //printf("X: %d Y: %d\n",x,y);
        int i_l = (*n) * x / sqrt_p;
        int i_u = (*n) * (x + 1) / sqrt_p - 1;
        int j_l = (*n) * y / sqrt_p;
        int j_u = (*n) * (y + 1) / sqrt_p - 1;

        //assign args into arg_struct to pass into pthread
        struct arg_struct* args = malloc(sizeof(struct arg_struct));
        //printf("HERE\n");
        args->A = A; args->B = B; args->C = C; args->i_l = i_l; args->i_u = i_u; args->j_l = j_l;args->j_u = j_u;args->n = n; args->thread_id = k;

        if(pthread_create(&threads[k],NULL,&calc_partial_matrix,(void *) args) != 0){
            //printf("thread failed to create. uhoh\n");
            return -1;
        }

        //printf("HERE2 %d\n",k);

    }

    //printf("HERE3\n");

    for(int k=0;k<p;k++){
        pthread_join(threads[k],NULL);
        //printf("JOINED %d\n",k);
    }

    GET_TIME(end);

    Lab1_saveoutput(*C,n,end - start);
    

}