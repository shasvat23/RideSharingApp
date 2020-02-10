

#include <iostream>
#include <fstream>
#include <string.h>
#include<vector>
#include <unordered_map>
#include <queue>
#include<math.h>
#include <climits>
#include <set>

#define ARRAYSIZE(a) (sizeof(a)/sizeof((a)[0]))
using namespace std; 
struct Node
{
	int x, y;	
	vector<Node> parent;
        
	bool const operator==(const Node& ob) const
	{
		return x == ob.x && y == ob.y;
	}

	bool operator<(const Node& ob) const
	{
		return x < ob.x || (x == ob.x && y < ob.y);
	}
};

struct coordinates
{
    int x,y;
};
struct PassengerInfo{
    coordinates start;
    coordinates end; 
    string name ; 
};
class Route {
private:
    int xMax, yMax ;
    int xCurrent;
    int yCurrent; 
    int xNext, yNext; 
    int xEnd,yEnd;
    unordered_map<string,PassengerInfo> PassengerList ;
    vector<PassengerInfo> PassengerPickupList;
    vector<PassengerInfo> PassengersInCar;
    bool firstIter;
    
    int row[4] = { -1, 0, 0, 1 };
    int col[4] = { 0, -1, 1, 0 };

    bool isValid(int x, int y)
    {
	return (x >= 0 && x <= xMax) && (y >= 0 && y <= yMax);
    }
    PassengerInfo  strtokens(const char *str)
    {
     PassengerInfo EmptyList; //For cases where users start or end pts are out of range
    PassengerInfo tokenList;
    char *pch;
    char *str1; 
    str1 = (char *)malloc(strlen(str)*sizeof(char));
    strcpy(str1,str);
    int i =0; 
    pch = strtok (str1," "); 
    while (pch != NULL)
    {
      i++;
      switch(i)
      {
          case 1:
            tokenList.name = pch; 
           // EmptyList.name = NULL;
            break;
          case 2:
              if(atoi(pch) > xMax )
                  return EmptyList;
              tokenList.start.x = atoi(pch);
              break;
          case 3:
              if(atoi(pch) > yMax)
                  return EmptyList; 
              tokenList.start.y = atoi(pch); 
              break; 
          case 4:
              if(atoi(pch) > xMax )
                  return EmptyList;
              tokenList.end.x = atoi(pch);
              break;
          case 5:
              if(atoi(pch) > yMax)
                  return EmptyList; 
              tokenList.end.y = atoi(pch); 
              break; 
          default:
              break;
              
        }
      
    pch = strtok (NULL, " ");
    
    }    
  
    return tokenList;
   }
    void ReadFileAndUpdatePassengerPickUpList()
    {
        
        std::ifstream ffile ("PassengerList.txt",std::ifstream::in);
        if(!ffile.is_open())
        {
           // cout << "Input file not found ";
            return  ;
        } 
        string line ;
        while(getline(ffile,line))
        {
            PassengerInfo temp = strtokens(line.c_str());
            if(temp.name.empty())
                continue;
            PassengerPickupList.push_back(temp);
            
        }
        ffile.close();
        remove("PassengerList.txt");
    }
    void printPath(queue<coordinates> &q,vector<Node> path)
    {
	if (path.size() == 0)
		return ;

	printPath(q,path[0].parent);        
	q.push({path[0].x,path[0].y});
	return ;
    }
    queue<coordinates> FindShortestPath(coordinates end_pt)
    {
        int Mat[xMax][yMax];
        for(int i =0; i<=xMax ;i++)
            for(int j =0;j <yMax;j++)
                Mat[i][j]=1;
        queue<Node> q;
	Node src = {xCurrent, yCurrent};
	q.push(src);	
        queue<coordinates> ret; 
	set<Node> visited;
	visited.insert(src);	
	while (!q.empty())
	{
		// pop front node from the queue and process it
		Node curr = q.front();
		q.pop();

		int i = curr.x;
		int j = curr.y;

		// if destination is found, print the shortest path and
		// return the shortest path length
		if (i == end_pt.x && j == end_pt.y)
		{
                    printPath(ret,{curr});
                    return ret; 
			
		}

		// get value of the current cell
		int n = Mat[i][j];

		// check all 4 possible movements from current cell
		// and recur for each valid movement
		for (int k = 0; k < 4; k++)
		{
			// get next position coordinates using value of current cell
			int x = i + row[k] * n;
			int y = j + col[k] * n;

			// check if it is possible to go to next position
			// from current position
			if (isValid(x, y))
			{
				// construct the next cell node
				Node next = { x, y, {curr} };

				// if it not visited yet
				if (!visited.count(next))
				{
					// push it into the queue and mark it as visited
					q.push(next);
					visited.insert(next);
				}
			}
		}
	}

	// return -1 if path is not possible
	return ret;       
    }
    float distance(coordinates tmp) const 
    {
      	return std::sqrt(std::pow((xCurrent-tmp.x),2) + std::pow((yCurrent-tmp.y),2));
    }
    coordinates FindClosestNextNode()
    {
        float minDist_P = 10000;
        int idx_PickupList ; 
        float minDist_I = 10000; 
        int idx_Incar; 
        if(!PassengerPickupList.size() && !PassengersInCar.size())
            return {0,0};
        for(int i =0; i < PassengerPickupList.size();i++)
        {
            float temp_minDist = distance(PassengerPickupList[i].start);
            if(temp_minDist < minDist_P )
            {
                minDist_P = temp_minDist;  
                idx_PickupList = i;
            }
            
        }
        
        for(int i =0; i< PassengersInCar.size(); i++)
        {
            int temp_minDist = distance(PassengersInCar[i].end);
            if(temp_minDist < minDist_I )
            {
                minDist_I = temp_minDist;  
                idx_Incar = i;
            }
        }
        
        return minDist_I <= minDist_P ? PassengersInCar[idx_Incar].end : PassengerPickupList[idx_PickupList].start;
    }
    void CheckPassengersToBePickedAtCurrentBlock()
    {
        if(!PassengerPickupList.size())
            return; 
        
        vector<PassengerInfo> temp_list; 
        for( auto i = PassengerPickupList.begin() ; i != PassengerPickupList.end();i++)
        {
            if((*i).start.x == xCurrent && (*i).start.y == yCurrent )
            {
                temp_list.push_back((*i));
                PassengersInCar.push_back((*i));
                //ToDo :: Erase this passenger from PassengerPickUpList
                i = PassengerPickupList.erase(i);
                i--;
            }
        }
        printPassengerNamesPickedUp(temp_list);
    
        
    }
    
    
    void CheckPassengersToBeDroppedAtCurrentBlock()
    {
        if(!PassengersInCar.size())
            return; 
        
        vector<PassengerInfo> temp_list; 
        for( auto i = PassengersInCar.begin(); i != PassengersInCar.end();i++)
        {
            if((*i).end.x == xCurrent && (*i).end.y == yCurrent )
            {
                temp_list.push_back(*i);
               //ToDO Erase this passengers from InCar List
                i = PassengersInCar.erase(i);
                i--;
            }
        }     
       printPassengerNamesDropping(temp_list);
    }
        
    
    
        
    void printCurrentPos()
    {
        cout<<"*******************************************************"<<endl;
        cout<<"Current pos x :"<<xCurrent<<" ,y :"<<yCurrent<<endl<<endl;
    }   
    
    
    void printPassengerNamesPickedUp(vector<PassengerInfo> list)
    {
        cout<<"Passenger PickupList at current block:" <<endl; 
        for(int i =0; i<list.size();i++)
            cout<<list[i].name<<endl; 
         cout<< endl;
    }
    void printPassengerNamesDropping(vector<PassengerInfo> list)
    {
        
        cout<<"Passenger DropList at current block:" <<endl; 
        for(int i =0; i<list.size();i++)
            cout<<list[i].name<<endl; 
         cout<< endl;
    }
    
   void printPassengersYetToPickUpWithLocation()
   {
       if(!PassengerPickupList.size())
        {
            cout<<"No Passengers to pick up"<<endl;
            return;
        }
        cout<<"Passengers to pick up :"<<endl<<endl;
        for(int i =0; i<PassengerPickupList.size();i++)
            cout<<"Name : "<<PassengerPickupList[i].name<<" ;PickUp Pt x : "<<PassengerPickupList[i].start.x<<" y: "<<PassengerPickupList[i].start.y<<endl; 
        cout<< endl;
   }
    void printInCar()
    {
        if(!PassengersInCar.size())
        {
            cout<<"No Passengers in car"<<endl<<endl;
            return;
        }
        cout<<"Passengers In Car:"<<endl<<endl;
        for(int i =0; i<PassengersInCar.size();i++)
            cout<<"Name : "<<PassengersInCar[i].name<<" ;Drop Pt x : "<<PassengersInCar[i].end.x<<" y: "<<PassengersInCar[i].end.y<<endl; 
         cout<< endl;
    }
    void SetNextBlockMove(int x, int y)
    {
        xNext = x; 
        yNext = y;
    }
    void SetEndPtForCurrentRoute(int x, int y)
    {
        xEnd = x; 
        yEnd = y;
    }
    void MoveToNextBlock()
    {
        xCurrent = xNext;
        yCurrent = yNext;
    }
public : 
    Route()
    {
        xMax =0; 
        yMax = 0; 
        xNext=0;
        yNext = 0; 
        firstIter = true;
    }
    Route(int x, int y):xMax(x),yMax(y)
    {
        xNext=0;
        yNext =0;
        firstIter = true;
    }
    
    void Start()
    {
        if(firstIter)
        {
            
            xCurrent =0; 
            yCurrent = 0;
            ReadFileAndUpdatePassengerPickUpList();
            firstIter = false; 
            coordinates end_tmp = FindClosestNextNode(); 
            queue<coordinates> q = FindShortestPath(end_tmp);//ReRoute for new end pt; 
            q.pop();
             coordinates tmp = q.front(); 
        
            SetNextBlockMove(tmp.x, tmp.y);
            tmp = q.back();
            SetEndPtForCurrentRoute(tmp.x,tmp.y);
            cout<<"This was startup, if file had information it has been read and app is ready for block moves"<<endl;
            if(!PassengerPickupList.size() && !PassengerPickupList.size())
            {
               // cout<<"No new person to pickup or drop; idling down "<<endl;
                return ;
            }
           
        }
        else
        {
            MoveOneBlockForward(); 
        }
    }
    void MoveOneBlockForward()
    {
        MoveToNextBlock();
        ReadFileAndUpdatePassengerPickUpList();     
        printCurrentPos();
        
        
        if(PassengerPickupList.size())
            CheckPassengersToBePickedAtCurrentBlock(); //Check if passengers are to be dropped, pickedup at this block
        
        if(PassengersInCar.size())
            CheckPassengersToBeDroppedAtCurrentBlock();
        
        
        printInCar();
        
        
        if(!PassengerPickupList.size() && !PassengersInCar.size())
        {
            cout<<"No new person to pickup or drop; idling down "<<endl;
            return ;
        }
        else
        {
           
            coordinates end_tmp = FindClosestNextNode();  //Check for closest end pt if there was new passenger added 
            queue<coordinates> q = FindShortestPath(end_tmp);//ReRoute for new end pt; 
            q.pop();
            coordinates tmp = q.front(); 
        
            SetNextBlockMove(tmp.x, tmp.y);
            tmp = q.back();
            SetEndPtForCurrentRoute(tmp.x,tmp.y);
            
            
            q.pop();
            
             printPassengersYetToPickUpWithLocation();
        }
    }
    
      
};



int main(int argc, char**argv) {
    
    std::cout << "Welcome to not so uber app" << std::endl;
    int x,y; 
    
    
    cout<< "Enter the max streets on the grid in x and y direction :  " ; 
    cin >> x>> y; 
    
    Route R(x,y); 
    
    while (1)
	{
                cout<<"Type 'n' for next move, type 'q' to quit"<<endl;
		char buf[100];
		char *ptr = fgets(buf, ARRAYSIZE(buf), stdin);
		if (!ptr)
			return 0;
		char cmd = *ptr;
               
		switch (cmd)
		{
		default:
			                    
			break;
		case 'n':                       
                        R.Start();
			break;
		case 'q':                       
			return 0;
			break;
		}
	}
       
    
    return 0;
}
