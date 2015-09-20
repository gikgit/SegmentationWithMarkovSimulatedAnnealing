#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Function.h"


int main(int argc, char *argv[])
{
  int i,j,NB;
  float alpha = 5;
  float mean0,mean1,var0,var1,sum0,sum1,count0,count1,count2,count3,T,k,l,p0,p1;
  int length,width;

  float** y;  
  float** x;  
  float** temp_x;

  y=LoadImagePgm(argv[1],&length,&width);
  x=fmatrix_allocate_2d(length,width);
  temp_x=fmatrix_allocate_2d(length,width);

  srand(time(NULL));
  /* k-means */
  mean0=50; mean1=100; NB=0;

  do{
	sum1=0;sum0=0;
	count1=0;count0=0;

	k=mean0; l=mean1;
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(pow(y[i][j]-mean0,2)>pow(y[i][j]-mean1,2)){
				x[i][j]=1;
			}
			else if(pow(y[i][j]-mean0,2)<pow(y[i][j]-mean1,2)){
				x[i][j]=0;
			}
			else{
				x[i][j]=(0||1);
			}
		}
	}		
		
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(x[i][j]==1){
				sum1=sum1+y[i][j];
				count1++;
			}
			else {
				sum0=sum0+y[i][j];
				count0++;
			}
		}
	}

	NB++;
	mean0=sum0/count0; mean1=sum1/count1;
		 
    }while(mean1!=l||mean0!=k);

    sum1=0;sum0=0;count1=0;count0=0; 

    for(i=0;i<length;i++){
	  for(j=0;j<width;j++){
		if(x[i][j]==1){
			sum1=sum1+pow(y[i][j]-mean1,2);
			count1++;	
		}
		if(x[i][j]==0){
			sum0=sum0+pow(y[i][j]-mean0,2);
			count0++;
		}	
	  }
     }	
	   	
     var1=sum1/count1; var0=sum0/count0;
		   
     /* simulated annealing*/
     float probAcc,t,t_init=50;
     int m;
    
     for(m=0;;m++){   

	 
     	 t=t_init*powf(0.99,m);
     	 if(t<0.1)  break;
	
	 for(i=1;i<length-1;i++){
		for(j=1;j<width-1;j++){
			p1=exp(-1*(1/t)*(-1*log(funcgauss(y[i][j],mean1,var1))+alpha*U(i,j,1,x)));
			p0=exp(-1*(1/t)*(-1*log(funcgauss(y[i][j],mean0,var0))+alpha*U(i,j,0,x)));

			if(p1>p0){
				x[i][j]=1;
			}
			if(p1<p0){
				x[i][j]=0;
			}
		}
	 }
 
        
         sum1=0;
	 for(i=1;i<length-1;i++){
		for(j=1;j<width-1;j++){
			if(x[i][j]==1){
				sum1=sum1-1*log(funcgauss(y[i][j],mean1,var1))+alpha*U(i,j,1,x);	
			}	
			else{
				sum1=sum1-1*log(funcgauss(y[i][j],mean0,var0))+alpha*U(i,j,0,x);
			}
		}
	 }

	 if(!(m%10)) printf("Energy=%f\n",sum1);
    }
		
 
    Recal(x,length,width);
    SaveImagePgm("image_name",x,length,width);

    system("display image_name.pgm&"); 
  
    /*retour sans probleme*/ 
    printf("\n Ending ... \n\n\n");

    return 0;
	
}

