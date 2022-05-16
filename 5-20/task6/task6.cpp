#include "LineFitting.hpp"
using namespace std;

int main(){
    string fdstring="points.txt";
    int num;
    cout<<"Please enter the number of coordinates you want input next."<<endl;
    cin>>num;
    FileOp::board2file(num,fdstring);
    vector<coordinate> points=FileOp::file2attr(fdstring);
    LineFitting line(points);
    line.cal_fitline();
    cout<<"The best fit line is: ("<<line.param[0]<<")x+("<<line.param[1]<<")y+("<<line.param[2]<<")=0"<<endl;
    cout<<"Please enter a point:";
    cout<<endl;
    double x,y;
    cin>>x>>y;
    // debug output
    // cout<<(FileOp::isInfile(fdstring,coordinate{x,y}))<<endl;
    bool isIn = FileOp::isInfile(fdstring,coordinate{x,y});
    if (isIn){
        cout<<"The point is in the file:("<<x<<" "<<y<<")"<<endl;
    }else{
        cout<<"The point is not in the file!"<<endl;
    }
    line.cal_distance();
    coordinate worst_point=line.find_worst_point();
    cout<<"The worst point that has a maximal distance to the best fit line in all the points: ("<<worst_point.x<<" "<<worst_point.y<<")"<<endl;
    cout<<"Please enter a point and we will delete it from the file:";
    cout<<endl;
    coordinate del_p;
    cin>>del_p.x>>del_p.y;
    if (FileOp::del_point(fdstring,del_p)){
        cout<<"Delete success!"<<endl;
    }
    else
        cout<<"The point is not in the file!"<<endl;
}