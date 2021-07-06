#include <list>
#include <iostream>
#include <cstdlib>
#include <set>
#include<fstream>
#include <math.h>
#include "matrix.cpp"
#include <omp.h>
#include <new>

const int NUM_MAX_THREAD=8;


using namespace std;


double startTime;
double endTime;
int ROW;
int COL;
//This will contain the grid read from the file "Matrix.txt"
int *matrix;


/* Contains all the information about the element of the matrix:
	-Nrow,Ncol are int values that rappresent the number of column and row of each Node
	- g is the cost to move from the source node to a given node
	-h is heuristic function computed by "Euclide" method 
	-Prow,Pcol are int values that represents the column and the row of the previous node (The one that generates the current one)
	
	Note that: the cost function f is not defined because the information was reduntant as it can be easly obtained by the sum of g and h.
*/
struct Node
{
    int Nrow,Ncol;
    double g,h;
    int Prow,Pcol;
    

    public:
    //This is needed to compare 2 nodes using their cost function f
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
					return 			  (g+h)<(other.g+other.h);			
				}
				else
				{
					return true;
				}
			}
	    }
	    //needed to free the memory after the path is found
	    static void operator delete(void* ptr, std::size_t sz)
    	    {
        		//cout << "custom delete for size " << sz <<endl;
        		delete (ptr); // ::operator delete(ptr) can also be used
    	    }
	
	   
};
/* 
 This struct will contain:
	-The number of thread that have founded the path
	-the cost of the path founded if there is one.
*/

// This function will be called when the memory is not enough to allocate dynamically space (usefull only with "new" operator)
void no_memory()
{
	cout<<"not enough memory";
	exit (1);
}

/*
	Overloaded function
	Search find the only occurrance of a given node into a set.
	Notice that a Node can be contained in the set just one time(no repetition allowed)
	INPUTS: 
		-Node toSearch: the starting node to find neighbours
		-set<Node> l: the set from which search
	OUTPUT:
		-int i: displacement of the found element in the set
*/
int Search(Node& toSearch, set<Node> l)
{
	set<Node>::iterator it;
	
	int i=0;
	for(it=l.begin();it!=l.end();++it)
	{	
		Node c = *it;	
		if((c.Ncol==toSearch.Ncol)&&(c.Nrow==toSearch.Nrow))
		{
			return i;
		}
		i++;
	}
	return -1;
}
/*
	Overloaded function
	Search find the first occurrance of a given node into a list.
	INPUTS: 
		-Node toSearch: the starting node to find neighbours
		-list<Node> l: the list from which search
	OUTPUT:
		-int i: displacement of the found element in the list
*/
int Search(Node& toSearch, list<Node> l)
{
	list<Node>::iterator it;
	
	int i=0;
	for(it=l.begin();it!=l.end();++it)
	{	
		Node c = *it;	
		if((c.Ncol==toSearch.Ncol)&&(c.Nrow==toSearch.Nrow))
		{
			return i;
		}
		i++;
	}
	return -1;
}

/*
	setNeighbours find all valid neighbours of a given node where valid means the neighbour is not a blocking node or out of grid range.
	INPUTS: 
		-Node current: the starting node to find neighbours
		-int c: a int variable to store the number of found neighbour
	OUTPUT:
		-neighbours_array: array of nodes which contains the valid neighbours
*/
Node * setNeighbours(Node current, int *c)
{

    Node * neighbours_array = new Node[8];
	int count=0;
	int r= current.Nrow -1;
	int col= current.Ncol -1;
	for(int i = r; i<=r+2; i++)
	{
		for (int j = col; j<= col+2; j++) {
			if((i>=0 && i<COL) && (j >= 0 && j< ROW)) {
				if(!(i == current.Nrow && j == current.Ncol)&& matrix[i * ROW + j] ==1)
				{
					neighbours_array[count].Nrow=i;
					neighbours_array[count].Ncol=j;
					
					//*(neighbours+count)=tmp;
					
					//cout <<"\n boh "<<neighbours_array[count].Nrow<<"  "<<neighbours_array[count].Ncol <<"\n";
					count += 1;
				}
			}
		}
	}
	*c= count;
	//array_neigh = neighbours_array;
	return neighbours_array;	
}

/*
	isValid: an utility function to check whether given cell (row, col) is a valid cell or not.
	INPUTS:
		-int row: row number to check
		-int col: col number to check
	OUTPUTS:
		- true if the node is valid
		- false if is not.
*/
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return ((row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL)) &&(matrix[row*ROW+col]!=0); //if matrix[][] =0 the is an obstacle
}

/*
	isDestination check if the given node is equal to the destination (in terms of row and col number).
	INPUTS: 
		-Node succ: the node to check
		-Node dest: the destination 
	OUTPUT:
		-true if the succ col and row are equal to the destination col and row
		-false if not.
*/
bool isDestination(Node succ,Node dest) 
{ 
	if (succ.Ncol == dest.Ncol && succ.Nrow == dest.Nrow) 
		return true; 
	else
		return false; 
} 
/*
	heuristic: compute the heuristic function with Euclidean formula given the node we are visiting node and the fixed destination.
	INPUTS: 
		-Node* current_cell: pointer to the node from where we want to compute heuristic
		-Node* goal: pointer to the final node we want to reach
	OUTPUT:
		-double i: result value of heuristic function
*/
double heuristic(Node* current_cell,Node *goal)
{

	double i=sqrt((goal->Nrow-current_cell->Nrow)*(goal->Nrow-current_cell->Nrow)+(goal->Ncol-current_cell->Ncol)*(goal->Ncol-current_cell->Ncol));
	//manhattan 
	//double i=abs(current_cell->Nrow-goal->Nrow)+abs(current_cell->Ncol-goal->Ncol);
	return i;
}
/*
	printPath: prints on screen the path found between the start node and destination node.
	in order to print the path, it searches in the ClosedList starting from the last element on the list and proceding badckwords printing the parent node until it reaches the start.
	INPUTS: 
		-list<Node> closedList: list which contains all the visited Nodes
		-Node start: initial node.
*/
void printPath(list<Node> closedList,Node start)
{
	Node c=closedList.back();;
	
	list<Node>::iterator i;
	for(i=closedList.end();i!=closedList.begin();--i)
	{
		
		cout << "(" << c.Nrow << "," << c.Ncol <<") ->";
		if((c.Nrow==start.Nrow)&(c.Ncol==start.Ncol))
		{
			break;
		}
		Node a;
		a.Ncol=c.Pcol;
		a.Nrow=c.Prow;
		int pos=0;
		pos=Search(a,closedList);
		list<Node>::iterator beg;
		beg=closedList.begin();
		advance(beg,pos);
		c=*beg;		
	}
}

/*
	a_star: compute astar alghorithm by calling all the auxiliary functions
	INPUTS: 
		-Node *start: pointer to the initial node from where we want to start to search
		-Node *destination: Node that we want to reach.
*/
void a_star(Node *start, Node *destination)
{
    Node * neighbours1= new Node[8];
    Node * new_starts = new Node[8];
    
	int counterNeg=0; //count the neighbours which are valid

    //initialize start
    start->g=0.0;
    start->h=heuristic(start,destination);
    cout<<"Start\n";

    			
    					
	set<Node> openList;
	list<Node> *closedList=new list<Node>;
	bool found = false;

	openList.insert(*start);
	//closedList->push_back(*start);
	list<Node>::iterator it=closedList->begin();				
	while(!openList.empty()) 
		{
			if(found) break;
			    	
			Node current = *openList.begin();
			openList.erase(openList.begin());
			closedList->push_back(current);
			    
			neighbours1= setNeighbours(current,&counterNeg);
			for(int pind = 0; pind<counterNeg; pind++) //8 possible positions
				{
								
					neighbours1[pind].g=current.g+1;
					neighbours1[pind].h=heuristic(&neighbours1[pind],destination);
							
					if(isDestination(neighbours1[pind],*destination))
						{
							cout<<"arrivo\n";
							neighbours1[pind].Prow=current.Nrow;
							neighbours1[pind].Pcol=current.Ncol;
							closedList->push_back(neighbours1[pind]);
							openList.erase(openList.begin(),openList.end());
							
							printPath(*closedList,*start);	
							//FREE memory 
							delete closedList;
							delete neighbours1;
							//exit condition 
							found=true;
					
						}
						else
						{
							int disp=Search(neighbours1[pind],openList);
							if(disp!=-1) //node is already in OpenList. This function return the displacement for the iterator
							{
									//int disp=0;
					           	
									//startTime=omp_get_wtime();
										//disp=Search(neighbours1[pind],openList);
										//cout<<"trovato in openList\n";
									set<Node>::iterator it=openList.begin(); 
									advance(it,disp);
									Node app= *it;	 
									if((app.g+app.h)>(neighbours1[pind].g+neighbours1[pind].h))
									{
										openList.erase(it); // debugger
					            			
					            		neighbours1[pind].Prow=current.Nrow;
										neighbours1[pind].Pcol=current.Ncol;
										
										openList.insert(neighbours1[pind]);
									}
								}
								disp=Search(neighbours1[pind],*closedList);
								if(disp!=-1) //node is already in ClosedList. This function return the displacement for the iterator
					           	{
									list<Node>::iterator it=closedList->begin(); 
									advance(it,disp);
									Node app= *it;	 
									if((app.g+app.h)>(neighbours1[pind].g+neighbours1[pind].h))
									{
										closedList->erase(it); //debugger
					
												neighbours1[pind].Prow=current.Nrow;
												neighbours1[pind].Pcol=current.Ncol;
												closedList->push_back(neighbours1[pind]);
									}
										
								}
								else
								{
									neighbours1[pind].Prow=current.Nrow;
									neighbours1[pind].Pcol=current.Ncol;
					            	
										//	closedList.insert(successor);
									openList.insert(neighbours1[pind]);	
								}
							}
				}
			
			}

  

}

/*
	printmatrix: prints on screen the read or generated grid.
*/
void printmatrix()
{
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL;c++)
	       {
	       		cout <<" "<<matrix[i*ROW+c];
	       	
	       }
	       
	   cout<<"\n";  
	}
	cout<<"\n";	
}
/*
	readPath: used to read a file containing the grid to fill the matrix
	INPUTS: 
		-String fileName: the name of file to read, in this case it will be "Matrix.txt"
	OUTPUT:
		-int matrix [ROW][COL]: it is a global variable so it is just filled, doesn't need return value.	
*/
void readPath(string fileName)
{
	char c;
	int num;
	int i=0;
	int j=0;
	ifstream fin(fileName);
	//add file not exist error
	
	while(fin.get(c))
	{
		if(c != '\n')
		{
			//to cast from char to number
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

/* Inputs from command line:
	-argv[1] number of rows in your grid
	-argv[2] number of columns in your grid
	-argv[3] t: if you want to generate a new matrix or f: if you want to read it from file named "Matrix.txt"
	-argv[4] row of start Node
	-argv[5] column of start Node 
	-argv[6] row of destination Node
	-argv[7] column of destination Node
*/
int main(int argc, char * argv[])
{
	set_new_handler(no_memory);
	if (argc >= 7) 
	{
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
		if(argv[3][0]=='f')
		{
			readPath("Matrix.txt");
			cout << "File read!\n";
		}
		else
		{
			generateMatrix(ROW,COL);
			writeFile(ROW,COL);
			cout << "Grid generated!\n";
			readPath("Matrix.txt");
		}
		
    	
		// set start and destination node: 
		Node start;
		Node dest;
		
		//checks if the start node is valid ( valid means in range(ROW,COL) and is not a blocking element
		if(isValid(atoi(argv[4]),atoi(argv[5]))==false)
		{
			cout <<"Invalid start";
			return 5;
		}
		start.Nrow= atoi(argv[4]);
		start.Ncol= atoi(argv[5]);
		
		//Destination: 
		
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
		return 0;
	}
	else
	{
		cout<<"Wrong number of paramters!They must be: \n 1.Grid rows \n 2. Grid columns\n 3.t or f to generate grid or not \n 4.Node start row \n 5.Node start column \n 6.Node destination row\n 7.Node destination column\n 8.Number of threads\n";
		return 0;
	}
}
