#include <list>
#include <iostream>
#include <set>
#include <math.h>
#include "matrix.cpp"
#include <omp.h>

using namespace std;
// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;


double startTime;
double endTime;
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

int Search(Node& toSearch, set<Node> l)
{
	set<Node>::iterator it;
	
	int i=0;
	int find=-1;
	//#pragma omp parallel
	//{
		cout<<"Thread: " << omp_get_thread_num(); ;
		cout<<"\n";
			
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
           && (col < COL)) &&(matrix[row][col]!=0); //if matrix[][] =0 the is an obstacle
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

    while(!openList.empty()) //per parallelizzazione meglio for
    {
    	
    	
        Node current = *openList.begin();
        openList.erase(openList.begin());
        closedList.push_back(current);
        
        cout<<"nodo corrente"<<current.Ncol<<" " <<current.Nrow<<"\n";
        //static threads
        //#pragma omp parallel for schedule(static)
        for(int pind = 0; pind<8; pind++) //4 position possible 
	{
		
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
	            	closedList.push_back(successor);
	            	openList.erase(openList.begin(),openList.end());
	            	printPath(closedList,*start);
	            	return 0;
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
		            		openList.erase(it); // debugger
		            		
		            		successor.Prow=successor.Nrow-dy[pind];
		            		successor.Pcol=successor.Ncol-dx[pind];
		            		openList.insert(successor);
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
		            		closedList.erase(it); //debugger
		            		
		            		successor.Prow=successor.Nrow-dy[pind];
		            		successor.Pcol=successor.Ncol-dx[pind];
		            		closedList.push_back(successor);
						}
					}
					else
					{
						
						successor.Prow=current.Nrow;
						successor.Pcol=current.Ncol;
		            	
		            //	closedList.insert(successor);
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
   cout<<"can't reach the destination";

}
void printmatrix2()
{
	//#pragma omp parallel for schedule(dynamic)
	for(int i=0;i<ROW;i++)
	{
	       for(int c=0;c<COL;c++)
	       {
	       		cout <<" "<<matrix[i][c];
	       	
	       }
	       
	   cout<<"\n";  
	}
	cout<<"\n";
	endTime=omp_get_wtime();
	
}

int main()
{
//	generateMATRIX();
//	printmatrix();
	generateMATRIX();

	Node start;
	cout<<"Ncol: " <<COL<<"\n";
	start.Nrow=0;
	start.Ncol=3;
	startTime=omp_get_wtime();
	printmatrix2();
	
	if(isValid(start.Nrow,start.Ncol)==false)
	{
		cout <<"Invalid start";
		return 0;
	}
	
	
	Node dest;
	
	dest.Nrow=999; 
	dest.Ncol=997;
	cout<< "\nscelta: " << matrix[dest.Nrow][dest.Ncol]; 
	if(isValid(dest.Nrow,dest.Ncol)==false)
	{
		cout <<"Invalid destination";
		return 0;
	}
	a_star(&start,&dest);
	cout<<"stampa: " << endTime-startTime <<"\n";
	return 0;
	
}
