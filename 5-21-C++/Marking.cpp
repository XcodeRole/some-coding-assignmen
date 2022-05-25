#include <string>
#include <vector>
#include <regex>
#include <map>
#include <fstream>

using namespace std;


enum class Credits{
    TenCredits=10,
    TwentyCredits=20
};

class CourseResult{
    private:
    double score;
    bool result;
    public:
    CourseResult(double score,bool result):score(score),result(result){};
    double getScore(){
        return this->score;
    }
    bool getResult(){
        return this->result;
    }
};
class courseHolder;
class studentHolder;

class Student{
    private:
    string identifier;
    string givenName;
    string familyName;
    string email;
    map <string,vector<double>> grades;
    map <string,CourseResult> results;

    public:
    Student(){};
    Student(string fileName);
    string getIdentifier(){
        return this->identifier;
    }
    string getGivenName(){
        return this->givenName;
    }
    string getFamilyName(){
        return this->familyName;
    }
    string getEmail(){
        return this->email;
    }
    bool getGrades(std::string courseCode,vector<double> &grade){
        map<string, vector<double>>::iterator iter=this->grades.find(courseCode);
        if (iter==this->grades.end())
            return false;
        else{
            grade=iter->second;
            return true;
        }
    }
    // bool getResult(std::string courseCode,bool result){
    //     map<string, bool>::iterator iter=this->results.find(courseCode);
    //     if (iter==this->results.end())
    //         return false;
    //     else{
    //         result=iter->second;
    //         return true;
    //     }
    // }

    void populateResults(courseHolder ch);

    smatch validateIdentifier();
    smatch validateEmail();

    bool needsResit(){
        for(auto elem:this->results){
            if(!elem.second.getResult())
                return true;
        }
        return false;
    }
    int numberOfCredits(courseHolder ch);
};
/*
    Since I don't quite understand the arrangement order of csv, 
    I directly use the format of key:value to read in
*/
Student::Student(string filename){
    fstream in;
    in.open(filename,ios::in);
    in>>this->identifier>>this->familyName>>this->givenName>>this->email;
    vector<double> grades;
    int line=0;
    while(!in.eof()){
        in>>grades[line++];
    }
    in.close();
}

class Course{
    protected:
    string identifier;
    vector<double> weights;
    Credits numberOfCredits;

    public:
    Course(){};
    smatch validateCourseCode();
    bool validateWeight();
    vector<double> getWeights(){
        return this->weights;
    }
    Credits getNumberOfCredits(){
        return this->numberOfCredits;
    }
    virtual vector<double> getGrade()=0;
};

class ExamOnly:public Course{
    string identifier;
    vector<double> getGrade(string stu_id,studentHolder sh);
};

class Hybrid:public Course{
    string identifier;
    vector<double> getGrade(string stu_id,studentHolder sh);
};

class CourseworkOnly:public Course{
    string identifier;
    vector<double> getGrade(string stu_id,studentHolder sh);
};

class studentHolder{
    public:
    map <string,Student> studentsLMap;
    void getAllGrades();
};

class courseHolder{
    public:
    map<string,Course*> coursesMap;
};

void Student::populateResults(courseHolder ch){
    vector<double> eachAssignmentGrade;
    for(auto &elem:ch.coursesMap){
        if(this->getGrades(elem.first,eachAssignmentGrade)){
            double score=0;
            for(int i=0;i<eachAssignmentGrade.size();i++){
                vector<double> weights=elem.second->getWeights();
                score+=eachAssignmentGrade[i]*weights[i]/100;
            }
            bool result;
            if(score>40)
                result=false;
            else
                result=true;
            CourseResult insertCR(score,result);
            this->results.insert(map<string,CourseResult>::value_type(elem.first,insertCR));
        }
    }
}

int Student::numberOfCredits(courseHolder ch){
    int sumCredit=0;
    for(auto elem:this->results){
        if(elem.second.getResult()){
            sumCredit+=int(ch.coursesMap[elem.first]->getNumberOfCredits());
        }
    }
    return sumCredit;
}

vector<double> ExamOnly::getGrade(string stu_id,studentHolder sh){
    vector<double> grades;
    sh.studentsLMap[stu_id].getGrades(this->identifier,grades);
    return grades;
}

    vector<double> Hybrid::getGrade(string stu_id,studentHolder sh){
    vector<double> grades;
    sh.studentsLMap[stu_id].getGrades(this->identifier,grades);
    return grades;
}

    vector<double> CourseworkOnly::getGrade(string stu_id,studentHolder sh){
    vector<double> grades;
    sh.studentsLMap[stu_id].getGrades(this->identifier,grades);
    return grades;
}