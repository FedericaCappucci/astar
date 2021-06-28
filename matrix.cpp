#include <iostream>
#include <math.h>
#include <time.h>
#include<cstdlib>
#include<fstream>
using namespace std;
using std::endl;

const int ROW=450;
const int COL=450;
int destR;
int destC;



//1 not obstacle 0 is obstacle
//int matrix[ROW][COL];
int *Matrix= (int*) calloc(ROW*COL,sizeof(int));

bool exi(int i,int c)
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
void generateMatrix()
{
	srand(time(NULL));
		for(int i=0;i<ROW;i++)
		{
		       for(int c=0;c<COL;c++)
		       {   		
		       		bool esci=true;
		       		do{
			       		int v2 = rand() % 100 + 1;
			       		bool valid = exi(i,c);
			       		if((v2>=0)&&(v2<70))
			       		{
			       			//then not obstacle
			       			if(valid==true)
			       				//my_file << "1" ;
			       				Matrix[i*ROW+c]=1;
			       		}
			       		else
			       		{
			       			//obstacle
			       			if(valid==true)
			       				Matrix[i*ROW+c]=0;
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
void writeFile()
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
			    my_file <<endl;
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

void printmatrix()
{
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL;c++)
	       {
	       		cout <<" "<<Matrix[i*ROW+c];
	       	
	       }
	    cout<<"\n";
	}
}
bool valida(int riga, int colonna)
{
	if(Matrix[riga*ROW+colonna]==1)
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
/*int main()
{
	generateMatrix();
	writeFile();
	return 0;
}*/


