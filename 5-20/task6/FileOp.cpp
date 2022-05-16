#include "FileOp.hpp"
#include <iomanip> 
using namespace std;

//Num is the number of points we wanna enter from the keyboard
void FileOp::board2file(int num,string filename){
    ofstream out;
    out.setf(ios::fixed, ios::floatfield);
    out.open(filename,ios::trunc);
    cout<<"Please input "<<num<<" points"<<endl;
    for(int i=0;i<num;i++){
        double x,y;
        cin>>x>>y;
        out.precision(3);
        out<<x<<" "<<y<<"\n";
    }
    out.close();
}

vector<coordinate> FileOp::file2attr(string filename){
    vector<coordinate> vec_cor;
    string tmp;
    ifstream in;
    in.open(filename,ios::in);
    //read till file end
    while(!in.eof()){
        //read line split with '\n'
        getline(in,tmp);
        //split string with space using istringstream
        istringstream istr(tmp);
        coordinate cor;
        istr>>cor.x>>cor.y;
        vec_cor.push_back(cor);
    }
    return vec_cor;
}

bool FileOp::del_point(string filename,coordinate cor){
    //representing wether delete success
    bool success=false;
    ifstream in;
    //read points.txt file
    in.open(filename,ios::in);
    ostringstream oss;
    string str_all_points;
    //Read the entire contents of a file at once
    oss<<in.rdbuf();
    str_all_points=oss.str();
    // debug print 
    // cout<<str_all_points<<endl;
    in.close();
    //build string
    ostringstream ostr;
    ostr.setf(ios::fixed,ios::floatfield);
    ostr.precision(3);
    //=to_string(cor.x)+" "+to_string(cor.y)+"\n";
    ostr<<cor.x<<" "<<cor.y<<"\n";
    string del_str=ostr.str();
    cout<<del_str;
    //find position
    int pos = str_all_points.find(del_str);
    //delete substring
    if (pos > -1){
        str_all_points.erase(pos,del_str.length());
        success=true;
    }
    ofstream out;
    //clear the file and rewrite it
    out.open(filename,ios::trunc);
    out<<str_all_points;
    out.close();
    return success;
}

bool FileOp::isInfile(string filename,coordinate cor){
    ifstream in;
    in.open(filename,ios::in);
    string line;
    //Solve the problem that the last line of the points file is an empty("\n") and the stod parameter does not match the correct type
    getline(in,line);
    while(!in.eof()&&line!="\n"){
        istringstream istr(line);
        string x,y;
        istr>>x>>y;
        // debug print
        // cout<<x<<" "<<y<<endl;
        if(stod(x)==cor.x&&stod(y)==cor.y)
            return true;
        getline(in,line);
    }
    return false;
}