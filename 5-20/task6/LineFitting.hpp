#include "FileOp.hpp"

class LineFitting{
    public:
    LineFitting();
    ~LineFitting();
    //storage point
    vector<coordinate> points;
    //storage distance between each points and the best fit line
    vector<double> distance;
    //param storage a b and c
    vector<double> param;
    
    void cal_fitline();
    
    void cal_distance();

    coordinate find_best_point();

    coordinate find_worst_point();

    double cal_SE();
};

