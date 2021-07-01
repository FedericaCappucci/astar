#include <list>
#include <iostream>
#include <cstdlib>
#include <set>
#include<fstream>
#include <math.h>
#include "matrix.cpp"
#include <omp.h>
#include <new>
#define N_DIRECTION 8

using namespace std;


double startTime;
double endTime;
int *matrix= (int*) calloc(ROW*COL,sizeof(int));

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
					return 			  (g+h)<(other.g+other.h);			
				}
				else
				{
					return true;
				}
			}
	    }
	    static void operator delete(void* ptr, std::size_t sz)
    	    {
        		//cout << "custom delete for size " << sz <<endl;
        		delete (ptr); // ::operator delete(ptr) can also be used
    	    }
	
	   
};
typedef struct path
{
	double cost;
	int numThread;
	
} Path;

void no_memory()
{
	cout<<"not enough memory";
	exit (1);
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

void a_star(Node *start, Node *destination)
{
    Node * neighbours1= new Node[8];
    Node * new_starts = new Node[8];
    double beg=0;
    double end=0;
    int counterNeg=0; //count the neighbours which are valid
    omp_set_dynamic(0); // Explicitly disable dynamic teams
    omp_set_num_threads(8); // Use N threads for all parallel regions
    
    
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
		    			int nThread = omp_get_thread_num();
		    			//cout <<"\n thread: " << nThread;
						set<Node> openList;
				    	list<Node> *closedList=new list<Node>;
				    	bool found = false;
					//Node * neighbours1=new Node[8];
					
				    	//beg=omp_get_wtime();
				    	//openList.insert(*start);
						
						openList.insert(new_starts[neg]);
						closedList->push_back(*start);
						list<Node>::iterator it=closedList->begin();
					//Node c =*it;
					
					
			    		//to create successors
						int vc, vr;
			    
			    		int dy[8] = {0, 1, 0, -1,1,-1,-1,1}; //row
			    		int dx[8] = {-1, 0, 1, 0,1,-1,1,-1}; //col
			
			    		while(!openList.empty()) //per parallelizzazione meglio for
			    		{
			    	
			    			if(found) break;
			    	
							Node current = *openList.begin();
							openList.erase(openList.begin());
							closedList->push_back(current);
			        
			        		//cout<<"nodo corrente"<<current.Ncol<<" " <<current.Nrow<<"\n";
			        		//static threads
			        		//#pragma omp parallel for schedule(static)
			        		//find near nodes of my current one 
			        		neighbours1= setNeighbours(current,&counterNeg);
							for(int pind = 0; pind<counterNeg; pind++) //8 position possible 
							{
								
						    	//vr = current.Nrow + dy[pind]; //row
								//vc = current.Ncol + dx[pind]; //col
							    
							    neighbours1[pind].g=current.g+1;
							    //not sure it will work
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
									int nThread = omp_get_thread_num();
									double cost=neighbours1[pind].g;
									//end=omp_get_wtime();
									//printPath(*closedList,*start);
									path_array[neg].cost=cost;
									path_array[neg].numThread=nThread;
									
									//FREE memory 
									delete closedList;
									delete neighbours1;
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
					    //delete closedList;
					   if(found!=true)
						{
							cout<<"can't reach the destination";
						  	delete closedList;
							delete [] neighbours1;
						}
						else
						{
							//cout<<"Trovato!\n" << "time " << end-beg;
							//cout<<"il nodo ";
							
							
						}
						
						
					//cout<<"\n closedList: \n";
					//stampa closedList
					
					/*for(i=closedList->end();i!=closedList->begin();--i)
					{
		
						cout << "(" << c.Nrow << "," << c.Ncol <<") ->";
					}
					for (auto it = closedList->cbegin(); it != closedList->cend(); it++) {
						pino=*it;
						cout << "(" << pino.Nrow << "," << pino.Ncol <<") ->parente" << pino.Prow <<", "<<pino.Pcol <<") costo " << pino.g+pino.h <<"\n";
        				//std::cout << *it << std::endl;
        								}*/	
        			} //end task
       				
				}//for 
			} //end of single thread
		}//end of parallel implicit barrier
		for (int g=0; g<counterNeg;g++)
		{
			//cout <<"\n costi: "<< path_array[g].cost<<"\n";
		}
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

int main()
{
	//generateMATRIX();
//	printmatrix();
	//generatematrix();
	readPath("Matrix.txt");
	set_new_handler(no_memory);
	Node start;
	Node dest;
	//cout<<"Ncol: " <<COL<<"\n";
	start.Nrow=0;
	start.Ncol=2; //7 for 5000x5000 matrix 6  for 3000x3000 2
 //       startTime=omp_get_wtime();
	//printmatrix2();
	
	if(isValid(start.Nrow,start.Ncol)==false)
	{
		cout <<"Invalid start";
		return 0;
	}
	
	

	//Node dest;
	/*generateDest();
	dest.Nrow=destR; 
	dest.Ncol=destC;*/
	dest.Nrow=999; //4999 for 5000x5000 matrix 2440  then 2999 
	dest.Ncol=999; //4998 for 5000x5000 matrix 2445 then 2999

	

	//cout<< "\ndestinazione : " << dest.Nrow <<" " <<dest.Ncol<<"\n"; 
	if(isValid(dest.Nrow,dest.Ncol)==false)
	{
		cout <<"Invalid destination";
		return 0;
	}
	a_star(&start,&dest);
	free(matrix);
//	cout<<"stampa: " << endTime-startTime <<"\n";
	return 0;
	
}
