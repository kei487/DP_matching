#include <stdio.h>
#include <time.h>

#define MAX_N 140
#define MAX_CSTLIST MAX_N*MAX_N*3
#define LOOP1 100
#define LOOP2 100
#define GEIN 0.03

typedef struct{
    int from_x,from_y;
    int flag;
    double data;
    double cst_r;
    double cst_s[3]; 
}NODE;
typedef struct{
    double cst;
    int x,y;
    int from_x,from_y;
    int drec; //0 means right::1 means down::2 means down right 
}COST;


int main(){
    //variable initialize
    FILE *fp1,*fp2;
    char name1[20],content1[20];
    char name2[20],content2[20];
    int i,j,k,n_i,n_j,max_n=0;
    int loop1,loop2;
    int score=0;
    char file1[30]="data/city012/city012_001.txt";
    char file2[30]="data/city011/city011_001.txt";
    double I[MAX_N][15],J[MAX_N][15];
    double total_cst[MAX_N];
    COST cstlist[MAX_CSTLIST];
    NODE maps[MAX_N][MAX_N];
    clock_t start_clock, end_clock;

    // 処理開始前のクロックを取得 
    start_clock = clock();

    //=====Loop1======
    file1[21]='0';file1[22]='0';file1[23]='1';
    for(loop1=0;loop1<LOOP1;loop1++){
        //data scan1
        if((fp1 = fopen(file1,"r"))==NULL){
		    printf("Can not open txte file 1\n");
		    return -1;
	    }
        fscanf(fp1,"%s",name1);
        fscanf(fp1,"%s",content1);
        fscanf(fp1,"%d",&n_i);
        //printf("n_i:%d\n",n_i);
        for(i=0;i<n_i;i++){
            for(j=0;j<15;j++){
                fscanf(fp1,"%lf", &I[i][j]);
            }
	    }

        //======Loop2=======
        file2[21]='0';file2[22]='0';file2[23]='1';
        for(loop2=0;loop2<LOOP2;loop2++){
            //data scan2
            if((fp2 = fopen(file2,"r"))==NULL){
		        printf("Can not open txte file 2\n");
		        return -1;
	        }
            fscanf(fp2,"%s",name2);
            fscanf(fp2,"%s",content2);
            fscanf(fp2,"%d",&n_j);
            //printf("n_j:%d\n",n_j);
            for(i=0;i<n_j;i++){
                for(j=0;j<15;j++){
                    fscanf(fp2,"%lf",&J[i][j]);
                }
            }
            //mapping
            for(i=0;i<n_i;i++){
                for(j=0;j<n_j;j++){
                    maps[i][j].data=0;
                    for(k=0;k<15;k++){
                        maps[i][j].data += (I[i][k]-J[j][k])*(I[i][k]-J[j][k]);
                    }    
                    maps[i][j].flag=1;
                }maps[i][j].flag=0;
            }for(j=0;j<n_j;j++){maps[i][j].flag=0;}

            //A* algorithm
            int cl_i=0;
            int loop_counter=0;
            int tmp_x=0,tmp_y=0;
            int goal_x=n_i-1,goal_y=n_j-1;
            maps[tmp_x][tmp_y].cst_r = maps[tmp_x][tmp_y].data;
            for(i=0;i<n_i;i++){
                for(j=0;j<n_j;j++){
                    if(i==0&&j==~)
                    //calcualte real cost
                    maps[i][j].cst_s[0] = maps[i+1][j].flag * (maps[tmp_x][tmp_y].cst_r + maps[tmp_x+1][tmp_y].data );
                    maps[i][j].cst_s[1] = maps[i][j+1].flag * (maps[tmp_x][tmp_y].cst_r + maps[tmp_x][tmp_y+1].data );
                    maps[i][j].cst_s[2] = maps[i+1][j+1].flag * (maps[tmp_x][tmp_y].cst_r + 2*maps[tmp_x+1][tmp_y+1].data );
                }
                
                //handover
                tmp_x = cstlist[min_index].x;
                tmp_y = cstlist[min_index].y;
                maps[tmp_x][tmp_y].from_x = cstlist[min_index].from_x;
                maps[tmp_x][tmp_y].from_y = cstlist[min_index].from_y;
                maps[tmp_x][tmp_y].cst_r = ((tmp_x-maps[tmp_x][tmp_y].from_x)+(tmp_y-maps[tmp_x][tmp_y].from_y))*maps[tmp_x][tmp_y].data + maps[maps[tmp_x][tmp_y].from_x][maps[tmp_x][tmp_y].from_y].cst_r;

                //finishing judge
                if(tmp_x==goal_x && tmp_y==goal_y){
                    break;
                }
            }

             /*
            for(i=0;i<n_i;i++){
                for(j=0;j<n_j;j++){
                    printf("%d ",maps[i][j].flag);
                }printf("\n");
            }printf("\n");
            // */

            //registe total cost
            total_cst[loop2] = maps[tmp_x][tmp_y].cst_r / (n_i+n_j);
            //printf("%d:%lf\n",loop2,total_cst[loop2]);

            fclose(fp2);
            //update file2 adress
            file2[23]++;
            if(file2[23]==':'){
                file2[23]='0';
                file2[22]++;
                if(file2[22]==':'){
                    file2[22]='0';
                    file2[21]++;
                }
            }
        }

        int min_cst_index=0;
        for(i=1;i<LOOP2;i++){
            if(total_cst[i]<total_cst[min_cst_index]){
                min_cst_index=i;
            }
        }
        if(min_cst_index==loop1){
            score++;
        }
        //printf("%d:%d\n",loop1,min_cst_index);

        fclose(fp1);
        //update file1 adress
        file1[23]++;
        if(file1[23]==':'){
            file1[23]='0';
            file1[22]++;
            if(file1[22]==':'){
                file1[22]='0';
                file1[21]++;
            }
        }
        
    }

    // 処理終了後のクロックを取得
    end_clock = clock();
    // 計測時間を表示
    printf("clock:%f\n",(double)(end_clock - start_clock) / CLOCKS_PER_SEC);

    //output
    printf("score:%d\n",score);
    return 0;
}