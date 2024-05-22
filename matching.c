#include <stdio.h>
#define MAX_N 140
#define MAX_CSTLIST MAX_N*MAX_N*3
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
    int drec; //0 means right, 1 means down, 2 means down right 
}COST;


int main(){
    //variable initialize
    FILE *fph,*fpt;
    char name1[20],content1[20];
    char name2[20],content2[20];
    int i,j,k,n_i,n_j,max_n=0;
    int loop1,loop2;
    char file1[30]="data/city021/city021_001.txt";
    char file2[30]="data/city011/city011_001.txt";
    double I[MAX_N][15],J[MAX_N][15];
    COST cstlist[MAX_CSTLIST];
    NODE maps[MAX_N][MAX_N];
    
    //Loop1
    for(loop1=0;loop1<1;loop1++){
        //data scan1
        if((fph = fopen(file1,"r"))==NULL){
		    printf("Can not open txte file\n");
		    return -1;
	    }
        fscanf(fph,"%s",name1);
        fscanf(fph,"%s",content1);
        fscanf(fph,"%d",&n_i);
        //printf("n_i:%d\n",n_i);
        for(i=0;i<n_i;i++){
            for(j=0;j<15;j++){
                fscanf(fph,"%lf", &I[i][j]);
            }
	    }

        //Loop2
        for(loop2=0;loop2<10;loop2++){
            //data scan2
            if((fpt = fopen(file2,"r"))==NULL){
		        printf("Can not open txte file\n");
		        return -1;
	        }
            fscanf(fpt,"%s",name2);
            fscanf(fpt,"%s",content2);
            fscanf(fpt,"%d",&n_j);
            //printf("n_j:%d\n",n_j);
            for(i=0;i<n_j;i++){
                for(j=0;j<15;j++){
                    fscanf(fpt,"%lf",&J[i][j]);
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
                }
            }
            
            //A* algorithm
            int cl_i=0;
            int loop_counter=0;
            int tmp_x=0,tmp_y=0;
            int goal_x=n_i-1,goal_y=n_j-1;
            while(tmp_x!=goal_x||tmp_y!=goal_y){
                //calcualte cost:real and virtual
                maps[tmp_x][tmp_y].flag=0;
                maps[tmp_x][tmp_y].cst_r = ((tmp_x-maps[tmp_x][tmp_y].from_x)+(tmp_y-maps[tmp_x][tmp_y].from_y))*maps[tmp_x][tmp_y].data + maps[maps[tmp_x][tmp_y].from_x][maps[tmp_x][tmp_y].from_y].cst_r;
                if(tmp_x==goal_x){
                    maps[tmp_x][tmp_y].cst_s[0] = 0;
                    maps[tmp_x][tmp_y].cst_s[1] = maps[tmp_x][tmp_y].cst_r + maps[tmp_x][tmp_y+1].data + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                    maps[tmp_x][tmp_y].cst_s[2] = 0;
                }else if(tmp_y==goal_y){
                    maps[tmp_x][tmp_y].cst_s[0] = maps[tmp_x][tmp_y].cst_r + maps[tmp_x+1][tmp_y].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1));
                    maps[tmp_x][tmp_y].cst_s[1] = 0;
                    maps[tmp_x][tmp_y].cst_s[2] = 0;
                }else{
                    maps[tmp_x][tmp_y].cst_s[0] = maps[tmp_x][tmp_y].cst_r + maps[tmp_x+1][tmp_y].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1)) + (goal_y-(tmp_y))*(goal_y-(tmp_y));
                    maps[tmp_x][tmp_y].cst_s[1] = maps[tmp_x][tmp_y].cst_r + maps[tmp_x][tmp_y+1].data + (goal_x-(tmp_x))*(goal_x-(tmp_x)) + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                    maps[tmp_x][tmp_y].cst_s[2] = maps[tmp_x][tmp_y].cst_r + 2*maps[tmp_x+1][tmp_y+1].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1)) + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                }
                //register cost for costlist
                for(i=0;i<3;i++){
                    cstlist[cl_i].cst = maps[tmp_x][tmp_y].cst_s[i];
                    cstlist[cl_i].drec = i;
                    cstlist[cl_i].x = tmp_x + (i!=1 ? 1:0);
                    cstlist[cl_i].y = tmp_y + (i!=0 ? 1:0);
                    cl_i++;
                }
                //search min cost in cstlist
                int min_index=-1;
                for(i=0;i<cl_i;i++){
                    if(cstlist[i].cst>0.0001){
                        if(min_index==-1) min_index=i;
                        if(cstlist[i].cst < cstlist[min_index].cst){
                            min_index=i;
                        }
                    }
                    //printf("%d %lf\n",i,cstlist[i].cst);
                }
                //printf("%d:%d  x:%d y:%d\n",loop_counter++,min_index,tmp_x,tmp_y);
                //handover
                cstlist[min_index].cst = 0;
                maps[cstlist[min_index].x][cstlist[min_index].y].from_x = tmp_x;
                maps[cstlist[min_index].x][cstlist[min_index].y].from_y = tmp_y;
                tmp_x = cstlist[min_index].x;
                tmp_y = cstlist[min_index].y;
            }
            //calculate all cost
            tmp_x = goal_x;
            tmp_y = goal_y;
            double total_cst;
            maps[tmp_x][tmp_y].cst_r = ((tmp_x-maps[tmp_x][tmp_y].from_x)+(tmp_y-maps[tmp_x][tmp_y].from_y))*maps[tmp_x][tmp_y].data + maps[maps[tmp_x][tmp_y].from_x][maps[tmp_x][tmp_y].from_y].cst_r;
            total_cst = maps[tmp_x][tmp_y].cst_r;
            printf("total cst%d:%lf\n",loop2,total_cst);

            fclose(fpt);
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

        fclose(fph);
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

        //output
        for(i=0;i<n_i;i++){
            for(j=0;j<n_j;j++){
                //printf("%d ",maps[i][j].flag);
            }
            //printf("\n");
        }
    }
    return 0;
}