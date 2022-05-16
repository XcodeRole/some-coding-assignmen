#include<iostream>
#include <math.h>
using namespace std;

#define PI acos(-1)

//Use enum types to represent relationships
enum class relation{
    interaction,
    tangency,
    separation,
    inside
};

ostream& operator << (ostream& out,relation r){
    switch(r)
    {
        case relation::interaction:
            out<<"interaction";
            break;
        case relation::tangency:
            out<<"tangency";
            break;
        case relation::separation:
            out<<"separation";
            break;
        case relation::inside:
            out<<"inside";
            break;
        default:
            out<<"errs";
    }
    return out;
}

class Circle{
    public:
    int x;
    int y;
    double radius;

    Circle(int x,int y,int radius):x(x),y(y),radius(radius){}

    double cal_area() const ;

    Circle operator + (const Circle& c);

};
//calculate area of a circle
double Circle::cal_area() const {
    return PI*pow(this->radius,2);
}
//override + operator of Circle
Circle Circle::operator+(const Circle& b){
    double a_area=this->cal_area();
    double b_area=b.cal_area();
    double c_area=a_area+b_area;

    double c_radius=pow(c_area/PI,0.5);

    return Circle(this->x,this->y,c_radius);

}

relation judgeRelation(Circle c1,Circle c2){
    double x_distance=abs(c1.x-c2.x);
    double y_distance=abs(c1.y-c2.y);
    double distance=pow(pow(x_distance,2)+pow(y_distance,2),0.5);
    //r1 add r1
    double r_a_distance=c1.radius+c2.radius;
    //r1 sub r2 
    double r_s_distance=fabs(c1.radius-c2.radius);
    // debug print
    // cout<<x_distance<<" "<<y_distance<<" "<<distance<<" "<<r_a_distance<<" "<<r_s_distance<<endl;
    if(fabs(distance-r_a_distance)<1e-8)
        return relation::tangency;
    else if(distance>r_a_distance)
        return relation::separation;
    else if(distance>r_s_distance&&distance<r_a_distance)
        return relation::interaction;
    else
        return relation::inside;
}

int main(){
    int x1,y1,x2,y2;
    double r1,r2;
    cout<<"Please enter the first circle's coordinate(x and y) and radius:"<<endl;
    cin>>x1>>y1>>r1;
    cout<<"Please enter the second circle's coordinate(x and y) and radius:"<<endl;
    cin>>x2>>y2>>r2;
    Circle c1(x1,y1,r1);
    Circle c2(x2,y2,r2);
    relation rel=judgeRelation(c1,c2);
    cout<<"There relationship is "<<rel<<endl;
    Circle c3=c1+c2;
}