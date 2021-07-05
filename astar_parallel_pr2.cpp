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
	-Nrow,Ncol are int values that rappresent the number of coloumn and row of each Node
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
typedef struct path
{
	double cost;
	int numThread;
	
} Path;

// This function will be called when the memory is not enough to allocate dynamically space (usefull only with "new" operator)
void no_memory()
{
	cout<<"not enough memory";
	exit (1);
}

//finds the position of the node in a given set
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
			//#pragma omp task
			//{
				Node c = *it;	
				if((c.Ncol==toSearch.Ncol)&&(c.Nrow==toSearch.Nrow))
				{
					find=i;
				}
				i++;
			//}
		}
	//}
	//double end=omp_get_wtime();
	//cout<<"ricerca: " << end-startTime <<"\n";
	return find;
}
Node * setNeighbours(Node current, int *c)
{
	//struct Node* neighbours;
    //Node * neighbours_array =(Node *) malloc(N_DIRECTION * sizeof(Node));
    Node * neighbours_array = new Node[8];
    
    
    //Node neighbours_array[8];
    //Node * neighbours = &neighbours_array;
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

//finds the position of a node in a list
int Search(Node& toSearch, list<Node> l)
{
	list<Node>::iterator it;
	
	int i=0;
	//task
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

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range

    return ((row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL)) &&(matrix[row*ROW+col]!=0); //if matrix[][] =0 the is an obstacle
}

//check if a node is the destination node
bool isDestination(Node succ,Node dest) 
{ 
	if (succ.Ncol == dest.Ncol && succ.Nrow == dest.Nrow) 
		return true; 
	else
		return false; 
} 

//euclidean distance
double heuristic(Node* current_cell,Node *goal)
{

    
	double i=sqrt((goal->Nrow-current_cell->Nrow)*(goal->Nrow-current_cell->Nrow)+(goal->Ncol-current_cell->Ncol)*(goal->Ncol-current_cell->Ncol));
	//manhattan 
	//double i=abs(current_cell->Nrow-goal->Nrow)+abs(current_cell->Ncol-goal->Ncol);
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
	
//	endTime=omp_get_wtime();
        //cout<< "\n\n time: " << endTime-startTime<<"\n";
}

void swap(Path * array, int l, int r) {
    Path tmp = array[l];
    array[l] = array[r];
    array[r] = tmp;
}

//This function orders the array in an ascending order with respect
//to the cost of paths
void quickSort(Path * array, int begin, int end) {
    float pivot;
    int l, r;
    if (end > begin) {
        pivot = array[begin].cost;
        l = begin + 1;
        r = end + 1;
        while (l < r)
            if (array[l].cost < pivot)
                l++;
            else {
                r--;
                swap(array, l, r);
            }
        l--;
        swap(array, begin, l);
        quickSort(array, begin, l);
        quickSort(array, r, end);
    }
}


void a_star(Node *start, Node *destination)
{
    Node * neighbours1= new Node[8];
    Node * new_starts = new Node[8];
    double beg=0;
    double end=0;
    int counterNeg=0; //count the neighbours which are valid
    
    
    
    neighbours1 = setNeighbours((Node)(*start), &counterNeg);
	cout <<"vicini : " << counterNeg;
    for (int sub=0; sub<counterNeg; sub++)
    {
    	Node t;
    	t=neighbours1[sub];
    	t.g=1;
    	t.h=heuristic(&neighbours1[sub],destination);
    	//Node t1;
    	
    	new_starts[sub] = t; 	
		//t1= new_starts[sub];
	}
    
    
    
    //destroying neg1
    //delete neighbours1;
    //int c= *(neighbours1+0)->Ncol; 
   
    
    //initizializate start
    start->g=0.0;
    start->h=heuristic(start,destination);
    
  // cout <<"dest" <<destination->Ncol;
    if(counterNeg!=0)
    { 	
    	cout<<"Start\n";
    	//cout<<"vicini: " <<counterNeg <<"\n";
		Path path_array[counterNeg];
    		double beg = omp_get_wtime();
		#pragma omp parallel
		{ 
    		
    		#pragma omp single nowait
    		{
		
    			for(int neg=0;neg<counterNeg;neg++)
    			{  	
    				#pragma omp task private(neighbours1,counterNeg)
    				{
    					//cout<<"\n Numero thread: "<<omp_get_thread_num();
    					//definition of variables: 
		    			double startTimeTh,endTimeTh;
						set<Node> openList;
				    	list<Node> *closedList=new list<Node>;
				    	bool found = false;
				    	//starting time: 
				    	startTimeTh=omp_get_wtime();
				    	//openList.insert(*start);
						
						openList.insert(new_starts[neg]);
						closedList->push_back(*start);
						list<Node>::iterator it=closedList->begin();
					
			    		while(!openList.empty()) 
			    		{
			    	
			    			if(found) break;
			    	
							Node current = *openList.begin();
							openList.erase(openList.begin());
							closedList->push_back(current);
			    
			        		neighbours1= setNeighbours(current,&counterNeg);
							for(int pind = 0; pind<counterNeg; pind++) //8 position possible 
							{
								
							    neighbours1[pind].g=current.g+1;
							    neighbours1[pind].h=heuristic(&neighbours1[pind],destination);
							//    Node successor= neighbours1[pind];
			 
			//	            	cout<<"nodo successor"<<neighbours1[pind].Ncol<<" " <<neighbours1[pind].Nrow<<"\n";
								if(isDestination(neighbours1[pind],*destination))
								{
									cout<<"arrivo\n";
									neighbours1[pind].Prow=current.Nrow;
									neighbours1[pind].Pcol=current.Ncol;
									closedList->push_back(neighbours1[pind]);
									openList.erase(openList.begin(),openList.end());
									
									//save the information about thread that found the path and about cost
									int nThread = omp_get_thread_num();
									double cost=neighbours1[pind].g;
									//printPath(*closedList,*start);
									path_array[neg].cost=cost;
									path_array[neg].numThread=nThread;
									
									//time check
									endTimeTh=omp_get_wtime();
									
									//FREE memory 
									delete closedList;
									delete neighbours1;
									//exit condition 
									found=true;
									
								}
								else
								{
									if(Search(neighbours1[pind],openList)!=-1) //node is already in OpenList. This function return the displacement for the iterator
									{
										int disp=0;
					            	
										//startTime=omp_get_wtime();
										disp=Search(neighbours1[pind],openList);
										//cout<<"trovato in openList\n";
										set<Node>::iterator it=openList.begin(); 
										advance(it,disp);
										Node app= *it;	 
										if((app.g+app.h)>(neighbours1[pind].g+neighbours1[pind].h))
										{
											openList.erase(it); // debugger
					            			
					            			neighbours1[pind].Prow=current.Nrow;
											neighbours1[pind].Pcol=current.Ncol;
											/*neighbours1[pind].Prow=neighbours1[pind].Nrow-dy[pind];
											neighbours1[pind].Pcol=neighbours1[pind].Ncol-dx[pind];*/
											openList.insert(neighbours1[pind]);
										}
									}	
									if(Search(neighbours1[pind],*closedList)==-1) //node is already in ClosedList. This function return the displacement for the iterator
					            			{
					            			/*int disp=0;
					            			disp=Search(neighbours1[pind],*closedList);*/
					            	
											//cout <<"trovato in closed list\n";
											/*list<Node>::iterator it=closedList->begin(); 
											advance(it,disp);
											Node app= *it;	 
											//remove?
											if((app.g+app.h)>(neighbours1[pind].g+neighbours1[pind].h))
											{
												closedList->erase(it); //debugger
												/*neighbours1[pind].Prow=neighbours1[pind].Nrow-dy[pind];
												neighbours1[pind].Pcol=neighbours1[pind].Ncol-dx[pind];
												neighbours1[pind].Prow=current.Nrow;
												neighbours1[pind].Pcol=current.Ncol;
												closedList->push_back(neighbours1[pind]);
											}*/
										neighbours1[pind].Prow=current.Nrow;
										neighbours1[pind].Pcol=current.Ncol;
					            	
										//	closedList.insert(successor);
										openList.insert(neighbours1[pind]);
									}
									/*else
									{
										
									}*/
								}
							}
						
				    		/*else
				    		{
				    			cout<<"non valido";
							}*/
							//fflush(stdout);
							
							
							
					    }
					    
					    // if path found(there could be walls between starting node and destination
					   if(found!=true)
						{
							
							cout<<"Thread: " << path_array[neg].numThread << " can't reach the destination! Time: " << endTimeTh-startTimeTh <<"\n";
							
							//free allocated memory
						  	delete closedList;
							delete [] neighbours1;
						}
						else
						{
							cout<<"Thread: " << path_array[neg].numThread << "path found in time:  " << endTimeTh-startTimeTh <<" with cost: " << path_array[neg].cost <<"\n";
					
						}
					
        			} //end task
       				
				}//for 
			} //end of single thread
		}//end of parallel implicit barrier
		quickSort(path_array,0,counterNeg-1);
		
	
		cout <<"\nThe thread that have found the path with lowest cost is: " << path_array[0].numThread <<" with cost: " << path_array[0].cost <<"\n";
		
	} //if

  

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
	set_new_handler(no_memory);
	if (argc >= 8) 
	{
		if ((atoi(argv[8]) <= 0)||(atoi(argv[8])>NUM_MAX_THREAD))
			{
		    	cout<<"Not a valid number for threads.Please set a value between 0 and " << NUM_MAX_THREAD <<"\n";
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
	else
	{
		cout<<"Wrong number of paramters!They must be: \n 1.Grid rows \n 2. Grid columns\n 3. t or f to generate grid or not \n 4. node start row \n 5. node start column \n 6. Node destination row\n 7.Node destination column\n 8. number of threads\n";
		return 0;
	}
}
