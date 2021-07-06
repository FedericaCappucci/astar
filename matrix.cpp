#include <iostream>
#include <math.h>
#include <time.h>
#include<cstdlib>
#include<fstream>
using namespace std;
using std::endl;

//This will contain the generated grid 
int *Matrix;
/*
	generateMatrix generates a grid by filling it with 1(not obstacle)with a probability of 75% and 0(obstacle) with probability of 15%
	INPUTS: 
		-int row: number of rows of the generated grid
		-int col: number of columns of the generated grid
*/
void generateMatrix(int row,int col)
{
	Matrix = (int*) calloc(row*col,sizeof(int));
	srand(time(NULL));
		for(int i=0;i<row;i++)
		{
		       for(int c=0;c<col;c++)
		       {   		
				int v2 = rand() % 100 + 1;
				if((v2>=0)&&(v2<75))
				{
			       		//then not obstacle
			       		Matrix[i*row+c]=1;
			       	}
			       	else
			       	{
			       		//obstacle
			       		Matrix[i*row+c]=0;
			       	}      
		       }		       
		}
}

/*
	writeFile writes in "Matrix.txt" the grid that was generated before
	INPUTS:
		-int ROW: number of rows of the generated grid we write in the file
		-int COL: number of columns of the generated grid in the file
*/
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

/*int main()
{
	generateMatrix(4000,4000);
	writeFile(4000,4000);
	return 0;
}*/


