#include<iostream>
#include<vector>
#include<cmath>
#include<climits>
#include<ctime>
#include<utility>
#include<set>
#include<queue>
#include<map>
#include<unordered_set>
using namespace std;

map<vector<vector<int> > , vector<vector<int> > > parent; 
vector<vector<int> >goal(3, vector<int>(3,0));

int merge(vector<int> &arr, vector<int>&temp, int left, int mid ,int right){
    int i,j;
    int temp_i;
    int inv=0;
    i = left; j = mid; temp_i=left;
    while(i<=mid-1 && j<=right){
        if(arr[j]<arr[i]){
            inv+=(mid-i);
            temp[temp_i]=arr[j];
            temp_i++;
            j++;
        }else{
            temp[temp_i]=arr[i];
            temp_i++;
            i++;
        }
    }
    while(i<=mid-1){
        temp[temp_i]=arr[i];
        i++;
        temp_i++;
    }
    while(j<=right){
        temp[temp_i]=arr[j];
        j++;
        temp_i++;
    }
    for(int e = left; e<=right; e++){
        arr[e]=temp[e];
    }
    return inv;
}

int msort(vector<int>&array, vector<int>&temp, int left, int right){
    int mid; int inv=0;
    if(right>left){
        mid = (left+right)/2;
        inv+=msort(array, temp, left, mid);
        inv+=msort(array, temp, mid+1, right);
        inv+=merge(array, temp, left, mid+1, right);
    }
    return inv;
}

int countInv(vector<vector<int> >state){
    vector<int>linear;
    for(int i=0; i<state.size(); i++){
        for(int j=0; j<state[i].size(); j++){
            if(state[i][j]!=0){
                linear.push_back(state[i][j]);
            };
        }
    }
    vector<int>temp(linear.size(),0);
    return msort(linear, temp, 0, linear.size()-1);
}

bool isSolvable(vector<vector<int> >start, vector<vector<int> >end){
    int inv_start = countInv(start);
    int inv_end = countInv(end);
    if(inv_start%2==0 && inv_end%2==0){
        return true;
    }else if(inv_start%2==1 && inv_end%2==1){
        return true;
    }else{
        return false;
    }
}

void generate_instances(vector<vector<int> >&start, vector<vector<int> >&end){
    vector<int>start_used(9,0);
    vector<int>end_used(9,0);
    int filled_s=0;
    int filled_e=0;
    srand(time(0));
    int i=0, j=0;
        while(filled_s<9 && i<3 && j<3){ 
            int s = rand()%9;
            if(start_used[s]==0){
                start[i][j]=s;
                start_used[s]=1;
                filled_s++;
                if(i==2){
                i=0;
                j++;
                }else{
                    i++;
                }
            }
        }
    i=0; j=0;
        while(filled_e<9 && i<3 && j<3){
            int s = rand()%9;
            if(end_used[s]==0){
                end[i][j]=s;
                end_used[s]=1;
                filled_e++;
                if(i==2){
                i=0;
                j++;
                }else{
                    i++;
                }
            }
        }
}

void print_instance(vector<vector<int> >start, vector<vector<int> >end){
    cout<<"Start: "<<endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<start[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"End: "<<endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<end[i][j]<<" ";
        }
        cout<<endl;
    }
}

int count_Misplaced_Tiles(vector<vector<int> >start, vector<vector<int> >end){
    int misplaced=0;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(start[i][j]!=end[i][j]){
                misplaced++;
            }
        }
    }
    if(misplaced==0){
        return 0;
    }else{
        return misplaced-1;
    }
}

int manhattan (vector<vector<int> > a , int moves){
	int dist = moves;
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++){
			if(a[i][j] != 0){
				for (int k=0; k<3; k++){
					for (int l=0; l<3; l++){
						if(a[i][j]==goal[k][l])
							dist+=abs(i-k)+abs(j-l);
					}
				}
			}
		}
	}
	return dist;	
}

typedef pair<vector<vector<int> > , int> state; //defining a state with its g value

bool visit(vector<vector<int> > a, map<vector<vector<int> > , bool> visited ){
	if(visited[a]==true)
		return true;
	
	return false;
}

bool safe(int i,int j){
	if(i>=0 && i<=2 && j>=0 && j<=2)
		return true;
	
	return false;
}

int dx[]={-1,1,0,0};
int dy[]={0,0,1,-1};
vector<vector<vector<int> > > neighbours(vector<vector<int> > a) {
	int x, y;
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++){
			if(a[i][j] == 0) 
			{
				x=i;
				y=j;
				break;
			 } 												
		}		
		
	}
	
	vector<vector<vector<int> > > ans;
	for (int k=0; k<4; k++)
	{
		int cx = x; 
		int cy = y; 
		vector<vector<int> > n = a;
		if(safe(cx+dx[k], cy+dy[k]))
		{
			swap(n[cx+dx[k]][cy+dy[k]], n[cx][cy]);
			ans.push_back(n);			
		}
	}
		
	return ans;
}

void print_state(vector<vector<int> >state){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<state[i][j]<<" ";
        }
        cout<<endl;
    }
}

struct cmp_misplaced_tiles {
	bool operator() (state &a, state &b) // overloading the unary operator "()"
	{
		int am = count_Misplaced_Tiles(a.first,goal);
		int bm = count_Misplaced_Tiles(b.first,goal);
		return am<bm;		
	}	
};

struct cmp_manhattan {
	bool operator() (state &a, state &b) // overloading the unary operator "()"
	{
		int am = manhattan(a.first,a.second);
		int bm = manhattan(b.first,b.second);
		return am<bm;		
	}	
};


void print_path(vector<vector<int> > s){
	if(parent.count(s)) 
	print_path(parent[s]);
	
    for(int i=0;i<3;i++)
    {
 	  for(int j=0;j<3;j++)
	  {
		 printf("%d ",s[i][j]);
	  }
	 cout<<endl;
    }
   cout<<endl;
   
 return;
}

void a_star_using_misplaced_tiles(vector<vector<int> > a, int moves){
	
	map<vector<vector<int> > , bool> visited;
	priority_queue<state, vector<state>, cmp_misplaced_tiles> Q; //Priority queue named "Q". 
	Q.push(state(a,moves));
	while(!Q.empty())
	{
		vector<vector<int> > s = Q.top().first;
		Q.pop();
		visited[s]=true;
		if(s==goal)
		{
		    cout<<endl;
			print_path(s);
			break;
		}
		vector<vector<vector<int> > > ns = neighbours(s);
		vector<vector<vector<int> > >::iterator it;
		for(it=ns.begin();it!=ns.end();it++)
		{
			vector<vector<int> > temp = *it;
			if(!visit(temp, visited))
			{
				parent.insert(pair<vector<vector<int> > , vector<vector<int> > >(temp,s));
				Q.push(state(temp,moves+1));
			}
		}
	}
	return;
}

void a_star_using_manhattan(vector<vector<int> > a, int moves){
	
	map<vector<vector<int> > , bool> visited;
	priority_queue<state, vector<state>, cmp_manhattan> Q; //Priority queue named "Q". 
	Q.push(state(a,moves));
	while(!Q.empty())
	{
		vector<vector<int> > s = Q.top().first;
		Q.pop();
		visited[s]=true;
		if(s==goal)
		{
		    cout<<endl;
			print_path(s);
			break;
		}
		vector<vector<vector<int> > > ns = neighbours(s);
		vector<vector<vector<int> > >::iterator it;
		for(it=ns.begin();it!=ns.end();it++)
		{
			vector<vector<int> > temp = *it;
			if(!visit(temp, visited))
			{
				parent.insert(pair<vector<vector<int> > , vector<vector<int> > >(temp,s));
				Q.push(state(temp,moves+1));
			}
		}
	}
	return;
}

int main(){
    vector<vector<int> >initial(3, vector<int>(3,0));
    generate_instances(initial, goal);
    while(!isSolvable(initial, goal)){
        generate_instances(initial, goal);
    }
    print_instance(initial, goal);
    cout<<"Using misplaced tile heuristic:\n";
    a_star_using_misplaced_tiles(initial, 0);
    cout<<"Using manhattan distance:\n";
    a_star_using_manhattan(initial,0);
    
    return 0;
}
