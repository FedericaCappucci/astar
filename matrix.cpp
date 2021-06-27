#include <iostream>
#include <math.h>
#include <time.h>
#include<cstdlib>
#include<fstream>
using namespace std;
using std::endl;

const int ROW=10;
const int COL=10;
int destR;
int destC;



//1 not obstacle 0 is obstacle
//int matrix[ROW][COL];
int *matrix= (int*) calloc(ROW*COL,sizeof(int));

bool exi(int i,int c)
{
	if(i!=0)
	{
		if((matrix[(i-1)*ROW+c]==0)&&(matrix[i*ROW+(c-1)]==0)&&(matrix[(i+1)*ROW+(c-1)]==0)&&(matrix[(i-1)*ROW+(c-1)]==0))
			//if((matrix[(i-1)*ROW+c]==0)||(matrix[i*ROW+(c-1)]==0)||(matrix[(i+1)*ROW+(c-1)]==0)||(matrix[(i-1)*ROW+(c-1)]==0))
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
void generateMatrix()
{
	srand(time(NULL));
	ofstream my_file("Matrix.txt");
    	
	if (my_file.is_open())
  	{
		for(int i=0;i<ROW;i++)
		{
		       for(int c=0;c<=COL;c++)
		       {   		
		       		bool esci=true;
		       		do{
			       		int v2 = rand() % 100 + 1;
			       		bool valid = exi(i,c);
			       		if((v2>=0)&&(v2<70))
			       		{
			       			//then not obstacle
			       			if(valid==true)
			       				my_file << "1" ;
			       				//matrix[i*ROW+c]=1;
			       		}
			       		else
			       		{
			       			//obstacle
			       			if(valid==true)
			       				//matrix[i*ROW+c]=0;
			       				my_file << "0";
			       		}
			       		//exi(i,c);
			       	}
			       	while(esci==false);
		       }
		       my_file <<"\n" <<endl;
		}
		my_file.close();
	}
	else
	{
		cout<<"cant open the file";	
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
	srand(time(NULL));
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
int main()
{
	generateMatrix();
	return 0;
}


