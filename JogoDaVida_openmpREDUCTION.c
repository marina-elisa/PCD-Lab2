#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#define SRAND_VALUE 1985

int getNeighbors(int **grid, int posX, int posY, int N){
    int startX, startY, x, y, count = 0;
    startX = posX == 0 ? N - 1 : posX - 1;
    startY = posY == 0 ? N - 1 : posY - 1;
    for(int i = 0; i <= 2; i++){
        x = (startX + i) >= N ? (startX + i - N) : startX + i;
        for(int j = 0; j <= 2; j++){
            y = (startY + j) >= N ? (startY + j - N) : startY + j;
            count += grid[x][y];
        }
    }
    count -= grid[posX][posY];
    return count;
}

int main(){
    struct timeval begin, end;
    long long tmili;
    int N, nGeracoes, **grid, **newGrid, count, sum = 0;
    
    N= 2048;
    nGeracoes = 2000;
    gettimeofday(&begin,NULL);
    grid = malloc(N*sizeof(int*));
    newGrid = malloc(N*sizeof(int*));
    for(int i = 0; i < N; i++){
        grid[i] = malloc(N*sizeof(int));
        newGrid[i] = malloc(N*sizeof(int));
    }
    srand(SRAND_VALUE);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            grid[i][j] = rand()%2;
            //printf("%d ", grid[i][j]);
        }
        //printf("\n");
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sum += grid[i][j];
        }
    }
    omp_set_num_threads(4);
    printf("soma %d\n",sum);
    
    
    for(int i = 0; i < nGeracoes; i++){
        printf("geracao %d\n",i);

        for(int j = 0; j < N; j++){
            
            for(int k = 0; k < N; k++){
             
                newGrid[j][k] = grid[j][k];
                count = getNeighbors(grid,j,k,N);
                //printf("[%d][%d] = %d\n", j, k, count);
                if(grid[j][k] == 1){
                    if(count < 2)
                        newGrid[j][k] = 0;
                    else if(count >= 4)
                        newGrid[j][k] = 0;
                }
                else{
                    if(count == 3)
                        newGrid[j][k] = 1;
                }
            }
        }
        sum = 0;
        for(int j = 0; j < N; j++){
            #pragma omp parallel for reduction (+ : sum)
            for(int k = 0; k < N; k++){
                
                grid[j][k] = newGrid[j][k];
                
                
                sum += grid[j][k];
            }
           
        }
        printf("%d\n",sum);

    }

     gettimeofday(&end,NULL);

    tmili = (int)((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));

    printf("%lld ms\n",tmili);
}