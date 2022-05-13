#include<iostream>
#include<vector>
// #include <iterator>
#include <algorithm>
using namespace std;

#define LEN_A 5
#define LEN_B 4
vector<int> union_Array (vector<int> &a, int lenA, vector<int> &b, int lenB);
int main(){
    vector<int> a(LEN_A);
    vector<int> b(LEN_B);
    cout<<"Please enter "<<LEN_A<<" elements into A vector"<<endl;
    for (int i=0;i<LEN_A;i++)
        cin>>a[i];
    cout<<"Please enter "<<LEN_B<<" elements into B vector"<<endl;
    for (int i=0;i<LEN_B;i++)
        cin>>b[i];
    vector<int> c=union_Array(a,LEN_A,b,LEN_B);
    cout<<"output:"<<endl;
    for (auto elem:c)
        cout<<elem<<" ";
    cout<<endl;
}

vector<int> union_Array (vector<int> &a, int lenA, vector<int> &b, int lenB) {
    vector<int> c;
    for(auto elem:a)
        if(find(c.begin(),c.end(),elem)==c.end())
            c.push_back(elem);
    for(auto elem:b)
        if(find(c.begin(),c.end(),elem)==c.end())
            c.push_back(elem);
    return c;
}