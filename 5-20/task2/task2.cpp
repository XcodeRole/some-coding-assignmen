#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int len=2;

class Student{
    public:
    string stuId;
    string name;
    double mathScore;
    double chineseScore;
    double englishScore;
};

//use to caculate average mark and sort them
class Student_avg{
    public:
    Student student;
    double avg_mark;
};


bool Greater (Student_avg &s1,Student_avg &s2){
    return s1.avg_mark>s2.avg_mark;
}

int main(){
    Student info[len];
    Student_avg info_add[len];
    cout<<"Please enter "<<len<<" students' information"<<endl;
    for (int i=0;i<len;i++){
        cout<<"No."<<i<<" input:"<<endl;
        cin>>info[i].stuId;
        cin>>info[i].name;
        cin>>info[i].mathScore;
        cin>>info[i].chineseScore;
        cin>>info[i].englishScore;
        info_add[i].student=info[i];
        info_add[i].avg_mark=(info[i].mathScore+info[i].chineseScore+info[i].englishScore)/3;
    }

    int avg_all=0;
    sort(info_add,info_add+len,Greater);
    for(auto elem:info_add){
        avg_all+=elem.avg_mark;
        cout<<"stuId: "<<elem.student.stuId<<" , "<<"avg_score: "<<elem.avg_mark<<endl;
    }
    cout<<"Those student  whose average mark is smaller than the average mark of all students:"<<endl;
    avg_all/=len;
    for(auto elem:info_add){
        if(elem.avg_mark<avg_all)
            cout<<"stuId: "<<elem.student.stuId<<" ";
    }
    cout<<endl;
    return 0;
}