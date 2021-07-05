#include <list>
#include <iostream>
#include <set>
#include <math.h>
#include "matrix.cpp"
#include <omp.h>

using namespace std;


const int NUM_MAX_THREAD=8;

double startTime;
double endTime;
int ROW;
int COL;
//This will contain the grid read from the file "Matrix.txt"
int *matrix;

struct Node
{
    int Nrow,Ncol;
    double g,h;
    int Prow,Pcol;
    bool isObstacle = false;
    public:
	    bool operator<(const Node& other )const
	    {
	    	if((Ncol==other.Ncol)&&(Nrow==other.Nrow))
	    	{
	    		return false;
			}
			else
			{
				if((g+h)!=(other.g+other.h))
				{
					return (g+h)<(other.g+other.h);			
				}
				else
				{
					return true;
				}
			}
	    }
	   
};
void readPath(string nomeFile)
{
	char c;
	int num;
	int i=0;
	int j=0;
	ifstream fin(nomeFile);
	
	while(fin.get(c))
	{
		if(c != '\n')
		{
			num= (int) c-48;
			matrix[i*ROW + j] = num;
			j++;
		}
		else
		{
			i++;
			j=0;
		}
		
		
		
	}
	fin.close();
	
}

int Search(Node& toSearch, set<Node> l)
{
	set<Node>::iterator it;
	
	int i=0;
	int find=-1;
	//#pragma omp parallel
	//{
		/*cout<<"Thread: " << omp_get_thread_num(); ;
		cout<<"\n";*/
			
		for(it=l.begin();it!=l.end();++it)
		{	
	//		#pragma omp task
	//		{
				Node c = *it;	
				if((c.Ncol==toSearch.Ncol)&&(c.Nrow==toSearch.Nrow))
				{
					find=i;
				}
				i++;
	//		}
		}
	//}
	//double end=omp_get_wtime();
	//cout<<"ricerca: " << end-startTime <<"\n";
	return find;
}
int Search(Node& toSearch, list<Node> l)
{
	list<Node>::iterator it;
	
	int i=0;
	int find=-1;
	//task
	
		for(it=l.begin();it!=l.end();++it)
		{	
			//#pragma omp task
			
				Node c = *it;	
				if((c.Ncol==toSearch.Ncol)&&(c.Nrow==toSearch.Nrow))
				{
					return i;
				}
				i++;
			
		}
	
	return -1;
}

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return ((row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL)) &&(matrix[row*ROW+col]!=0); //if matrix[][] =0 the is an obstacle
}
bool isDestination(Node succ,Node dest) 
{ 
	if (succ.Ncol == dest.Ncol && succ.Nrow == dest.Nrow) 
		return true; 
	else
		return false; 
} 

//euclide
double heuristic(Node* current_cell,Node *goal)
{

    
	//double i=sqrt((goal->Nrow-current_cell->Nrow)*(goal->Nrow-current_cell->Nrow)+(goal->Ncol-current_cell->Ncol)*(goal->Ncol-current_cell->Ncol));
	//manhattan 
	double i=abs(current_cell->Nrow-goal->Nrow)+abs(current_cell->Ncol-goal->Ncol);
	return i;
}
void printPath(list<Node> closedList,Node start)
{
	//list<Node>::iterator it;
/*	int cont=0;
	for(it=closedList.begin();it!=closedList.end();++it)
	{
		Node c;
		Node prev;
		
		c = *it;	
		cout << "(" << c.Nrow << "," << c.Ncol <<") ;"<< "(" << c.Prow << "," << c.Pcol <<") ->";
		
	
		
	}*/

	
	Node c=closedList.back();;
	
	list<Node>::iterator i;
	//#pragma omp parallel
	//{
	// task ??
	for(i=closedList.end();i!=closedList.begin();--i)
	{
		
		cout << "(" << c.Nrow << "," << c.Ncol <<") ->";
		if((c.Nrow==start.Nrow)&(c.Ncol==start.Ncol))
		{
			break;
		}
		//#pragma omp task
		//{
		Node a;
		a.Ncol=c.Pcol;
		a.Nrow=c.Prow;
		int pos=0;
		pos=Search(a,closedList);
		list<Node>::iterator beg;
		beg=closedList.begin();
		advance(beg,pos);
		c=*beg;		
	//}
	
	}
	//}
	//double end=omp_get_wtime();
	//cout<<"stampa: " << end-startTime <<"\n";
	
	
}
int a_star(Node *start, Node *destination)
{
    set<Node> openList;
    list<Node> closedList;
    
    start->g=0.0;
    start->h=heuristic(start,destination);
    
   cout <<"dest" <<destination->Ncol;
    
	openList.insert(*start);
	//closedList.push_back(*start);
	list<Node>::iterator it=closedList.begin();
	Node c =*it; 
	
    //to create successors
	int vc, vr;
    
    int dy[8] = {0, 1, 0, -1,1,-1,-1,1}; //row
    int dx[8] = {-1, 0, 1, 0,1,-1,1,-1}; //col
    bool esci=false;

    while((!openList.empty())||(esci!=true)) //per parallelizzazione meglio for
    {
    	
    	
        Node current = *openList.begin();
        openList.erase(openList.begin());
        closedList.push_back(current);
        
        cout<<"nodo corrente"<<current.Ncol<<" " <<current.Nrow<<"\n";
        //static threads
        omp_set_num_threads(16);
        #pragma omp parallel for schedule(static) shared(openList,closedList) private(vc,vr,current)
        for(int pind = 0; pind<8; pind++) //4 position possible 
		{
		    cout<<"\n Thread astar_main: " << omp_get_thread_num(); ;
		
            		vr = current.Nrow + dy[pind]; //row
			vc = current.Ncol + dx[pind]; //col
            
            
		    if(isValid(vr,vc))
	        {
	            Node successor;
	            successor.g=current.g+1;
	            successor.Nrow=vr;
	            successor.Ncol=vc;
	            successor.h=heuristic(&successor,destination);
   
	            cout<<"nodo successor"<<successor.Ncol<<" " <<successor.Nrow<<"\n";
	            if(isDestination(successor,*destination))
	            {
	            	cout<<"arrivo\n";
	            	
	            	successor.Prow=current.Nrow;
	            	successor.Pcol=current.Ncol;
	            	#pragma omp critical (closedList)
	            	{
	            		closedList.push_back(successor);
	            		openList.erase(openList.begin(),openList.end());
	            		
	            		esci=true;
	            		//return 0;
	            	}
		     }
		     else
				{
				
		            if(Search(successor,openList)!=-1) //node is already in OpenList. This function return the displacement for the iterator
		            {
		            	int disp=0;
		            	
		            	//startTime=omp_get_wtime();
		            	
		            	disp=Search(successor,openList);
		            	cout<<"trovato in openList\n";
		            	set<Node>::iterator it=openList.begin(); 
		            	advance(it,disp);
		            	Node app= *it;	 
		            	if((app.g+app.h)>(successor.g+successor.h))
		            	{
		           		#pragma omp critical (openList)
	            			{
				    		openList.erase(it); // debugger
				    		
				    		successor.Prow=successor.Nrow-dy[pind];
				    		successor.Pcol=successor.Ncol-dx[pind];
				    		openList.insert(successor);
					}
				}
			    }	
			    if(Search(successor,closedList)!=-1) //node is already in ClosedList. This function return the displacement for the iterator
		            {
		            	int disp=0;
		            	disp=Search(successor,closedList);
		            	
		            	cout <<"trovato in closed list\n";
		            	list<Node>::iterator it=closedList.begin(); 
		            	
		            	advance(it,disp);
		            	Node app= *it;	 
		            	if((app.g+app.h)>(successor.g+successor.h))
		            	{
		            		#pragma omp critical (notInList)
	            			{
		            			closedList.erase(it); //debugger
		            		
		            			successor.Prow=successor.Nrow-dy[pind];
		            			successor.Pcol=successor.Ncol-dx[pind];
		            			closedList.push_back(successor);
							}
				}
			    }
			    else
				{
						
					successor.Prow=current.Nrow;
					successor.Pcol=current.Ncol;
		            	
		            //	closedList.insert(successor);
		            		#pragma omp critical (inserimento)
		            		openList.insert(successor);
				}
			}
	    	}
	    	else
	    	{
	    		cout<<"non valido";
		}
	    }
    }
    endTime=omp_get_wtime();
    cout<<"\n\ntempo:" << endTime-startTime;
    if(esci==true)
    {
    	printPath(closedList,*start);
    }
    else
    {
   	cout<<"can't reach the destination";
    }

}
void printmatrix2()
{
	//#pragma omp parallel for schedule(dynamic)
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL;c++)
	       {
	       		cout <<" "<<matrix[i*ROW+c];
	       	
	       }
	       
	   cout<<"\n";  
	}
	cout<<"\n";
	//endTime=omp_get_wtime();
	
}

/* Inputs from command line:
	-argv[1] number of rows in your grid
	-argv[2] number of columns in your grid
	-argv[3] t: if you want to generate a new matrix or f: if you want to read it from file named "Matrix.txt"
	-argv[4] row of start Node
	-argv[5] column of start Node 
	-argv[6] row of destination Node
	-argv[7] column of destination Node
	-argv[8] number of thread that you want to use 0 < n <=8 
*/
int main(int argc, char * argv[])
{
	//generateMATRIX();
//	printmatrix();
	//generatematrix();
	//set_new_handler(no_memory);
	if (argc >= 8) 
	{
		if ((atoi(argv[8]) <= 0)||(atoi(argv[8])>NUM_MAX_THREAD))
			{
		    cout<<"Not a valid number for threads.Please set a value between o and " << NUM_MAX_THREAD <<"\n";
		    return 5;
		}
		
		//if number of threads is valid then set the parallel regions threads.
		omp_set_dynamic(0); // Explicitly disable dynamic teams
	    	omp_set_num_threads(atoi(argv[8])); // Use N threads for all parallel regions
	    	
	    	//set column and row of your grid: (the one you read or the one you wanto to generate)
	    	if((atoi(argv[1])<=0)&&(atoi(argv[2])<=0))
		{
			cout<<"Rows and columns numbers not valid!\n";
			return 5;
		}
		ROW=atoi(argv[1]);
		COL=atoi(argv[2]);
		matrix= (int*) calloc(ROW*COL,sizeof(int));
		
		//read matrix choice
		if(argv[3]=="f")
		{
			readPath("Matrix.txt");
			cout << "File read!\n";
		}
		else
		{
			generateMatrix(ROW,COL);
			writeFile(ROW,COL);
			readPath("Matrix.txt");
		}
		
    	
		// set start and destination node: 
		Node start;
		Node dest;
	
		//start.Nrow=0;
		//start.Ncol=2; //7 for 5000x5000 matrix 6  for 3000x3000 2, 2 for 1000x1000
		
		//checks if the start node is valid
		if(isValid(atoi(argv[4]),atoi(argv[5]))==false)
		{
			cout <<"Invalid start";
			return 5;
		}
		start.Nrow= atoi(argv[4]);
		start.Ncol= atoi(argv[5]);
		
		
		//Destination: 
	
		//4999 for 5000x5000 matrix 2440  then 2999, 999 for 1000x1000, 1999 for 2000x2000
		//4998 for 5000x5000 matrix 2445 then 2999, 998 for 1000x1000,1999 for 2000x2000
	
		//cout<< "\ndestinazione : " << dest.Nrow <<" " <<dest.Ncol<<"\n"; 
		
		//checks if the destination node is valid
		if(isValid(atoi(argv[6]),atoi(argv[7]))==false)
		{
			cout <<"Invalid destination";
			return 5;
		}
		dest.Nrow= atoi(argv[6]);
		dest.Ncol= atoi(argv[7]);
		
		a_star(&start,&dest);
		free(matrix);
	//	cout<<"stampa: " << endTime-startTime <<"\n";
		return 0;
	
	}
}
