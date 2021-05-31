#include <iostream>
using namespace std;
const int ROW=100;
const int COL=100;
//1 not obstacle 0 is obstacle
int matrix[ROW][COL];
bool exi(int i,int c)
{
	if((i>0)&(c>0))
	{
		if((matrix[i-1][c]==0)||(matrix[i][c-1]==0)||(matrix[i+1][c-1]==0)||(matrix[i-1][c-1]==0))
		{
			
			return false;
		}
	}
	return true;
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
		       			matrix[i][c]=1;
		       			
		       		}
		       		else
		       		{
		       			//obstacle
		       			matrix[i][c]=0;
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
	       		cout <<" "<<matrix[i][c];
	       	
	       }
	     
	}
}

