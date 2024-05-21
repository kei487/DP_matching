#include <stdio.h>
#define MAX_N 140
typedef struct{
    int data;
    int from_x,from_y;
    int flag;
    double scr_r,scr_d,scr_rd; 
}MAP;



int main(){
    //variable initialize
    FILE *fph,*fpt;
    char name1[20],content1[20];
    char name2[20],content2[20];
    int i,j,k,n_i,n_j,max_n=0;
    int loop1,loop2;
    char fail1[30]="data/city021/city021_001.txt";
    char fail2[30]="data/city011/city011_001.txt";
    double I[MAX_N][15],J[MAX_N][15];
    MAP maps[MAX_N][MAX_N];

    //Loop1
    for(loop1=0;loop1<1;loop1++){
        //data scan1
        if((fph = fopen(fail1,"r"))==NULL){
		    printf("Can not open txte file\n");
		    return -1;
	    }
        fscanf(fph,"%s",name1);
        fscanf(fph,"%s",content1);
        fscanf(fph,"%d",&n_i);
        for(i=0;i<n_i;i++){
            for(j=0;j<15;j++){
                fscanf(fph,"%lf", &I[i][j]);
            }
	    }

        //Loop2
        for(loop2=0;loop2<1;loop2++){
            //data scan2
            if((fpt = fopen(fail2,"r"))==NULL){
		        printf("Can not open txte file\n");
		        return -1;
	        }
            fscanf(fpt,"%s",name2);
            fscanf(fpt,"%s",content2);
            fscanf(fpt,"%d",&n_j);
            printf("%d",n_j);
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
                    maps[i][j].flag=0;
                }
            }
            int tmp_x=0,tmp_y=0;
            int goal_x=n_i-1,goal_y=n_j-1;
            while(tmp_x!=goal_x||tmp_y!=goal_y){
                if(tmp_x==goal_x){
                    maps[tmp_x][tmp_y].scr_d = maps[tmp_x][tmp_y+1].data + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                }else if(tmp_y==goal_y){
                    maps[tmp_x][tmp_y].scr_r = maps[tmp_x+1][tmp_y].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1));
                }else{
                    maps[tmp_x][tmp_y].scr_d = maps[tmp_x][tmp_y+1].data + (goal_x-(tmp_x))*(goal_x-(tmp_x)) + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                    maps[tmp_x][tmp_y].scr_r = maps[tmp_x+1][tmp_y].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1)) + (goal_y-(tmp_y))*(goal_y-(tmp_y));
                    maps[tmp_x][tmp_y].scr_rd = 2*maps[tmp_x+1][tmp_y+1].data + (goal_x-(tmp_x+1))*(goal_x-(tmp_x+1)) + (goal_y-(tmp_y+1))*(goal_y-(tmp_y+1));
                }
            }

            fclose(fpt);
            //update fail2 adress
            fail2[23]++;
            if(fail2[23]==':'){
                fail2[23]='0';
                fail2[22]++;
                if(fail2[22]==':'){
                    fail2[22]='0';
                    fail2[21]++;
                }
            }
        }

        fclose(fph);
        //update fail1 adress
        fail1[23]++;
        if(fail1[23]==':'){
            fail1[23]='0';
            fail1[22]++;
            if(fail1[22]==':'){
                fail1[22]='0';
                fail1[21]++;
            }
        }

        //output
        for(i=0;i<n_i;i++){
            for(j=0;j<n_j;j++){
                printf("%5.5lf ",maps[i][j].data);
            }
            printf("\n");
        }
    }
    return 0;
}