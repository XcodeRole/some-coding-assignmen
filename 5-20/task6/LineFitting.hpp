#include "FileOp.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
using namespace std;

class LineFitting{
    public:
    LineFitting(){};
    LineFitting(vector<coordinate> points);
    ~LineFitting(){};

    //storage point
    vector<coordinate> points;
    //param storage a b c in vector type to represent a line
    vector<double> param;
    //storage distance between each points and the best fit line
    vector<double> distances;
    
    void cal_fitline();
    
    void cal_distance();

    coordinate find_best_point();

    coordinate find_worst_point();

    double cal_SE();
};
