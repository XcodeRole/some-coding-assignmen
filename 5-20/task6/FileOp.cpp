#include "FileOp.hpp"

//Num is the number of points we wanna enter from the keyboard
void FileOp::board2file(int num,string filename){
    ofstream out;
    out.open(filename,ios::trunc);
    cout<<"Please input "<<num<<" points"<<endl;
    for(int i=0;i<num;i++){
        int x,y;
        cin>>x>>y;
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

void FileOp::del_point(string filename,coordinate cor){
    fstream fd;
    fd.open(filename,ios::in);
    string str_tmp;
    coordinate cor_tmp;
    //read file
    fd>>str_tmp;
    fd.close();
    //build string
    string del_str=to_string(cor.x)+" "+to_string(cor.y)+"\n";
    //find position
    int pos = str_tmp.find(del_str);
    //delete substring
    str_tmp.erase(pos,del_str.length());
    //clear the file and rewrite it
    fd.open(filename,ios::trunc);
    fd<<str_tmp;
}

bool FileOp::isInfile(string filename,coordinate cor){
    ifstream in;
    in.open(filename,ios::in);
    string line;
    while(!in.eof()){
        getline(in,line);
        istringstream istr(line);
        string x,y;
        istr>>x>>y;
        if(stod(x)==cor.x&&stod(y)==cor.y)
            return true;
    }
    return false;
}