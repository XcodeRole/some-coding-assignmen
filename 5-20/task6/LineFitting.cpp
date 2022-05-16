#include "LineFitting.hpp"

LineFitting::LineFitting(vector<coordinate> points):points(points){}

void LineFitting::cal_fitline(){
    double a1=0;
    double a2=0;
    double b1=0;
    double b2=this->points.size();
    double d1=0;
    double d2=0;
    for (auto point:this->points){
        a1+=point.x*point.x;
        a2+=point.x;
        b1+=point.x;
        d1-=point.x*point.y;
        d2-=point.y;
    }
    double a=(b1*d2-b2*d1)/(a1*b2-a2*b1);
    double b=(a1*d2-a2*d1)/(a1*b2-a2*b1);
    // y=ax+b => ax-y+b=0 
    this->param={a,-1,b};
}

void LineFitting::cal_distance(){
    double numerator;
    double deumerator;
    double distance;
    double a=this->param[0];
    double b=this->param[1];
    double c=this->param[2];
    for (auto point:this->points){
        numerator=fabs(a*point.x+b*point.y+c);
        deumerator=pow(a*a+b*b,0.5);
        distance=numerator/deumerator;
        this->distances.push_back(distance);
    }
}

coordinate LineFitting::find_best_point(){
    int min_element_index=min_element(this->distances.begin(),this->distances.end())-this->distances.begin();
    return this->points[min_element_index];
}

coordinate LineFitting::find_worst_point(){
    int max_element_index=max_element(this->distances.begin(),this->distances.end())-this->distances.begin();
    return this->points[max_element_index];
}

double LineFitting::cal_SE(){
    double dis_acc=accumulate(this->distances.begin(),this->distances.end(),0);
    double dis_mean=dis_acc/this->distances.size();
    double numerator=0;
    for (auto distance:this->distances){
        numerator+=pow(distance-dis_mean,2);
    }
    double SE=pow(numerator/this->distances.size(),0.5);
    return SE;
}