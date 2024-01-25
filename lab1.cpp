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

// struct Compare_sort {
//     bool operator()(pair<int, vector<vector<int> >> state1, pair<int, vector<vector<int> > > state2)
//     {
//         return state1.first>state2.first;
//     };
// };

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

vector<vector<vector<int> > >a_star_using_misplaced_tiles(vector<vector<int> >start, vector<vector<int> >end){
    multiset<pair<int, pair<char, pair<int, vector<vector<int> > > > > >open; //the first integer in the pair denotes the heuristic value, second integer the g value
    map<vector<vector<int> >,int> open2; //the first value stores the state, second value stores the g value for comparison
    map<pair<char,vector<vector<int> > >,pair<char, vector<vector<int> > > > parents;
    multiset<pair<int, vector<vector<int> > > >closed;
    //map<vector<vector<int> >,int> closed2;
    vector<vector<vector<int> > >path;
    auto current_state = make_pair('o',make_pair(0,start));
    // vector<vector<int> >next_state;
    open.insert(make_pair(count_Misplaced_Tiles(start, end), current_state));
    open2.insert(make_pair(current_state.second.second, current_state.second.first));
    auto next_state = make_pair(current_state.first, current_state.second);
    int counter=0;
    int max_iterations = 100000;
    //char move = 'o';
    while(!open.empty() && counter<max_iterations ){
        if(current_state.second.second==end){
            cout<<"check2"<<endl;
            // for(auto it = parents.begin(); it!=parents.end(); it++){
            //     path.push_back((*it).first.second);
            // }
            auto it = parents.begin();
            while(it!=parents.end()){
                path.push_back((*it).first.second);
                it = parents.find((*it).second);
            }
            return path;
        }
        else{
            current_state = (*open.begin()).second; //current state is the state with the least heuristic value
            open.erase(open.begin());
            open2.erase(open2.find(current_state.second.second));
            closed.insert(current_state.second);
            //cout<<"check1"<<endl;
            vector<pair<char,pair<int,vector<vector<int> > > > >next_states;
            //vector<char>moves;
            //closed2.insert(make_pair(current_state.second, current_state.first));
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if(current_state.second.second[i][j]==0){
                        //cout<<"check3"<<endl;
                        if(i>0 && current_state.first!='d'){ 
                            //cout<<"u check"<<endl;
                            next_state = make_pair('u',make_pair(current_state.first+1, generate_state(current_state.second.second, 'u', i, j)));
                            next_states.push_back(next_state); 
                            //print_state(next_state.second.second);
                            //cout<<endl<<endl;
                            //current_state.first='u';
                        }
                        if(i<2 && current_state.first!='u'){ 
                            //cout<<"d check"<<endl;
                            next_state = make_pair('d',make_pair(current_state.first+1, generate_state(current_state.second.second, 'd', i, j)));
                            next_states.push_back(next_state); 
                            //print_state(next_state.second.second);
                            //cout<<endl<<endl;
                            //current_state.first='d';
                        }
                        if(j>0 && current_state.first!='r'){ 
                            //cout<<"l check"<<endl;
                            next_state = make_pair('l',make_pair(current_state.first+1, generate_state(current_state.second.second, 'l', i, j)));
                            next_states.push_back(next_state); 
                            //print_state(next_state.second.second);
                            //cout<<endl<<endl;
                            //current_state.first='l';
                        }
                        if(j<2 && current_state.first!='l'){ 
                            //cout<<"r check"<<endl;
                            next_state = make_pair('r',make_pair(current_state.first+1, generate_state(current_state.second.second, 'r', i, j)));
                            next_states.push_back(next_state); 
                            //print_state(next_state.second.second);
                            //cout<<endl<<endl;
                            //current_state.first='r';
                        }
                        for(int k=0; k<next_states.size(); k++){
                            next_state = next_states[k];
                            if(closed.find(next_state.second)==closed.end()){ //checking if the state is already in closed
                                    
                                //checking if the state is already in open
                                if(open2.find(next_state.second.second)!=open2.end() && (((open2.find(next_state.second.second))->second)>(next_state.first))){
                                    for(auto it = open.begin(); it!=open.end(); it++){
                                        if(((*it).second).second==next_state.second){
                                            open.erase(it);
                                            break;
                                        }
                                    }
                                    open2.erase(open2.find(next_state.second.second));
                                    open.insert(make_pair(next_state.first+count_Misplaced_Tiles(next_state.second.second,end), next_state));
                                    open2.insert(make_pair(next_state.second.second, next_state.second.first));
                                    //parents.insert_or_assign(current_state.second, next_state.second);
                                    if(parents.find(next_state.second)!=parents.end()){
                                        parents.erase(parents.find(current_state.second));
                                    }
                                    parents.insert(make_pair(current_state.second, next_state.second));
                                }else{

                                    //when the current state is not in open or closed
                                    open.insert(make_pair(next_state.first+count_Misplaced_Tiles(next_state.second.second,end), next_state));
                                    open2.insert(make_pair(next_state.second.second, next_state.second.first));
                                    if(parents.find(next_state.second)!=parents.end()){
                                        parents.erase(parents.find(current_state.second));
                                    }
                                    parents.insert(make_pair(current_state.second, next_state.second));
                                }
                            }

                        }
                    }
                }
            }
        }
        counter++;
    }
    //cout<<counter<<endl;
    path.push_back(start);
    path.push_back(end);
    return path;
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