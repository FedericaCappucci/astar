#include <iostream>
using namespace std;

const int ROW=301;
const int COL=301;
int destR;
int destC;

const int ROW=200;
const int COL=200;

//1 not obstacle 0 is obstacle
//int matrix[ROW][COL];
int *matrix= (int*) calloc(ROW*COL,sizeof(int));
bool exi(int i,int c)
{
	if((i>0)&(c>0))
	{

		if((matrix[(i-1)*ROW+c]==0)||(matrix[i*ROW+(c-1)]==0)||(matrix[(i+1)*ROW+(c-1)]==0)||(matrix[(i-1)*ROW+(c-1)]==0))

		if((matrix[(i-1)*ROW+c]==0)&&(matrix[i*ROW+(c-1)]==0)&&(matrix[(i+1)*ROW+(c-1)]==0)&&(matrix[(i-1)*ROW+(c-1)]==0))

		{
			
			return false;
		}
	}
	return true;
}
int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}
void generateMATRIX()
{
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<=COL;c++)
	       {   		
	       		bool esci=true;
	       		do{
		       		int v2 = rand() % 100 + 1;
		       		if((v2>=0)&(v2<50))
		       		{
		       			//then not obstacle
		       			matrix[i*ROW+c]=1;
		       			
		       		}
		       		else
		       		{
		       			//obstacle
		       			matrix[i*ROW+c]=0;
		       		}
		       		exi(i,c);
		       	}
		       	while(esci==false);
	       }
	}
}

void printmatrix()
{
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL;c++)
	       {
	       		cout <<" "<<matrix[i*ROW+c];
	       	
	       }
	     
	}
}
bool valida(int riga, int colonna)
{
	if(matrix[riga*ROW+colonna]==1)
	{	
		return true;
	}
	return false;
}
void generateDest()
{
	int riga;
	int colon;
	do{
		riga = random_int(ROW-3,ROW);
		colon = random_int(COL-3,COL);
		
	}
	while(valida(riga,colon)!=true);
	destR=riga; 
	destC=colon;
}



