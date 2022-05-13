#include<iostream>
#include<list>
#include<algorithm>

using namespace std;

struct student{
    string stuId;
    string name;
    double mathScore;
    double chineseScore;
    double englishScore;
};
typedef struct student student;

typedef struct student_avg{
    student s;
    double avg_mark;
}student_avg;

bool Greater(student_avg &s1,student_avg &s2){
    return s1.avg_mark>s2.avg_mark;
}

int len;

int main(){
    cout<<"Please enter the number of students:"<<endl;
    cin>>len;
    list<student_avg> students_list(len);
    cout<<"Please enter information of students:"<<endl;
    int i=0;
    int avg_all=0;
    for (auto &elem:students_list){
        cout<<"No."<<i<<" input:"<<endl;
        cin>>elem.s.stuId>>elem.s.name>>elem.s.mathScore>>elem.s.chineseScore>>elem.s.englishScore;
        elem.avg_mark=(elem.s.mathScore+elem.s.chineseScore+elem.s.englishScore)/3;
        avg_all+=elem.avg_mark;
        i++;
    }
    avg_all/=len;
    students_list.sort(Greater);
    for(auto &elem:students_list){
        cout<<"stuId: "<<elem.s.stuId<<" avg_mark: "<<elem.avg_mark<<endl;
    }
    cout<<"Those students whose average mark is smaller than the average mark of all students:"<<endl;
    for(auto &elem:students_list){
        if (elem.avg_mark<avg_all)
            cout<<"stuId: "<<elem.s.stuId<<" ";
    }
    cout<<endl;

}