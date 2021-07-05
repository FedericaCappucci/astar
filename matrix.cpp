#include <iostream>
#include <math.h>
#include <time.h>
#include<cstdlib>
#include<fstream>
using namespace std;
using std::endl;

/*const int ROW=4000;
const int COL=4000;
int destR;
int destC;*/



//1 not obstacle 0 is obstacle
//int matrix[ROW][COL];
int *Matrix;

bool exi(int i,int c,int ROW)
{
	if(i!=0)
	{
		if((Matrix[(i-1)*ROW+c]==0)&&(Matrix[i*ROW+(c-1)]==0)&&(Matrix[(i+1)*ROW+(c-1)]==0)&&(Matrix[(i-1)*ROW+(c-1)]==0))
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
void generateMatrix(int row,int col)
{
	Matrix = (int*) calloc(row*col,sizeof(int));
	srand(time(NULL));
		for(int i=0;i<row;i++)
		{
		       for(int c=0;c<col;c++)
		       {   		
		       		bool esci=true;
		       		do{
			       		int v2 = rand() % 100 + 1;
			       		cout<< "generate : " << v2 <<"\n";
			       		//bool valid = exi(i,c,row);
			       		if((v2>=0)&&(v2<75))
			       		{
			       			//then not obstacle
			       			//if(valid==true)
			       				//cout<<"uno: ";
			       				//my_file << "1" ;
			       				Matrix[i*row+c]=1;
			       				//cout<<Matrix[i*row+c]<<"\n";
			       		}
			       		else
			       		{
			       			//obstacle
			       			//if(valid==true)
			       				//cout<<"zero: ";
			       				Matrix[i*row+c]=0;
			       				//cout<<Matrix[i*row+c]<<"\n";
			       				//my_file << "0";
			       		}
			       		//exi(i,c);
			       	}
			       	while(esci==false);
		       }
		       //my_file << endl;
		}
		//my_file.close();
	
}
void writeFile(int ROW,int COL)
{
	ofstream my_file("Matrix.txt",std::ofstream::out);
	//exception handling
  try {
		if (my_file.is_open())
	  	{
	  		for(int i=0;i<ROW;i++)
			{
				for(int c=0;c<COL;c++)
			    {
			    	my_file << Matrix[i*ROW+c];
			    }
			    if(i<ROW-1)
			    {
			    	my_file <<endl;
			    }
			}
			my_file.close();
	  	}
	  	else
	  	{
	  		cout<<"cant open the file";	
		}
	}
	catch (const char* msg) {
    cerr << msg << endl;
  }
	
}

/*void printmatrix()
{
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL1;c++)
	       {
	       		cout <<" "<<Matrix[i*ROW1+c];
	       	
	       }
	    cout<<"\n";
	}
}*/
bool valida(int riga, int colonna,int ROW)
{
	if(Matrix[riga*ROW+colonna]==1)
	{	
		return true;
	}
	return false;
}
/*void generateDest()
{
	srand(time(NULL));
	int riga;
	int colon;
	do{
		riga = random_int(ROW1-3,ROW1);
		colon = random_int(COL1-3,COL1);
		
	}
	while(valida(riga,colon)!=true);
	destR=riga; 
	destC=colon;
}*/
/*int main()
{
	generateMatrix(4000,4000);
	writeFile(4000,4000);
	return 0;
}*/


