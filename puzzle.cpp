
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

struct cmp 
{
	bool operator() (state &a, state &b) // overloading the unary operator "()"
	{
		int am = manhattan(a.first,a.second);
		int bm = manhattan(b.first,b.second);
		return am<bm;		
	}	
};


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

typedef pair<vector<vector<int> >, int> state;

struct cmp_misplaced_tiles 
{
	bool operator() (state &a, state &b) // overloading the unary operator "()"
	{
		int am = manhattan(a.first,a.second);
		int bm = manhattan(b.first,b.second);
		return am<bm;		
	}	
};
vector<vector <int> >generate_state(vector<vector<int> >state, char move, int i, int j){
    vector<vector<int> >new_state = state;
    if(move=='u' && i>0){
        new_state[i][j]=new_state[i-1][j];
        new_state[i-1][j]=0;
    }else if(move=='d' && i<2){
        new_state[i][j]=new_state[i+1][j];
        new_state[i+1][j]=0;
    }else if(move=='l' && j>0){
        new_state[i][j]=new_state[i][j-1];
        new_state[i][j-1]=0;
    }else if(move=='r' && j<2){
        new_state[i][j]=new_state[i][j+1];
        new_state[i][j+1]=0;
    }else{
        cout<<"Invalid move"<<endl;
        return state;
    }
    return new_state;
}

void print_state(vector<vector<int> >state){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<state[i][j]<<" ";
        }
        cout<<endl;
    }
}



void print_path(vector<vector<vector<int> > >path){
    for(int i=0; i<path.size(); i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                cout<<path[i][j][k]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl;
    }
}

int main(){
    vector<vector<int> >start(3, vector<int>(3,0));
    vector<vector<int> >end(3, vector<int>(3,0));
    generate_instances(start, end);
    while(!isSolvable(start, end)){
        generate_instances(start, end);
    }
    print_instance(start, end);
    vector<vector<vector<int> > >path = a_star_using_misplaced_tiles(start, end);
    print_path(path);
    return 0;
}